#include "MoviePlayer.h"
#include "Utility.h"
#include "MessageTypes.h"
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

void MoviePlayer::Play()
{
	if(ConnectToStream())
	{
		pthread_create(&m_Tid, NULL, &MoviePlayer::Intern_Play, this);
	}
}

void* MoviePlayer::Intern_Play(void* moviePlayer)
{
	MoviePlayer* player = static_cast<MoviePlayer*>(moviePlayer);
	Utility::PrintDebugMessage("Beginning movie...");
	char buffer[2000];
	memset(&buffer, 0, 2000);
	while(1)
	{
		//system("clear");
		printf("\033[2J");
		printf("\033[0;0f");
		if(recv(player->m_StreamSocket, &buffer, 2000, 0) < 1)
		{
			break;
		}
		if(buffer[0] == (char)0x03)
		{
			break;
		}
		//cout << string(buffer) << endl;
		printf("%s", string(buffer).c_str());
	}

	printf("\033[2J");
	printf("\033[0;0f");
	cout << "Enter the name of a movie to watch: ";

	close(player->m_StreamSocket);
	pthread_exit(NULL);
}

bool MoviePlayer::ConnectToStream()
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));

	hostent* server;

	server = gethostbyname(m_StreamIP.c_str());

	addr.sin_family = AF_INET;
	//addr.sin_addr.s_addr = inet_addr(m_StreamIP.c_str());
	memcpy((char*)&addr.sin_addr.s_addr, (char*)server->h_addr, server->h_length);
	addr.sin_port = htons(m_StreamPort);

	m_StreamSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_StreamSocket < 0)
	{
		Utility::PrintError("Error creating stream socket");
		return false;
	}
	if(connect(m_StreamSocket, (sockaddr*)&addr, sizeof(addr)) < 0)
	{
		Utility::PrintError("Error connecting to remote peer");
		return false;
	}

	return true;
}