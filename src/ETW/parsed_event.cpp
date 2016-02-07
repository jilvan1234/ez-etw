#include <etw/parsed_event.h>

using ez_etw::parsed_event;
using ez_etw::event;

parsed_event::parsed_event(const ez_etw::event& evt, unsigned long pointer_size)
:m_timestamp(evt.get_timestamp()), m_pointer_size(pointer_size), m_type(evt.get_type()) {
}

time_t parsed_event::get_timestamp() const {
    return m_timestamp;
}

parsed_event::status parsed_event::get_status() const {
	return m_status;
}

event::type parsed_event::get_type() const {
    return m_type;
}