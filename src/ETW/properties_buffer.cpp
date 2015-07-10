#include "inc/properties_buffer.h"
#include <memory>

using ez_etw::resolution_speed;
using ez_etw::log_mode;
using ez_etw::kernel_flags;
using ez_etw::properties_buffer;
using std::memset;

properties_buffer::properties_buffer(const resolution_speed& speed, const log_mode& mode, const kernel_flags& flags) {
    const size_t buff_size = sizeof(m_u);
    memset(&m_u, 0, buff_size);
    m_u.m_struct.Wnode.BufferSize = buff_size;
    m_u.m_struct.Wnode.ClientContext = speed;
    m_u.m_struct.LoggerNameOffset = buff_size;
    m_u.m_struct.LogFileNameOffset = buff_size + MAX_STRING_LENGTGH;
    m_u.m_struct.LogFileMode = mode;
    if(flags != 0) {
        m_u.m_struct.Wnode.Guid = SystemTraceControlGuid;
    }
}

properties_buffer::buffer_type* const properties_buffer::get_buffer() {
    return &m_u.m_buff;
}

EVENT_TRACE_PROPERTIES* const properties_buffer::get_struct() {
    return &m_u.m_struct;
}
