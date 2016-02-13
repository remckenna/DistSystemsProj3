#ifndef __NUTELLA_CLIENT_H__
#define __NUTELLA_CLIENT_H__

#include "NetworkMessageHandler.h"
#include "MultiCastServer.h"

#define STREAM_REQUEST_TIMEOUT 5

class NutellaClient : public NetworkMessageHandler
{
private:
	MultiCastServer m_Server;

	bool m_bShouldRun;

	bool m_bSearchResponseRecvd;

	bool m_bIsExpectingSearchResponse;

public:
	NutellaClient();
	~NutellaClient() {}

	virtual bool OnMessage(string msg, char messageType);

	bool WaitForSearchResponse();

	bool HandleSearchRequest(string query);

	bool HandleSuccessfulSearch(string portIPCombo);
};
#endif