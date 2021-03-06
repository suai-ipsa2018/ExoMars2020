#pragma once
#include <iomanip>
#include <systemc.h>
#include "crc.h"

class Packet
{
	static const sc_uint<16> EOP;

	size_t header_size;

	sc_uint<16> header_checksum{ 0 };
	sc_uint<16> checksum{ 0 };
	size_t i{ 0 };

public:
	std::vector<sc_uint<16>> header;
	std::vector<sc_uint<16>> data;

	Packet(size_t _header_size = 15);

	Packet& operator<<(const sc_uint<16> &f);
	Packet& operator>>(sc_uint<16> &f);
	explicit operator bool();
	sc_uint<16>& operator[](size_t index);

	const sc_uint<16>& destination_address() const { return header[0]; }
	const sc_uint<16>& source_address() const { return header[4]; }
	sc_uint<16> get_crc() { return checksum; }
	sc_uint<16> get_header_crc() { return header_checksum; }

	void receive_header_crc(sc_uint<16> byte);

	size_t size();

	friend ostream& operator<<(ostream& flux, Packet &p);

	void reset();
};
