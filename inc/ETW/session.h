#ifndef EZETW_SESSION_H
#define EZETW_SESSION_H

#include <etw/properties.h>
#include <etw/event_parser.h>
#include <etw/guid_comparator.h>
#include <string>
#include <thread>
#include <mutex>
#include <map>

namespace ez_etw {
    struct session_trace;

    struct session {
        using trace_type = std::shared_ptr<session_trace>;
        session(const std::wstring& name, const bool consume_from_file, const ez_etw::log_mode& mode);
        ~session();
        bool parsers_add(std::shared_ptr<event_parser> parser);
        bool is_running();
		bool start();
		bool stop();
	private:
		void parsers_remove_all();
        trace_type m_trace;
        std::unique_ptr<std::thread> m_trace_thread;
		std::shared_ptr<unsigned long> m_pointer_size;
		uint64_t m_trace_handle;
		std::mutex m_mutex_is_running;
		std::map<GUID, std::shared_ptr<event_parser>, guid_comparator> m_parsers;
	};

}

#endif
