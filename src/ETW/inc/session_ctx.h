#ifndef EZETW_PRIVATE_SESSION_CTX_H
#define EZETW_PRIVATE_SESSION_CTX_H

//#define INITGUID
#include <etw/properties.h>
#include <etw/event_parser.h>
#include <windows.h>
#include <wmistr.h>
#include <evntrace.h>
#include <string>
#include <thread>

namespace ez_etw {

    struct session_ctx {
        session_ctx(const std::wstring& name, const bool consume_from_file, const ez_etw::log_mode& mode);
		bool parsers_add(const event_parser& parser);
        bool is_running() const;
        ~session_ctx();
		bool start();
		bool stop();
    private:    
        EVENT_TRACE_LOGFILEW m_trace_log;
        std::unique_ptr<std::thread> m_trace_thread;
    };
}

#endif
