#include "helperlib.h"

std::string formatted_time_stamp()
{
	unsigned short fs(0), ps(0), ns(0), us(0), ms(0), s(0), m(0), h(0);
	uint64 val = (uint64)(sc_time_stamp().value()*(1e-12 / sc_get_time_resolution().to_seconds()));
	ps = val % 1000; val /= 1000;
	ns = val % 1000; val /= 1000;
	us = val % 1000; val /= 1000;
	ms = val % 1000; val /= 1000;
	s = val % 60; val /= 60;
	h = val % 60; val /= 60;

	std::string tstr;
	if (h) tstr += std::to_string(h) + "h ";
	if (m) tstr += std::to_string(m) + "m ";
	if (s) tstr += std::to_string(s) + "s ";
	if (ms) tstr += std::to_string(ms) + "ms ";
	if (us) tstr += std::to_string(us) + "us ";
	if (ns) tstr += std::to_string(ns) + "ns ";
	if (ps) tstr += std::to_string(ps) + "ps ";

	if (tstr.empty()) tstr += "0s ";

	return tstr;
}

const NodeConfig ConfigLoader::node_defaults{0, 8, sc_time(1./24e6, SC_SEC), 16};
const TransmissionConfig ConfigLoader::transmission_defaults{ 0, 0, sc_time(100, SC_US), sc_time(0, SC_SEC), sc_time(-1, SC_SEC), SIZE_MAX };

ConfigLoader::ConfigLoader(std::string path) : file(path)
{
	std::string line, tmp, node, target, property_type;
	int n(-1), addr, val;
	size_t l(0);
	if (file)
	{
		while (std::getline(file, line))
		{
			l++;
			if (!line.empty() && line.find_first_not_of(' ') != line.npos && !(line = line.substr(line.find_first_not_of(' '), line.find_first_of('#') - line.find_first_not_of(' '))).empty())
			{
				std::stringstream stream(line);
				if (line.find("--") != line.npos)
				{
					stream >> tmp >> tmp;
					if (tmp != "Part")
					{
						std::cerr << "line " << l << ' ' << "'Part n' expected after '--', found " << tmp << std::endl;
						exit(-1);
					}
					stream >> n;
					if (n < 1)
					{
						std::cerr << "line " << l << ' ' << "Parts of network must be positive integers, found " << n << std::endl;
						exit(-1);
					}
					else
						n--; // Convert network part to array index
				}
				else if (line.find(':') != line.npos)
				{
					if (n >= 0)
					{
						stream >> node;
						stream >> tmp >> addr;
						la[n][node] = node_defaults;
						if (!la[n][node].address)
							la[n][node].address = addr;
						else
						{
							if (la[n][node].address != addr)
								std::cerr << "line " << l << ' ' << "Logical address mismatch for " << node << ", keeping old one: " << la[n][node].address << std::endl;
						}
						stream >> tmp;

						while (stream >> property_type)
						{
							stream >> tmp;
							if (tmp != "=")
							{
								std::cerr << "line " << l << ' ' << tmp << " found after property, was waiting for '='" << std::endl;
								exit(-1);
							}
							stream >> val;
							if (property_type == "fsize")
							{
								la[n][node].fsize = val;
								stream >> tmp;
							}
							else if (property_type == "speed")
							{
								la[n][node].delay_between_bytes = sc_time(1. / val, SC_SEC);
								stream >> tmp;
							}
							else if (property_type == "psize")
							{
								la[n][node].psize = val;
								stream >> tmp;
							}
							else
							{
								std::cerr << "line " << l << ' ' << "Unknown property " << tmp << std::endl;
								exit(-1);
							}
						}
					}
					else
					{
						std::cerr << "line " << l << ' ' << "Network part not specified before instrument declaration" << std::endl;
						exit(-1);
					}
				}
				else if (line.find("->") != line.npos)
				{
					if (n >= 0)
					{
						TransmissionConfig cfg(transmission_defaults);
						stream >> node;
						stream >> target >> target;
						if (target[target.size() - 1] == ',') target = target.substr(0, target.length() - 1);
						if (!la[n][node].address)
						{
							std::cerr << "line " << l << ' ' << "Logical address for " << node << " not set in network part " << n + 1 << std::endl;
							exit(-1);
						}
						else if (!la[n][target].address)
						{
							std::cerr << "line " << l << ' ' << "Logical address for " << target << " not set in network part " << n + 1 << std::endl;
							exit(-1);
						}
						else
						{
							cfg.sender_address = la[n][node].address;
							cfg.receiver_address = la[n][target].address;
						}

						while (stream >> property_type)
						{
							stream >> tmp;
							if (tmp != "=")
							{
								std::cerr << "line " << l << ' ' << tmp << " found after property, was waiting for '='" << std::endl;
								exit(-1);
							}
							stream >> val;

							if (property_type == "delay_between_packets")
							{
								std::string unit;
								stream >> unit;
								if (unit[unit.length() - 1] == ',') unit = unit.substr(0, unit.length() - 1);
								cfg.delay_between_packets = sc_time(val, unit.c_str());
							}
							else if (property_type == "t_start")
							{
								std::string unit;
								stream >> unit;
								if (unit[unit.length() - 1] == ',') unit = unit.substr(0, unit.length() - 1);
								cfg.t_start = sc_time(val, unit.c_str());
							}
							else if (property_type == "t_end")
							{
								std::string unit;
								stream >> unit;
								if (unit[unit.length() - 1] == ',') unit = unit.substr(0, unit.length() - 1);
								cfg.t_end = sc_time(val, unit.c_str());
							}
							else if (property_type == "n_packets")
							{
								cfg.n_packets = val;
								stream >> tmp;
							}
						}
						parts[n].push_back(cfg);
					}
					else
					{
						std::cerr << "line " << l << ' ' << "Network part not specified before communication declaration" << std::endl;
						exit(-1);
					}
				}
				else
				{
					std::cerr << "line " << l << ' ' << "Unknown declaration : " << line << std::endl;
					exit(-1);
				}
			}
		}
	}
	else
		std::cerr << "File not found at " << path << std::endl;
}

ConfigLoader::~ConfigLoader()
{
	file.close();
}

const std::vector<TransmissionConfig>& ConfigLoader::get_desc(size_t part)
{
	if (part == 1)
		return parts[0];
	else if (part == 2)
		return parts[1];
	else
	{
		flatten_parts.reserve(parts[0].size() + parts[1].size());
		flatten_parts.insert(flatten_parts.end(), parts[0].begin(), parts[0].end());
		flatten_parts.insert(flatten_parts.end(), parts[1].begin(), parts[1].end());
		return flatten_parts;
	}
}

const std::map<std::string, NodeConfig>& ConfigLoader::get_la(size_t part)
{
	if (part == 1)
		return la[0];
	else if (part == 2)
		return la[1];
	else
	{
		flatten_la.insert(la[0].begin(), la[0].end());
		flatten_la.insert(la[1].begin(), la[1].end());
		return flatten_la;
	}
}