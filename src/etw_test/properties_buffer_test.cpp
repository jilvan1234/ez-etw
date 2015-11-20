#include <catch/catch.hpp>
#include "../../src/etw/inc/properties_buffer.h"
#include <etw/properties.h>
#include <string>

using std::string;
using std::memcmp;
using ez_etw::properties_buffer;
using ez_etw::kernel_flags;

static const string TEST_TAG = "[properties_buffer]";
TEST_CASE("initialization of the proeprties_buffer need flags", TEST_TAG.c_str()) {
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

TEST_CASE("a pointer to the internal buffer of the properties_buffer can be accessed", TEST_TAG.c_str()) {
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

TEST_CASE("a pointer to the internal structure of the properties buffer can be accessed", TEST_TAG.c_str()) {
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
