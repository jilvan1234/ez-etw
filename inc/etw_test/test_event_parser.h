#ifndef EZETW_TESTEVENTPARSER_H
#define EZETW_TESTEVENTPARSER_H

#include <etw/event_parser.h>

static const GUID test_guid0 = { 0x1, 0x2, 0x3, 0x4 };
static const GUID test_guid1 = { 0x5, 0x6, 0x7, 0x8 };
static const std::string test_buffer("hello world!");
static const unsigned long long test_timestamp = 0;

struct test_event : public ez_etw::parsed_event {
	test_event(const GUID& guid, unsigned int pid)
    :parsed_event(ez_etw::event(guid, test_timestamp, pid, test_buffer.c_str(), test_buffer.length())) {
	}
	virtual ~test_event() = default;
};

struct test_event_parser : public ez_etw::event_parser {
	test_event_parser(const GUID& evt_guid)
	:event_parser(evt_guid) {
	}
	virtual ~test_event_parser() = default;
private:
	virtual bool parse_event(const std::shared_ptr<ez_etw::event>& evt, std::deque<std::shared_ptr<ez_etw::parsed_event>>& events) const {
		events.emplace_back(std::make_shared<test_event>(evt->get_guid(), evt->get_process_id()));
		return true;
	}
};

#endif
