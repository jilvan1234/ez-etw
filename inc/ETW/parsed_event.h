#ifndef	EZETW_PARSEDEVENT_H
#define EZETW_PARSEDEVENT_H

#include <time.h>
#include <etw/event.h>

namespace ez_etw {
	struct parsed_event {
        parsed_event(const ez_etw::event& evt);
		virtual ~parsed_event() = default;
        time_t get_timestamp() const;
        unsigned int get_pid() const;
    private:
        unsigned int m_pid;
        time_t m_timestamp;
	};
}

#endif
