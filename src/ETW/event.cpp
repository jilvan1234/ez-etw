#include <ETW/event.h>

using ez_etw::event;

event::event(unsigned int process_id, const char* buffer, size_t buffer_size)
:m_process_id(process_id) {
	m_buffer.write(buffer, buffer_size);
}
