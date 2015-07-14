#include <etw/properties.h>
#include "inc/properties_buffer.h"

using ez_etw::resolution_speed;
using ez_etw::log_mode;
using ez_etw::kernel_flags;
using ez_etw::properties;
using ez_etw::properties_buffer;

properties::properties(const kernel_flags& flags)
:m_buff(std::make_unique<properties_buffer>(flags))
{
}

properties::~properties() {
}

void properties::set_resolution_speed(const resolution_speed& speed) {
    m_buff->get_struct()->Wnode.ClientContext = speed;
}

void properties::set_log_mode(const log_mode& mode) {
    m_buff->get_struct()->LogFileMode = mode;
}

properties::ptr_type::element_type* const properties::get_ptr() const {
    return m_buff.get();
}
