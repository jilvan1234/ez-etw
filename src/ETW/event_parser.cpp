#include <etw/event_parser.h>

using ez_etw::event_parser;
using ez_etw::event;
using ez_etw::parsed_event;
using std::shared_ptr;

event_parser::event_parser(const GUID& event_guid)
:m_event_guid(event_guid) {
}

const parsed_event* const event_parser::get_event() const {
	return m_events.front().get();
}

void event_parser::remove_event() {
	if(!events_empty()) {
		m_events.pop_front();
	}
}

bool event_parser::events_empty() const {
	return m_events.empty();
}

GUID event_parser::get_event_type() const {
	return m_event_guid;
}

bool event_parser::parse(std::shared_ptr<event> evt) {
	bool parsed = false;
	if(evt->get_guid() == m_event_guid)
	{
		parsed = parse_event(evt, m_events);
	}
	return parsed;
}
