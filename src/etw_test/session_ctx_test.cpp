#include <catch/catch.hpp>
#include "../etw/inc/session_ctx.h"
#include <etw/controller.h>
#include <etw_test/test_event_parser.h>

//SCENARIO("TEST SESSION CTX") {
//    ez_etw::controller ctrl(ez_etw::controller::KERNEL_NAME);
//    ez_etw::properties props(ez_etw::log_mode::real_time, ez_etw::kernel_flags::process);
//    if(ez_etw::status::SUCCESS == ctrl.start(props)) {
//        ez_etw::session_ctx session(ez_etw::controller::KERNEL_NAME, FALSE, ez_etw::log_mode::real_time);
//        Sleep(5000);
//    }
//    ctrl.stop();
//}
using std::wstring;
using ez_etw::event_parser;
using ez_etw::event;
using ez_etw::parsed_event;
static const wstring GENERIC_SESSION_NAME(L"ez_etw__session_ctx_test");

TEST_CASE("a new session context is not running by default", "[session_ctx]") {
    GIVEN("consuming from a kernel session") {
        ez_etw::session_ctx session_ctx(ez_etw::controller::KERNEL_NAME, FALSE, ez_etw::log_mode::real_time);
        THEN("the session context is not running") {
            REQUIRE_FALSE(session_ctx.is_running());
        }
    }
    GIVEN("consumign from a generic session") {
        ez_etw::session_ctx session_ctx(GENERIC_SESSION_NAME, FALSE, ez_etw::log_mode::real_time);
        THEN("the session context is not running") {
            REQUIRE_FALSE(session_ctx.is_running());
        }
    }
}

TEST_CASE("a session context can be started to log events", "[session_ctx]") {
	ez_etw::session_ctx session_ctx(ez_etw::controller::KERNEL_NAME, FALSE, ez_etw::log_mode::real_time);
	GIVEN("that the session was not started") {
		THEN("it cannot be stopped") {
			REQUIRE_FALSE(session_ctx.stop());
		}
	}
	GIVEN("that the session is started") {
		REQUIRE(session_ctx.start());
		THEN("it can be stopped") {
			REQUIRE(session_ctx.stop());
		}
	}
}

TEST_CASE("event parser can be added to a session context", "[session_ctx]") {
	ez_etw::session_ctx session_ctx(ez_etw::controller::KERNEL_NAME, FALSE, ez_etw::log_mode::real_time);
	auto parser = std::make_shared<test_event_parser>(test_guid0);
	GIVEN("that the session is started") {
		REQUIRE(session_ctx.start());
		THEN("event parsers cannot be added") {
			REQUIRE_FALSE(session_ctx.parsers_add(parser));
		}
	}
	GIVEN("that the session is not running") {
		THEN("event parser can be added") {
			REQUIRE(session_ctx.parsers_add(parser));
			AND_WHEN("a parser with the same guid is added") {
				REQUIRE_FALSE(session_ctx.parsers_add(parser));
			}
			REQUIRE(session_ctx.start());
		}
	}
}