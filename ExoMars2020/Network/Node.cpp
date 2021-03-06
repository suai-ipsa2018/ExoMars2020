#include "Node.h"

SC_HAS_PROCESS(Node);
Node::Node(sc_module_name mn, const NodeConfig& _cfg, bool _verbose)
	: sc_module(mn), cfg(_cfg), verbose(_verbose),
	dist(0, (unsigned int)(pow(2,_cfg.fsize) - 2)),
	port((std::string((const char*)mn) + "_port").c_str()),
	logfile("logs/" + (std::string((const char*)mn)+ '_' + std::to_string(NETWORK_PART) + ".log"))
{
	rng.seed(std::random_device()());
	logfile << "Node " << name() << " with logic address " << cfg.address << ":" << std::endl;

	SC_THREAD(receiver_daemon); // Only receiver daemon here because we need to wait for all configuration to be loaded
	// by NetworkUnit, which is done after the end of this constructor. The two other daemons are spawned at the beginning
	// of the simulation, and thus at the beginning of receiver_daemon.
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

void Node::add_generation(const GenerationConfig & c)
{
	generations.push_back(c);
}

void Node::init_db(sqlite3 * _db)
{
	db = _db;
	create_table("send");
	create_table("recv");

	std::ostringstream db_init_stream;
	char* zErrMsg(nullptr);
	int rc(0);
	db_init_stream << "DROP TABLE IF EXISTS " << basename() << "_gen" << ';' << std::endl <<
		"CREATE TABLE " << basename() << "_gen" << '(' <<
		R"sql(
TIME         INT   PRIMARY KEY   NOT NULL,
MEM_ADDRESS  INT                 NOT NULL,)sql" << std::endl;
	if (verbose) db_init_stream << "DATA   TEXT," << std::flush;
	db_init_stream << R"sql(
DATA_SIZE INT NOT NULL
);
)sql" << std::endl;

	rc = sqlite3_exec(db, db_init_stream.str().c_str(), nullptr, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Failed to create table " << basename() << "_gen: " << zErrMsg << std::endl
			<< "Statement: " << db_init_stream.str() << std::endl;
	}
}

void Node::send_raw(Packet & p)
{
	send_mutex.lock();
	std::cout << sc_time_stamp() << ' ' << name() << " sending packet of size " << p.size() << " to " << p.destination_address() << std::endl;
	if (verbose) std::cout << p << std::endl;
	logfile << sc_time_stamp() << ' ' << name() << " sending packet of size " << p.size() << " to " << p.destination_address() << std::endl;
	if (verbose) logfile << p << std::endl;
	
	if (db)
	{
		insert_into("send", p);
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


void Node::send(Packet &p)
{
	bool reply_received(false);
	send_raw(p); // Send the packet

	// Wait for confirmation
	while (!reply_received)
	{
		for (size_t i = 0; i < reply_queue.size(); i++)
		{
			if (p.header[1] == reply_queue[i].header[1] && p.header[5] == reply_queue[i].header[5] && p.header[2][5] == reply_queue[i].header[2][5])
			{
				if (reply_queue[i].get_header_crc() || reply_queue[i].get_crc())
				{
					std::cout << sc_time_stamp() << ' ' << name() << " \33[1;38;5;197m" << "wrong " << (reply_queue[i].get_header_crc() ? "header crc" : "") << 
						(reply_queue[i].get_header_crc() && reply_queue[i].get_crc() ? " and " : "") <<
						(reply_queue[i].get_crc() ? "data crc" : "") << ", must send command again" << ' ' << reply_queue[i].get_crc() << "\33[0m" << std::endl;
					reply_queue.erase(reply_queue.begin() + i);
					send(p);
				}
				else
				{
					if (reply_queue[i].header[3])
					{
						std::cout << sc_time_stamp() << ' ' << name() << " \33[1;38;5;40m" << "received reply, positive status !" << "\33[0m" << std::endl;
						reply_queue.erase(reply_queue.begin() + i);
					}
					else
					{
						std::cout << sc_time_stamp() << ' ' << name() << " \33[1;38;5;197m" << "received reply, negative status, must send again" << "\33[0m" << std::endl;
						reply_queue.erase(reply_queue.begin() + i);
						send(p);
					}
				}
				reply_received = true;
				break;
			}
		}
		if(!reply_received) wait(reply_reception);
	}
}

sc_time Node::recv_raw(Packet & p)
{
	p.reset();
	sc_uint<16> tmp(0);
	sc_time t0(sc_time_stamp());
	int i(0);
	bool command;
	bool mode;
	while (!tmp.and_reduce())
	{
		tmp = port.read();
		if (i == 2)
		{
			command = tmp[6]; // 0 = reply, 1 = command
			mode = tmp[5]; // 0 = read, 1 = write
			p << tmp;
		}
		else if (i == 7 && !command && mode) // Write reply : header size = 8
		{
			p.receive_header_crc(tmp);
		}
		else if (i == 11 && !command && !mode) // Read reply : header size = 12
		{
			p.receive_header_crc(tmp);
		}
		else if (i == 15 && command) // All commands : header size = 16
		{
			p.receive_header_crc(tmp);
		}
		else
			p << tmp;
		i++;
	}
	sc_time t1(sc_time_stamp());

	if (db)
	{
		insert_into("recv", p);
	}
	return t1-t0;
}

void Node::receiver_daemon()
{
	for (const TransmissionConfig& c : transmissions)
		sc_spawn(sc_bind(&Node::sending_daemon, this, c));
	for (const GenerationConfig& c : generations)
		sc_spawn(sc_bind(&Node::generating_daemon, this, c));

	sc_time t;
	while (true)
	{
		Packet p;
		t = recv_raw(p);

		if (p.header[1]==0x01) //Type RMAP
		{
			if (p.header[2][5]) // Write
			{
				if (p.header[2][6]) // Command
				{
					logfile << sc_time_stamp() << ' ' << name() << " received packet of size " << p.size() << " from " << p.source_address() << " in " << t.to_seconds() << "s" << std::endl;
					if (verbose) logfile << p << std::endl;
					std::cout << sc_time_stamp() << ' ' << name() << " received packet of size " << p.size() << " from " << p.source_address() << " in " << t.to_seconds() << "s" << std::endl;
					if (verbose) std::cout << "\33[48;5;194;38;5;0m" << p << "\33[0m" << std::endl;

					if (p.get_header_crc() || p.get_crc())
					{
						Packet answer(7);
						answer << p.source_address() << p.header[1] << 0b00110001 << 0 << p.destination_address() << p.header[5] << p.header[6];
						std::cout << sc_time_stamp() << " " << name() << " \33[1;38;5;197m" << "WRONG CRC" << "\33[0m" << std::endl;
						sc_spawn(sc_bind(&Node::send_raw, this, answer)); // Spawns a thread to send an ack packet with data 0, signaling a transmission error
					}
					else
					{
						Packet answer(7);
						sc_uint<64> mem_address((p.header[8], p.header[9], p.header[10], p.header[11]));
						answer << p.source_address() << p.header[1] << 0b00110001 << 1 << p.destination_address() << p.header[5] << p.header[6];
						std::cout << sc_time_stamp() << " " << name() << " \33[1;38;5;40m" << "CORRECT CRC, writing to address " << mem_address << "\33[0m" << std::endl;
						mem[mem_address] = p.data;
						sc_spawn(sc_bind(&Node::send_raw, this, answer)); // Spawns a thread to send an ack packet with data 1, signaling a successful transmission
					}
				}
				else // Answer
				{
					if (verbose) std::cout << sc_time_stamp() << " write reply received by " << name() << ": " << std::endl << "\33[1;48;5;33m" << p << "\33[0m" << std::endl;
					logfile << sc_time_stamp() << " write reply received: " << std::endl << p << std::endl;
					reply_queue.push_back(p);
					reply_reception.notify(SC_ZERO_TIME);
				}
			}
			else // Read
			{
				if (p.header[2][6]) // Command received
				{
					if (p.get_header_crc() || p.get_crc())
					{
						Packet answer;
						answer << p.source_address() << 0x01 << 0b00000001 << 0 << p.destination_address() << p.header[5] << p.header[6] << 0 << 0 << 0 << 0;
						std::cout << sc_time_stamp() << " " << name() << " \33[1;38;5;197m" << "WRONG CRC" << "\33[0m" << std::endl;
						sc_spawn(sc_bind(&Node::send_raw, this, answer)); // Spawns a thread to send an ack packet with data 0, signaling a transmission error
					}
					else
					{

						sc_uint<64> mem_address = (p.header[8], p.header[9], p.header[10], p.header[11]);
						std::cout << sc_time_stamp() << " " << name() << " Asking to read at address " << mem_address << std::endl;
						logfile << sc_time_stamp() << " " << name() << " Asking to read at address " << mem_address << std::endl;

						std::vector<sc_uint<16>> data = mem[mem_address];
						sc_uint<48> dsize = data.size();

						bool status;
						if (!data.empty())
							status = 1;
						else
						{
							std::cout << sc_time_stamp() << " " << name() << " No data at " << mem_address << std::endl;
							status = 0;
						}
						Packet answer(11);
						answer << p.source_address() << 0x01 << 0b00000001 << status << p.destination_address() << 0 << p.header[6] << 0 << dsize.range(47, 32) << dsize.range(31, 16) << dsize.range(15, 0);
						for (sc_uint<16>& ve : data)
							answer << ve;
						sc_spawn(sc_bind(&Node::send_raw, this, answer));
					}
				}
				else // Answer received
				{
					std::cout << sc_time_stamp() << " " << name() << " Received answer for transaction " << p.header[6] << " (read node " << p.header[4] << ")" << std::endl;
					if (verbose)
					{
						std::cout << "\33[1;48;5;33m" << p << "\33[0m" << std::endl;
					}
					reply_queue.push_back(p);
					reply_reception.notify(SC_ZERO_TIME);
				}
			}
		}
		else // In case of ack reception
		{
			std::cout << sc_time_stamp() << " " << name() << "Protocol not known: " << p.header[1] << std::endl;
			std::cout << p << std::endl;
			p.reset();
		}
	}
}

void Node::generating_daemon(const GenerationConfig & c)
{
	int n_generated(0);
	wait(c.t_start);
	while (c.n_generations == -1 || (n_generated < c.n_generations && sc_time_stamp() < c.t_end))
	{
		std::vector<sc_uint<16>> data;
		data.reserve(c.dsize);
		for (size_t i = 0; i < c.dsize; i++)
			data.emplace_back(rand());

		mem[c.mem_address] = data;
		n_generated++;

		if (db)
		{
			char* zErrMsg;
			std::ostringstream db_insert_stream;
			db_insert_stream << "INSERT INTO " << basename() << "_gen" << " VALUES("
				<< sc_time_stamp().value() << ',' << c.mem_address << ',';
			if (verbose)
			{
				db_insert_stream << '\"';
				for (size_t i = 0; i < c.dsize - 1; i++)
					db_insert_stream << data[i] << ',';
				db_insert_stream << data[c.dsize - 1] << "\",";
			}
				
				db_insert_stream << c.dsize << ");" << std::endl;

			int rc = sqlite3_exec(db, db_insert_stream.str().c_str(), nullptr, 0, &zErrMsg);
			if (rc != SQLITE_OK)
			{
				std::cerr << "Failed to insert in table " << basename() << "_gen: " << zErrMsg << std::endl
					<< "Statement: " << db_insert_stream.str() << std::endl;
			}
		}
	}
}

void Node::sending_daemon(const TransmissionConfig& c)
{
	sc_time t0, t1;
	int n_packets_sent(0);
	wait(c.t_start);
	while (c.n_packets == -1 || (n_packets_sent < c.n_packets && sc_time_stamp() < c.t_end))
	{
		t0 = sc_time_stamp();
		Packet p;
		p.data.reserve(c.psize);
		sc_uint<64> mem_address(c.mem_address);
		sc_uint<48> dsize(c.psize);
		p << c.receiver_address << 0x01 << (0b01010001 | (c.mode << 5)) << 0 << cfg.address << 0 << c.id << 0 << mem_address.range(63, 48) << mem_address.range(47, 32) << mem_address.range(31, 16) << mem_address.range(15, 0) << dsize.range(47, 32) << dsize.range(31, 16) << dsize.range(15, 0);
		if (c.mode) // If in writing mode
		{
			for (size_t i = 0; i < c.psize; i++)
				p << rand();
		}

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

void Node::create_table(std::string name)
{
	std::ostringstream db_init_stream;
	char* zErrMsg(nullptr);
	int rc(0);
	db_init_stream << "DROP TABLE IF EXISTS " << basename() << '_' << name << ';' << std::endl <<
		"CREATE TABLE " << basename() << '_' << name << '(' <<
		R"sql(
TIME         INT   PRIMARY KEY   NOT NULL,
DESTINATION_ADDRESS INT          NOT NULL,
PROTOCOL_ID         INT          NOT NULL,
TYPE                TEXT         NOT NULL,
MODE                TEXT         NOT NULL,
SOURCE_ADDRESS      INT          NOT NULL,
TRANSACTION_ID      INT          NOT NULL,
EXTENDED_WA         INT,
WRITE_ADDRESS       INT,
DATA_LENGTH         INT,
STATUS              INT,
HEADER_CRC          INT,)sql" << std::endl;
	if (verbose) db_init_stream << "DATA   TEXT," << std::flush;
	db_init_stream << R"sql(
DATA_CRC                INT
);
)sql" << std::endl;

	rc = sqlite3_exec(db, db_init_stream.str().c_str(), nullptr, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Failed to create table " << basename() << '_' << name << ": " << zErrMsg << std::endl
			<< "Statement: " << db_init_stream.str() << std::endl;
	}
}

void Node::insert_into(std::string name, Packet& p)
{
	char* zErrMsg;
	std::ostringstream db_insert_stream;
	db_insert_stream << "INSERT INTO " << basename() << '_' << name << " VALUES("
		<< sc_time_stamp().value() << ','
		<< p.header[0] << ','
		<< p.header[1] << ','
		<< (p.header[2][6] ? "\"Command\"" : "\"Reply\"") << ','
		<< (p.header[2][5] ? "\"Write\"" : "\"Read\"") << ','
		<< p.header[4] << ','
		<< p.header[6] << ',';

	if (p.header[2][6] && p.header[2][5])
		db_insert_stream << p.header[7] << ',' << (p.header[8], p.header[9], p.header[10], p.header[11]) << ',' << p.header[14] << ',';
	else
		db_insert_stream << "NULL,NULL,NULL,";
	if (!p.header[2][6])
		db_insert_stream << p.header[3] << ',';
	else
		db_insert_stream << "NULL,";

	db_insert_stream << p.get_header_crc() << ',';
	if (verbose)
	{
		if (p.header[2][6] == p.header[2][5])
		{
			db_insert_stream << '\"';
			for (size_t i = 0; i < p.data.size() - 1; i++)
				db_insert_stream << p.data[i] << ',';
			db_insert_stream << p.data[p.data.size() - 1] << "\",";
		}
		else
			db_insert_stream << "NULL,";
	}
	if (p.header[2][6] == p.header[2][5])
		db_insert_stream << p.get_crc() << ");" << std::endl;
	else
		db_insert_stream << "NULL);" << std::endl;

	int rc = sqlite3_exec(db, db_insert_stream.str().c_str(), nullptr, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Failed to insert in table " << basename() << '_' << name << ": " << zErrMsg << std::endl
			<< "Statement: " << db_insert_stream.str() << std::endl;
	}
}
