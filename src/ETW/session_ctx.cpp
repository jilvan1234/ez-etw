#include "inc/session_ctx.h"
#include <iostream>
#include <map>
//#include <Guiddef.h>
#include <atomic>

using std::memset;
using std::thread;
using ez_etw::session_ctx;
using std::atomic_bool;
using std::map;
using std::shared_ptr;
using ez_etw::event;
using std::make_shared;
using ez_etw::event_parser;

enum buffer_state {
	buffer_state_query,
	buffer_state_stop,
	buffer_state_reset
};

static atomic_bool& get_cb_buffer_state_running(buffer_state new_state) {
	static atomic_bool run = false;
	if(new_state == buffer_state_stop) {
		run = false;
	} else if(new_state == buffer_state_reset) {
		run = true;
	}
	return run;
}


enum event_parser_action {
	event_parser_action_query,
	event_parser_action_insert
};

struct guid_comparator {
	bool operator()(const GUID& left, const GUID& right) {
		return IsEqualGUID(left, right) == TRUE;
	}
};
/*
static map<GUID, event_parser, guid_comparator>& event_parser_do(event_parser_action action, GUID key, event_parser value, bool& success) {
	static map<GUID, event_parser, guid_comparator> parsables;
	if(get_cb_buffer_state_running(buffer_state_query) && event_parser_action_insert == action) {
		success = parsables.insert(std::pair<GUID, event_parser>(key, value)).second;
	}
	return parsables;
};
*/
static shared_ptr<event> create_event(const EVENT_TRACE* ptr_event) {
	auto evt = make_shared<event>(ptr_event->Header.Guid, ptr_event->Header.ProcessId, ptr_event->Header.TimeStamp.QuadPart, static_cast<char*>(ptr_event->MofData), ptr_event->MofLength);
	evt->set_type(ptr_event->Header.Class.Type);
	return evt;
}

static void cb_event(EVENT_TRACE* const ptr_event) {
	auto evt = create_event(ptr_event);
	// TODO iteration over parsables
}

static unsigned long cb_buffer(EVENT_TRACE_LOGFILEW* const ptr_event_trace) {
    return get_cb_buffer_state_running(buffer_state_query);
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

bool session_ctx::parsers_add(const event_parser& parser) {
	bool is_added = false;
	
	//event_parser_do(event_parser_action_insert, guid_event, parser, is_added);
	return is_added;
}

session_ctx::~session_ctx() { 
	stop();
}

bool session_ctx::is_running() const {
	return get_cb_buffer_state_running(buffer_state_query);
}

bool session_ctx::start() {
	bool started = false;
	if(!get_cb_buffer_state_running(buffer_state_query)) {
		started = get_cb_buffer_state_running(buffer_state_reset);
		m_trace_thread = std::make_unique<std::thread>(trace_thread, std::ref(m_trace_log));
	}
	return started;
}

bool session_ctx::stop() {
	bool stopped = false;
	if(get_cb_buffer_state_running(buffer_state_query)) {
		stopped = !get_cb_buffer_state_running(buffer_state_stop);
		m_trace_thread->join();
	}
	return stopped;
}
