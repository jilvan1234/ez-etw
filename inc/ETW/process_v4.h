#ifndef EZETW_PARSEDEVENTS_PROCESS_VISTA_H
#define EZETW_PARSEDEVENTS_PROCESS_VISTA_H

#include <etw/parsed_event_process.h>
#include <sstream>

namespace ez_etw {
    namespace parsed_events {
        namespace process {
            struct v4 : ez_etw::parsed_events::parse_event_process {
				v4(const event& evt, unsigned long pointer_size);
                virtual ~v4() = default;
			private:
				bool set_sid(std::stringstream& ss, std::string& buffer_str, uintptr_t pointer_size);
				bool set_image_filename(std::stringstream& ss, std::string& buffer_str);
				bool set_command_line(std::stringstream& ss, std::string& buffer_str);
				uintptr_t m_unique_key;
				unsigned int m_session_id;
				signed int m_exit_status;
				std::wstring m_command_line;
            };
        }
    }
}

#endif
