#pragma once
#include "Node.h"
class Ma_MISS : public Node
{
public:
	Ma_MISS(size_t logical_address, sc_time delay_between_bytes=sc_time(1./24e6, SC_SEC), sc_module_name mn="Ma_MISS");

private:
	void gen_thread();
};

