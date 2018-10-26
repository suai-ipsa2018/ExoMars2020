#pragma once

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc.h"
#include "Node.h"

/** @file printunit.h
 * @brief Declares the output module
 */

class PrintUnit : public Node /// A sink, printing every packet it recives on its port
{
public:
    /**
     * @param mn Module's name
     */
    PrintUnit(sc_module_name mn, double speed=24e6);

private:
    void printing_thread(); //!< Thread in charge of reading from the port as much as possible, computing crc, and sending back ack packets
    /**
     * @brief send_ack Send an acknowledgment packet at a given address
     * @param address Address of the receiver of the ack packet
     * @param state 0 for failure, 1 for success
     */
    void send_ack(size_t address, bool state);
    std::string name(); //!< Little function to color sc_module's name() result
    sc_time delay_between_bytes; //!< Delay between two consecutive bytes, calculated with the network's speed
};