#include "Node.h"

SC_HAS_PROCESS(Node);
Node::Node(sc_module_name mn, const sc_uint<16> &_logical_address, sc_time _delay_between_bytes) : sc_module(mn), logical_address(_logical_address), delay_between_bytes(_delay_between_bytes), port((std::string((const char*)mn) + "_port").c_str())
{
	rng.seed(std::random_device()());
}

sc_uint<16>& Node::get_logical_address()
{
	return logical_address;
}

void Node::send(Packet & p)
{
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
	return sc_time_stamp()-t0;
}

unsigned Node::rand()
{
	return dist(rng);
}
