#pragma once
#include "systemc.h"

#include "Ma_MISS.h"
#include "MicrOmega.h"
#include "RLS.h"
#include "MOMA.h"

#include "io_port.h"
#include "switchunit.h"
#include "printunit.h"

/** @file networkunit.h
 * @brief Declares the network module, using all the other modules
 */

class NetworkUnit : sc_module /// Top-level module, creating all sub-modules from the simple network simulated
{
	sc_time delay_between_bytes;
public:
    /**
     * @param mn Module's name
     * @param psize Packet size
     * @param speed Speed of the network, in bit/s
     */
    NetworkUnit(sc_module_name mn, size_t psize, double speed=24e6);

private:
    
	Ma_MISS m_Ma_MISS;
	MicrOmega m_MicrOmega;
	RLS m_RLS;
	MOMA m_MOMA;

	PrintUnit pu;

	SwitchUnit router;

	io_channel PrintUnit_channel{ "PrintUnit_channel" },
		Ma_MISS_channel{ "Ma_MISS_channel" },
		MicrOmega_channel{ "MicrOmega_channel" }, 
		RLS_channel{ "RLS_channel" },
		MOMA_channel{ "MOMA_channel" };
    double network_speed; //!< Keeps track of the network's speed
};