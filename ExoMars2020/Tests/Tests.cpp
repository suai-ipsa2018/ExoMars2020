#include <iostream>
#include <systemc.h>
#include "example.h"


int sc_main(int argc, char* argv[])
{
	for (char c = CHAR_MIN; c < CHAR_MAX; c++)
	{
		std::cout << (int)c << ' ' << c << std::endl;
	}

	return 0;
}

