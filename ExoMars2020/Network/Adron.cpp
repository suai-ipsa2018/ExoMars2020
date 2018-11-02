#include "Adron.h"


SC_HAS_PROCESS(Adron);
Adron::Adron(size_t logical_address, size_t psize, size_t bit, double speed, sc_time delay_between_bytes, bool verbose_, sc_module_name mn) :
	Node(mn, logical_address, psize, delay_between_bytes, bit), verbose(verbose_)
{
	SC_THREAD(printing_thread);
	SC_THREAD(gen_thread);
}

void Adron::printing_thread()
{
	while (true)
	{
		wait(port->data_written_event()); // idles when no data enters
		std::cout << sc_time_stamp() << " " << name() << " printing_thread receiving packet :" << std::endl;
		Packet p;
		sc_time t(recv(p));

		std::cout << "It took " << t.to_seconds() << " to receive the packet sent by " << p.get_sender_address() << std::endl;
		if (verbose) std::cout << "\33[48;5;194;38;5;0m" << p << "\33[0m" << std::endl;

		if (p.get_crc())
		{
			std::cout << sc_time_stamp() << " " << name() << " \33[1;38;5;197m" << "WRONG CRC" << "\33[0m" << std::endl;
			sc_spawn(sc_bind(&Adron::send_ack, this, p.get_sender_address(), 0)); // Spawns a thread to send an ack packet with data 0, signaling a transmission error
		}
		else
		{
			std::cout << sc_time_stamp() << " " << name() << " \33[1;38;5;40m" << "CORRECT CRC" << "\33[0m" << std::endl;
			sc_spawn(sc_bind(&Adron::send_ack, this, p.get_sender_address(), 1)); // Spawns a thread to send an ack packet with data 1, signaling a successful transmission
		}
	}
}

void Adron::gen_thread()
{
	while (true)
	{
		Packet p;
		p << 32 << logical_address;
		if (verbose) std::cout << "psize_" << name() << " = " << psize << std::endl;
		for (size_t i = 0; i < psize; i++)
			p << rand();

		send_with_ack(p, verbose);

		wait(100, SC_US);
	}
}

std::string Adron::name()
{
	std::string unitname(sc_module::name());
	return "\33[38;5;36m" + unitname + "\33[0m";
}
