#include "MultiCastServer.h"
#include "Utility.h"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


MultiCastServer::MultiCastServer(string address, int port) :
											m_MultiCastSock(-1),
											m_MultiCastAddr(address),
											m_Port(port)										
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

	if(bind(m_MultiCastSock, (struct sockaddr*)&m_Addr, sizeof(m_Addr)) < 0)
	{
		Utility::PrintError("Error binding socket");
		Shutdown();
		return false;
	}

	return true;
}

size_t MultiCastServer::Receive(void* buffer, size_t bufferSize)
{
	size_t bytesReceived = -1;
	socklen_t addrLen = sizeof(m_Addr);

	bytesReceived = recvfrom(m_MultiCastSock, buffer, bufferSize, 0, (struct sockaddr*)&m_Addr, &addrLen);

	return bytesReceived;
}

size_t MultiCastServer::Send(const void* buffer, size_t bufferSize)
{
	size_t bytesSent = -1;
	socklen_t addrLen = sizeof(m_GroupAddr);

	bytesSent = sendto(m_MultiCastSock, buffer, bufferSize, 0, (struct sockaddr*)&m_GroupAddr, addrLen);

	return bytesSent;
}



void MultiCastServer::Shutdown()
{
	if(close(m_MultiCastSock) < 0)
	{
		Utility::PrintError("Error shutdown mcast server");
	}
}