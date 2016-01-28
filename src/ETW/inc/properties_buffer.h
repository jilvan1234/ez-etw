#ifndef EZETW_PRIVATE_PROPERTIES_BUFFER_H
#define EZETW_PRIVATE_PROPERTIES_BUFFER_H

#define INITGUID
#include <windows.h>
#include <wmistr.h>
#include <evntrace.h>

namespace ez_etw {
    enum resolution_speed;
    enum log_mode;
    enum kernel_flags : unsigned long;

    struct properties_buffer {
        static const unsigned long max_name_length = 1024 * sizeof(wchar_t);
        static const unsigned long buffer_size = sizeof(EVENT_TRACE_PROPERTIES) + 2 * (max_name_length);
        using buffer_type = unsigned char[buffer_size];
        
        properties_buffer(const kernel_flags& flags);
        ~properties_buffer() = default;
        buffer_type* const get_buffer();
        EVENT_TRACE_PROPERTIES* const get_struct();
		void zero();
    private:
        union {
            buffer_type m_buff;
            EVENT_TRACE_PROPERTIES m_struct;
        } m_u;
    };
}

#endif
