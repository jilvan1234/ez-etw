#include <etw/parsed_event.h>
#include <iterator>

using ez_etw::parsed_event;
using ez_etw::event;
using std::streampos;
using std::copy;
using std::back_inserter;

parsed_event::parsed_event(const ez_etw::event& evt, unsigned long pointer_size)
:m_timestamp(evt.get_timestamp()), m_pointer_size(pointer_size), m_trace_type(evt.get_type()), m_type(evt.get_guid()), m_version(evt.get_version()) {
}

time_t parsed_event::get_timestamp() const {
    return m_timestamp;
}

parsed_event::status parsed_event::get_status() const {
	return m_status;
}

event::type parsed_event::get_trace_type() const {
    return m_trace_type;
}

const GUID& parsed_event::get_type() const {
    return m_type;
}

unsigned short parsed_event::get_version() const {
    return m_version;
}

bool parsed_event::set(std::stringstream& ss, const std::string& buffer, std::wstring& str) {
    const uint64_t current_stream_pos = ss.tellg();
    const wchar_t* line_start = reinterpret_cast<const wchar_t*>(buffer.data() + current_stream_pos);
    const size_t line_len = wcslen(reinterpret_cast<const wchar_t*>(line_start));
    bool is_set = false;
    if(line_len > 0) {
        const wchar_t* line_end = line_start + line_len;
        copy(line_start, line_end, back_inserter(str));
        const wchar_t* c = str.c_str();
        const size_t bytes_count = line_len * sizeof(wchar_t);
        ss.seekg(bytes_count, std::ios::cur);
        is_set = ss.good();
    }
    return is_set;	
}
