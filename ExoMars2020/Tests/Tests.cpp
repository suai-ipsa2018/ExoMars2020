#include <iostream>
#include <systemc.h>


int sc_main(int argc, char* argv[])
{
	std::vector<int> v = { 0,1,2,3,4,5,6,7,8,9 };
	std::cout << v.size() << std::endl;

	v.erase(v.begin() + 5);
	std::cout << v.size() << std::endl;

	return 0;
}
