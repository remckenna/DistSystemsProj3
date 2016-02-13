#include "NutellaClient.h"
#include "NutellaFile.h"
#include "MessageTypes.h"
#include "Utility.h"
#include "MovieStream.h"
#include "MoviePlayer.h"
#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>
#include <sstream>
#include <stdlib.h>

using namespace std;
using std::string;

NutellaClient::NutellaClient() : m_Server("239.0.0.1", 8953),
								m_bShouldRun(true),
								m_bSearchResponseRecvd(false),
								m_bIsExpectingSearchResponse(false)
{
	string movieToStream;

	m_Server.SetMessageHandler(this);
	m_Server.Listen();

	cout << "Connected to the Nutella network..." << endl;

	while(m_bShouldRun)
	{
		cout << "Enter the name of a movie to watch: ";
		cin >> movieToStream;
		m_Server.SendWithType(MessageTypes::SearchRequest, movieToStream);
		m_bIsExpectingSearchResponse = true;

		if(!WaitForSearchResponse())
		{
			cout << "Unable to find the requested movie." << endl;
		}

	}
	m_Server.Shutdown();
}

bool NutellaClient::OnMessage(string msg, char messageType)
{
	bool bWasHandled = true;
	switch(messageType)
	{
		case MessageTypes::SearchRequest:
		{
			HandleSearchRequest(msg);
			break;
		}
		case MessageTypes::SearchSuccess:
		{
			m_bSearchResponseRecvd = true;
			HandleSuccessfulSearch(msg);
			break;
		}
		default:
			bWasHandled = false;
	}
	return bWasHandled;
}

bool NutellaClient::WaitForSearchResponse()
{	
	clock_t startTime = clock();

	while(!m_bSearchResponseRecvd)
	{	
		clock_t curTime = clock();
		clock_t tickSoFar = curTime - startTime;
		double timeInSeconds = tickSoFar / (double) CLOCKS_PER_SEC;
		if(timeInSeconds >= STREAM_REQUEST_TIMEOUT) return false;
	}

	m_bSearchResponseRecvd = false;
	return true;
}

bool NutellaClient::HandleSearchRequest(string query)
{
	NutellaFile movieListFile;
	movieListFile.Load("test.nutella");
	if(movieListFile.Contains(query))
	{
		MovieStream* mStream = new MovieStream();
		mStream->CreateSocket();
		string portAndIP = mStream->GetPortAndIP();
		//Get the path to the movie file.
		string moviePath = movieListFile.GetMoviePath(query);

		mStream->Stream(moviePath);
		//Send IP address and port to stream the movie from.
		m_Server.SendWithType(MessageTypes::SearchSuccess, portAndIP);
		return true;
	}

	return false;
}

bool NutellaClient::HandleSuccessfulSearch(string portIPCombo)
{
	if(m_bIsExpectingSearchResponse)
	{
		string ip, port;
		istringstream stringStream(portIPCombo);

		getline(stringStream, ip);
		getline(stringStream, port);

		Utility::PrintDebugMessage(ip);
		Utility::PrintDebugMessage(port);

		MoviePlayer player(40, atoi(port.c_str()), ip);
		player.Play();

		m_bIsExpectingSearchResponse = false;
	}
	return true;
}