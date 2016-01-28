#include <etw/event_parsers_process.h>
#include <etw/parsed_event_process.h>
#include <etw/process_v4.h>

using ez_etw::event_parsers::process;
using event_process = ez_etw::parsed_events::parse_event_process;
using std::make_shared;

//DEFINE_GUID( /* 3d6fa8d0-fe05-11d0-9dda-00c04fd7ba7c */
//    ProcessGuid,
//    0x3d6fa8d0,
//    0xfe05,
//    0x11d0,
//    0x9d, 0xda, 0x00, 0xc0, 0x4f, 0xd7, 0xba, 0x7c
//    );

const GUID process::guid = {0x3d6fa8d0, 0xfe05, 0x11d0, {0x9d, 0xda, 0x00, 0xc0, 0x4f, 0xd7, 0xba, 0x7c}};
process::process()
:event_parser(guid) {
}

bool process::parse_event(const std::shared_ptr<ez_etw::event>& evt, std::deque<std::shared_ptr<ez_etw::parsed_event>>& events) const {
	auto version = evt->get_version();
	std::shared_ptr<ez_etw::parsed_events::parse_event_process> parsed;	
	bool is_supported = true;
	switch(version) {
		case version::v4: {
			parsed = make_shared<ez_etw::parsed_events::process::v4>(*evt, get_pointer_size());
			break;
		}
		default:
			is_supported = false;
	}
	bool is_parsed = is_supported && parsed->get_status() == parsed_event::status::success;
	if(is_parsed) {
		events.push_back(parsed);
	}
	return is_parsed;
}

