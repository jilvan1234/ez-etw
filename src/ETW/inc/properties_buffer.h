#ifndef EZETW_PRIVATE_PROPERTIES_BUFFER_H
#define EZETW_PRIVATE_PROPERTIES_BUFFER_H

#include <windows.h>
#include <wmistr.h>
#include <evntrace.h>

namespace ez_etw {
    static const unsigned long properties_buffer_size = sizeof(EVENT_TRACE_PROPERTIES) + 2 * (1024 * sizeof(wchar_t));
    union properties_buffer {
        unsigned char m_buff[properties_buffer_size];
        EVENT_TRACE_PROPERTIES m_struct;
    };
}

#endif
