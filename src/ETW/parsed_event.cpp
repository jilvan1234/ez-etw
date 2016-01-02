#include <etw/parsed_event.h>

using ez_etw::parsed_event;

parsed_event::parsed_event(const ez_etw::event& evt)
:m_timestamp(evt.get_timestamp()), m_pid(evt.get_process_id()) {

}

time_t parsed_event::get_timestamp() const {
    return m_timestamp;
}

unsigned int parsed_event::get_pid() const {
    return m_pid;
}
