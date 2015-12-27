#ifndef EZETW_PRIVATE_SESSION_TRACE_H
#define EZETW_PRIVATE_SESSION_TRACE_H

#include <windows.h>
#include <wmistr.h>
#include <evntrace.h>
#include <string>
#include <memory>
#include "properties_buffer.h"

namespace ez_etw {
    struct session_trace {
        using event_callback_type = PEVENT_CALLBACK;
        using buffer_callback_type = PEVENT_TRACE_BUFFER_CALLBACKW;
        session_trace(const std::wstring& name, const bool consume_from_file, const ez_etw::log_mode& mode, event_callback_type cb_event, buffer_callback_type cb_buffer);
        EVENT_TRACE_LOGFILEW* const get_trace_logfile();
    private:
        static const std::string exception_invalid_callbacks;
        EVENT_TRACE_LOGFILEW m_trace_log;
    };
}

#endif
