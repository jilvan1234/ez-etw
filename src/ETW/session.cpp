#include <etw/session.h>
#include <etw/guid_comparator.h>
#include "inc/session_trace.h"
#include "inc/properties_buffer.h"
#include "inc/session_global.h"
#include <iostream>
#include <map>
#include <mutex>
#include <condition_variable>
#include <algorithm>

using std::memset;
using std::thread;
using std::map;
using std::shared_ptr;
using std::make_shared;
using std::mutex;
using std::lock_guard;
using std::unique_lock;
using std::condition_variable;
using std::end;
using ez_etw::event_parser;
using ez_etw::guid_comparator;
using ez_etw::event;
using ez_etw::session;
using ez_etw::session_trace;
using ez_etw::session_global::event_parser_do;
using ez_etw::session_global::event_parser_action;

// ---
// global variables
static bool g_trace_is_running = false;
// ---
using lock_type = std::lock_guard<std::mutex>;

static shared_ptr<event> create_event(const EVENT_TRACE* ptr_event) {
    auto evt = make_shared<event>(ptr_event->Header.Guid, ptr_event->Header.TimeStamp.QuadPart, static_cast<char*>(ptr_event->MofData), ptr_event->MofLength);
	evt->set_type(ptr_event->Header.Class.Type);
    evt->set_version(ptr_event->Header.Class.Version);
	return evt;
}

static void __stdcall cb_event(EVENT_TRACE* const ptr_event) {
	auto evt = create_event(ptr_event);
	auto parsers = event_parser_do(event_parser_action::query, nullptr, nullptr, nullptr);
	auto parser = parsers.find(evt->get_guid());
	if(parser != end(parsers)) {
		parser->second->parse(evt);
	}
}

static unsigned long __stdcall cb_buffer(EVENT_TRACE_LOGFILEW* const ptr_event_trace) {
	return g_trace_is_running;
}

void trace_thread(EVENT_TRACE_LOGFILEW* log, std::shared_ptr<unsigned long>& pointer_size, condition_variable& session_cv, uint64_t& trace_handle) {
	std::cout << "started" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	trace_handle = OpenTraceW(log);
    if(GetLastError() == ERROR_SUCCESS) {
		std::cout << "notify" << std::endl;
		*pointer_size = log->LogfileHeader.PointerSize;
		std::cout << "Pointer size is " << log->LogfileHeader.PointerSize << std::endl;
		session_cv.notify_all();
        ULONG err = ProcessTrace(&trace_handle, 1, 0, 0);
        CloseTrace(trace_handle);
    }
}

session::session(const std::wstring& name, const bool consume_from_file, const ez_etw::log_mode& mode)
:m_trace(make_shared<session_trace>(name, consume_from_file, mode, cb_event, cb_buffer)), m_pointer_size(std::make_shared<unsigned long>(0)), m_trace_handle(0) {
}

bool session::parsers_add(std::shared_ptr<event_parser> parser) {
    lock_type lock(m_mutex_is_running);
	bool is_added_global = false;
	bool is_added_local = false;
	if(!g_trace_is_running) {
		event_parser_do(event_parser_action::insert, parser, m_pointer_size, &is_added_global);
		if(is_added_global) {
			auto itt = m_parsers.emplace(parser->get_event_type(), parser);
			is_added_local = itt.second;
		}
	}
	return is_added_global && is_added_local;
}

void session::parsers_remove_all() {
	using std::for_each;
	using std::pair;
	lock_type lock(m_mutex_is_running);
	if(!g_trace_is_running) {
		for_each(begin(m_parsers), end(m_parsers), [](decltype(m_parsers)::value_type& elem) {
			event_parser_do(event_parser_action::remove, elem.second, nullptr, nullptr);
		});
	}
	m_parsers.clear();
}

session::~session() {
	stop();
	parsers_remove_all();
}

bool session::is_running() {
    lock_type lock(m_mutex_is_running);
    return g_trace_is_running;
}

bool session::start() {
    lock_type lock(m_mutex_is_running);
    if (!g_trace_is_running) {
		mutex mut;
		unique_lock<mutex> session_lk(mut);
		condition_variable session_cv;
		std::cout << "starting" << std::endl;
		m_trace_thread = std::make_unique<std::thread>(trace_thread, m_trace.get()->get_trace_logfile(), std::ref(m_pointer_size), std::ref(session_cv), std::ref(m_trace_handle));
		std::cout << "waiting" << std::endl;
		session_cv.wait(session_lk);
		std::cout << "STARTED with pointer_size " << *m_pointer_size << std::endl;
		g_trace_is_running = m_trace_thread != nullptr;
	}
	return g_trace_is_running;
}

bool session::stop() {
    lock_type lock(m_mutex_is_running);
	if(g_trace_is_running) {
		g_trace_is_running = false;
		if(m_trace_thread != nullptr) {
			m_trace_thread->join();
		}
    }
	return !g_trace_is_running;
}
