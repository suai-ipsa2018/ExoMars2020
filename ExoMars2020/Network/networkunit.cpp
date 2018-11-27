#include "networkunit.h"


NetworkUnit::NetworkUnit(sc_module_name mn, double speed, bool verbose) :
	sc_module(mn),
	delay_between_bytes(sc_time(1. / speed, SC_SEC)),

	router("router", 32, verbose),
	network_speed(speed)
{
	ConfigLoader cfg("config/Network.cfg");

	std::map<std::string, std::array<int, 5>> addresses = cfg.get_la(NETWORK_PART);
	std::vector<std::array<int, 2>> traffic_map = cfg.get_map(NETWORK_PART);

	tf = sc_create_vcd_trace_file("traces/network");
	sqlite3_open("logs/Network.db", &db);

	instruments.reserve(addresses.size());
	channels.reserve(addresses.size());
	for (auto me : addresses)
	{
		instruments.push_back(std::make_unique<Node>(me.first.c_str(),me.second[0],
			me.second[1], me.second[2], sc_time(1./me.second[3], SC_SEC), sc_time(me.second[4], SC_US), verbose));

		channels.push_back(std::make_unique<io_channel>((me.first + "_channel").c_str()));
		router.connect(*instruments.back(), *channels.back());

		sc_trace(tf, *channels.back(), channels.back()->basename());
		instruments.back()->init_db(db);
	}
	router.connections_done();
	for (std::unique_ptr<Node> &i : instruments)
	{
		for (auto pair : traffic_map)
			if (pair[0] == i->get_logical_address())
				i->add_destination(pair[1]);
	}
}

NetworkUnit::~NetworkUnit()
{
	sqlite3_close(db);
}
