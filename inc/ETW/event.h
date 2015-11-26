#ifndef EZETW_EVENT_H
#define EZETW_EVENT_H

#include <sstream>

namespace ez_etw {
	struct event {
		event(unsigned int process_id, const char* buffer, size_t buffer_size);
		unsigned int m_process_id;
		std::stringstream m_buffer;
	};
}

#endif
