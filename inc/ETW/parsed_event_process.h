#ifndef EZETW_EVENT_PROCESS_H
#define EZETW_EVENT_PROCESS_H

#include <etw/parsed_event.h>

namespace ez_etw {
    namespace parsed_events {
        struct parsed_event_process : public ez_etw::parsed_event {
			parsed_event_process(const event& evt, unsigned long pointer_size);
            virtual ~parsed_event_process() = default;
			const std::string& get_image_filename() const;
			unsigned int get_pid() const;
		protected:
			bool set_sid(const char* sid_start, uintptr_t pointer_size, size_t& user_sid_length);
			unsigned int m_pid;
			unsigned int m_parent_pid = std::numeric_limits<unsigned int>::max();
			std::string m_user_sid;
			std::string m_image_filename;
        };
    }
}

#endif
