#include <etw/event_parsers_process.h>
#include <etw/parsed_events_process.h>

using ez_etw::event_parsers::process;
using event_process = ez_etw::parsed_events::process;
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
/*
Process
    v0: 2000
    v1: xp
    v2: vista

*/


bool process::parse_event(const std::shared_ptr<ez_etw::event>& evt, std::deque<std::shared_ptr<ez_etw::parsed_event>>& events) const {
    bool parsed = false;
    //auto parsed_evt = make_shared<event_process>(1);
//    evt->


    return parsed;
}

