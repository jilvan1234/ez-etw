#include <catch/catch.hpp>
#include <etw_test/test_event_parser.h>

using ez_etw::event_parser;
using ez_etw::event;
using std::make_shared;

TEST_CASE("cannot remove events from an event parser without events", "[event_parser]") {
	test_event_parser parser(test_guid0);
	REQUIRE(parser.events_empty());
	parser.remove_event();
	REQUIRE(parser.events_empty());
}

TEST_CASE("can parse event", "[event_parser]") {
	test_event_parser parser(test_guid0);
	WHEN("the event have the same guid than the parser") {
		auto evt = make_shared<event>(test_guid0, 0, nullptr, 0);
		THEN("event can be parsed") {
			REQUIRE(parser.parse(evt));
			REQUIRE_FALSE(parser.events_empty());
			AND_THEN("an event can be removed") {
				parser.remove_event();
				REQUIRE(parser.events_empty());
			}
			AND_THEN("event can be accessed") {
				const test_event* const top_evt = static_cast<const test_event* const>(parser.get_event());
				REQUIRE(top_evt != nullptr);
				REQUIRE_FALSE(parser.events_empty());
			}
			AND_WHEN("more event can be parsed at a time") {
				evt = make_shared<event>(test_guid0, 0, nullptr, 0);
				REQUIRE(parser.parse(evt));
				REQUIRE_FALSE(parser.events_empty());
				AND_THEN("events can be removed") {
					parser.remove_event();
					REQUIRE_FALSE(parser.events_empty());
					parser.remove_event();
					REQUIRE(parser.events_empty());
				}
				AND_THEN("events can be accessed") {
					const test_event* const top_evt0 = static_cast<const test_event* const>(parser.get_event());
					REQUIRE(top_evt0 != nullptr);
					parser.remove_event();
					REQUIRE_FALSE(parser.events_empty());
					const test_event* const top_evt1 = static_cast<const test_event* const>(parser.get_event());
					REQUIRE(top_evt1 != nullptr);
					parser.remove_event();
					REQUIRE(parser.events_empty());
				}
			}
		}

	}
	WHEN("the event does not have the same guid than the parser") {
		auto evt = make_shared<event>(test_guid1, 0, nullptr, 0);
		THEN("event cannot be parsed") {
			REQUIRE_FALSE(parser.parse(evt));
			REQUIRE(parser.events_empty());
		}
	}
}
