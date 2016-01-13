#include <etw/parsed_event.h>

using ez_etw::parsed_event;

parsed_event::parsed_event(const ez_etw::event& evt, unsigned long pointer_size)
:m_timestamp(evt.get_timestamp()), m_pointer_size(pointer_size) {
}

time_t parsed_event::get_timestamp() const {
    return m_timestamp;
}

parsed_event::status parsed_event::get_status() const {
	return m_status;
}
