#include <catch/catch.hpp>
#include <etw/controller.h>
#include <string>

using std::string;
using std::wstring;
using ez_etw::controller;
using ez_etw::properties;
using ez_etw::kernel_flags;
using ez_etw::status;
using ez_etw::log_mode;

static const wstring CONTROLLER_NAME_GENERIC(L"ez_etw__controller_test");

SCENARIO("construction of a controller needs a name") {
    GIVEN("the kernel logger name") {
        controller ctrl(controller::KERNEL_NAME);
        THEN("it is a kernel logger controller") {
            REQUIRE(ctrl.is_kernel_logger());
            REQUIRE_FALSE(ctrl.is_running());
        }
    }
    GIVEN("a generic name") {
        controller ctrl(CONTROLLER_NAME_GENERIC);
        THEN("it is not a kernel logger controller") {
            REQUIRE_FALSE(ctrl.is_kernel_logger());
            REQUIRE_FALSE(ctrl.is_running());
        }
    }
}

SCENARIO("a controller can be started and stopped") {
    GIVEN("generic arguments") {
        controller ctrl(CONTROLLER_NAME_GENERIC);
        properties props;
        props.set_log_mode(log_mode::real_time);
        THEN("it cannot be stopped if it is not running") {
            REQUIRE_FALSE(ctrl.is_running());
            status ctrl_status(ctrl.stop());
            REQUIRE(ctrl_status == status::NOT_RUNNING);
        }
        THEN("it can start") {
            status ctrl_status(ctrl.start(props));
            REQUIRE(ctrl_status == status::SUCCESS);
            REQUIRE(ctrl.is_running());
            AND_WHEN("it is asked to stop") {
                ctrl_status = ctrl.stop();
                AND_THEN("it is stopped") {
                    REQUIRE(ctrl_status == status::SUCCESS);
                    REQUIRE_FALSE(ctrl.is_running());
                }
            }
        }
        WHEN("two context with the same name starts") {
            controller ctrl_dup(CONTROLLER_NAME_GENERIC);
            status status_ctrl(ctrl.start(props));
            status status_ctrl_dup(ctrl.start(props));
            THEN("only the first context be started") {
                REQUIRE(status_ctrl == status::SUCCESS);
                REQUIRE(status_ctrl_dup == status::ALREADY_EXIST);
                REQUIRE(ctrl.is_running());
                REQUIRE_FALSE(ctrl_dup.is_running());
                WHEN("both are asked to stop") {
                    status_ctrl = ctrl.stop();
                    status_ctrl_dup = ctrl_dup.stop();
                    THEN("only the one that was started can stop") {
                        REQUIRE(status_ctrl == status::SUCCESS);
                        REQUIRE(status_ctrl_dup == status::NOT_RUNNING);
                        REQUIRE_FALSE(ctrl.is_running());
                        REQUIRE_FALSE(ctrl_dup.is_running());
                    }
                }
            }
        }
    }
}

SCENARIO("starting a controller needs the right properties") {
    GIVEN("the kernel logger name") {
        controller ctrl(controller::KERNEL_NAME);
        WHEN("properties are created with flags") {
            properties props(kernel_flags::process);
            props.set_log_mode(log_mode::real_time);
            THEN("the kernel controller is started") {
                status ctrl_status(ctrl.start(props));
                REQUIRE(ctrl_status == status::SUCCESS);
                REQUIRE(ctrl.is_running());
                AND_WHEN("it is asked to stop") {
                    ctrl_status = ctrl.stop();
                    THEN("it stops") {
                        REQUIRE(ctrl_status == status::SUCCESS);
                        REQUIRE_FALSE(ctrl.is_running());
                    }
                }
            }
        }
        WHEN("properties are created without flags") {
            properties props;
            props.set_log_mode(log_mode::real_time);
            THEN("the kernel controller is not started") {
                status ctrl_status(ctrl.start(props));
                REQUIRE(ctrl_status == status::INVALID_ARGUMENT);
                REQUIRE_FALSE(ctrl.is_running());
                AND_WHEN("it is asked to stop") {
                    ctrl_status = ctrl.stop();
                    THEN("it cannot stop") {
                        REQUIRE(ctrl_status == status::NOT_RUNNING);
                        REQUIRE_FALSE(ctrl.is_running());
                    }
                }
            }
        }
        WHEN("properties are created with none flags") {
            properties props(kernel_flags::none);
            props.set_log_mode(log_mode::real_time);
            THEN("the kernel controller is not started") {
                status ctrl_status(ctrl.start(props));
                REQUIRE(ctrl_status == status::INVALID_ARGUMENT);
                REQUIRE_FALSE(ctrl.is_running());
                AND_WHEN("it is asked to stop") {
                    ctrl_status = ctrl.stop();
                    THEN("it cannot stop") {
                        REQUIRE(ctrl_status == status::NOT_RUNNING);
                        REQUIRE_FALSE(ctrl.is_running());
                    }
                }
            }
        }
    }
    GIVEN("a generic name") {
        controller ctrl(CONTROLLER_NAME_GENERIC);
        WHEN("properties are created with flags") {
            properties props(kernel_flags::process);
            props.set_log_mode(log_mode::real_time);
            THEN("the generic controller is started") {
                status ctrl_status(ctrl.start(props));
                REQUIRE(ctrl_status == status::INVALID_ARGUMENT);
                REQUIRE_FALSE(ctrl.is_running());
                AND_WHEN("it is asked to stop") {
                    ctrl_status = ctrl.stop();
                    THEN("it cannot stop") {
                        REQUIRE(ctrl_status == status::NOT_RUNNING);
                        REQUIRE_FALSE(ctrl.is_running());
                    }
                }
            }
        }
        WHEN("properties are created without flags") {
            properties props;
            props.set_log_mode(log_mode::real_time);
            THEN("the generic controller is started") {
                status ctrl_status(ctrl.start(props));
                REQUIRE(ctrl_status == status::SUCCESS);
                REQUIRE(ctrl.is_running());
                AND_WHEN("it is asked to stop") {
                    ctrl_status = ctrl.stop();
                    THEN("it stops") {
                        REQUIRE(ctrl_status == status::SUCCESS);
                        REQUIRE_FALSE(ctrl.is_running());
                    }
                }
            }
        }
        AND_WHEN("properties are created with none flags") {
            properties props(kernel_flags::none);
            props.set_log_mode(log_mode::real_time);
            THEN("the generic controller is started") {
                status ctrl_status(ctrl.start(props));
                REQUIRE(ctrl_status == status::SUCCESS);
                REQUIRE(ctrl.is_running());
                AND_WHEN("it is asked to stop") {
                    ctrl_status = ctrl.stop();
                    THEN("it stops") {
                        REQUIRE(ctrl_status == status::SUCCESS);
                        REQUIRE_FALSE(ctrl.is_running());
                    }
                }
            }
        }
    }
}
