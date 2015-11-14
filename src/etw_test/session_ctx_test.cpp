#include <catch/catch.hpp>
#include "../etw/inc/session_ctx.h"

#include <etw/controller.h>

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
static const wstring GENERIC_SESSION_NAME(L"ez_etw__session_ctx_test");

SCENARIO("a new session context is not running by default") {
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
