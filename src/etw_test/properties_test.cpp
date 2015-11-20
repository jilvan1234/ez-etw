#include <catch/catch.hpp>
#include <etw/properties.h>
#include "../etw/inc/properties_buffer.h"
#include <string>

using std::string;
using std::memcmp;
using ez_etw::properties;
using ez_etw::kernel_flags;
using ez_etw::resolution_speed;
using ez_etw::log_mode;

static const string TEST_TAG = "[properties]";
TEST_CASE("initialization of properties might need flags", TEST_TAG.c_str()) {
    GIVEN("no flags are provided") {
        const properties props(log_mode::real_time);
        THEN("it is not a kernel logger") {
            auto buff = props.get_ptr();
            REQUIRE_FALSE(buff == nullptr);
            REQUIRE(buff->get_struct()->EnableFlags == kernel_flags::none);
        }
    }
    GIVEN("none flags are provided") {
        const properties props(log_mode::real_time, kernel_flags::none);
        THEN("it is not a kernel logger") {
            auto buff = props.get_ptr();
            REQUIRE_FALSE(buff == nullptr);
            REQUIRE(buff->get_struct()->EnableFlags == kernel_flags::none);
        }
    }
    GIVEN("flags are provided") {
        const properties props(log_mode::real_time, kernel_flags::process);
        THEN("it is a kernel logger") {
            auto buff = props.get_ptr();
            REQUIRE_FALSE(buff == nullptr);
            REQUIRE(buff->get_struct()->EnableFlags == kernel_flags::process);
        }
    }
}

TEST_CASE("different resolution speed are available for the properties", TEST_TAG.c_str()) {
    properties props(log_mode::real_time);
    GIVEN("low speed is given") {
        resolution_speed speed(resolution_speed::low);
        props.set_resolution_speed(speed);
        THEN("low value is in the structure") {
            auto buff = props.get_ptr();
            REQUIRE_FALSE(buff == nullptr);
            REQUIRE(buff->get_struct()->Wnode.ClientContext == resolution_speed::low);
        }
    }
    GIVEN("high speed is given") {
        resolution_speed speed(resolution_speed::high);
        props.set_resolution_speed(speed);
        THEN("high value is in the structure") {
            auto buff = props.get_ptr();
            REQUIRE_FALSE(buff == nullptr);
            REQUIRE(buff->get_struct()->Wnode.ClientContext == resolution_speed::high);
        }
    }
    GIVEN("highest speed is given") {
        resolution_speed speed(resolution_speed::highest);
        props.set_resolution_speed(speed);
        THEN("highest value is in the structure") {
            auto buff = props.get_ptr();
            REQUIRE_FALSE(buff == nullptr);
            REQUIRE(buff->get_struct()->Wnode.ClientContext == resolution_speed::highest);
        }
    }
}

TEST_CASE("different log mode are available for the properties", TEST_TAG.c_str()) {
    GIVEN("real time is given") {
        properties props(log_mode::real_time);
        THEN("the real time value is in the structure") {
            auto buff = props.get_ptr();
            REQUIRE_FALSE(buff == nullptr);
            REQUIRE(buff->get_struct()->LogFileMode == log_mode::real_time);
        }
    }
}
