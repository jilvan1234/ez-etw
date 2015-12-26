#ifndef EZETW_SESSION_H
#define EZETW_SESSION_H

#include <etw/properties.h>
#include <etw/event_parser.h>
#include <string>
#include <thread>

namespace ez_etw {
    struct session_trace;

    struct session {
        using trace_type = std::shared_ptr<session_trace>;
        session(const std::wstring& name, const bool consume_from_file, const ez_etw::log_mode& mode);
        ~session();
        bool parsers_add(std::shared_ptr<event_parser> parser);
        bool is_running() const;
		bool start();
		bool stop();
    private:
        trace_type m_trace;
        std::unique_ptr<std::thread> m_trace_thread;
    };

}

#endif
