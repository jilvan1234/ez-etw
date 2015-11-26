#include <catch/catch.hpp>
#include <ETW/event.h>

using ez_etw::event;

#define ARRAY_COUNT(arr) ((sizeof(arr) / sizeof(*arr)))

TEST_CASE("event creation", "[event]") {
	static const unsigned int process_id = 42;
	char buffer[] = "Hello world!";
	event evt(process_id, buffer, ARRAY_COUNT(buffer) - 1 * sizeof(*buffer));
	REQUIRE(evt.m_process_id == process_id);
	std::string event_buffer;
	evt.m_buffer >> event_buffer;

	REQUIRE(evt.m_buffer.str() == buffer);
}
