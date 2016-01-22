#include <catch/catch.hpp>
#include <etw/session.h>
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
static const wstring session_generic_name(L"ez_etw__session_test");

TEST_CASE("a new session context is not running by default", "[session]") {
    GIVEN("consuming from a kernel session") {
        ez_etw::session session(ez_etw::controller::kernel_name, false, ez_etw::log_mode::real_time);
        THEN("the session context is not running") {
            REQUIRE_FALSE(session.is_running());
        }
    }
    GIVEN("consumign from a generic session") {
        ez_etw::session session(session_generic_name, false, ez_etw::log_mode::real_time);
        THEN("the session context is not running") {
            REQUIRE_FALSE(session.is_running());
        }
    }
}

TEST_CASE("a session context can be started to log events", "[session]") {
    ez_etw::session session(ez_etw::controller::kernel_name, false, ez_etw::log_mode::real_time);
	GIVEN("that the session was not started") {
		THEN("trying to stop it won't do anything") {
			REQUIRE(session.stop());
		}
	}
	GIVEN("that the session is started") {
		REQUIRE(session.start());
		THEN("it can be stopped") {
			REQUIRE(session.stop());
		}
	}
}

TEST_CASE("event parser can be added to a session context", "[session]") {
	auto parser = std::make_shared<test_event_parser>(test_guid0);
	GIVEN("that the session is started") {
		ez_etw::session session(ez_etw::controller::kernel_name, false, ez_etw::log_mode::real_time);
		REQUIRE(session.start());
		THEN("event parsers cannot be added") {
			REQUIRE_FALSE(session.parsers_add(parser));
		}
	}
	GIVEN("that the session is not running") {
		ez_etw::session session(ez_etw::controller::kernel_name, false, ez_etw::log_mode::real_time);
		THEN("event parser can be added") {
			REQUIRE(session.parsers_add(parser));
			AND_WHEN("a parser with the same guid is added") {
				REQUIRE_FALSE(session.parsers_add(parser));
			}
			REQUIRE(session.start());
		}
	}
}

TEST_CASE("start/stop", "[session][test]") {
	ez_etw::session session(ez_etw::controller::kernel_name, false, ez_etw::log_mode::real_time);
	REQUIRE(session.start());
	REQUIRE(session.stop());

}
