#include "Node.h"

SC_HAS_PROCESS(Node);
Node::Node(sc_module_name mn, const sc_uint<16> &_logical_address, const size_t & _psize, sc_time _delay_between_bytes, size_t _bit, bool _verbose)
	: sc_module(mn), logical_address(_logical_address), psize(_psize), delay_between_bytes(_delay_between_bytes), bit(_bit), verbose(_verbose),
	port((std::string((const char*)mn) + "_port").c_str()),
	logfile("logs/" + (std::string((const char*)mn) + ".log"))
{
	rng.seed(std::random_device()());
	logfile << "Node " << name() << " with logic address " << logical_address << ":" << std::endl;
}

Node::~Node()
{
	logfile.close();
}

sc_uint<16>& Node::get_logical_address()
{
	return logical_address;
}

void Node::send(Packet & p)
{
	send_mutex.lock();
	logfile << formatted_time_stamp() << ' ' << name() << " sending packet of size " << p.size() << " to " << p.get_receiver_address() << std::endl;
	if (verbose) logfile << p << std::endl;
	p.reset();
	sc_uint<16> tmp;
	while (p >> tmp)
	{
		port.write(tmp);
		wait(delay_between_bytes);
	}
	send_mutex.unlock();
}

void Node::send_ack(size_t dest, bool state) // Function spawned to send an ack packet (avoids blocking the PrintUnit for this)
{
	Packet p;
	p << dest << logical_address << state;
	send(p);
}

void Node::send_with_ack(Packet &p)
{
	Packet ack;
	do
	{
		send(p);

		recv(ack);
		if (verbose) std::cout << "ack received: " << std::endl << ack << std::endl;
		logfile << "ack received: " << std::endl << ack << std::endl;
	} while (!ack[0]);
}

sc_time Node::recv(Packet & p)
{
	recv_mutex.lock();
	p.reset();
	sc_uint<16> tmp(0);
	sc_time t0(sc_time_stamp());
	while (!tmp.and_reduce())
	{
		tmp = port.read();
		p << tmp;
	}
	sc_time t1(sc_time_stamp());


	if (p.size() > 1)
	{
		logfile << formatted_time_stamp() << ' ' << name() << " received packet of size " << p.size() << " from " << p.get_sender_address() << " in " << (t1 - t0).to_seconds() << "s" << std::endl;
		if (verbose) logfile << p << std::endl;
		std::cout << sc_time_stamp() << ' ' << name() << " received packet of size " << p.size() << " from " << p.get_sender_address() << " in " << (t1 - t0).to_seconds() << "s" << std::endl;
		if (verbose) std::cout << "\33[48;5;194;38;5;0m" << p << "\33[0m" << std::endl;

		if (p.get_crc())
		{
			std::cout << sc_time_stamp() << " " << name() << " \33[1;38;5;197m" << "WRONG CRC" << "\33[0m" << std::endl;
			sc_spawn(sc_bind(&Node::send_ack, this, p.get_sender_address(), 0)); // Spawns a thread to send an ack packet with data 0, signaling a transmission error
		}
		else
		{
			std::cout << sc_time_stamp() << " " << name() << " \33[1;38;5;40m" << "CORRECT CRC" << "\33[0m" << std::endl;
			sc_spawn(sc_bind(&Node::send_ack, this, p.get_sender_address(), 1)); // Spawns a thread to send an ack packet with data 1, signaling a successful transmission
		}
	}
	recv_mutex.unlock();
	return t1-t0;
}

unsigned Node::rand()
{
	return dist(rng);
}
