#ifndef __NETWORK_MESSAGE_HANDLER_H__
#define __NETWORK_MESSAGE_HANDLER_H__

#include <string>

using std::string;

class NetworkMessageHandler
{
public:
	NetworkMessageHandler(){}

	virtual ~NetworkMessageHandler(){}

	virtual bool OnMessage(string msg, char messageType, string fromIP) = 0;
};

#endif