#include <etw/controller.h>
#include "inc/controller_ctx.h"

#include <windef.h>
#include <winbase.h>
#include <wmistr.h>
#include <evntrace.h>

using std::make_unique;
using std::wstring;
using ez_etw::controller;
using ez_etw::controller_ctx;
using ez_etw::status;
using ez_etw::properties;

const wstring& controller::kernel_name(KERNEL_LOGGER_NAMEW);

controller::controller(const wstring& name)
:m_ctx(std::make_unique<controller_ctx>(name)) {
}

controller::~controller() {
}

status controller::start(const properties& props) {
    return m_ctx->start(props);
}

status controller::stop() {
    return m_ctx->stop();
}

bool controller::is_kernel_logger() const {
    return m_ctx->is_kernel_logger();
}

bool controller::is_running() const {
    return m_ctx->is_running();
}
