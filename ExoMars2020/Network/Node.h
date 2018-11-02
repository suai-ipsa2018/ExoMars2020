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
	size_t psize;
	size_t bit;
	std::mt19937 rng;
	std::uniform_int_distribution<std::mt19937::result_type> dist{ 0, (int)(pow(2,Node::bit) - 1) };

public:
	Node(sc_module_name mn, const sc_uint<16> &_logical_address, const size_t &_psize, sc_time _delay_between_bytes, size_t _bit = 8);
	sc_uint<16>& get_logical_address();
protected:
	void send(Packet &p);
	void send_ack(size_t address, bool state);
	void send_with_ack(Packet &p, bool verbose = false);
	Packet recv();
	sc_time recv(Packet &p);
	unsigned rand();
};

