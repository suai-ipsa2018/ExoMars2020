#pragma once
#include "Node.h"
class Ma_MISS : public Node
{
public:
	Ma_MISS(size_t logical_address, size_t psize, sc_time delay_between_bytes=sc_time(1./24e6, SC_SEC), bool verbose_ = true, sc_module_name mn="Ma_MISS");

private:
	void gen_thread();
	bool verbose;
};

