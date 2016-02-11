#include "Utility.h"
#include <stdio.h>

void Utility::PrintError(string msg)
{
	#ifdef DEBUG
		perror(msg.c_str());
	#endif
}