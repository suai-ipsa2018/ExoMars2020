#include "io_port.h"


io_channel::io_channel(sc_module_name mn, sc_time transmission_time_, int error_frequency_) : sc_prim_channel((const char*)mn), transmission_time(transmission_time_), error_frequency(error_frequency_)
{
}

void io_channel::write(const sc_uint<16>& n, size_t id)
{
	new_d = n; // New data written
	if (error_frequency > 0)
	{
		int rn = rand();
		if (!(rn % 1024/error_frequency))
		{
			std::cout << sc_time_stamp() << " " << name() << " simulated transmission error ! Byte changed from  " << new_d << " to ";
			new_d ^= rn;
			std::cout << new_d << std::endl;
		}
	}
    new_wid = id; // New id also
    request_update(); // Request an update, for the end of the delta cycle, to set the current data as the new data
}

size_t io_channel::get_wid()
{
    return cur_wid;
}

sc_uint<16> io_channel::read()
{
    e_read.notify(SC_ZERO_TIME);
    return cur_d;
}

const sc_event& io_channel::default_event() const { return e_write; }
const sc_event& io_channel::data_written_event() const { return e_write; }
const sc_event& io_channel::data_read_event() const { return e_read; }

void io_channel::update()
{
    if (cur_wid != new_wid) cur_wid = new_wid;
    if (cur_d != new_d) cur_d = new_d;

    e_write.notify(transmission_time);
}


size_t io_port::n_ports=0;

io_port::io_port() : port_id(n_ports)
{
    n_ports++;
}
io_port::io_port(sc_module_name mn) : sc_port<io_if, 1, SC_ONE_OR_MORE_BOUND>(mn), port_id(n_ports)
{
    n_ports++;
}

sc_uint<16> io_port::read()
{
    while (!(*this)->access_locked || (*this)->get_wid() == port_id) // Waits if the channel bound is not locked for writing (no write since the last read), or if this port was the last to write something in the channel
        wait((*this)->data_written_event());

    byte = (*this)->read(); // Reads from the channel
    (*this)->access_locked = false; // Unlocks the channel so that a new write can occur

    return byte;
}

void io_port::write(const sc_uint<16>& n)
{
    while ((*this)->access_locked) // Waits if the channel bound is locked (no read since the last write)
        wait((*this)->data_read_event());

    byte = n;
    (*this)->access_locked = true; // Locks the channel so that a read can occur
    (*this)->write(byte, port_id); // Writes to the channel
}

void sc_trace(sc_trace_file *_f, const io_channel& object, std::string name_file)
{
    sc_trace(_f, object.cur_d, name_file);
}

void sc_trace(sc_trace_file *_f, const io_port& object, std::string name_file)
{
    sc_trace(_f, object.byte, name_file);
}
