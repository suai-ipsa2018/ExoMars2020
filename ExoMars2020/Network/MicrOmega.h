#pragma once
#include "Node.h"
class MicrOmega : public Node
{
public:
	MicrOmega(size_t logical_address, sc_time delay_between_bytes = sc_time(1. / 24e6, SC_SEC), sc_module_name mn="MicrOmega");

private:
	void gen_thread();
};

