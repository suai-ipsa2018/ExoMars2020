#pragma once
#include "systemc.h"
#include "crc.h"

class Packet
{
	sc_uint<16> receiver_address;
	sc_uint<16> sender_address;
	sc_uint<16> checksum{ 0 };
	static const sc_uint<16> EOP;

	static const size_t header_size;

	size_t i{ 0 };

public:
	std::vector<sc_uint<16>> data;

	Packet();

	Packet& operator<<(const sc_uint<16> &f);
	Packet& operator>>(sc_uint<16> &f);
	explicit operator bool();
	sc_uint<16>& operator[](size_t index);

	sc_uint<16>& get_receiver_address() { return receiver_address; }
	sc_uint<16>& get_sender_address() { return sender_address; }
	sc_uint<16>& get_crc() { return checksum; }

	friend ostream& operator<<(ostream& flux, Packet &p);

	void reset();
};