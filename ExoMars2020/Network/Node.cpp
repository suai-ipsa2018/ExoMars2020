#include "Node.h"

SC_HAS_PROCESS(Node);
Node::Node(sc_module_name mn, const sc_uint<16> &_logical_address, const size_t & _psize, sc_time _delay_between_bytes) 
	: sc_module(mn), logical_address(_logical_address), psize(_psize), delay_between_bytes(_delay_between_bytes),
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
	logfile << formatted_time_stamp() << "Sending packet of size " << p.size() << " to " << p.get_receiver_address() << std::endl;
	p.reset();
	sc_uint<16> tmp;
	while (p >> tmp)
	{
		port.write(tmp);
		wait(delay_between_bytes);
	}
}

Packet Node::recv()
{
	sc_uint<16> tmp(0);
	Packet p;
	while (!tmp.and_reduce())
	{
		tmp = port.read();
		p << tmp;
	}
	logfile << formatted_time_stamp() << "Received packet of size " << p.size() << " from " << p.get_sender_address() << std::endl;
	return p;
}

sc_time Node::recv(Packet & p)
{
	p.reset();
	sc_uint<16> tmp(0);
	sc_time t0(sc_time_stamp());
	while (!tmp.and_reduce())
	{
		tmp = port.read();
		p << tmp;
	}
	logfile << formatted_time_stamp() << "Received packet of size " << p.size() << " from " << p.get_sender_address() << std::endl;
	return sc_time_stamp()-t0;
}

unsigned Node::rand()
{
	return dist(rng);
}
