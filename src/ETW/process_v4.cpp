#include <etw/process_v4.h>

using ez_etw::event;
using ez_etw::parsed_event;
using ez_etw::parsed_events::process::v4;

#include <iomanip>
#include <array>
#include <Windows.h>
#include <Sddl.h>
#include <list>

/*
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
using std::stringstream;
using std::array;
//using std::stoi;
using std::string;
using std::streampos;
using std::list;


static bool set_sid_ex(stringstream& ss, uintptr_t pointer_size) {

	bool success = false;

	auto p = ss.rdbuf();
	auto s = p->str();

	return success;
}

#include <iostream>

v4::v4(const event& evt, unsigned long pointer_size)
:parse_event_process(evt, pointer_size) {
	if(evt.get_type() == event::type::start ||
		evt.get_type() == event::type::end ||
		evt.get_type() == event::type::data_collection_start ||
		evt.get_type() == event::type::data_collection_end ||
		evt.get_type() == event::type::defunct) {
		
		string buffer_str = *evt.get_buffer();
		stringstream ss(buffer_str);
		
		unsigned int directory_table_base;	
		unsigned int flags;
		if(ss.read(reinterpret_cast<char*>(&m_unique_key), sizeof(m_unique_key)).good() &&
			ss.read(reinterpret_cast<char*>(&m_pid), sizeof(m_pid)).good() &&
			ss.read(reinterpret_cast<char*>(&m_parent_pid), sizeof(m_parent_pid)).good() &&
			ss.read(reinterpret_cast<char*>(&m_session_id), sizeof(m_session_id)).good() &&
			ss.read(reinterpret_cast<char*>(&m_exit_status), sizeof(m_exit_status)).good() &&
			ss.read(reinterpret_cast<char*>(&directory_table_base), sizeof(directory_table_base)).good() &&
			ss.read(reinterpret_cast<char*>(&flags), sizeof(flags)) )// &&
			//ss.read(reinterpret_cast<char*>(&), sizeof()) &&
			//ss.read(reinterpret_cast<char*>(&), sizeof()) &&
			//ss.read(reinterpret_cast<char*>(&), sizeof()) &&
		{
			std::cout << m_pid << "\t";
			//static const unsigned int sid_marker = 0x4;
			streampos current_stream_pos = ss.tellg();
			char* byte_ptr = const_cast<char*>(buffer_str.data()) + current_stream_pos;
			ULONG* blob_start = reinterpret_cast<ULONG*>(byte_ptr);
			bool sid_exists = blob_start != nullptr && *blob_start != 0;
			//if(sid_exists) {
				ULONG delta = sizeof(ULONG) + 2 * m_pointer_size;
				PSID sid = byte_ptr + delta;
				DWORD length = GetLengthSid(sid);
				LPTSTR sid_str = nullptr;
				if(IsValidSid(sid) != 0 && 
					ConvertSidToStringSidA(sid, &sid_str) != 0) {
					std::cout << "SID:" << sid_str << std::endl;
					LocalFree(sid_str);
				} else {
					std::cout << "NO SID" << std::endl;
				}
			//} else {
			//	std::cout << "\tNo sid" << std::endl;
			//}
			if(!sid_exists) {
				std::cout << "\tNo SID..." << std::endl;
			}
			m_status = success;
		}		
	} else {
		m_status = unsupported_type;
	}
}
