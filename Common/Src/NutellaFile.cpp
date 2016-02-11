#include "NutellaFile.h"
#include "Utility.h"
#include <sys/stat.h> 
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <iostream>


using namespace std;

bool NutellaFile::Load(string path)
{
	FILE* fp = fopen(path.c_str(), "r");
	if(fp == NULL)
	{
		Utility::PrintError("Error on open");
		return false;
	}
	char buffer[MAX_LINE_BUFF];
	char c;
	int i = 0;
	string curKey;
	while((c = fgetc(fp)) != EOF)
	{
		if(c == ' ')
		{
			buffer[i] = '\0';
			curKey = string(buffer);
			memset(buffer, 0, MAX_LINE_BUFF);
			i = 0;
			continue;
		}

		if(c == '\n')
		{
			buffer[i] = '\0';
			m_sFileContents[curKey] = string(buffer);
			memset(buffer, 0, MAX_LINE_BUFF);
			i = 0;
			continue;
		}
		buffer[i] = c;
		i++;
	}


	return true;
}

void NutellaFile::Print()
{
	map<string, string>::iterator it;

	for(it = m_sFileContents.begin(); it != m_sFileContents.end(); ++it)
	{
		cout << it->first <<": " << it->second << endl;
	}
}