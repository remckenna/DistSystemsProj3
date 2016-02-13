#ifndef __NUTELLAFILE_H__
#define __NUTELLAFILE_H__

#define MAX_LINE_BUFF 500

#include <string>
#include <map>

using std::string;
using std::map;

class NutellaFile
{
private:
	map<string, string> m_sFileContents;

public:
	NutellaFile(){}

	bool Contains(string key){return (m_sFileContents.count(key) > 0);}

	string GetMoviePath(string name) {return m_sFileContents[name];}

	bool Load(string path);

	void Print();
};
#endif