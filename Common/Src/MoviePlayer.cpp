#include "MoviePlayer.h"
#include "Utility.h"
#include <string.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

void MoviePlayer::Play()
{
	if(ConnectToStream())
	{
		Utility::PrintDebugMessage("Beginning movie...");
		char buffer[2000];
		while(1)
		{
			recv(m_StreamSocket, &buffer, 2000, 0);
			cout << string(buffer) << endl;
		}
	}

}

bool MoviePlayer::ConnectToStream()
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(m_StreamIP.c_str());
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