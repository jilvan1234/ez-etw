#include <catch/catch.hpp>
#include <ETW/event.h>

#include <Windows.h>
#include <Wmistr.h>
#include <Evntrace.h>

using ez_etw::event;

#define ARRAY_COUNT(arr) ((sizeof(arr) / sizeof(*arr)))

GUID get_guid() {
	GUID guid;
	guid.Data1 = 0x1;
	guid.Data2 = 0x2;
	guid.Data3 = 0x3;
	guid.Data4[0] = 0x4;
	guid.Data4[1] = 0x5;
	guid.Data4[2] = 0x6;
	guid.Data4[3] = 0x7;
	guid.Data4[4] = 0x8;
	guid.Data4[5] = 0x9;
	guid.Data4[6] = 0xa;
	guid.Data4[7] = 0xb;
	return guid;
}

TEST_CASE("event creation", "[event]") {
	static const long long timestamp = 123;
	std::string buffer = "Hello world!";
    event evt(get_guid(), timestamp, buffer.c_str(), buffer.length());
	REQUIRE(evt.get_timestamp() == timestamp);
	REQUIRE(IsEqualGUID(evt.get_guid(), get_guid()));
	auto event_buffer = evt.get_buffer();
	REQUIRE(*evt.get_buffer() == buffer);
	REQUIRE(evt.get_type() == event::type::info);
}

TEST_CASE("event type supported", "[event]") {
	std::string buffer = "";
	event evt(get_guid(), 0, buffer.c_str(), buffer.length());
	REQUIRE(evt.set_type(EVENT_TRACE_TYPE_START));
	REQUIRE(evt.get_type() == event::type::start);
	REQUIRE(evt.set_type(EVENT_TRACE_TYPE_END));
    REQUIRE(evt.get_type() == event::type::end);
	REQUIRE(evt.set_type(EVENT_TRACE_TYPE_DC_START));
    REQUIRE(evt.get_type() == event::type::data_collection_start);
	REQUIRE(evt.set_type(EVENT_TRACE_TYPE_DC_END));
    REQUIRE(evt.get_type() == event::type::data_collection_end);
	REQUIRE(evt.set_type(EVENT_TRACE_TYPE_STOP));
    REQUIRE(evt.get_type() == event::type::end);
	REQUIRE(evt.set_type(EVENT_TRACE_TYPE_INFO));
    REQUIRE(evt.get_type() == event::type::info);
	REQUIRE_FALSE(evt.set_type(EVENT_TRACE_TYPE_EXTENSION));
	REQUIRE_FALSE(evt.set_type(EVENT_TRACE_TYPE_REPLY));
	REQUIRE_FALSE(evt.set_type(EVENT_TRACE_TYPE_DEQUEUE));
	REQUIRE_FALSE(evt.set_type(EVENT_TRACE_TYPE_RESUME));
	REQUIRE_FALSE(evt.set_type(EVENT_TRACE_TYPE_CHECKPOINT));
	REQUIRE_FALSE(evt.set_type(EVENT_TRACE_TYPE_SUSPEND));
	REQUIRE_FALSE(evt.set_type(EVENT_TRACE_TYPE_WINEVT_SEND));
	REQUIRE_FALSE(evt.set_type(EVENT_TRACE_TYPE_WINEVT_RECEIVE));
	REQUIRE(evt.get_type() == event::type::info);
}
