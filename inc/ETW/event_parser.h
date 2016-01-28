#ifndef EZETW_EVENTPARSER_H
#define EZETW_EVENTPARSER_H

#include <etw/event.h>
#include <etw/parsed_event.h>
#include <memory>
#include <deque>

namespace ez_etw {
	struct event_parser {
		event_parser(const GUID& event_guid);
		virtual ~event_parser() = default;
		bool parse(std::shared_ptr<event> evt);
		const parsed_event* const get_event() const;
		const std::deque<std::shared_ptr<parsed_event>>& get_events() const;
		void remove_event();
		bool events_empty() const;
		GUID get_event_type() const;
		void set_pointer_size(std::shared_ptr<unsigned long> pointer_size);
		unsigned long get_pointer_size() const;
	private:
		virtual bool parse_event(const std::shared_ptr<event>& evt, std::deque<std::shared_ptr<parsed_event>>& events) const = 0;
		const GUID& m_event_guid;
		std::deque<std::shared_ptr<parsed_event>> m_events;
		std::shared_ptr<unsigned long> m_pointer_size;
	};
}

#endif
