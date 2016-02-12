#include <catch/catch.hpp>
#include <etw/event_parsers_image.h>
#include <etw/session.h>
#include <etw/controller.h>
#include <etw_test/utils.h>
#include <etw/image.h>

using ez_etw::session;
using ez_etw::controller;
using ez_etw::properties;
using namespace std::literals;

TEST_CASE("parse image events", "[event_parsers_image]") {
    GIVEN("an image parser added to a running session") {
        controller ctrl(controller::kernel_name);
        ctrl.stop();
        REQUIRE(ctrl.start(properties(ez_etw::log_mode::real_time, ez_etw::kernel_flags::image)) == ez_etw::status::success);
        session session_ctx(ez_etw::controller::kernel_name, false, ez_etw::log_mode::real_time);
        auto parser = std::make_shared<ez_etw::event_parsers::image>();
        REQUIRE(session_ctx.parsers_add(parser));
        REQUIRE(session_ctx.start());
        WHEN("a process is launched and closed") {
            static const std::string process_name("cmd.exe");
            static const std::string process_path("c:\\windows\\system32\\" + process_name);
            uintptr_t process_handle;
            uintptr_t process_id;
            REQUIRE(test_utils::process::launch(process_path, process_handle, process_id));
            
            std::this_thread::sleep_for(10s);
            REQUIRE(test_utils::process::terminate(process_handle));
        }
        REQUIRE(ez_etw::status::success == ctrl.stop());
        REQUIRE(session_ctx.stop());

    }
}
