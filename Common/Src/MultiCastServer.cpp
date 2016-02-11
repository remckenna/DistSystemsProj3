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
	int err = -1, on = 1;

	m_MultiCastSock = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_MultiCastSock < 0)
	{
		Utility::PrintError("Error Creating mCast socket");
		return false;
	}

	bzero((char*)&m_Addr, sizeof(m_Addr));
	m_Addr.sin_family = AF_INET;
	m_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_Addr.sin_port = htons(port);

	err = setsockopt(m_MultiCastSock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if(err < 0)
	{
		Utility::PrintError("Error setting sock options");
		Shutdown();
		return false;
	}

	err = bind(m_MultiCastSock, (struct sockaddr*)&m_Addr, sizeof(m_Addr));
	if(err < 0)
	{
		Utility::PrintError("Error binding mcast socket.");
		Shutdown();
		return false;
	}

	mreq.imr_multiaddr.s_addr = inet_addr(address.c_str());
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	err = setsockopt(m_MultiCastSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
	if(err < 0)
	{
		Utility::PrintError("Error setsockopt(2)");
		Shutdown();
		return false;
	}

	memcpy(&m_Addr, &m_GroupAddr, sizeof(m_Addr));
	//inet_aton(address.c_str(), &m_GroupAddr.sin_addr);
	m_GroupAddr.sin_addr.s_addr = inet_addr(address.c_str()); 

	return true;
}

size_t MultiCastServer::Receive(void* buffer, size_t bufferSize)
{
	size_t bytesReceived = -1;

	socklen_t addrLen = sizeof(m_Addr);
	bytesReceived = recvfrom(m_MultiCastSock, buffer, bufferSize, 0, 
		(struct sockaddr*)&m_Addr, &addrLen);

	return bytesReceived;
}

size_t MultiCastServer::Send(const void* buffer, size_t bufferSize)
{
	size_t bytesSent = -1;

	sendto(m_MultiCastSock, buffer,  bufferSize, 0,
		(struct sockaddr*)&m_GroupAddr,  sizeof(m_GroupAddr));

	return bytesSent;
}



void MultiCastServer::Shutdown()
{
	if(close(m_MultiCastSock) < 0)
	{
		Utility::PrintError("Error shutdown mcast server");
	}
}