#include "Node.h"

SC_HAS_PROCESS(Node);
Node::Node(sc_module_name mn, const NodeConfig& _cfg, bool _verbose)
	: sc_module(mn), cfg(_cfg), verbose(_verbose),
	dist(0, (int)(pow(2,_cfg.fsize) - 1)),
	port((std::string((const char*)mn) + "_port").c_str()),
	logfile("logs/" + (std::string((const char*)mn) + ".log"))
{
	rng.seed(std::random_device()());
	logfile << "Node " << name() << " with logic address " << cfg.address << ":" << std::endl;

	SC_THREAD(receiver_daemon);
}

Node::~Node()
{
	logfile.close();
}

sc_uint<16>& Node::get_logical_address()
{
	return cfg.address;
}

void Node::add_transmission(const TransmissionConfig & c)
{
	transmissions.push_back(c);
}

void Node::init_db(sqlite3 * _db)
{
	db = _db;
	std::ostringstream db_init_stmt;
	db_init_stmt << "DROP TABLE IF EXISTS " << basename() << "_send;" << std::endl <<
		"CREATE TABLE " << basename() << "_send(" <<
		R"sql(
TIME               INT   PRIMARY KEY   NOT NULL,
RECEIVER_ADDRESS   INT                 NOT NULL,
SENDER_ADDRESS     INT                 NOT NULL,)sql" << std::endl;
	if (verbose) db_init_stmt << "DATA TEXT NOT NULL," << std::endl;
	db_init_stmt << R"sql(
CRC                INT                 NOT NULL,
SIZE               INT                 NOT NULL
);
)sql" << std::endl;


	db_init_stmt << "DROP TABLE IF EXISTS " << basename() << "_recv;" << std::endl <<
		"CREATE TABLE " << basename() << "_recv(" <<
		R"sql(
TIME               INT   PRIMARY KEY   NOT NULL,
RECEIVER_ADDRESS   INT                 NOT NULL,
SENDER_ADDRESS     INT                 NOT NULL,)sql" << std::endl;
	if (verbose) db_init_stmt << "DATA TEXT NOT NULL," << std::endl;
	db_init_stmt << R"sql(
CRC                INT                 NOT NULL,
SIZE               INT                 NOT NULL,
RECEPTION_TIME     REAL                NOT NULL
);
)sql" << std::endl;

	char* zErrMsg;
	int rc = sqlite3_exec(db, db_init_stmt.str().c_str(), [](void *NotUsed, int argc, char **argv, char **azColName)->int { return 0; }, nullptr, &zErrMsg);
	if (rc != SQLITE_OK) {
		std::cerr << "SQL error: " << zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
		exit(2);
	}
}

void Node::send_raw(Packet & p)
{
	send_mutex.lock();
	logfile << sc_time_stamp() << ' ' << name() << " sending packet of size " << p.size() << " to " << p.get_receiver_address() << std::endl;
	if (verbose) logfile << p << std::endl;
	
	if (db)
	{
		std::ostringstream new_packet_stmt;
		new_packet_stmt << "INSERT INTO " << basename() << "_send VALUES(" << sc_time_stamp().value() << ',' << p.get_receiver_address() << ',' << p.get_sender_address() << ',';
		if (verbose)
		{
			new_packet_stmt << '\"';
			for (size_t i = 0 ; i < p.size()-1 ; i++)
				new_packet_stmt << p[i] << ',';
			new_packet_stmt << p[p.size()-1] << "\",";
		}
		new_packet_stmt << p.get_crc() << ',' << p.size() << ");" << std::endl;
		char* zErrMsg;
		int rc = sqlite3_exec(db, new_packet_stmt.str().c_str(), [](void *NotUsed, int argc, char **argv, char **azColName)->int { return 0; }, nullptr, &zErrMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error: " << zErrMsg << std::endl;
			sqlite3_free(zErrMsg);
			exit(3);
		}
	}

	p.reset();
	sc_uint<16> tmp;
	while (p >> tmp)
	{
		port.write(tmp);
		wait(cfg.delay_between_bytes);
	}
	send_mutex.unlock();
}

void Node::send_ack(sc_uint<16> dest, bool state) // Function spawned to send an ack packet (avoids blocking the PrintUnit for this)
{
	Packet p;
	p << dest << cfg.address << state;
	send_raw(p);
}

void Node::send(Packet &p)
{
	bool ack_received(false);
	send_raw(p); // Send the packet

	// Wait for confirmation
	while (!ack_received)
	{
		for (size_t i = 0; i < ack_queue.size(); i++)
		{
			if (ack_queue[i].get_sender_address() == p.get_receiver_address())
			{
				if (ack_queue[i][0])
				{
					std::cout << sc_time_stamp() << ' ' << name() << " received ack, positive response !" << std::endl;
					ack_queue.erase(ack_queue.begin() + i);
				}
				else
				{
					std::cout << sc_time_stamp() << ' ' << name() << " received ack, negative response, must send again" << std::endl;
					ack_queue.erase(ack_queue.begin() + i);
					send(p);
				}
				ack_received = true;
				break;
			}
		}
		if(!ack_received) wait(ack_reception);
	}
}

sc_time Node::recv_raw(Packet & p)
{
	p.reset();
	sc_uint<16> tmp(0);
	sc_time t0(sc_time_stamp());
	while (!tmp.and_reduce())
	{
		tmp = port.read();
		p << tmp;
	}
	sc_time t1(sc_time_stamp());

	if (db)
	{
		std::ostringstream new_packet_stmt;
		new_packet_stmt << "INSERT INTO " << basename() << "_recv VALUES(" << sc_time_stamp().value() << ',' << p.get_receiver_address() << ',' << p.get_sender_address() << ',';
		if (verbose)
		{
			new_packet_stmt << '\"';
			for (size_t i = 0; i < p.size() - 1; i++)
				new_packet_stmt << p[i] << ',';
			new_packet_stmt << p[p.size() - 1] << "\",";
		}
		new_packet_stmt << p.get_crc() << ',' << p.size() << ',' << (t1 - t0).to_seconds() << ");" << std::endl;

		char* zErrMsg;
		int rc = sqlite3_exec(db, new_packet_stmt.str().c_str(), [](void *NotUsed, int argc, char **argv, char **azColName)->int { return 0; }, nullptr, &zErrMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error: " << zErrMsg << std::endl;
			sqlite3_free(zErrMsg);
			exit(3);
		}
	}
	return t1-t0;
}

void Node::receiver_daemon()
{
	for (const TransmissionConfig& c : transmissions)
		sc_spawn(sc_bind(&Node::sending_daemon, this, c));

	sc_time t;
	while (true)
	{
		Packet p;
		t = recv_raw(p);

		if (p.size() > 1)
		{
			logfile << sc_time_stamp() << ' ' << name() << " received packet of size " << p.size() << " from " << p.get_sender_address() << " in " << t.to_seconds() << "s" << std::endl;
			if (verbose) logfile << p << std::endl;
			std::cout << sc_time_stamp() << ' ' << name() << " received packet of size " << p.size() << " from " << p.get_sender_address() << " in " << t.to_seconds() << "s" << std::endl;
			if (verbose) std::cout << "\33[48;5;194;38;5;0m" << p << "\33[0m" << std::endl;

			if (p.get_crc())
			{
				std::cout << sc_time_stamp() << " " << name() << " \33[1;38;5;197m" << "WRONG CRC" << "\33[0m" << std::endl;
				sc_spawn(sc_bind(&Node::send_ack, this, p.get_sender_address(), 0)); // Spawns a thread to send an ack packet with data 0, signaling a transmission error
			}
			else
			{
				std::cout << sc_time_stamp() << " " << name() << " \33[1;38;5;40m" << "CORRECT CRC" << "\33[0m" << std::endl;
				sc_spawn(sc_bind(&Node::send_ack, this, p.get_sender_address(), 1)); // Spawns a thread to send an ack packet with data 1, signaling a successful transmission
			}
			packet_queue.push_back(p);
			packet_reception.notify(SC_ZERO_TIME);
		}
		else // In case of ack reception
		{
			if (verbose) std::cout << "ack received by " << name() << ": " << std::endl << p << std::endl;
			logfile << "ack received: " << std::endl << p << std::endl;
			ack_queue.push_back(p);
			ack_reception.notify(SC_ZERO_TIME);
		}
	}
}

void Node::sending_daemon(const TransmissionConfig& c)
{
	sc_time t0, t1;
	size_t n_packets_sent(0);
	wait(c.t_start);
	while (n_packets_sent < c.n_packets && sc_time_stamp() < c.t_end)
	{
		t0 = sc_time_stamp();
		Packet p;
		p << c.receiver_address << cfg.address;
		for (size_t i = 0; i < cfg.psize; i++)
			p << rand();

		send(p);
		t1 = sc_time_stamp();
		n_packets_sent++;

		wait(c.delay_between_packets - t1 + t0);
	}
}

unsigned Node::rand()
{
	return dist(rng);
}

void Node::get_packet(Packet &p)
{
	while (!packet_queue.size())
		wait(packet_reception);
	p = packet_queue[0];
	packet_queue.erase(packet_queue.begin());
}
