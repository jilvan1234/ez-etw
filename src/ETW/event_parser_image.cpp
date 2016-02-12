#include <etw/event_parsers_image.h>
#include <etw/image.h>
#include <etw/image_v3.h>

using ez_etw::event_parsers::image;
using ez_etw::parsed_event;
using std::make_shared;

//DEFINE_GUID ( /* 2cb15d1d-5fc1-11d2-abe1-00a0c911f518 */
//    ImageLoadGuid,
//    0x2cb15d1d,
//    0x5fc1,
//    0x11d2,
//    0xab, 0xe1, 0x00, 0xa0, 0xc9, 0x11, 0xf5, 0x18
//    );

const GUID image::guid = {0x2cb15d1d, 0x5fc1, 0x11d2, {0xab, 0xe1, 0x00, 0xa0, 0xc9, 0x11, 0xf5, 0x18}};

image::image()
:event_parser(guid) {
}

std::pair<bool, std::shared_ptr<ez_etw::parsed_event>> image::parse_event(const std::shared_ptr<ez_etw::event>& evt) const {
    auto version = evt->get_version();
    std::shared_ptr<ez_etw::parsed_events::image> parsed;
    bool is_supported = true;
    switch(version) {
        case version::v3: {
            parsed = make_shared<ez_etw::parsed_events::images::v3>(*evt, get_pointer_size());
            break;
        }
        default:
            is_supported = false;
    }
    const bool is_parsed = is_supported && parsed->get_status() == parsed_event::status::success;
    auto pair = std::make_pair(is_parsed, parsed);
    return pair;
}