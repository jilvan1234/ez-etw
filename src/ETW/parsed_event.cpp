#include <etw/parsed_event.h>
#include <iterator>

using ez_etw::parsed_event;
using ez_etw::event;
using std::streampos;
using std::copy;
using std::back_inserter;

parsed_event::parsed_event(const ez_etw::event& evt, unsigned long pointer_size)
:m_timestamp(evt.get_timestamp()), m_pointer_size(pointer_size), m_type(evt.get_type()) {
}

time_t parsed_event::get_timestamp() const {
    return m_timestamp;
}

parsed_event::status parsed_event::get_status() const {
	return m_status;
}

event::type parsed_event::get_type() const {
    return m_type;
}

bool parsed_event::set(std::stringstream& ss, const std::string& buffer, std::wstring& str) {
    const streampos current_stream_pos = ss.tellg();
    const char* cmd_line_start = buffer.data() + current_stream_pos;
    const size_t cmd_line_len = wcslen(reinterpret_cast<const wchar_t*>(cmd_line_start));
    bool is_set = false;
    if(cmd_line_len > 0) {
        const size_t bytes_count = cmd_line_len * sizeof(wchar_t);
        copy(cmd_line_start, cmd_line_start + bytes_count, back_inserter(str));
        ss.seekg(bytes_count, std::ios::cur);
        is_set = ss.good();
    }
    return is_set;	
}
