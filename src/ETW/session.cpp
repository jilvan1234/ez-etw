#include <etw/session.h>
#include "inc/guid_comparator.h"
#include "inc/session_trace.h"
#include <iostream>
#include <map>
#include <mutex>

using std::memset;
using std::thread;
using std::map;
using std::shared_ptr;
using std::make_shared;
using std::mutex;
using std::lock_guard;
using ez_etw::event_parser;
using ez_etw::guid_comparator;
using ez_etw::event;
using ez_etw::session;
using ez_etw::session_trace;

// ---
// global variables
static mutex g_mutex_is_running;
static bool g_is_running = false;
// ---
using lock_type = std::lock_guard<std::mutex>;

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
    auto evt = make_shared<event>(ptr_event->Header.Guid, ptr_event->Header.TimeStamp.QuadPart, ptr_event->Header.ProcessId, static_cast<char*>(ptr_event->MofData), ptr_event->MofLength);
	evt->set_type(ptr_event->Header.Class.Type);
    evt->set_version(ptr_event->Header.Class.Version);
	return evt;
}

static void __stdcall cb_event(EVENT_TRACE* const ptr_event) {
	auto evt = create_event(ptr_event);
	auto parsers = event_parser_do(event_parser_action_query, nullptr, nullptr, nullptr);
	auto parser = parsers.find(evt->get_guid());
	if(parser != end(parsers)) {
		parser->second->parse(evt);
	}
}

static unsigned long __stdcall cb_buffer(EVENT_TRACE_LOGFILEW* const ptr_event_trace) {
    lock_type lock(g_mutex_is_running);
	return g_is_running;
}

void trace_thread(EVENT_TRACE_LOGFILEW* log) {
    TRACEHANDLE h = OpenTraceW(log);
    if(GetLastError() == ERROR_SUCCESS) {
        ULONG err = ProcessTrace(&h, 1, 0, 0);
        CloseTrace(h);
    }
}

session::session(const std::wstring& name, const bool consume_from_file, const ez_etw::log_mode& mode)
:m_trace(make_shared<session_trace>(name, consume_from_file, mode, cb_event, cb_buffer)) {
}

bool session::parsers_add(std::shared_ptr<event_parser> parser) {
    lock_type lock(g_mutex_is_running);
	bool is_added = false;
	event_parser_do(event_parser_action::event_parser_action_insert, &parser->get_event_type(), parser, &is_added);
	return is_added;
}

session::~session() {
	stop();
}

bool session::is_running() const {
    lock_type lock(g_mutex_is_running);
    return g_is_running;
}

bool session::start() {
    lock_type lock(g_mutex_is_running);
    if (!g_is_running) {
		m_trace_thread = std::make_unique<std::thread>(trace_thread, m_trace.get()->get_trace_logfile());
		g_is_running = m_trace_thread != nullptr;
	}
	return g_is_running;
}

bool session::stop() {
    lock_type lock(g_mutex_is_running);
	if(g_is_running) {
		m_trace_thread->join();
        g_is_running = false;
    }
	return !g_is_running;
}
