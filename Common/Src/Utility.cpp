#include "Utility.h"
#include <stdio.h>
#include <iostream>

using namespace std;

void Utility::PrintError(string msg)
{
	#ifdef DEBUG
		perror(msg.c_str());
	#endif
}

void Utility::PrintDebugMessage(string msg)
{
	#ifdef DEBUG
		cout << msg << endl;
	#endif
}