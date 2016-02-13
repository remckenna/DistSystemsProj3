#include "NutellaClient.h"
#include "NutellaFile.h"
#include "MessageTypes.h"
#include "Utility.h"
#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>
#include <sstream>

using namespace std;
using std::string;

NutellaClient::NutellaClient() : m_Server("239.0.0.1", 8953),
								m_bShouldRun(true),
								m_bSearchResponseRecvd(false)
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
		//Get the path to the movie file.
		string moviePath = movieListFile.GetMoviePath(query);

		//Send IP address and port to stream the movie from.

		stringstream stream;

		stream << "120.0.0.1" << endl;
		stream << "8945" << endl;

		m_Server.SendWithType(MessageTypes::SearchSuccess, stream.str());
		return true;
	}

	return false;
}

bool NutellaClient::HandleSuccessfulSearch(string portIPCombo)
{
	string ip, port;
	istringstream stringStream(portIPCombo);

	getline(stringStream, ip);
	getline(stringStream, port);

	Utility::PrintDebugMessage(ip);
	Utility::PrintDebugMessage(port);
	return true;
}