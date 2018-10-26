#include "networkunit.h"


NetworkUnit::NetworkUnit(sc_module_name mn, size_t psize, double speed) :
	sc_module(mn),
	delay_between_bytes(sc_time(1. / speed, SC_SEC)),
	router("router"),

	m_Ma_MISS(40, delay_between_bytes),
	m_MicrOmega(41, delay_between_bytes),
	m_RLS(42, delay_between_bytes),
	m_MOMA(43, delay_between_bytes),

	pu("PrintUnit", speed),
	network_speed(speed)
{
	router.connect(pu, PrintUnit_channel);
	router.connect(m_Ma_MISS, Ma_MISS_channel);
	router.connect(m_MicrOmega, MicrOmega_channel);
	router.connect(m_RLS, RLS_channel);
	router.connect(m_MOMA, MOMA_channel);
	router.connections_done();
}