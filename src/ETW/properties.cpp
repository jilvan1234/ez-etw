#include <etw/properties.h>

#include "inc/properties_buffer.h"

using ez_etw::resolution_speed;
using ez_etw::log_mode;
using ez_etw::kernel_flags;
using ez_etw::properties;
using ez_etw::properties_buffer;

properties::properties(const resolution_speed& speed, const log_mode& mode, const kernel_flags& flags)
:m_buff(std::make_unique<properties_buffer>(speed, mode, flags))
{
}

const properties::properties_buffer_ptr& properties::get_buffer() const {
    return m_buff;
}
