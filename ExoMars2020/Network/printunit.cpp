#include "printunit.h"

SC_HAS_PROCESS(PrintUnit);
PrintUnit::PrintUnit(sc_module_name mn, double speed) : Node(mn, 32, sc_time(1.0 / speed, SC_SEC))
{
    SC_THREAD(printing_thread);
}

void PrintUnit::printing_thread()
{
    while (true)
    {
        wait(port->data_written_event()); // idles when no data enters
        std::cout << sc_time_stamp() << " " << name() << " printing_thread receiving packet :" << std::endl;
		Packet p;
		sc_time t(recv(p));

		std::cout << p.get_receiver_address() << " " << p.get_sender_address() << std::endl;


		std::cout << "It took " << t.to_seconds() << " to receive this packet °o°" << std::endl;
		std::cout << "\33[48;5;194;38;5;0m" << p << "\33[0m" << std::endl;

        if (p.get_crc())
        {
            std::cout << sc_time_stamp() << " " << name() << " \33[1;38;5;197m" << "WRONG CRC" << "\33[0m" << std::endl;
            sc_spawn(sc_bind(&PrintUnit::send_ack, this, p.get_sender_address(), 0)); // Spawns a thread to send an ack packet with data 0, signaling a transmission error
        }
        else
        {
            std::cout << sc_time_stamp() << " " << name() << " \33[1;38;5;40m" << "CORRECT CRC" << "\33[0m" << std::endl;
            sc_spawn(sc_bind(&PrintUnit::send_ack, this, p.get_sender_address(), 1)); // Spawns a thread to send an ack packet with data 1, signaling a successful transmission
        }
    }
}

void PrintUnit::send_ack(size_t dest, bool state) // Function spawned to send an ack packet (avoids blocking the PrintUnit for this)
{
	Packet p;
	p << dest;
	p << logical_address;
	p << state;
	send(p);
}

std::string PrintUnit::name()
{
    std::string unitname(sc_module::name());
    return "\33[38;5;36m" + unitname + "\33[0m";
}
