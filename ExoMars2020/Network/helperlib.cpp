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


JsonConfigLoader::JsonConfigLoader(std::string path)
{
	std::ifstream file(path);
	rapidjson::IStreamWrapper isw(file);

	rapidjson::Document d;
	d.ParseStream(isw);
	if (d.HasMember("Channels"))
	{
		channels = { sc_time(1. / d["Channels"]["speed"].GetDouble(), SC_SEC), (size_t)d["Channels"]["error_period"].GetInt() };
	}
	if (d.HasMember("Parts"))
	{
		for (rapidjson::Value::MemberIterator part = d["Parts"].MemberBegin(); part != d["Parts"].MemberEnd(); part++)
		{
			int n = atoi(part->name.GetString());
			if (n > 0)
			{
				if (part->value.HasMember("Nodes"))
				{
					for (rapidjson::Value::MemberIterator node = part->value["Nodes"].MemberBegin();
						node != part->value["Nodes"].MemberEnd();
						node++)
					{
						nodes[n][node->name.GetString()] = {
							node->value["logical_address"].GetInt(),
							(size_t)node->value["fsize"].GetInt(),
							sc_time(1. / node->value["speed"].GetDouble(), SC_SEC)
						};
					}
				}
				if (part->value.HasMember("Connections"))
				{
					connections[n].reserve(part->value["Connections"].GetArray().Size());
					for (auto &desc : part->value["Connections"].GetArray())
					{
						if (!nodes[n][desc["sender"].GetString()].address)
						{
							std::cerr << "JsonConfigLoader: No address found for node " << desc["sender"].GetString() << std::endl;
							exit(-1);
						}
						else if (!nodes[n][desc["sender"].GetString()].address)
						{
							std::cerr << "JsonConfigLoader: No address found for node " << desc["sender"].GetString() << std::endl;
							exit(-1);
						}
						else
						{
							auto to_time = [](const char* s) -> sc_time {
								std::istringstream iss(s);
								double val;
								std::string unit;
								iss >> val >> unit;
								return sc_time(val, unit.c_str());
							};
							connections[n].push_back(
								TransmissionConfig({
								nodes[n][desc["sender"].GetString()].address,
								nodes[n][desc["receiver"].GetString()].address,
								desc["id"].GetInt(),
								std::string(desc["mode"].GetString()) == "write",
								desc["mem_address"].GetInt(),
								(size_t)desc["psize"].GetInt(),
								to_time(desc["delay_between_packets"].GetString()),
								to_time(desc["t_start"].GetString()),
								to_time(desc["t_end"].GetString()),
								(size_t)desc["n_packets"].GetInt()
								})
							);
						}
					}
				}
			}
			else
			{
				std::cerr << "JsonConfigLoader: found " << part->name.GetString() << " for network part" << std::endl;
				exit(-1);
			}
		}
	}
	else
	{
		std::cerr << "JsonConfigLoader: No network part declared" << std::endl;
		exit(-1);
	}

	file.close();
}

const std::vector<TransmissionConfig>& JsonConfigLoader::get_desc(size_t part)
{
	if (part == 0)
	{
		if (!flattenned_descs.size())
		{
			flattenned_descs.reserve(connections.size());
			for (auto &description : connections)
			{
				flattenned_descs.insert(flattenned_descs.end(), description.second.begin(), description.second.end());
			}
		}
		return flattenned_descs;
	}
	else
	{
		return connections[part];
	}
}

const std::map<std::string, NodeConfig>& JsonConfigLoader::get_nodes(size_t part)
{
	if (part == 0)
	{
		if (!flattenned_nodes.size())
		{
			flattenned_descs.reserve(connections.size());
			for (auto &declaration : nodes)
			{
				flattenned_nodes.insert(declaration.second.begin(), declaration.second.end());
			}
		}
		return flattenned_nodes;
	}
	else
	{
		return nodes[part];
	}
}

const ChannelConfig & JsonConfigLoader::get_channels()
{
	return channels;
}
