#ifndef __MOVIEPLAYER_H__
#define __MOVIEPLAYER_H__

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using std::string;

class MoviePlayer
{
private:
	int m_PlayRate;

	int m_StreamPort;

	string m_StreamIP;

	int m_StreamSocket;

	bool ConnectToStream();


public:
	MoviePlayer(int playRate, int streamPort, string streamIP) : m_PlayRate(playRate),
																m_StreamPort(streamPort),
																m_StreamIP(streamIP)
    {}


	~MoviePlayer() {}

	void Play();

	void SetStreamIP(string ip) {m_StreamIP = ip;}
	string GetStreamIP() const {return m_StreamIP;}

	void SetStreamPort(int port) {m_StreamPort = port;}
	int GetStreamPort() const {return m_StreamPort;}

	void SetPlayRate(int playRate) {m_PlayRate = playRate;}
	int GetPlayRate() const {return m_PlayRate;}
};
#endif
