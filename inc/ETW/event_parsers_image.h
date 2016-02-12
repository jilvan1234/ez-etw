#ifndef EZETW_EVENTPARSER_IMAGE_H 
#define EZETW_EVENTPARSER_IMAGE_H

#include <etw/event_parser.h>

namespace ez_etw {
    namespace event_parsers {
        struct image : public event_parser {
            image();
            virtual ~image() = default;
        private:
            enum version {
                v3 = 3
            };
            static const GUID guid;
            virtual std::pair<bool, std::shared_ptr<ez_etw::parsed_event>> parse_event(const std::shared_ptr<event>& evt) const;

        };
    }
}

#endif
