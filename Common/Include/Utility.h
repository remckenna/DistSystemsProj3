#ifndef __UTILITY_H__
#define __UTILITY_H__

#define DEBUG

#include <string>

using std::string;

namespace Utility {

	void PrintError(string msg);

	void PrintDebugMessage(string msg);
};

#endif