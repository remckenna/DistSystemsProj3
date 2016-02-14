#include "MovieStream.h"
#include "Utility.h"
#include "MessageTypes.h"
#include <string.h>
#include <arpa/inet.h>
#include <string.h>
#include <sstream>
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <istream>

using namespace std;

bool MovieStream::Stream(string moviePath)
{
	m_MoviePath = moviePath;
	pthread_create(&m_tid, NULL, &MovieStream::intern_Stream, this);
	return true;
}

bool MovieStream::CreateSocket()
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = 0;

	m_StreamSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_StreamSocket < 0)
	{
		Utility::PrintError("Error creating stream socket");
		return false;
	}

	if(bind(m_StreamSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		Utility::PrintError("Error binding socket.");
		return false;
	}

	listen(m_StreamSocket, 10);

	return true;
}

void* MovieStream::intern_Stream(void* stream)
{
 	MovieStream* movieStream = static_cast<MovieStream*>(stream);
 	struct sockaddr_in addr;
 	socklen_t addrLen = sizeof(addr);
 	memset(&addr, 0, sizeof(addr));

 	cout << endl << movieStream->GetStreamSocket() << endl;
 	int newSock = accept(movieStream->GetStreamSocket(), (sockaddr*)&addr, &addrLen);
 	if(newSock < 0)
 	{
 		Utility::PrintError("Error accepting connection");
 		pthread_exit(NULL);
 	}

 	Utility::PrintDebugMessage("Connected to movie player.");
 	string test = "test stream.";


 	string movie = LoadMovie(movieStream->m_MoviePath);


 	istringstream iMovieStream(movie);

 		string frame, line;
 		while(getline(iMovieStream, line))
 		{
 			if(line == "end")
 			{
 				sleep(1);
 				send(newSock, frame.c_str(), frame.size(), 0);
 				frame.clear();
 				continue;
 			}
 			else
 			{
 				frame.append(line + '\n');
 				line.clear();
 			}
 		}

 		//send(newSock, test.c_str(), test.size(), 0);
 	pthread_exit(NULL);
}

string MovieStream::GetPort()
{
	struct sockaddr_in addr;
	socklen_t addrLen = sizeof(addr);
	stringstream stream;


	memset(&addr, 0, sizeof(sockaddr));
	if(getsockname(m_StreamSocket, (sockaddr*)&addr, &addrLen) == 0)
	{
		int port;
		port = ntohs(addr.sin_port);

		stream << port << endl;
	}
	else
	{
		Utility::PrintError("Error getting sock name");
	}


	return stream.str();
}

string MovieStream::LoadMovie(string moviePath)
{
	string result;
	char* workingDir;
	string fullMoviePath = string(workingDir = getcwd(NULL, 0));
	fullMoviePath.append(moviePath);
	delete workingDir; 
	FILE* movieFile = fopen(fullMoviePath.c_str(), "r");
	char* line;
	size_t len = 0;

	if(!movieFile)
	{
		Utility::PrintError("Unabled to open movie file");
		return result;
	}

	while((getline(&line, &len, movieFile) != EOF))
	{
		result.append(string(line));
	}
	return result;
}