#pragma once

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "Node.h"
class Adron : public Node
{
public:
	Adron(size_t logical_address, size_t psize, size_t bit, double speed = 24e6, sc_time delay_between_bytes = sc_time(1. / 24e6, SC_SEC), bool verbose_ = true, sc_module_name mn="Adron");
private:
	void printing_thread();
	void gen_thread();
	std::string name();
	sc_time delay_between_bytes;
	bool verbose;
};

