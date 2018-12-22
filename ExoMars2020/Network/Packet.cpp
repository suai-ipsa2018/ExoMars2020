#include "Packet.h"

const sc_uint<16> Packet::EOP(-1);

Packet::Packet(size_t _header_size) : header_size(_header_size), header(_header_size) {}

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
		{
			if (data.size()) data.pop_back();
		}
	}
	i++;
	return *this;
}

Packet& Packet::operator>>(sc_uint<16> &f)
{
	if (i < header_size) f = header[i];
	else if (i == header_size) f = crc<16>(header_checksum, 0);
	else if (header_size + 1 <= i && i < header_size + 1 + data.size()) f = data[i - (header_size + 1)];
	else if (data.size() && i == header_size + 1 + data.size()) { f = crc<16>(checksum, 0); }
	else { f = EOP; }
	i++;
	return *this;
}

Packet::operator bool()
{
	return (i == header_size + data.size() + 4 - (data.size() ? 0 : 1)) ? false : true;
}

sc_uint<16>& Packet::operator[](size_t index)
{
	return data[index];
}


void Packet::receive_header_crc(sc_uint<16> byte)
{
	header_checksum = crc<16>(header_checksum, byte);
	header_size = i;
	header.resize(header_size);
	i++;
}

size_t Packet::size()
{
	return data.size();
}

void Packet::reset() { i = 0; }

ostream& operator<<(ostream &flux, Packet &p)
{
	for (size_t i = 0; i < p.header.size(); i++)
	{
		flux << std::setw(5) << p.header[i] << '\t' << p.header[i].to_string(SC_BIN_US) << std::endl;
	}
	flux << std::setw(5) << p.header_checksum << '\t' << p.header_checksum.to_string(SC_BIN_US) << std::endl;
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
	if (!p.data.empty()) flux << std::setw(5) << p.checksum << '\t' << p.checksum.to_string(SC_BIN_US) << " <- CRC " << std::endl;
	return flux;
}
