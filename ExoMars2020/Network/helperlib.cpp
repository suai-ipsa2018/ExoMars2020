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

std::array<int, 4> ConfigLoader::defaults{ 8, 24000000, 16, 100 };

ConfigLoader::ConfigLoader(std::string path) : file(path)
{
	std::string line, tmp, node, target, property;
	int n(-1), addr, val;
	size_t l(0);
	if (file)
	{
		while (std::getline(file, line))
		{
			l++;
			if (!line.empty() && !(line = line.substr(line.find_first_not_of(' '), line.find_first_of('#') - line.find_first_not_of(' '))).empty())
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
						if (!la[n][node][0])
							la[n][node][0] = addr;
						else
						{
							if (la[n][node][0] != addr)
								std::cerr << "line " << l << ' ' << "Logical address mismatch for " << node << ", keeping old one: " << la[n][node][0] << std::endl;
						}

						for (int i = 0; i < 4; i++)
							la[n][node][i + 1] = defaults[i];

						while (stream >> tmp)
						{
							stream >> property;
							stream >> tmp;
							if (tmp != "=")
							{
								std::cerr << "line " << l << ' ' << property << " found after property, was waiting for '='" << std::endl;
								exit(-1);
							}
							stream >> val;
							if (property == "fsize") la[n][node][1] = val;
							else if (property == "speed") la[n][node][2] = val;
							else if (property == "psize") la[n][node][3] = val;
							else if (property == "delay_between_packets") la[n][node][4] = val;
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
						stream >> node;
						stream >> target >> target;
						if (!la[n][node][0])
						{
							std::cerr << "line " << l << ' ' << "Logical address for " << node << " not set in network part " << n + 1 << std::endl;
							exit(-1);
						}
						else if (!la[n][target][0])
						{
							std::cerr << "line " << l << ' ' << "Logical address for " << target << " not set in network part " << n + 1 << std::endl;
							exit(-1);
						}
						else
						{
							parts[n].push_back({ la[n][node][0],la[n][target][0] });
						}
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

const std::vector<std::array<int, 2>>& ConfigLoader::get_map(size_t part)
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

const std::map<std::string, std::array<int, 5>>& ConfigLoader::get_la(size_t part)
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