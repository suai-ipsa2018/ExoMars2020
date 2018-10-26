#pragma once
#include <random>
#include "systemc.h"
#include "io_port.h"
#include "Packet.h"

class Node : public sc_module
{
public:
	io_port port;
protected:
	sc_uint<16> logical_address;
	sc_time delay_between_bytes;
	std::mt19937 rng;
	std::uniform_int_distribution<std::mt19937::result_type> dist{ 0, 127 };

public:
	Node(sc_module_name mn, const sc_uint<16> &_logical_address, sc_time _delay_between_bytes);
	sc_uint<16>& get_logical_address();
protected:
	void send(Packet &p);
	Packet recv();
	sc_time recv(Packet &p);
	unsigned rand();
};

