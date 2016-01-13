#include <etw/parsed_event_process.h>
//#include <win/ntifs.h>

using ez_etw::parsed_events::parse_event_process;

parse_event_process::parse_event_process(const ez_etw::event& evt, unsigned long pointer_size)
:parsed_event(evt, pointer_size) {
}

bool parse_event_process::set_sid(std::stringstream& ss, unsigned long pointer_size) {
	static unsigned int sid_blob_start = 0x4;
	//if(pointer_size > 0)
	{
		unsigned int blob_start;
		bool success = ss.read(reinterpret_cast<char*>(&blob_start), sizeof(blob_start)).good();
		if(success && blob_start == sid_blob_start) {


			uintptr_t blob_inc = pointer_size * 2;
			//ss.rea
			
			//SeLengthSid(
			


		}
	}
	return false && ss.good();
}
