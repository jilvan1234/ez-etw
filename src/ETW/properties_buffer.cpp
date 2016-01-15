#include "inc/properties_buffer.h"
#include <memory>

using ez_etw::resolution_speed;
using ez_etw::log_mode;
using ez_etw::kernel_flags;
using ez_etw::properties_buffer;
using std::memset;

properties_buffer::properties_buffer(const kernel_flags& flags) {
    const size_t buff_size = sizeof(m_u);
    memset(&m_u, 0, buff_size);
    m_u.m_struct.Wnode.BufferSize = buff_size;
    m_u.m_struct.LoggerNameOffset = sizeof(m_u.m_struct);
    m_u.m_struct.LogFileNameOffset = sizeof(m_u.m_struct) + max_name_length;
    if(flags != 0) {
        m_u.m_struct.Wnode.Guid = SystemTraceControlGuid;
        m_u.m_struct.EnableFlags = flags;
    }
}

properties_buffer::buffer_type* const properties_buffer::get_buffer() {
    return &m_u.m_buff;
}

EVENT_TRACE_PROPERTIES* const properties_buffer::get_struct() {
    return &m_u.m_struct;
}
