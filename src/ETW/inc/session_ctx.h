#ifndef EZETW_SESSION_H
#define EZETW_SESSION_H

#define INITGUID
#include <windows.h>
#include <wmistr.h>
#include <evntrace.h>
#include <string>
#include <etw/properties.h>

//#include <list>
#include <thread>

namespace ez_etw {

    struct event_parser {
        typedef void(*parse_event_fn)(const EVENT_TRACE* const ptr_event);
        GUID m_event_guid;
        parse_event_fn m_parse;
    };
    //typedef void (*parse_event)(const EVENT_TRACE* const ptr_event);
    //static map<GUID, list<parse_event>> static_guids;


    struct session_ctx {
        session_ctx(const std::wstring& name, const bool consume_from_file, const ez_etw::log_mode& mode);
        bool is_running() const;
        ~session_ctx();
    private:    
        EVENT_TRACE_LOGFILEW m_trace_log;
        std::unique_ptr<std::thread> m_trace_thread;
    };
}

#endif
