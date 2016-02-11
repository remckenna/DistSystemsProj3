#include "NutellaFile.h"
#include "MultiCastServer.h"
#include <iostream>
#include <string>

using namespace std;
using std::string;

int main(int argc, char** argv)
{
	NutellaFile file;
	MultiCastServer server("239.0.2.1", 8953);

	string test;
	char buffer[200];
	cin >> test;

	server.Send(test.c_str(), test.size());

	server.Receive(&buffer, 200);

	cout << string(buffer) << endl;
	server.Shutdown();

	file.Load("test.nutella");
	file.Print();
	return 0;
}