#pragma once
#include <systemc.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <map>
#include <array>
#include <vector>

std::string formatted_time_stamp();


struct NodeConfig
{
	sc_uint<16> address;
	size_t fsize;
	sc_time delay_between_bytes;
	size_t psize;
};

struct TransmissionConfig
{
	sc_uint<16> sender_address;
	sc_uint<16> receiver_address;
	sc_time delay_between_packets;
	sc_time t_start;
	sc_time t_end;
	size_t n_packets;
};

struct ChannelConfig
{
	sc_time transmission_time;
	size_t error_period;
};

class ConfigLoader
{
public:
	ConfigLoader(std::string path);
	~ConfigLoader();

	const std::vector<TransmissionConfig>& get_desc(size_t part);
	const std::map<std::string, NodeConfig>& get_la(size_t part);

	static NodeConfig node_defaults;
	static ChannelConfig channel_defaults;
	static TransmissionConfig transmission_defaults;
private:
	std::ifstream file;
	std::array<std::vector<TransmissionConfig>, 2> parts;
	std::array<std::map<std::string, NodeConfig>, 2> la;

	std::vector<TransmissionConfig> flatten_parts;
	std::map<std::string, NodeConfig> flatten_la;

	ChannelConfig channels;
};