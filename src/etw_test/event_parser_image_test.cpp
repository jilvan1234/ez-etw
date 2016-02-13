#include <catch/catch.hpp>
#include <etw/event_parsers_image.h>
#include <etw/session.h>
#include <etw/controller.h>
#include <etw_test/utils.h>
#include <etw/image.h>

using ez_etw::session;
using ez_etw::controller;
using ez_etw::properties;
using ez_etw::event;
using ez_etw::parsed_event;
using namespace std::literals;
using std::begin;
using std::end;
using std::wstring;

TEST_CASE("parse image events", "[event_parsers_image]") {
    GIVEN("an image parser added to a running session") {
        controller ctrl(controller::kernel_name);
        ctrl.stop();
        REQUIRE(ctrl.start(properties(ez_etw::log_mode::real_time, ez_etw::kernel_flags::image)) == ez_etw::status::success);
        session session_ctx(ez_etw::controller::kernel_name, false, ez_etw::log_mode::real_time);
        auto parser = std::make_shared<ez_etw::event_parsers::image>();
        REQUIRE(session_ctx.parsers_add(parser));
        REQUIRE(session_ctx.start());
        static const wstring dll(L"hal.dll");
        //bool found_ntdll = false;
        size_t count_dll_hal = 0;
        WHEN("a process is launched and closed") {
            std::this_thread::sleep_for(10s);
            auto events = parser->get_events();
            count_dll_hal = std::count_if(begin(events), end(events), [](decltype(events)::const_reference evt) {
                auto ptr_evt = evt.get();
                auto ptr_image_evt = static_cast<ez_etw::parsed_events::image*>(ptr_evt);
                auto filename = ptr_image_evt->get_image_filename();
                const wchar_t* pp = filename.c_str();
                ptrdiff_t t = filename.length() - dll.length();
                wchar_t* p = &filename[t];
                return std::wmemcmp(p, dll.c_str(), dll.length()) == 0;
            });
        }
        REQUIRE(ez_etw::status::success == ctrl.stop());
        REQUIRE(session_ctx.stop());
        REQUIRE(count_dll_hal == 1);
    }
}
