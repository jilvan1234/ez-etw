#include <catch/catch.hpp>
#include <etw/event_parsers_process.h>
#include <etw/session.h>
#include <etw/controller.h>
#include <etw_test/utils.h>
#include <etw/process.h>
#include <string>
#include <utility>

using ez_etw::session;
using ez_etw::controller;
using ez_etw::properties;
using std::begin;
using std::end;
using std::count_if;
using std::string;
using ez_etw::event;
using std::make_pair;
using std::pair;

static std::pair<bool, event::type> find_target(std::shared_ptr<ez_etw::event_parsers::process> parser, const uintptr_t process_id, const string& process_name) {
    static const size_t iteration_count_max = 60;
    bool target_found = false;
    size_t iteration_count = 0;
    event::type type = event::type::info;
    do {
        using namespace std::literals;
        auto events = parser->get_events();
        target_found = std::find_if(
            begin(events), 
            end(events), 
            [process_id, process_name, &type](decltype(events)::const_reference evt) {
            auto ptr_evt = evt.get();
            auto ptr_process_evt = static_cast<ez_etw::parsed_events::process*>(ptr_evt);
            type = ptr_evt->get_type();
            return ptr_process_evt->get_image_filename() == process_name &&
                ptr_process_evt->get_pid() == process_id;
        }) != end(events);
        if(!target_found) {
            std::this_thread::sleep_for(1s);
            ++iteration_count;
        }
    } while(!target_found && iteration_count < iteration_count_max);
    return std::make_pair(target_found, type);

}

TEST_CASE("parse process events", "[event_parsers_process]") {
	GIVEN("a process parser added to a running session") {
        auto target_found_start(make_pair(false, event::type::info));
        auto target_found_end(make_pair(false, event::type::info));
		controller ctrl(controller::kernel_name);
		ctrl.stop();
		REQUIRE(ctrl.start(properties(ez_etw::log_mode::real_time, ez_etw::kernel_flags::process)) == ez_etw::status::success);
		session session_ctx(ez_etw::controller::kernel_name, false, ez_etw::log_mode::real_time);
		auto parser = std::make_shared<ez_etw::event_parsers::process>();
		REQUIRE(session_ctx.parsers_add(parser));
		REQUIRE(session_ctx.start());
		WHEN("a process is launched and closed") {
			static const std::string process_name("cmd.exe");
			static const std::string process_path("c:\\windows\\system32\\" + process_name);
			uintptr_t process_handle;
			uintptr_t process_id;
			REQUIRE(test_utils::process::launch(process_path, process_handle, process_id));
            target_found_start = find_target(parser, process_id, process_name);
			REQUIRE(test_utils::process::terminate(process_handle));
            target_found_end = find_target(parser, process_id, process_name);
		}
        REQUIRE(ez_etw::status::success == ctrl.stop());
        REQUIRE(session_ctx.stop());
        REQUIRE(target_found_start.first);
        REQUIRE(target_found_start.second == event::type::start);
        REQUIRE(target_found_end.first);
        REQUIRE(target_found_end.second == event::type::end);
	}
}
