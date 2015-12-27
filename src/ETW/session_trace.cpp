#include "inc/session_trace.h"
#include <etw/event.h>
#include <cassert>
#include <exception>

using std::memset;
using std::shared_ptr;
using std::string;
using ez_etw::session_trace;
using ez_etw::log_mode;
using ez_etw::event;

const std::string session_trace::exception_invalid_callbacks("invalid callbacks");

session_trace::session_trace(const std::wstring& name, const bool consume_from_file, const log_mode& mode, event_callback_type cb_event, buffer_callback_type cb_buffer) {
    if(cb_event == nullptr || cb_buffer == nullptr) {
        throw std::invalid_argument(exception_invalid_callbacks);
    }
    memset(&m_trace_log, 0, sizeof(m_trace_log));
    wchar_t* ptr_name = const_cast<wchar_t*>(name.c_str());
    if (consume_from_file) {
        m_trace_log.LogFileName = ptr_name;
    } else {
        m_trace_log.LoggerName = ptr_name;
    }
    m_trace_log.EventCallback = cb_event;
    m_trace_log.BufferCallback = cb_buffer;
    m_trace_log.ProcessTraceMode = mode;
}

EVENT_TRACE_LOGFILEW* const session_trace::get_trace_logfile() {
    return &m_trace_log;
}
