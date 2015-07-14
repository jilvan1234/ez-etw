#include <catch/catch.hpp>
#include "../../src/etw/inc/properties_buffer.h"
#include <etw/properties.h>
#include <string>

using std::string;
using std::memcmp;
using ez_etw::properties_buffer;
using ez_etw::kernel_flags;

static const string SCENARIO_NAME("properties_buffer");
#define NEW_SCENARIO(scenario_desc) SCENARIO(scenario_desc, SCENARIO_NAME.c_str())

NEW_SCENARIO("initialization") {
    GIVEN("properties with no flag") {
        kernel_flags flags(kernel_flags::none);
        properties_buffer props(flags);
        THEN("it is not a kernel logger") {
            REQUIRE_FALSE(InlineIsEqualGUID(props.get_struct()->Wnode.Guid, SystemTraceControlGuid));
        }
    }
    GIVEN("properties with process flag") {
        kernel_flags flags(kernel_flags::process);
        properties_buffer props(flags);
        THEN("it is a kernel logger") {
            REQUIRE(InlineIsEqualGUID(props.get_struct()->Wnode.Guid, SystemTraceControlGuid));
            REQUIRE(props.get_struct()->EnableFlags == kernel_flags::process);
        }
    }
}

NEW_SCENARIO("a pointer to the internal buffer is available") {
    GIVEN("a kernel logger") {
        kernel_flags flags(kernel_flags::process);
        properties_buffer props(flags);
        THEN("the pointer should not be null") {
            REQUIRE_FALSE(props.get_buffer() == nullptr);
            REQUIRE(static_cast<void*>(props.get_buffer()) == static_cast<void*>(props.get_struct()));
        }
    }
    GIVEN("not a kernel logger") {
        kernel_flags flags(kernel_flags::none);
        properties_buffer props(flags);
        THEN("the pointer should not be null") {
            REQUIRE_FALSE(props.get_buffer() == nullptr);
            REQUIRE(static_cast<void*>(props.get_buffer()) == static_cast<void*>(props.get_struct()));
        }
    }
}

NEW_SCENARIO("a pointer to the internal structure is available") {
    GIVEN("a kernel logger") {
        kernel_flags flags(kernel_flags::process);
        properties_buffer props(flags);
        THEN("the pointer should not be null") {
            REQUIRE_FALSE(props.get_struct() == nullptr);
            REQUIRE(static_cast<void*>(props.get_buffer()) == static_cast<void*>(props.get_struct()));
        }
    }
    GIVEN("not a kernel logger") {
        kernel_flags flags(kernel_flags::none);
        properties_buffer props(flags);
        THEN("the pointer should not be null") {
            REQUIRE_FALSE(props.get_struct() == nullptr);
            REQUIRE(static_cast<void*>(props.get_buffer()) == static_cast<void*>(props.get_struct()));
        }
    }
}
