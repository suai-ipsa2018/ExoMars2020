#pragma once
#include <random>
#include <sstream>
#include <sqlite3.h>

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>
#include "io_port.h"
#include "Packet.h"
#include "helperlib.h"

#ifndef NETWORK_PART
#define NETWORK_PART 0
#endif

class Node : public sc_module
{
public:
	io_port port;
protected:
	NodeConfig cfg;

	std::mt19937 rng;
	std::uniform_int_distribution<std::mt19937::result_type> dist;

	ofstream logfile;
	sqlite3* db{ NULL };
	bool verbose;
private:
	sc_mutex send_mutex, recv_mutex;
	sc_event packet_reception, reply_reception;

	std::vector<Packet> reply_queue;
	std::vector<TransmissionConfig> transmissions;
	std::map<sc_uint<16>, std::vector<sc_uint<16>>> mem;
public:
	Node(sc_module_name mn, const NodeConfig& _cfg, bool _verbose = false);
	virtual ~Node();
  
	sc_uint<16>& get_logical_address();
	void add_transmission(const TransmissionConfig& c);
	void init_db(sqlite3* _db);
protected:
	void send(Packet &p);
	unsigned rand();
private:
	void send_raw(Packet &p);
	sc_time recv_raw(Packet &p);
	void receiver_daemon();
	void sending_daemon(const TransmissionConfig& c);
};
