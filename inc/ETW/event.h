#ifndef EZETW_EVENT_H
#define EZETW_EVENT_H

#include <sstream>
#include <Guiddef.h>

namespace ez_etw {
	struct event {
		enum type {
			Info,
			Start,
			End,
			DataCollectionStart,
			DataCollectionEnd
		};
		event(const GUID& guid, unsigned int process_id, const long long& timestamp, const char* buffer, size_t buffer_size);
		virtual ~event() = default;
		bool set_type(unsigned int type);
		// members
		unsigned int m_process_id;
		std::stringstream m_buffer;
		type m_type;
		long long m_timestamp;
		GUID m_guid;
	};
}

#endif
