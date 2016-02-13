#include "MovieStream.h"
#include "Utility.h"
#include "MessageTypes.h"
#include <string.h>
#include <arpa/inet.h>
#include <string.h>
#include <sstream>
#include <pthread.h>
#include <iostream>

using namespace std;

bool MovieStream::Stream(string moviePath)
{
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

 	movieStream->CreateSocket();

 	cout << endl << movieStream->GetStreamSocket() << endl;
 	int newSock = accept(movieStream->GetStreamSocket(), (sockaddr*)&addr, &addrLen);
 	if(newSock < 0)
 	{
 		Utility::PrintError("Error accepting connection");
 		pthread_exit(NULL);
 	}

 	string test = "test stream.";

 	while(1)
 	{
 		send(newSock, test.c_str(), test.size(), 0);
 	}
 	

 	pthread_exit(NULL);
}

string MovieStream::GetPortAndIP()
{
	struct sockaddr_in addr;
	socklen_t addrLen = sizeof(addr);
	stringstream stream;


	memset(&addr, 0, sizeof(sockaddr));
	if(getsockname(m_StreamSocket, (sockaddr*)&addr, &addrLen) == 0)
	{
		char buffer[INET_ADDRSTRLEN];
		string IP;
		int port;

		inet_ntop(AF_INET, &addr, buffer, INET_ADDRSTRLEN);
		IP = string(buffer);
		port = ntohs(addr.sin_port);

		stream << IP << endl;
		stream << port << endl;
	}
	else
	{
		Utility::PrintError("Error getting sock name");
	}


	return stream.str();
}