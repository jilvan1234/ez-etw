#include <etw/parsed_event.h>

using ez_etw::parsed_event;

parsed_event::parsed_event(const ez_etw::event& evt)
:m_timestamp(evt.get_timestamp()) {
}

time_t parsed_event::get_timestamp() const {
    return m_timestamp;
}

parsed_event::status parsed_event::get_status() const {
	return m_status;
}
