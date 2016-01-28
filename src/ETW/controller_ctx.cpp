#include "inc/controller_ctx.h"
#include "inc/properties_buffer.h"
#include <etw/status.h>
#include <windef.h>
#include <winbase.h>
#include <wmistr.h>
#include <evntrace.h>

using std::wstring;
using ez_etw::controller_ctx;
using ez_etw::properties;
using ez_etw::properties_buffer;
using ez_etw::to_status;
using ez_etw::status;
using ez_etw::kernel_flags;

controller_ctx::controller_ctx(const wstring& name)
:m_name(name), 
m_is_kernel_logger(name == KERNEL_LOGGER_NAMEW), 
m_is_running(false) {
}

status controller_ctx::start(const properties& props) {
    status status(status::already_exist);
    if(!m_is_running) {
        const unsigned long flags(props.get_ptr()->get_struct()->EnableFlags);
        const bool have_coherent_flags = 
			(!m_is_kernel_logger && flags == kernel_flags::none) ||
            (m_is_kernel_logger && flags != kernel_flags::none);
        if(have_coherent_flags) {
            TRACEHANDLE handle = 0;
            properties::ptr_type::element_type* const props_buffer = props.get_ptr();
            const ULONG start_status = StartTraceW(&handle, m_name.c_str(), props_buffer->get_struct());
            status = to_status(start_status);
            m_is_running = start_status == status::success;
        } else {
            status = status::invalid_argument;
        }
    }
    return status;
}

status controller_ctx::stop() {
    status status = status::not_running;
    if(m_is_running) {
		status = system_stop(m_name);
        m_is_running = status != status::success;
    }
    return status;
}

bool controller_ctx::is_running() const {
	return m_is_running;
}

bool controller_ctx::is_kernel_logger() const {
    return m_is_kernel_logger;
}

status controller_ctx::system_stop(const wstring& name) {
	static properties_buffer props(ez_etw::kernel_flags::none);
	props.zero();
	auto status = StopTraceW(0, name.c_str(), props.get_struct());
	return to_status(status);
}
