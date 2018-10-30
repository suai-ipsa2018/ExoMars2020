#pragma once
#include "Node.h"
class Adron : public Node
{
public:
	Adron(size_t logical_address, size_t psize, sc_time delay_between_bytes = sc_time(1. / 24e6, SC_SEC));
private:
	void gen_thread();
};

