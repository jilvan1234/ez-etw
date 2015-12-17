#ifndef EZETW_TESTEVENTPARSER_H
#define EZETW_TESTEVENTPARSER_H

#include <etw/event_parser.h>

static const GUID test_guid0 = { 0x1, 0x2, 0x3, 0x4 };
static const GUID test_guid1 = { 0x5, 0x6, 0x7, 0x8 };

struct test_event : public ez_etw::parsed_event {
	test_event(const GUID& guid, unsigned int pid)
	:m_pid(pid) {
	}
	virtual ~test_event() = default;
	unsigned int m_pid;
};

struct test_event_parser : public ez_etw::event_parser {
	test_event_parser(const GUID& evt_guid)
	:event_parser(evt_guid) {
	}
	virtual ~test_event_parser() = default;
private:
	virtual bool parse_event(const std::shared_ptr<ez_etw::event>& evt, std::deque<std::shared_ptr<ez_etw::parsed_event>>& events) const {
		events.emplace_back(std::make_shared<test_event>(evt->m_guid, evt->m_process_id));
		return true;
	}
};

#endif
