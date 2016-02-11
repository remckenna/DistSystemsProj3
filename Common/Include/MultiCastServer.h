#ifndef __MULTICASTSERVER_H__
#define __MULTICASTSERVER_H__

#include <string>
#include <arpa/inet.h>

using std::string;

class MultiCastServer
{

private:
	int m_MultiCastSock;

	string m_MultiCastAddr;

	struct sockaddr_in m_Addr;

	struct sockaddr_in m_GroupAddr;

	int m_Port;

	MultiCastServer(){};

	bool CreateSocket(string address, int port);

public:
	MultiCastServer(string address, int port);

	size_t Receive(void* buffer, size_t bufferSize);

	size_t Send(const void* buffer, size_t bufferSize);

	void Shutdown();
};
#endif