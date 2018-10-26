#include "genunit.h"

SC_HAS_PROCESS(GenUnit);
GenUnit::GenUnit(sc_module_name mn, size_t psize, double speed, double delay_packets)
	: sc_module(mn),
	packet_size(psize),
	delay_between_bytes(1.0 / speed, SC_SEC), delay_between_packets(delay_packets, SC_SEC),
	packet_fifo1(psize), packet_fifo2(psize),
	dist(0, 127)
{
	rng.seed(std::random_device()()); // Initialisation of the random number generator, using the current time as seed
    SC_THREAD(packet_gen1);  // Declaration of the packet generating threads
    SC_THREAD(packet_gen2);
}

void GenUnit::packet_gen1()
{
    sc_uint<16> receiver(32), sender(33), byte, crc_remainder; // Receiver (address) and Sender (address), can be changed for another network configuration. The other vars are temporary, used for computation or buffering
    sc_uint<16> ack_sender, ack_receiver; // Vars to contain the header of the ack packet
    bool integrity_check; // True if packet was received correctly
    sc_time t0; // Simulation time at the beggining of the simulation. Used to measure the time taken to send one packet, and to determine how long to wait to ensure one packet per delay_between_packets

    while (true) // Always generate packets
    {
        t0 = sc_time_stamp();
        std::cout << t0 << " " << name() << " beginning packet1 generation with logic address " << receiver << std::endl;

        crc_remainder = 0; // Initializing crc calculation

        for (size_t i=0 ; i < packet_size ; i++) // Generates packet1's bytes
        {
			byte = dist(rng);
            crc_remainder = crc<16>(crc_remainder, byte); // Calculation of crc on the fly
            packet_fifo1.write(byte); // Storage in a fifo while finishing generation and crc calculation
        }
        crc_remainder = crc<16>(crc_remainder, 0); // Final value of the checksum

        std::cout << sc_time_stamp() << " " << name() << " packet1 generated" << std::endl;

        // Ready to send !
        integrity_check = false; // Boolean checking any transmission error
        while (!integrity_check)
        {
            port1.write(receiver); // Writing header
            wait(delay_between_bytes);
            port1.write(sender);
            wait(delay_between_bytes);

            for (size_t i=0 ; i < packet_size ; i++) // Consumes the fifo, and sends the whole data of a packet
            {
                byte = packet_fifo1.read();
                port1.write(byte);
                packet_fifo1.write(byte); // Write back in fifo, to obtain a bakcup of the packet in case of transmission failure
                wait(delay_between_bytes);
            }
            port1.write(crc_remainder);
            wait(delay_between_bytes);
            port1.write(-1); // EOP
            wait(delay_between_bytes);

            // Ack receiving
            ack_receiver = port1.read();
            std::cout << "ack_receiver = " << ack_receiver << std::endl;
            ack_sender = port1.read();
            std::cout << "ack_sender = " << ack_sender << std::endl;
            integrity_check = port1.read();
            std::cout << "integrity_check = " << integrity_check << std::endl;
            port1.read(); // Data CRC is discarded for the ack
            port1.read(); // So is the EOP
            std::cout << sc_time_stamp() << " " << name() << " Ack received | sender=" << ack_sender << " | receiver=" << ack_receiver << " | state=" << integrity_check << std::endl;
        }
        for (size_t i=0 ; i<packet_size ; i++) // Empty the fifo if transmission is a success, so new data can be written into
            packet_fifo1.read();
        wait(delay_between_packets+t0-sc_time_stamp());
    }
}

void GenUnit::packet_gen2() // Same as above, but for packet2. The only differences are about the port used and the logical address.
// We tried to create one general function spawned with different parameters as in SwitchUnit to avoid copy-paste, and possible differences when modifying something...
// But we had issues dynamically allocating fifos (about setting their size). Maybe we'll try again using std::array / std::vector
{
    sc_uint<16> receiver(32), sender(34), byte, crc_remainder;
    sc_uint<16> ack_sender, ack_receiver;
    bool integrity_check;
    sc_time t0;

    while (true)
    {
        t0 = sc_time_stamp();
        std::cout << t0 << " " << name() << " beginning packet2 generation with logic address " << receiver << std::endl;

        crc_remainder = 0; // Initializing crc calculation

        for (size_t i=0 ; i < packet_size ; i++) // Generates packet2's bytes
        {
			byte = dist(rng);
            crc_remainder = crc<16>(crc_remainder, byte); // Calculation of crc on the fly
            packet_fifo2.write(byte); // Storage in a fifo while finishing generation and crc calculation
        }
        crc_remainder = crc<16>(crc_remainder, 0); // Final value of the checksum

        std::cout << sc_time_stamp() << " " << name() << " packet2 generated" << std::endl;

        // Ready to send !
        integrity_check = false; // Boolean checking any transmission error
        while (!integrity_check)
        {
            port2.write(receiver); // Writing header
            wait(delay_between_bytes);
            port2.write(sender);
            wait(delay_between_bytes);

            for (size_t i=0 ; i < packet_size ; i++) // Consumes the fifo, and sends the whole data of a packet
            {
                byte = packet_fifo2.read();
                port2.write(byte);
                packet_fifo2.write(byte);
                wait(delay_between_bytes);
            }
            port2.write(crc_remainder);
            wait(delay_between_bytes);
            port2.write(-1); // EOP
            wait(delay_between_bytes);

            // Ack receiving
            ack_receiver = port2.read();
            std::cout << "ack_receiver = " << ack_receiver << std::endl;
            ack_sender = port2.read();
            std::cout << "ack_sender = " << ack_sender << std::endl;
            integrity_check = port2.read();
            std::cout << "integrity_check = " << integrity_check << std::endl;
            port2.read();
            port2.read();
            std::cout << sc_time_stamp() << " " << name() << " Ack received | sender=" << ack_sender << " | receiver=" << ack_receiver << " | state=" << integrity_check << std::endl;
        }
        for (size_t i=0 ; i<packet_size ; i++) // Empty the fifo
            packet_fifo2.read();
        wait(delay_between_packets+t0-sc_time_stamp());
    }
}

std::string GenUnit::name()
{
    std::string unitname(sc_module::name());
    return "\33[38;5;214m" + unitname + "\33[0m";
}

