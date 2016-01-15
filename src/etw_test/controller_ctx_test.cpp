#include <catch/catch.hpp>
#include "../src/etw/inc/controller_ctx.h"
#include <string>

using std::string;
using std::wstring;
using ez_etw::controller_ctx;
using ez_etw::properties;
using ez_etw::kernel_flags;
using ez_etw::status;
using ez_etw::log_mode;

static const wstring CONTROLLER_NAME_GENERIC(L"ez_etw__controller_ctx_test");
static const wstring& CONTROLLER_NAME_KERNEL(KERNEL_LOGGER_NAMEW);

static const string TEST_TAG = "[controller_ctx]";
TEST_CASE("construction of a controller context needs a name", TEST_TAG.c_str()) {
    GIVEN("the kernel logger name") {
        controller_ctx ctrl(CONTROLLER_NAME_KERNEL);
        THEN("it is a kernel logger controller") {
            REQUIRE(ctrl.is_kernel_logger());
            REQUIRE_FALSE(ctrl.is_running());
        }
    }
    GIVEN("a generic name") {
        controller_ctx ctrl(CONTROLLER_NAME_GENERIC);
        THEN("it is not a kernel logger controller") {
            REQUIRE_FALSE(ctrl.is_kernel_logger());
            REQUIRE_FALSE(ctrl.is_running());
        }
    }
}

TEST_CASE("a controller context can be started and stopped", TEST_TAG.c_str()) {
    GIVEN("generic arguments") {
        controller_ctx ctrl(CONTROLLER_NAME_GENERIC);
        properties props(log_mode::real_time);
        THEN("it cannot be stopped if it is not running") {
            REQUIRE_FALSE(ctrl.is_running());
            status ctrl_status(ctrl.stop());
            REQUIRE(ctrl_status == status::not_running);
        }
        THEN("it can start") {
            status ctrl_status(ctrl.start(props));
            REQUIRE(ctrl_status == status::success);
            REQUIRE(ctrl.is_running());
            AND_WHEN("it is asked to stop") {
                ctrl_status = ctrl.stop();
                AND_THEN("it is stopped") {
                    REQUIRE(ctrl_status == status::success);
                    REQUIRE_FALSE(ctrl.is_running());
                }
            }
        }
        WHEN("two context with the same name starts") {
            controller_ctx ctrl_dup(CONTROLLER_NAME_GENERIC);
            status status_ctrl(ctrl.start(props));
            status status_ctrl_dup(ctrl.start(props));
            THEN("only the first context be started") {
                REQUIRE(status_ctrl == status::success);
                REQUIRE(status_ctrl_dup == status::already_exist);
                REQUIRE(ctrl.is_running());
                REQUIRE_FALSE(ctrl_dup.is_running());
                WHEN("both are asked to stop") {
                    status_ctrl = ctrl.stop();
                    status_ctrl_dup = ctrl_dup.stop();
                    THEN("only the one that was started can stop") {
                        REQUIRE(status_ctrl == status::success);
                        REQUIRE(status_ctrl_dup == status::not_running);
                        REQUIRE_FALSE(ctrl.is_running());
                        REQUIRE_FALSE(ctrl_dup.is_running());
                    }
                }
            }
        }
    }
}

TEST_CASE("starting a controller context needs the right properties", TEST_TAG.c_str()) {
    GIVEN("the kernel logger name") {
        controller_ctx ctrl(CONTROLLER_NAME_KERNEL);
        WHEN("properties are created with flags") {
            properties props(log_mode::real_time, kernel_flags::process);
            THEN("the kernel controller is started") {
                status ctrl_status(ctrl.start(props));
                REQUIRE(ctrl_status == status::success);
                REQUIRE(ctrl.is_running());
                AND_WHEN("it is asked to stop") {
                    ctrl_status = ctrl.stop();
                    THEN("it stops") {
                        REQUIRE(ctrl_status == status::success);
                        REQUIRE_FALSE(ctrl.is_running());
                    }
                }
            }
        }
        WHEN("properties are created without flags") {
            properties props(log_mode::real_time);
            THEN("the kernel controller is not started") {
                status ctrl_status(ctrl.start(props));
                REQUIRE(ctrl_status == status::invalid_argument);
                REQUIRE_FALSE(ctrl.is_running());
                AND_WHEN("it is asked to stop") {
                    ctrl_status = ctrl.stop();
                    THEN("it cannot stop") {
                        REQUIRE(ctrl_status == status::not_running);
                        REQUIRE_FALSE(ctrl.is_running());
                    }
                }
            }
        }
        WHEN("properties are created with none flags") {
            properties props(log_mode::real_time, kernel_flags::none);
            THEN("the kernel controller is not started") {
                status ctrl_status(ctrl.start(props));
                REQUIRE(ctrl_status == status::invalid_argument);
                REQUIRE_FALSE(ctrl.is_running());
                AND_WHEN("it is asked to stop") {
                    ctrl_status = ctrl.stop();
                    THEN("it cannot stop") {
                        REQUIRE(ctrl_status == status::not_running);
                        REQUIRE_FALSE(ctrl.is_running());
                    }
                }
            }
        }
    }
    GIVEN("a generic name") {
        controller_ctx ctrl(CONTROLLER_NAME_GENERIC);
        WHEN("properties are created with flags") {
            properties props(log_mode::real_time, kernel_flags::process);
            THEN("the generic controller is started") {
                status ctrl_status(ctrl.start(props));
                REQUIRE(ctrl_status == status::invalid_argument);
                REQUIRE_FALSE(ctrl.is_running());
                AND_WHEN("it is asked to stop") {
                    ctrl_status = ctrl.stop();
                    THEN("it cannot stop") {
                        REQUIRE(ctrl_status == status::not_running);
                        REQUIRE_FALSE(ctrl.is_running());
                    }
                }
            }
        }
        WHEN("properties are created without flags") {
            properties props(log_mode::real_time);
            THEN("the generic controller is started") {
                status ctrl_status(ctrl.start(props));
                REQUIRE(ctrl_status == status::success);
                REQUIRE(ctrl.is_running());
                AND_WHEN("it is asked to stop") {
                    ctrl_status = ctrl.stop();
                    THEN("it stops") {
                        REQUIRE(ctrl_status == status::success);
                        REQUIRE_FALSE(ctrl.is_running());
                    }
                }
            }
        }
        AND_WHEN("properties are created with none flags") {
            properties props(log_mode::real_time, kernel_flags::none);
            THEN("the generic controller is started") {
                status ctrl_status(ctrl.start(props));
                REQUIRE(ctrl_status == status::success);
                REQUIRE(ctrl.is_running());
                AND_WHEN("it is asked to stop") {
                    ctrl_status = ctrl.stop();
                    THEN("it stops") {
                        REQUIRE(ctrl_status == status::success);
                        REQUIRE_FALSE(ctrl.is_running());
                    }
                }
            }
        }
    }
}
