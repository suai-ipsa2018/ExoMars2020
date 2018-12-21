#include "networkunit.h"


NetworkUnit::NetworkUnit(sc_module_name mn, double speed, bool use_database, bool verbose) :
	sc_module(mn),
	delay_between_bytes(sc_time(1. / speed, SC_SEC)),

	router("router", 32, verbose),
	network_speed(speed)
{
	JsonConfigLoader cfg("config/RMAP.json");

	std::map<std::string, NodeConfig> addresses = cfg.get_nodes(NETWORK_PART);
	std::vector<TransmissionConfig> traffic_desc = cfg.get_desc(NETWORK_PART);
	ChannelConfig channels_cfg = cfg.get_channels();

	tf = sc_create_vcd_trace_file(("traces/network_" + std::to_string(NETWORK_PART)).c_str());
	if (use_database) sqlite3_open(("logs/Network_" + std::to_string(NETWORK_PART) + ".db").c_str(), &db);

	instruments.reserve(addresses.size());
	channels.reserve(addresses.size());
	for (auto me : addresses)
	{
		instruments.push_back(std::make_unique<Node>(me.first.c_str(), me.second, verbose));

		channels.push_back(std::make_unique<io_channel>((me.first + "_channel").c_str(), channels_cfg.transmission_time, channels_cfg.error_period));
		router.connect(*instruments.back(), *channels.back());

		sc_trace(tf, *channels.back(), channels.back()->basename());
		if (use_database) instruments.back()->init_db(db);
	}
	router.connections_done();
	for (std::unique_ptr<Node> &i : instruments)
	{
		for (TransmissionConfig& cfg : traffic_desc)
			if (cfg.sender_address == i->get_logical_address())
				i->add_transmission(cfg);
	}
}

NetworkUnit::~NetworkUnit()
{
	sqlite3_close(db);
}
