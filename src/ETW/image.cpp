#include <etw/image.h>

using ez_etw::parsed_events::image;

using ez_etw::event;

image::image(const event& evt, unsigned long pointer_size)
:parsed_event(evt, pointer_size) {
}

uint64_t image::get_base_address() const {
    return m_base_address;
}

const std::wstring& image::get_image_filename() const {
    return m_image_filename;
}

uint64_t image::get_module_size() const {
    return m_module_size;
}

bool image::set_image_filename(std::stringstream& ss, const std::string& buffer_str) {
    return set(ss, buffer_str, m_image_filename);
}