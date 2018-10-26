#pragma once
#include "Node.h"

class RLS : public Node
{
public:
	RLS(size_t logical_address, sc_time delay_between_bytes = sc_time(1. / 24e6, SC_SEC), sc_module_name mn="RLS");

private:
	void gen_thread();
};

