#ifndef EZETW_EVENT_H
#define EZETW_EVENT_H

#include <sstream>
#include <Guiddef.h>
#include <memory>

namespace ez_etw {
	struct event {
		enum type {
			info,
			start,
			end,
			data_collection_start,
            data_collection_end,
			defunct
		};
        event(const GUID& guid, const unsigned long long& timestamp, const char* buffer, size_t buffer_size);
		virtual ~event() = default;
		bool set_type(unsigned int type);
        void set_version(unsigned short version);

        const GUID& get_guid() const;
        unsigned long long get_timestamp() const;
        type get_type() const;
        unsigned short get_version() const;
        const std::shared_ptr<std::string> get_buffer() const;
    private:
        GUID m_guid;
        unsigned long long m_timestamp;
        type m_type;
        unsigned short m_version;
        std::shared_ptr<std::string> m_buffer;
	};
}

#endif
