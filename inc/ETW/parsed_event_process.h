#ifndef EZETW_EVENT_PROCESS_H
#define EZETW_EVENT_PROCESS_H

#include <etw/parsed_event.h>

namespace ez_etw {
    namespace parsed_events {
        struct parse_event_process : public ez_etw::parsed_event {
			parse_event_process(const event& evt, unsigned long pointer_size);
            virtual ~parse_event_process() = default;
		protected:
			bool set_sid(std::stringstream& ss, unsigned long pointer_size);
			unsigned int m_pid;
			unsigned int m_parent_pid = std::numeric_limits<unsigned int>::max();
			std::string m_user_sid;
			std::string m_image_filename;
        };
    }
}

#endif
