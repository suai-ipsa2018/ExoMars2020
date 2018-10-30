#include "networkunit.h"


NetworkUnit::NetworkUnit(sc_module_name mn, double speed, bool verbose) :
	sc_module(mn),
	delay_between_bytes(sc_time(1. / speed, SC_SEC)),
	m_PanCam(33, 4, delay_between_bytes, verbose),
	m_NavCam(34, 25, delay_between_bytes, verbose),
	m_ISEM(35, 25, delay_between_bytes, verbose),
	m_CLUPI(36, 25, delay_between_bytes, verbose),
	m_Drill(37, 25, delay_between_bytes, verbose),
	m_Adron(38, 25, delay_between_bytes, verbose),
	m_WISDOM(39, 25, delay_between_bytes, verbose),
	m_Ma_MISS(40, 25, delay_between_bytes, verbose),
	m_MicrOmega(41, 25, delay_between_bytes, verbose),
	m_RLS(42, 25, delay_between_bytes, verbose),
	m_MOMA(43, 25, delay_between_bytes, verbose),

	pu("PrintUnit", speed, verbose),
	router("router", 32, verbose),
	network_speed(speed)
{
	router.connect(m_PanCam, PanCam_channel);
	router.connect(m_NavCam, NavCam_channel);
	router.connect(m_ISEM, ISEM_channel);
	router.connect(m_CLUPI, CLUPI_channel);
	router.connect(m_Drill, Drill_channel);
	router.connect(m_Adron, Adron_channel);
	router.connect(m_WISDOM, Wisdom_channel);
	router.connect(pu, PrintUnit_channel);
	router.connect(m_Ma_MISS, Ma_MISS_channel);
	router.connect(m_MicrOmega, MicrOmega_channel);
	router.connect(m_RLS, RLS_channel);
	router.connect(m_MOMA, MOMA_channel);
	router.connections_done();
}