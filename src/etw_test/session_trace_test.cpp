#include <catch/catch.hpp>
#include <exception>
#include "../etw/inc/session_trace.h"
#include <etw/properties.h>

using ez_etw::session_trace;
using ez_etw::log_mode;
static const std::wstring session_trace_name(L"session_name");

void WINAPI cb_event(PEVENT_TRACE event) {
    (event);
}

ULONG WINAPI cb_buffer(PEVENT_TRACE_LOGFILEW event) {
    (event);
    return 1;
}

TEST_CASE("constructor of a session_trace", "[session_trace]") {
    WHEN("invalid callbacks are given") {
        REQUIRE_THROWS_AS(session_trace(session_trace_name, true, log_mode::real_time, nullptr, nullptr), std::invalid_argument);
        REQUIRE_THROWS_AS(session_trace(session_trace_name, true, log_mode::real_time, cb_event, nullptr), std::invalid_argument);
        REQUIRE_THROWS_AS(session_trace(session_trace_name, true, log_mode::real_time, nullptr, cb_buffer), std::invalid_argument);
        REQUIRE_THROWS_AS(session_trace(session_trace_name, false, log_mode::real_time, nullptr, nullptr), std::invalid_argument);
        REQUIRE_THROWS_AS(session_trace(session_trace_name, false, log_mode::real_time, cb_event, nullptr), std::invalid_argument);
        REQUIRE_THROWS_AS(session_trace(session_trace_name, false, log_mode::real_time, nullptr, cb_buffer), std::invalid_argument);
    } 
    WHEN("valid arguments are given") {
        AND_WHEN("it consume from file") {
            session_trace session(session_trace_name, true, log_mode::real_time, cb_event, cb_buffer);
            auto trace_logfile = session.get_trace_logfile();
            REQUIRE((uintptr_t)trace_logfile->BufferCallback == (uintptr_t)cb_buffer);
            REQUIRE((uintptr_t)trace_logfile->EventCallback == (uintptr_t)cb_event);
            REQUIRE(trace_logfile->LogFileMode == log_mode::real_time);
            REQUIRE(trace_logfile->LogFileName == session_trace_name);
        }
        AND_WHEN("it does not consume from file") {
            session_trace session(session_trace_name, false, log_mode::real_time, cb_event, cb_buffer);
            auto trace_logfile = session.get_trace_logfile();
            REQUIRE((uintptr_t)trace_logfile->BufferCallback == (uintptr_t)cb_buffer);
            REQUIRE((uintptr_t)trace_logfile->EventCallback == (uintptr_t)cb_event);
            REQUIRE(trace_logfile->LogFileMode == log_mode::real_time);
            REQUIRE(trace_logfile->LoggerName == session_trace_name);
        }
    }
}
