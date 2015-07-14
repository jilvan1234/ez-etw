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

static const string SCENARIO_NAME("properties");
#define NEW_SCENARIO(scenario_desc) SCENARIO(scenario_desc, SCENARIO_NAME.c_str())

NEW_SCENARIO("initialization of the buffer") {
    GIVEN("no flags are provided") {
        const properties props;
        THEN("it is not a kernel logger") {
            auto buff = props.get_ptr();
            REQUIRE_FALSE(buff == nullptr);
            REQUIRE(buff->get_struct()->EnableFlags == kernel_flags::none);
        }
    }
    GIVEN("none flags are provided") {
        const kernel_flags flags(kernel_flags::none);
        const properties props(flags);
        THEN("it is not a kernel logger") {
            auto buff = props.get_ptr();
            REQUIRE_FALSE(buff == nullptr);
            REQUIRE(buff->get_struct()->EnableFlags == kernel_flags::none);
        }
    }
    GIVEN("flags are provided") {
        const kernel_flags flags(kernel_flags::process);
        const properties props(flags);
        THEN("it is a kernel logger") {
            auto buff = props.get_ptr();
            REQUIRE_FALSE(buff == nullptr);
            REQUIRE(buff->get_struct()->EnableFlags == kernel_flags::process);
        }
    }
}

NEW_SCENARIO("different resolution speed are available") {
    properties props;
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

NEW_SCENARIO("different log mode are available") {
    properties props;
    GIVEN("real time is given") {
        log_mode mode(log_mode::real_time);
        props.set_log_mode(mode);
        THEN("the real time value is in the structure") {
            auto buff = props.get_ptr();
            REQUIRE_FALSE(buff == nullptr);
            REQUIRE(buff->get_struct()->LogFileMode == log_mode::real_time);
        }
    }
}
