#include "MultiCastServer.h"
#include "Utility.h"
#include "MessageTypes.h"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


MultiCastServer::MultiCastServer(string address, int port) :
											m_MultiCastSock(-1),
											m_MultiCastAddr(address),
											m_Port(port),
											m_bIsListening(false),
											m_bStopListenThread(false)										
{
	CreateSocket(m_MultiCastAddr, m_Port);
}

bool MultiCastServer::CreateSocket(string address, int port)
{
	struct ip_mreq mreq;
	int on = 1;

	memset(&mreq, 0, sizeof(mreq));
	memset(&m_Addr, 0, sizeof(m_Addr));
	memset(&m_GroupAddr, 0, sizeof(m_GroupAddr));

	m_Addr.sin_family = AF_INET;
	m_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_Addr.sin_port = htons(port);

	memcpy(&m_GroupAddr, &m_Addr, sizeof(m_Addr));
	m_GroupAddr.sin_addr.s_addr = inet_addr(address.c_str());

	mreq.imr_multiaddr.s_addr = inet_addr(address.c_str());
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	m_MultiCastSock = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(m_MultiCastSock < 0)
	{
		Utility::PrintError("Error Creating mcast socket");
		return false;
	}

	if(setsockopt(m_MultiCastSock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	{
		Utility::PrintError("Error setting sockopt(1)");
		Shutdown();
		return false;
	}

	if(setsockopt(m_MultiCastSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
	{
		Utility::PrintError("Error setting sockopt(2)");
		Shutdown();
		return false;
	}

	int loop = 0;
	if(setsockopt(m_MultiCastSock, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0)
	{
		Utility::PrintError("Error setting sockopt(3)");
		Shutdown();
		return false;
	}

	if(bind(m_MultiCastSock, (struct sockaddr*)&m_Addr, sizeof(m_Addr)) < 0)
	{
		Utility::PrintError("Error binding socket");
		Shutdown();
		return false;
	}

	return true;
}

size_t MultiCastServer::Receive(void* buffer, size_t bufferSize, struct sockaddr* fromAddr)
{
	size_t bytesReceived = -1;
	socklen_t addrLen = sizeof(*fromAddr);

	bytesReceived = recvfrom(m_MultiCastSock, buffer, bufferSize, 0, fromAddr, &addrLen);

	return bytesReceived;
}

size_t MultiCastServer::Send(const void* buffer, size_t bufferSize)
{
	size_t bytesSent = -1;
	socklen_t addrLen = sizeof(m_GroupAddr);

	bytesSent = sendto(m_MultiCastSock, buffer, bufferSize, 0, (struct sockaddr*)&m_GroupAddr, addrLen);

	return bytesSent;
}

bool MultiCastServer::SendWithType(char type, string msg)
{
	bool result = true;
	char* msgAndHeader = new char[msg.size() + 1];
	msgAndHeader[0] = type;
	memcpy(msgAndHeader+1, msg.c_str(), msg.size());

	if(Send(msgAndHeader, msg.size() + 1) < 0)
	{
		Utility::PrintDebugMessage("Couldn't send search request.");
		result = false;
	}

	delete[] msgAndHeader;
	return result;
}


void MultiCastServer::Shutdown()
{
	if(close(m_MultiCastSock) < 0)
	{
		Utility::PrintError("Error shutdown mcast server");
	}
}

void MultiCastServer::Listen()
{
	if(!m_bIsListening)
	{
		m_bIsListening = true;
		if(pthread_create(&m_ListeningThreadID, NULL, &MultiCastServer::RecvLoop, this) != 0)
		{
			Utility::PrintDebugMessage("Error Starting Listen thread.");
		}
	}
	else
	{
		Utility::PrintDebugMessage("Server Already Listening.");
	}

}

void* MultiCastServer::RecvLoop(void* multiCastServer)
{
	MultiCastServer* server = static_cast<MultiCastServer*>(multiCastServer);
	if(server)
	{
		char buffer[MAX_RECEIVE_BUFFER];
		ssize_t bytesReceived = 0;
		char header = 0x00;
		while(!server->m_bStopListenThread)
		{
			sockaddr_in fromAddr;
			char ipBuffer[INET_ADDRSTRLEN];

			bytesReceived = server->Receive(&buffer, MAX_RECEIVE_BUFFER, (sockaddr*)&fromAddr);

			string fromAddress(inet_ntoa(fromAddr.sin_addr));

			header = MultiCastServer::GetAndStripHeader(buffer, bytesReceived);

			if(!server->m_MsgHandler->OnMessage(string(buffer), header,  fromAddress))
			{
				Utility::PrintDebugMessage("Network message received, but not handled.");
			}
		}
	}
	else
	{
		Utility::PrintDebugMessage("The expected message handler was not present.");
	}

	pthread_exit(NULL);
}

char MultiCastServer::GetAndStripHeader(char message[], ssize_t size)
{
	char header = *message;
	int i = 0;
	if(message)
	{
		for(i = 0; (i + 1) < size; i++)
		{
			message[i] = message[i + 1]; 
		}

		message[i] = '\0'; 
	}
	else
	{
		Utility::PrintDebugMessage("Message in GetAndStripHeader was null.");
	}


	return header;
}