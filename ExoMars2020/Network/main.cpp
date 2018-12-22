#include <iostream>
#include <systemc.h>
#include "networkunit.h"
#include "switchunit.h"

int sc_main(int argc, char *argv[])
{
	clock_t c_start = clock();

	bool verbose(false);
	bool use_database(false);
	sc_time t_sim(SC_ZERO_TIME);
	for (int i = 1; i < argc; i++)
	{
		if (std::string(argv[i]) == "-v" || std::string(argv[i]) == "--verbose") verbose = true;
		if (std::string(argv[i]) == "-d" || std::string(argv[i]) == "--use-databases") use_database = true;
		if (std::string(argv[i]).substr(0, 2) == "-t")
		{
			std::istringstream iss(argv[i] + 3);
			double val;
			std::string unit;
			iss >> val >> unit;
			t_sim = sc_time(val, unit.c_str());
		}
	}
	
    NetworkUnit nu("nu", 24e6, use_database, verbose);
    sc_start((t_sim == SC_ZERO_TIME) ? t_sim : sc_max_time()); // Starts simulation
	std::cout << "\33[38;5;208m -- End of simulation of part " << NETWORK_PART << " --\33[0m" << std::endl;
	std::cout << "Time of execution : " << (double)(clock() - c_start) / CLOCKS_PER_SEC << std::endl;
    return 0;

	// set CL = /DNETWORK_PART#2
	// msbuild Network.vcxproj /p:Configuration=Release;Platform=x86;TargetName=../Release/part1
}

