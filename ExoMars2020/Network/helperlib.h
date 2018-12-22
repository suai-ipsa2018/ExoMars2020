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
	sc_uint<16> id;
	sc_uint<16> mode;
	sc_uint<16> mem_address;
	size_t psize;
	sc_time delay_between_packets;
	sc_time t_start;
	sc_time t_end;
	size_t n_packets;
};

struct GenerationConfig
{
	sc_uint<16> generator;
	sc_uint<16> mem_address;
	size_t dsize;
	sc_time delay_between_generation;
	sc_time t_start;
	sc_time t_end;
	size_t n_generations;
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

	const std::vector<TransmissionConfig>& get_connections(size_t part);
	const std::vector<GenerationConfig>& get_generations(size_t part);
	const std::map<std::string, NodeConfig>& get_nodes(size_t part);
	const ChannelConfig& get_channels();

private:
	std::map<int, std::vector<TransmissionConfig>> connections;
	std::map<int, std::vector<GenerationConfig>> generations;
	std::map<int, std::map<std::string, NodeConfig>> nodes;

	std::vector<TransmissionConfig> flattenned_connections;
	std::vector<GenerationConfig> flattenned_generations;
	std::map<std::string, NodeConfig> flattenned_nodes;

	ChannelConfig channels;
};