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
	bool command = p.header[2][6]; // 0 = reply, 1 = command
	bool mode = p.header[2][5]; // 0 = read, 1 = write
	for (size_t i = 0; i < p.header.size(); i++)
	{
		flux << std::setw(5) << p.header[i] << '\t' << p.header[i].to_string(SC_BIN_US) << std::flush;
		if (i == 0) flux << " <- destination address   " << std::endl;
		if (i == 1) flux << " <- protocol identifier   " << std::endl;
		if (i == 2) flux << " <- packet information    " << std::endl;
		if (i == 4) flux << " <- source address        " << std::endl;
		if (i == 5) flux << ' ' << (char)-65 << " transaction id         " << std::endl;
		if (i == 6) flux << ' ' << (char)-39 << "                        " << std::endl;
		if (command)
		{
			if (i == 3) flux << " <- destination key       " << std::endl;
			if (i == 8 || i == 12) flux << ' ' << (char)-65 << "                        " << std::endl;
			if (i == 10) flux << ' ' << (char)-77 << "                        " << std::endl;
			if (i == 11 || i == 14) flux << ' ' << (char)-39 << "                        " << std::endl;
			if (i == 13) flux << ' ' << (char)-77 << " data length            " << std::endl;
			if (mode)
			{
				if (i == 7) flux << " <- extended write address" << std::endl;
				if (i == 9) flux << ' ' << (char)-77 << " write address          " << std::endl;
			}
			else
			{
				if (i == 7) flux << " <- extended read address " << std::endl;
				if (i == 9) flux << ' ' << (char)-77 << " read address        " << std::endl;
			}
		}
		else
		{
			if (i == 3) flux << " <- status                " << std::endl;
			if (!mode)
			{
				if (i == 7) flux << " <- reserved              " << std::endl;
				if (i == 8) flux << ' ' << (char)-65 << "                        " << std::endl;
				if (i == 9) flux << ' ' << (char)-77 << " data length            " << std::endl;
				if (i == 10) flux << ' ' << (char)-39 << "                        " << std::endl;
			}
		}
	}
	flux << std::setw(5) << p.header_checksum << '\t' << p.header_checksum.to_string(SC_BIN_US) << " <- header crc            " << std::endl;
	for (size_t i = 0; i < p.data.size(); i++)
	{
		flux << std::setw(5) << p.data[i] << '\t' << sc_uint<16>(p.data[i]).to_string(SC_BIN_US) << std::flush;
		if (i == 0)
			flux << ' ' << (char)-65 << "                        " << std::endl;
		else if (i == p.data.size() - 1)
			flux << ' ' << (char)-39 << "                        " << std::endl;
		else
		{
			flux << ' ' << (char)-77 << std::flush;
			if (i == p.data.size() / 2)
				flux << " data                   " << std::endl;
			else
				flux << "                        " << std::endl;
		}
	}
	if (!p.data.empty()) flux << std::setw(5) << p.checksum << '\t' << p.checksum.to_string(SC_BIN_US) << " <- CRC                   " << std::endl;
	return flux;
}
