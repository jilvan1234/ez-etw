#ifndef EZETW_EVENTPARSER_PROCESS_H
#define EZETW_EVENTPARSER_PROCESS_H

#include <etw/event_parser.h>

namespace ez_etw {
    namespace event_parsers {
        struct process : public event_parser {
            process();
            virtual ~process() = default;
        private:
            static const GUID guid;
            virtual bool parse_event(const std::shared_ptr<ez_etw::event>& evt, std::deque<std::shared_ptr<ez_etw::parsed_event>>& events) const;
        };
    }
}

#endif
