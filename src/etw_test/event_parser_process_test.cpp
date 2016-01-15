#include <catch/catch.hpp>
#include <etw/event_parsers_process.h>
#include <etw/session.h>
#include <etw/controller.h>
#include <etw_test/utils.h>

using ez_etw::event_parsers::process;
using ez_etw::session;
using ez_etw::controller;
using ez_etw::properties;

TEST_CASE("parse process events", "[event_parsers_process]") {
	controller ctrl(controller::kernel_name);
	ctrl.stop();
	REQUIRE(ctrl.start(properties(ez_etw::log_mode::real_time, ez_etw::kernel_flags::process)) == ez_etw::status::success);
	GIVEN("a kernel logging controller")
	{
		session session_ctx(ez_etw::controller::kernel_name, false, ez_etw::log_mode::real_time);
		auto parser = std::make_shared<process>();
		GIVEN("a session started with a process parser") {
			REQUIRE(session_ctx.parsers_add(parser));
			REQUIRE(session_ctx.start());
			WHEN("a process is launched") {
				/*
				uintptr_t process_handle;
				REQUIRE(test_utils::process::launch("c:\\windows\\system32\\calc.exe", process_handle));
				THEN("expect a new event to be parsed") {
					system("pause");
				}
				REQUIRE(test_utils::process::terminate(process_handle));
				*/
			}
		}
		REQUIRE(session_ctx.stop());
	}
	REQUIRE(ctrl.stop() == ez_etw::status::success);
}
