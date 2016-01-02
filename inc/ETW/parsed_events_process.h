#ifndef EZETW_EVENT_PROCESS_H
#define EZETW_EVENT_PROCESS_H

#include <etw/parsed_event.h>

namespace ez_etw {
    namespace parsed_events {
        struct process : public ez_etw::parsed_event {
            virtual ~process() = default;
        };
    }
}

#endif
