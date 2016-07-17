#include <etw/event_parser.h>

using ez_etw::event_parser;
using ez_etw::event;
using ez_etw::parsed_event;
using std::shared_ptr;
using std::cbegin;
using std::cend;

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
	if(evt->get_guid() == m_event_guid) {
        auto pair = parse_event(evt);
        parsed = pair.first;
        if(parsed) {
            m_events.push_back(pair.second);
        }
	}
	return parsed;
}

void event_parser::set_pointer_size(shared_ptr<unsigned long> pointer_size) {
	m_pointer_size = pointer_size;
}

unsigned long event_parser::get_pointer_size() const {
	return *m_pointer_size;
}

const std::deque<std::shared_ptr<parsed_event>> event_parser::get_events() {
	std::deque<std::shared_ptr<parsed_event>> events(cbegin(m_events), cend(m_events));
    m_events.clear();
	return events;
}
