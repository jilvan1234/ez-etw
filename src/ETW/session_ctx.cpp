#include "inc/session_ctx.h"

using std::memset;
using std::thread;
using ez_etw::session_ctx;

static void event_cb(EVENT_TRACE* const ptr_event) {
}

static unsigned long buffer_cb(EVENT_TRACE_LOGFILEW* const ptr_event_trace) {
    return true;
}

void trace_thread(EVENT_TRACE_LOGFILEW& log) {
    TRACEHANDLE h = OpenTraceW(&log);
    if(GetLastError() == ERROR_SUCCESS) {
        ProcessTrace(&h, 1, 0, 0);
        CloseTrace(h);
    }
}

session_ctx::session_ctx(const std::wstring& name, const bool consume_from_file, const ez_etw::log_mode& mode) {
    memset(&m_trace_log, 0, sizeof(m_trace_log));
    wchar_t* ptr_name = const_cast<wchar_t*>(name.c_str());
    if(consume_from_file) {
        m_trace_log.LogFileName = ptr_name;
    } else {
        m_trace_log.LoggerName = ptr_name;
    }
    m_trace_log.LogFileMode = mode;
    m_trace_log.EventCallback = &event_cb;
    m_trace_log.BufferCallback = &buffer_cb;

    //std::thread th(&trace_thread, std::ref(m_traceLog));
    //th.join();
}

session_ctx::~session_ctx() {

}

bool session_ctx::is_running() const {
    static const thread::id DEFAULT_THREAD_ID;
    bool is_running = nullptr != m_trace_thread.get();
    if(is_running)
    {
        is_running = m_trace_thread->get_id() != DEFAULT_THREAD_ID;
    }
    return is_running;
}
