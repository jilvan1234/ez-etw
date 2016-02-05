#include <etw/parsed_event_process.h>
#include <Windows.h>
#include <Sddl.h>
#include <algorithm>
#include <iterator>

using ez_etw::parsed_events::parsed_event_process;
using std::copy;
using std::back_inserter;

parsed_event_process::parsed_event_process(const ez_etw::event& evt, unsigned long pointer_size)
:parsed_event(evt, pointer_size) {
}

bool parsed_event_process::set_sid(const char* sid_start, uintptr_t pointer_size, size_t& user_sid_length) {
	bool is_set = false;
	if(nullptr != sid_start) {
		size_t delta = sizeof(ULONG) + 2 * pointer_size;
		PSID sid = static_cast<PSID>(const_cast<char*>(sid_start) + delta);
		LPTSTR sid_str = nullptr;
		user_sid_length = GetLengthSid(sid);
		if(user_sid_length > 0 &&
			IsValidSid(sid) != 0 && 
			ConvertSidToStringSidA(sid, &sid_str) != 0) {
			copy(sid_str, sid_str + strlen(sid_str), back_inserter(m_user_sid));
			LocalFree(sid_str);
			is_set = true;
		}
	}
	return is_set;
}

const std::string& parsed_event_process::get_image_filename() const {
	return m_image_filename;
}

unsigned int parsed_event_process::get_pid() const {
	return m_pid;
}
