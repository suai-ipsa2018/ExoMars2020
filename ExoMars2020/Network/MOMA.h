#pragma once
#include "Node.h"

class MOMA : public Node
{
public:
	MOMA(size_t logical_address, sc_time delay_between_bytes = sc_time(1. / 24e6, SC_SEC), sc_module_name mn="MOMA");

private:
	void gen_thread();
};

