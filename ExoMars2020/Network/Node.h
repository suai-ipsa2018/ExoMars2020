#pragma once
#include <random>
#include "systemc.h"
#include "io_port.h"
#include "Packet.h"
#include "helperlib.h"

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

	ofstream logfile;

public:
	Node(sc_module_name mn, const sc_uint<16> &_logical_address, const size_t &_psize, sc_time _delay_between_bytes, size_t _bit = 8, bool _verbose = false);
	virtual ~Node();
  
	sc_uint<16>& get_logical_address();
protected:
	void send(Packet &p);
	void send_with_ack(Packet &p);
	sc_time recv(Packet &p);
	unsigned rand();
	bool verbose;
};

