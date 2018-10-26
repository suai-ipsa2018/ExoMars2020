#include <iostream>
#include "systemc.h"
#include "networkunit.h"
#include "switchunit.h"

int sc_main(int argc, char *argv[])
{
    // Written to remove warnings about unused parameter argc and argv, but we don't use command line arguments...
    for (int i=0 ; i<argc ; i++) // Goes through potential command line arguments and print them (usually only the name of the file)
        std::cout << argv[i] << " ";
    std::cout << std::endl;
	
    // Actual program
    NetworkUnit nu("nu", 10, 24e6);
    sc_start(300, SC_US); // Starts simulation for 300 us

    return 0;
}

