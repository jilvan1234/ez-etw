#include "inc/session_ctx.h"
#include <iostream>
#include <map>
//#include <Guiddef.h>
#include <mutex>

using std::memset;
using std::thread;
using ez_etw::session_ctx;
using std::map;
using std::shared_ptr;
using ez_etw::event;
using std::make_shared;
using ez_etw::event_parser;
using std::mutex;
using std::lock_guard;

// ---
// global variables
static mutex g_is_running_mutex;
static bool g_is_running = false;
// ---

using lock_type = lock_guard<decltype(g_is_running_mutex)>;
/*
enum buffer_state {
	buffer_state_query,
	buffer_state_stop,
	buffer_state_reset
};
/*
bool get_cb_buffer_state_running(buffer_state new_state) {
	static bool is_running = false;
	lock_type lock(g_is_running_mutex);
	if(new_state == buffer_state_stop) {
		is_running = false;
	} else if(new_state == buffer_state_reset) {
		is_running = true;
	}
	return is_running;
}
*/
struct guid_comparator {
	bool operator()(const GUID& left, const GUID& right) {
		return left.Data1 < right.Data1 && 
			left.Data2 < right.Data2 && 
			left.Data3 < right.Data3 &&
			left.Data4 < right.Data4;
	}
};

enum event_parser_action {
	event_parser_action_query,
	event_parser_action_insert
};

static map<GUID, std::shared_ptr<event_parser>, guid_comparator>& event_parser_do(event_parser_action action, GUID* key, std::shared_ptr<event_parser> value, bool* success) {
	static map<GUID, std::shared_ptr<event_parser>, guid_comparator> parsables;
	if(!g_is_running && event_parser_action_insert == action && nullptr != key && nullptr != value) {
		if(parsables.find(*key) == end(parsables)) {
			std::pair<GUID, std::shared_ptr<event_parser>> p(*key, value);
			auto inserted = parsables.emplace(p);
			*success = inserted.second;
		}
	}
	return parsables;
};

static shared_ptr<event> create_event(const EVENT_TRACE* ptr_event) {
	auto evt = make_shared<event>(ptr_event->Header.Guid, ptr_event->Header.ProcessId, ptr_event->Header.TimeStamp.QuadPart, static_cast<char*>(ptr_event->MofData), ptr_event->MofLength);
	evt->set_type(ptr_event->Header.Class.Type);
	return evt;
}

static void cb_event(EVENT_TRACE* const ptr_event) {
	auto evt = create_event(ptr_event);
	auto parsers = event_parser_do(event_parser_action_query, nullptr, nullptr, nullptr); 
	auto parser = parsers.find(evt->m_guid);
	if(parser != end(parsers)) {
		parser->second->parse(evt);
	}
}

static unsigned long cb_buffer(EVENT_TRACE_LOGFILEW* const ptr_event_trace) {
	lock_type lock(g_is_running_mutex);
	return g_is_running;
}

void trace_thread(EVENT_TRACE_LOGFILEW& log) {
    TRACEHANDLE h = OpenTraceW(&log);
    if(GetLastError() == ERROR_SUCCESS) {
        ULONG err = ProcessTrace(&h, 1, 0, 0);
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
    m_trace_log.EventCallback = &cb_event;
    m_trace_log.BufferCallback = &cb_buffer;
	m_trace_log.ProcessTraceMode = mode;
}

bool session_ctx::parsers_add(std::shared_ptr<event_parser> parser) {
	lock_type lock(g_is_running_mutex);
	bool is_added = false;
	event_parser_do(event_parser_action::event_parser_action_insert, &parser->get_event_type(), parser, &is_added);
	return is_added;
}

session_ctx::~session_ctx() { 
	stop();
}

bool session_ctx::is_running() const {
	lock_type lock(g_is_running_mutex);
	return g_is_running;
}

bool session_ctx::start() {
	lock_type lock(g_is_running_mutex);
	if(!g_is_running) {
		m_trace_thread = std::make_unique<std::thread>(trace_thread, std::ref(m_trace_log));
		g_is_running = true;
	}
	return g_is_running;
}

bool session_ctx::stop() {
	lock_type lock(g_is_running_mutex);
	if(g_is_running) {
		g_is_running = false;
		m_trace_thread->join();
	}
	return !g_is_running;
}
