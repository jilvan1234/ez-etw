#ifndef EZETW_PRIVATE_PROPERTIES_BUFFER_H
#define EZETW_PRIVATE_PROPERTIES_BUFFER_H

#define INITGUID
#include <windows.h>
#include <wmistr.h>
#include <evntrace.h>

namespace ez_etw {
    enum resolution_speed;
    enum log_mode;
    enum kernel_flags;

    struct properties_buffer {
        static const unsigned long MAX_STRING_LENGTGH = 1024 * sizeof(wchar_t);
        static const unsigned long BUFFER_SIZE = sizeof(EVENT_TRACE_PROPERTIES) + 2 * (MAX_STRING_LENGTGH);
        using buffer_type = unsigned char[BUFFER_SIZE];
        
        properties_buffer(const kernel_flags& flags);
        ~properties_buffer() = default;
        buffer_type* const get_buffer();
        EVENT_TRACE_PROPERTIES* const get_struct();
    private:
        union {
            buffer_type m_buff;
            EVENT_TRACE_PROPERTIES m_struct;
        } m_u;
    };
}

#endif
