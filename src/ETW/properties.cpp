#include <etw/properties.h>

#include "inc/properties_buffer.h"

using ez_etw::properties;
using ez_etw::properties_buffer;

properties::properties()
:m_buff(std::make_unique<properties_buffer>())
{
}
