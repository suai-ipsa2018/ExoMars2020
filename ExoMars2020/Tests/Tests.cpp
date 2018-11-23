#include <iostream>
#include <systemc.h>
#include <sqlite3.h>

#include <map>

int sc_main(int argc, char* argv[])
{
	
	std::map<std::string, int> m;

	m["test"] = 5;
	m["test2"] = 6;

	for (auto me : m)
	{
		std::cout << me.first << std::endl;
	}

	return 0;
}

