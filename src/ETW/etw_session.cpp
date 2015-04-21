
#include <ETW/etw_session.h>
#include <Windows.h>
#include <evntrace.h>

using ez_etw::session;
using std::make_shared;

const unsigned long session::MAX_SESSION_NAME = 1024;
const unsigned long session::MAX_LOGFILE_NAME = 1024;
const unsigned long session::PROPERTIES_SIZE = sizeof(EVENT_TRACE_PROPERTIES) + MAX_SESSION_NAME + MAX_LOGFILE_NAME;

unsigned char props[session::PROPERTIES_SIZE];

template<typename T>
struct array_deleter {
    void operator()(T const p) {
        delete[] p;
    }
};

session::session(const string_type& logger)
:m_logger(logger), m_props(new unsigned char[PROPERTIES_SIZE], array_deleter<unsigned char[]>()) {
}

session::props_type session::get_properties() {
    return m_props;
}
/*
namespace ez_etw {
    struct controller::controller_impl {

        // most reliable value for most platform
        // Win2003, XP and XP SP1 does not support CPU cycle counter
        static const unsigned long PROPS_CLOCK_RESOLUTION_QPC = 1;

        controller_impl() {
            m_session_handle = 0;
            memset(m_props, sizeof(m_props), 0);
            m_session_name = KERNEL_LOGGER_NAMEW;
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
    };
}

controller_status controller::start() {
    //m_pimpl->init();
    ULONG win_status = 0;//StartTraceW(&m_pimpl->m_session_handle, m_pimpl->m_session_name, (PEVENT_TRACE_PROPERTIES)&m_pimpl->m_props);
    return to_status(win_status);
}

void controller::stop() {
    //ControlTraceW(m_pimpl->m_session_handle, m_pimpl->m_session_name, (PEVENT_TRACE_PROPERTIES)&m_pimpl->m_props, EVENT_TRACE_CONTROL_STOP);
}

controller_status controller::to_status(unsigned long status) {
    controller_status c_status;
    switch(status) {
        case ERROR_SUCCESS:
            c_status = controller_status::success;
            break;
        case ERROR_ALREADY_EXISTS:
            c_status = controller_status::already_exists;
            break;
        default:
            c_status = controller_status::failure;
            break;
    }
    return c_status;
}
*/