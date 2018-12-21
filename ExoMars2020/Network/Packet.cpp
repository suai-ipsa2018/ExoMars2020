#include "Packet.h"

const sc_uint<16> Packet::EOP(-1);

Packet::Packet() : header(header_size) {}

Packet& Packet::operator<<(const sc_uint<16> & f)
{
	if (i < header_size)
	{
		header[i] = f;
		header_checksum = crc<16>(header_checksum, f);
	}
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
	if (i < header_size) f = header[i];
	else if (i == header_size) f = crc<16>(header_checksum, 0);
	else if (header_size + 1 <= i && i < header_size + data.size()) f = data[i - 1 - header_size];
	else if (i == header_size + 1 + data.size()) { f = crc<16>(checksum, 0); }
	else if (i == header_size + data.size() + 2) { f = EOP; }

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


size_t Packet::size()
{
	return data.size();
}

void Packet::reset() { i = 0; }

ostream& operator<<(ostream &flux, Packet &p)
{
	flux << std::setw(5) << p.destination_address() << '\t' << p.destination_address().to_string(SC_BIN_US) << " <- destination address" << std::endl;
	flux << std::setw(5) << p.source_address() << '\t' << p.source_address().to_string(SC_BIN_US) << " <- source address" << std::endl;
	for (size_t i = 2; i < p.header.size(); i++)
	{
		flux << std::setw(5) << p.header[i] << '\t' << sc_uint<16>(p.header[i]).to_string(SC_BIN_US) << std::endl;
	}
	flux << std::setw(5) << crc<16>(p.header_checksum, 0) << '\t' << sc_uint<16>(crc<16>(p.header_checksum, 0)).to_string(SC_BIN_US) << std::endl;
	for (size_t i = 0; i < p.data.size(); i++)
	{
		flux << std::setw(5) << p.data[i] << '\t' << sc_uint<16>(p.data[i]).to_string(SC_BIN_US) << std::flush;
		if (p.data.size() > 1)
			if (i == 0)
				flux << ' ' << (char)-65 << "      " << std::endl;
			else if (i == p.data.size() - 1)
				flux << ' ' << (char)-39 << "      " << std::endl;
			else
			{
				flux << ' ' << (char)-77 << std::flush;
				if (i == p.data.size() / 2)
					flux << " data " << std::endl;
				else
					flux << "      " << std::endl;
			}
		else
			flux << " <- ack state" << std::endl;
	}
	flux << std::setw(5) << crc<16>(p.checksum, 0) << '\t' << crc<16>(p.checksum, 0).to_string(SC_BIN_US) << " <- CRC " << std::endl;
	return flux;
}
