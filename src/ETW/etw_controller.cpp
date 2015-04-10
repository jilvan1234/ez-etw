
#include <ETW/etw_controller.h>
#include <Windows.h>
#include <evntrace.h>

using ez_etw::controller_impl;
using ez_etw::controller;

using std::memset;

struct controller_impl {
    static const unsigned long MAX_SESSION_NAME = 1024;
    static const unsigned long MAX_LOGFILE_NAME = 1024;
    static const unsigned long PROPS_SIZE = sizeof(EVENT_TRACE_PROPERTIES) + MAX_SESSION_NAME + MAX_LOGFILE_NAME;

    // most reliable value for most platform
    // Win2003, XP and XP SP1 does not support CPU cycle counter
    static const unsigned long PROPS_CLOCK_RESOLUTION_QPC = 1;

    controller_impl() {
        m_session_handle = 0;
        memset(m_props, sizeof(m_props), 0);
        m_session_name = KERNEL_LOGGER_NAMEW;
        m_last_status = ERROR_SUCCESS;
    }

    void init() {
        PEVENT_TRACE_PROPERTIES props = (PEVENT_TRACE_PROPERTIES)m_props;
        memset(props, sizeof(m_props), 0);
        // init Wnode
        props->Wnode.BufferSize = sizeof(m_props);
        props->Wnode.Guid = SystemTraceControlGuid; // kernel logger
        props->Wnode.ClientContext = PROPS_CLOCK_RESOLUTION_QPC;
        props->Wnode.Flags = WNODE_FLAG_TRACED_GUID;
        // init props
        // since it's not the most accurant on Vista
        // we'll eventually to manually call flush on the
        // tracing session to get the most recent events
        props->LogFileMode = EVENT_TRACE_REAL_TIME_MODE;
        // process only for now
        props->EnableFlags = EVENT_TRACE_FLAG_PROCESS;
        props->LoggerNameOffset = sizeof(*props);
        props->LogFileNameOffset = props->LoggerNameOffset + sizeof(KERNEL_LOGGER_NAMEW);
    }

    TRACEHANDLE m_session_handle;
    unsigned char m_props[PROPS_SIZE]; 
    LPCWSTR m_session_name;
    ULONG m_last_status;
};

bool controller::start(bool force_start) {
    m_pimpl->init();

    if(force_start) {
        stop();
    }
    m_pimpl->m_last_status = StartTraceW(&m_pimpl->m_session_handle, m_pimpl->m_session_name, (PEVENT_TRACE_PROPERTIES)&m_pimpl->m_props);
    return ERROR_SUCCESS == m_pimpl->m_last_status || ERROR_ALREADY_EXISTS == m_pimpl->m_last_status;
}

void controller::stop() {
    m_pimpl->m_last_status = ControlTraceW(m_pimpl->m_session_handle, m_pimpl->m_session_name, (PEVENT_TRACE_PROPERTIES)&m_pimpl->m_props, EVENT_TRACE_CONTROL_STOP);
}

unsigned long controller::get_last_status() {
    return m_pimpl->m_last_status;
}