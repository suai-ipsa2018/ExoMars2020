#include "Ma_MISS.h"


SC_HAS_PROCESS(Ma_MISS);
Ma_MISS::Ma_MISS(size_t logical_address, sc_time delay_between_bytes, sc_module_name mn) :
	Node(mn, logical_address, delay_between_bytes)
{
	SC_THREAD(gen_thread);
}


void Ma_MISS::gen_thread()
{
	while (true)
	{
		Packet p;
		p << 32 << logical_address;
		for (size_t i = 0; i < 25; i++)
			p << rand();


		Packet ack;
		do
		{
			send(p);

			recv(ack);
			std::cout << "ack received: " << std::endl << ack << std::endl;
		} while (!ack[0]);
		wait(100, SC_US);
	}
}