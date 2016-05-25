#ifndef EZETW_PARSEDEVENTS_PROCESSES_V4_H
#define EZETW_PARSEDEVENTS_PROCESSES_V4_H

#include <etw/process.h>
#include <sstream>

namespace ez_etw {
    namespace parsed_events {
        namespace processes {
            struct v4 : ez_etw::parsed_events::process {
				v4(const event& evt, unsigned long pointer_size);
                virtual ~v4() = default;
                uintptr_t get_unique_key() const;
                unsigned int get_session_id() const;
                signed int get_exit_status() const;
                const std::wstring& get_command_line() const;
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
