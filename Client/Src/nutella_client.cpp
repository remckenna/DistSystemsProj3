#include "NutellaFile.h"
#include "MultiCastServer.h"
#include <iostream>
#include <string>

using namespace std;
using std::string;

int main(int argc, char** argv)
{
	NutellaFile file;
	MultiCastServer server("239.0.0.1", 8953);

	string test = "hello!";
	string command = "r";
	char buffer[200];
	cin >> command;

	if(command == "s")
	{
		server.Send(test.c_str(), test.size());
	}
	else
	{
		server.Receive(buffer, 200);
	}
	

	//server.Receive(&buffer, 200);

	cout << string(buffer) << endl;
	server.Shutdown();

	file.Load("test.nutella");
	file.Print();
	return 0;
}