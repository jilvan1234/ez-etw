#ifndef EZETW_EVENTPARSER_PROCESS_H
#define EZETW_EVENTPARSER_PROCESS_H

#include <etw/event_parser.h>

namespace ez_etw {
    namespace event_parsers {
        struct process : public event_parser {
            process();
            virtual ~process() = default;
        private:
			enum version {
				v4 = 4
			};
            static const GUID guid;
            virtual std::pair<bool, std::shared_ptr<ez_etw::parsed_event>> parse_event(const std::shared_ptr<event>& evt) const;
        };
    }
}

#endif
