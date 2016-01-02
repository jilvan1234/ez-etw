#include <catch/catch.hpp>
#include <etw/parsed_event.h>
#include <etw_test/test_event_parser.h>

using ez_etw::parsed_event;
using ez_etw::event;

TEST_CASE("construction requires arguments", "[parsed_event]") {
    parsed_event evt0(event(test_guid0, 0, 0, test_buffer.c_str(), test_buffer.length()));
    REQUIRE(evt0.get_timestamp() == 0);
    REQUIRE(evt0.get_pid() == 0);
    parsed_event evt1(event(test_guid0, 1, 42, test_buffer.c_str(), test_buffer.length()));
    REQUIRE(evt1.get_timestamp() == 1);
    REQUIRE(evt1.get_pid() == 42);
}