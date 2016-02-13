#ifndef __MULTICASTSERVER_H__
#define __MULTICASTSERVER_H__

#include "NetworkMessageHandler.h"
#include <string>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_RECEIVE_BUFFER 1000

using std::string;

class MultiCastServer
{

private:
	int m_MultiCastSock;

	string m_MultiCastAddr;

	struct sockaddr_in m_Addr;

	struct sockaddr_in m_GroupAddr;

	int m_Port;

	bool m_bIsListening;

	bool m_bStopListenThread;

	pthread_t m_ListeningThreadID;

	NetworkMessageHandler* m_MsgHandler;

	MultiCastServer(){};

	bool CreateSocket(string address, int port);

	static void* RecvLoop(void* multiCastServer);

	static char GetAndStripHeader(char message[], ssize_t size);

public:
	MultiCastServer(string address, int port);

	size_t Receive(void* buffer, size_t bufferSize);

	size_t Send(const void* buffer, size_t bufferSize);

	bool SendWithType(char type, string msg);

	void Listen();

	void Shutdown();

	void SetMessageHandler(NetworkMessageHandler* handler){m_MsgHandler = handler;}
};
#endif