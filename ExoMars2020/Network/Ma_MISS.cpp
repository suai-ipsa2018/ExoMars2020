#include "Ma_MISS.h"


SC_HAS_PROCESS(Ma_MISS);
Ma_MISS::Ma_MISS(size_t logical_address, size_t psize, sc_time delay_between_bytes, bool verbose_, sc_module_name mn) :
	Node(mn, logical_address, psize, delay_between_bytes), verbose(verbose_)
{
	SC_THREAD(gen_thread);
}


void Ma_MISS::gen_thread()
{
	while (true)
	{
		Packet p;
		p << 32 << logical_address;
		if (verbose) std::cout << "psize_" << name() << " = " << psize << std::endl;

		for (size_t i = 0; i < psize; i++)
			p << rand();


		Packet ack;
		do
		{
			send(p);

			recv(ack);
			if (verbose) std::cout << "ack received: " << std::endl << ack << std::endl;
		} while (!ack[0]);
		wait(100, SC_US);
	}
}