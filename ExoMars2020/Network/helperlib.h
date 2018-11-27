#pragma once
#include <systemc.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <map>
#include <array>
#include <vector>

std::string formatted_time_stamp();


class ConfigLoader
{
public:
	ConfigLoader(std::string path);
	~ConfigLoader();

	const std::vector<std::array<int, 2>>& get_map(size_t part);
	const std::map<std::string, std::array<int, 5>>& get_la(size_t part);

	static std::array<int, 4> defaults;
private:
	std::ifstream file;
	std::array<std::vector<std::array<int, 2>>, 2> parts;
	std::array<std::map<std::string, std::array<int, 5>>, 2> la;

	std::vector<std::array<int, 2>> flatten_parts;
	std::map<std::string, std::array<int, 5>> flatten_la;
};