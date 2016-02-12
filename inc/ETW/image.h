#ifndef EZETW_PARSEDEVENTS_IMAGE_H
#define EZETW_PARSEDEVENTS_IMAGE_H

#include <etw/parsed_event.h>

namespace ez_etw {
    namespace parsed_events {
        struct image : public ez_etw::parsed_event {
            image(const event& evt, unsigned long pointer_size);
            virtual ~image() = default;
			uintptr_t get_base_address() const;
			uintptr_t get_module_size() const;
			const std::wstring& get_image_filename() const;
        protected:
            bool set_image_filename(std::stringstream& ss, const std::string& buffer_str);
			uintptr_t m_base_address;
			uintptr_t m_module_size;
			std::wstring m_image_filename;
        };
    }
}

#endif
