#include <catch/catch.hpp>
#include <etw/parsed_event.h>
#include <etw_test/test_event_parser.h>

using ez_etw::parsed_event;
using ez_etw::event;

TEST_CASE("construction requires arguments", "[parsed_event]") {
    event evt0(test_guid0, 0, test_buffer.c_str(), test_buffer.length());
    parsed_event parsed_evt0(evt0, 0);
    REQUIRE(parsed_evt0.get_timestamp() == 0);
    REQUIRE(parsed_evt0.get_version() == 0);
    event evt1(test_guid0, 1, test_buffer.c_str(), test_buffer.length());
    evt1.set_version(42);
    parsed_event parsed_evt1(evt1, 0);
    REQUIRE(parsed_evt1.get_timestamp() == 1);
    REQUIRE(parsed_evt1.get_version() == 42);
}