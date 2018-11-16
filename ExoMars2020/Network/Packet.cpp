#include "Packet.h"

const sc_uint<16> Packet::EOP(-1);
const size_t Packet::header_size(2);

Packet::Packet() {}

Packet& Packet::operator<<(const sc_uint<16> & f)
{
	if (i == 0) receiver_address = f;
	else if (i == 1) sender_address = f;
	else
	{
		if (!f.and_reduce())
		{
			data.push_back(f);
			checksum = crc<16>(checksum, f);
		}
		else
			data.pop_back();
	}
	i++;
	return *this;
}

Packet& Packet::operator>>(sc_uint<16> &f)
{
	if (i == 0) f = receiver_address;
	else if (i == 1) f = sender_address;
	else if (header_size <= i && i < header_size + data.size()) f = data[i - header_size];
	else if (i == header_size + data.size()) { f = crc<16>(checksum, 0); }
	else if (i == header_size + data.size() + 1) { f = EOP; }

	i++;
	return *this;
}

Packet::operator bool()
{
	return (i == header_size + data.size() + 3) ? false : true;
}

sc_uint<16>& Packet::operator[](size_t index)
{
	return data[index];
}

void Packet::set_receiver_address(const sc_uint<16>& addr)
{
	if (32 <= addr && addr < 256)
		receiver_address = addr;
	else
	{
		std::cerr << "Address must be between 32 and 255, got " << addr << std::endl;
		exit(1);
	}
}

void Packet::set_sender_address(const sc_uint<16>& addr)
{
	if (32 <= addr && addr < 256)
		sender_address = addr;
	else
	{
		std::cerr << "Address must be between 32 and 255, got " << addr << std::endl;
		exit(1);
	}
}


size_t Packet::size()
{
	return data.size();
}

void Packet::reset() { i = 0; }

ostream& operator<<(ostream &flux, Packet &p)
{
	flux << "receiver address: " << p.receiver_address << std::endl;
	flux << "sender address: " << p.sender_address << std::endl;
	for (const sc_uint<16>& byte : p.data)
		flux << std::setw(5) << byte << "\t" << byte.to_string(SC_BIN_US) << std::endl;
	flux << std::setw(5) << p.checksum << '\t' << p.checksum.to_string(SC_BIN_US) << " <- CRC" << std::endl;
	return flux;
}
