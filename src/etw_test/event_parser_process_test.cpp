#include <catch/catch.hpp>
#include <etw/event_parsers_process.h>
#include <etw/session.h>
#include <etw/controller.h>
#include <etw_test/utils.h>
#include <etw/parsed_event_process.h>
#include <string>

using ez_etw::session;
using ez_etw::controller;
using ez_etw::properties;
using std::begin;
using std::end;
using std::count_if;
using std::string;

TEST_CASE("parse process events", "[event_parsers_process]") {
	GIVEN("a process parser added to a running session") {
        bool target_found = false;
		controller ctrl(controller::kernel_name);
		ctrl.stop();
		REQUIRE(ctrl.start(properties(ez_etw::log_mode::real_time, ez_etw::kernel_flags::process)) == ez_etw::status::success);
		session session_ctx(ez_etw::controller::kernel_name, false, ez_etw::log_mode::real_time);
		auto parser = std::make_shared<ez_etw::event_parsers::process>();
		REQUIRE(session_ctx.parsers_add(parser));
		REQUIRE(session_ctx.start());
		WHEN("a process is launched") {
			static const size_t iteration_count_max = 60;
			static const std::string process_name("cmd.exe");
			static const std::string process_path("c:\\windows\\system32\\" + process_name);
			uintptr_t process_handle;
			uintptr_t process_id;
			REQUIRE(test_utils::process::launch(process_path, process_handle, process_id));
			size_t iteration_count = 0;
			do {
				using namespace std::literals;
				auto events = parser->get_events();
				target_found = std::find_if(
					begin(events), 
					end(events), 
					[process_id](decltype(events)::const_reference evt) {
						auto ptr_evt = evt.get();
						auto ptr_process_evt = static_cast<ez_etw::parsed_events::parsed_event_process*>(ptr_evt);
						return ptr_process_evt->get_image_filename() == process_name &&
							ptr_process_evt->get_pid() == process_id;
				}) != end(events);
				if(!target_found) {
					std::this_thread::sleep_for(1s);
					++iteration_count;
				}
			} while(!target_found && iteration_count < iteration_count_max);
			REQUIRE(test_utils::process::terminate(process_handle));
		}
		REQUIRE(session_ctx.stop());
		REQUIRE(ez_etw::status::success == ctrl.stop());
        REQUIRE(target_found);
	}
}
