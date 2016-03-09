#ifndef EZETW_PARSEDEVENTS_IMAGES_V3_H
#define EZETW_PARSEDEVENTS_IMAGES_V3_H

#include <etw/image.h>

namespace ez_etw {
    namespace parsed_events {
        namespace images {
            struct v3 : ez_etw::parsed_events::image {
                v3(const event& evt, unsigned long pointer_size);
                virtual ~v3() = default;
            private:
                uint32_t m_process_id;
                uint32_t m_checksum;
                uint32_t m_timestamp_image;
                uint8_t m_sig_level;
                uint8_t m_sig_type;
                uint16_t m_reserved0;
                uint64_t m_default_base;
                uint32_t m_reserved1;
                uint32_t m_reserved2;
                uint32_t m_reserved3;
                uint32_t m_reserved4;
            };
        }
    }
}

#endif
