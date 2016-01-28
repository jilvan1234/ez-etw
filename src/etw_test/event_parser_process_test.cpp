#include <catch/catch.hpp>
#include <etw/event_parsers_process.h>
#include <etw/session.h>
#include <etw/controller.h>
#include <etw_test/utils.h>
#include <etw/parsed_event_process.h>

using ez_etw::session;
using ez_etw::controller;
using ez_etw::properties;
using std::begin;
using std::end;
using std::count_if;

#include <iostream>

TEST_CASE("parse process events", "[event_parsers_process]") {
	GIVEN("a process parser added to a running session") {
		controller ctrl(controller::kernel_name);
		ctrl.stop();
		REQUIRE(ctrl.start(properties(ez_etw::log_mode::real_time, ez_etw::kernel_flags::process)) == ez_etw::status::success);
		session session_ctx(ez_etw::controller::kernel_name, false, ez_etw::log_mode::real_time);
		auto parser = std::make_shared<ez_etw::event_parsers::process>();
		REQUIRE(session_ctx.parsers_add(parser));
		REQUIRE(session_ctx.start());
		WHEN("a process is launched") {
			uintptr_t process_handle;
			// count the occurences of calc
			auto events = parser->get_events();
			std::cout << "Event counts: " << events.size() << std::endl;
			size_t process_count = count_if(
				begin(parser->get_events()), 
				end(parser->get_events()), 
				[&process_count](decltype(events)::const_reference evt) {
					auto ptr_evt = evt.get();
					auto ptr_process_evt = static_cast<ez_etw::parsed_events::parsed_event_process*>(ptr_evt);
					std::cout << ptr_process_evt->get_image_filename() << std::endl;
					return ptr_process_evt->get_image_filename() == "calc.exe";
			});

			REQUIRE(test_utils::process::launch("c:\\windows\\system32\\calc.exe", process_handle));
			REQUIRE(test_utils::process::terminate(process_handle));
		}
		REQUIRE(session_ctx.stop());
		REQUIRE(ez_etw::status::success == ctrl.stop());
	}
}
