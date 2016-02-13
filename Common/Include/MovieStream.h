#ifndef __MOVIE_STREAM_H__
#define __MOVIE_STREAM_H__

#include <string>

using std::string;

class MovieStream
{

private:
	int m_StreamSocket;

	pthread_t m_tid;

	static void* intern_Stream(void* movieStream);

public:
	MovieStream(){}
	~MovieStream(){}

	bool Stream(string moviePath);

	bool CreateSocket();

	string GetPortAndIP();

	int GetStreamSocket() const {return m_StreamSocket;}
};

#endif