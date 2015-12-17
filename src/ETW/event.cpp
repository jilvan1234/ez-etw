#include <ETW/event.h>

#include <Windows.h>
#include <Wmistr.h>
#include <Evntrace.h>

using ez_etw::event;

event::event(const GUID& guid, unsigned int process_id, const long long& timestamp, const char* buffer, size_t buffer_size)
:m_process_id(process_id), m_type(type::Info), m_timestamp(timestamp), m_guid(guid) {
	m_buffer.write(buffer, buffer_size);
}

bool event::set_type(unsigned int type) {
	bool IsValid = true;
	switch(type) {
		case EVENT_TRACE_TYPE_INFO:
			m_type = type::Info;
			break;
		case EVENT_TRACE_TYPE_START:
			m_type = type::Start;
			break;
		case EVENT_TRACE_TYPE_END:
			m_type = type::End;
			break;
		case EVENT_TRACE_TYPE_DC_START:
			m_type = type::DataCollectionStart;
			break;
		case EVENT_TRACE_TYPE_DC_END:
			m_type = type::DataCollectionEnd;
			break;
		default:
			IsValid = false;
	}
	return IsValid;
}
