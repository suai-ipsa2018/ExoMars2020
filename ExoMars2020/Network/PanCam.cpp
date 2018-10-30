#include "PanCam.h"


SC_HAS_PROCESS(PanCam);
PanCam::PanCam(size_t logical_address, size_t psize, sc_time delay_between_bytes) :
	Node("PanCam", logical_address, psize, delay_between_bytes)
{
	SC_THREAD(gen_thread);
}


void PanCam::gen_thread()
{
	while (true)
	{
		Packet p;
		p << 32 << logical_address;
		std::cout << "psize_" << name() << " = " << psize << std::endl;
		for (size_t i = 0; i < psize; i++)
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