#ifndef	EZETW_PARSEDEVENT_H
#define EZETW_PARSEDEVENT_H

#include <time.h>
#include <etw/event.h>

namespace ez_etw {
	struct parsed_event {
		enum status {
			success,
			incomplete,
			unsupported_type
		};
        parsed_event(const ez_etw::event& evt, unsigned long pointer_size);
		virtual ~parsed_event() = default;
        time_t get_timestamp() const;
		status get_status() const;
        event::type get_trace_type() const;
        const GUID& get_type() const;
        unsigned short get_version() const;
	protected:
		status m_status = incomplete;
		unsigned long m_pointer_size;
        event::type m_trace_type;
        const GUID& m_type;
        unsigned short m_version;
        bool set(std::stringstream& ss, const std::string& buffer, std::wstring& str);
    private:
        time_t m_timestamp;
	};
}

#endif
