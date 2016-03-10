#include <etw/process_v4.h>
#include <iomanip>
#include <Windows.h> // needed for ULONG
#include <iterator>
#include <string>

using ez_etw::event;
using ez_etw::parsed_event;
using ez_etw::parsed_events::processes::v4;
using std::stringstream;
using std::string;
using std::streampos;
using std::copy;

/*
see: http://wutils.com/wmi/root/wmi/process_v4_typegroup1/
1 - uint32 UniqueProcessKey; (c)
2 - uint32 ProcessId; (p)
3 - uint32 ParentId; (p)
4 - uint32 SessionId; (c)
5 - sint32 ExitStatus; (c)
6 - uint32 DirectoryTableBase; -
7 - uint32 flags
8 - object UserSID; (p)
9 - string ImageFileName; (p)
10 - wstring CommandLine; (c)
*/

bool v4::set_sid(stringstream& ss, std::string& buffer_str, uintptr_t pointer_size) {
	const uint64_t current_stream_pos = ss.tellg();
	const char* byte_ptr = buffer_str.data() + current_stream_pos;
	size_t length = 0;
	bool is_set = ez_etw::parsed_events::process::set_sid(byte_ptr, pointer_size, length);
	if(is_set) {
        const size_t delta = sizeof(ULONG) + 2 * pointer_size;
        ss.seekg(length + delta, std::ios::cur);
		is_set &= ss.good();
	}
	return is_set;
}

bool v4::set_image_filename(std::stringstream& ss, std::string& buffer_str) {
	const uint64_t current_stream_pos = ss.tellg();
	const char* name_start = buffer_str.data() + current_stream_pos;
	const size_t name_len = strlen(name_start);
	bool is_set = false;
	if(name_len > 0) {
		copy(name_start, name_start + name_len, std::back_inserter(m_image_filename));
		ss.seekg(name_len + 1, std::ios::cur);
		is_set = ss.good();
	}
	return is_set;
}

bool v4::set_command_line(std::stringstream& ss, std::string& buffer_str) {
	return set(ss, buffer_str, m_command_line);
}

v4::v4(const event& evt, unsigned long pointer_size)
:process(evt, pointer_size) {
	if(evt.get_type() == event::type::start ||
		evt.get_type() == event::type::end ||
		evt.get_type() == event::type::data_collection_start ||
		evt.get_type() == event::type::data_collection_end ||
		evt.get_type() == event::type::defunct) {
		string& buffer_str = *evt.get_buffer();
		stringstream ss(buffer_str);
		unsigned int directory_table_base;	
		unsigned int flags;
        const char* p = buffer_str.data();
        if(ss.read(reinterpret_cast<char*>(&m_unique_key), sizeof(m_unique_key)).good() &&
			ss.read(reinterpret_cast<char*>(&m_pid), sizeof(m_pid)).good() &&
			ss.read(reinterpret_cast<char*>(&m_parent_pid), sizeof(m_parent_pid)).good() &&
			ss.read(reinterpret_cast<char*>(&m_session_id), sizeof(m_session_id)).good() &&
			ss.read(reinterpret_cast<char*>(&m_exit_status), sizeof(m_exit_status)).good() &&
			ss.read(reinterpret_cast<char*>(&directory_table_base), sizeof(directory_table_base)).good() &&
			ss.read(reinterpret_cast<char*>(&flags), sizeof(flags)).good() &&
			set_sid(ss, buffer_str, m_pointer_size) &&
			set_image_filename(ss, buffer_str) &&
			set_command_line(ss, buffer_str)) {
			m_status = success;
		}
		//std::cout << static_cast<signed int>(m_session_id) << "|" << m_pid << "(" << m_parent_pid << ") " << " " << m_image_filename <<  std::endl;
		//std::wcout << "\t" << m_command_line << std::endl;
		//std::cout << "\t" << m_user_sid << std::endl;
	} else {
		m_status = unsupported_type;
	}
}
