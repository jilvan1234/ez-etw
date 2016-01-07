#include <etw/event.h>

#include <Windows.h>
#include <Wmistr.h>
#include <Evntrace.h>

using ez_etw::event;
using std::make_shared;
using std::shared_ptr;
using std::string;

event::event(const GUID& guid, const unsigned long long& timestamp, const char* buffer, size_t buffer_size)
:m_timestamp(timestamp), m_guid(guid), m_type(type::info), m_version(0), m_buffer(make_shared<std::string>(buffer, buffer_size)) {
}

bool event::set_type(unsigned int type) {
	bool IsValid = true;
	switch(type) {
		case EVENT_TRACE_TYPE_INFO:
			m_type = type::info;
			break;
		case EVENT_TRACE_TYPE_START:
			m_type = type::start;
			break;
		case EVENT_TRACE_TYPE_END:
			m_type = type::end;
			break;
		case EVENT_TRACE_TYPE_DC_START:
			m_type = type::data_collection_start;
			break;
		case EVENT_TRACE_TYPE_DC_END:
			m_type = type::data_collection_end;
			break;
		case 39: // defunct
			m_type = type::defunct;
			break;
		default:
			IsValid = false;
	}
	return IsValid;
}

void event::set_version(unsigned short version) {
    m_version = version;
}

const GUID& event::get_guid() const {
    return m_guid;
}

unsigned long long event::get_timestamp() const {
    return m_timestamp;
}

event::type event::get_type() const {
    return m_type;
}

unsigned short event::get_version() const {
    return m_version;    
}

const shared_ptr<string> event::get_buffer() const {
    return m_buffer;
}
