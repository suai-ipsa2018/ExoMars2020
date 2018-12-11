#pragma once
#include <systemc.h>
#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <map>
#include <array>
#include <vector>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>

std::string formatted_time_stamp();

struct NodeConfig
{
	sc_uint<16> address;
	size_t fsize;
	sc_time delay_between_bytes;
};

struct TransmissionConfig
{
	sc_uint<16> sender_address;
	sc_uint<16> receiver_address;
	size_t psize;
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


class JsonConfigLoader
{
public:
	JsonConfigLoader(std::string path);

	const std::vector<TransmissionConfig>& get_desc(size_t part);
	const std::map<std::string, NodeConfig>& get_nodes(size_t part);
	const ChannelConfig& get_channels();

private:
	std::map<int, std::vector<TransmissionConfig>> descriptions;
	std::map<int, std::map<std::string, NodeConfig>> declarations;

	std::vector<TransmissionConfig> flattenned_descs;
	std::map<std::string, NodeConfig> flattenned_nodes;

	ChannelConfig channels;
};