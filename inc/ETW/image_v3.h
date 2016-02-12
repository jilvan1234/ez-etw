#ifndef EZETW_PARSEDEVENTS_IMAGES_V2_H
#define EZETW_PARSEDEVENTS_IMAGES_V2_H

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
                unsigned int m_timestamp;
                unsigned char m_sig_level;
                unsigned char m_sig_type;
                unsigned short m_reserved0;
                uintptr_t m_default_base;
                unsigned int m_reserved1;
                unsigned int m_reserved2;
                unsigned int m_reserved3;
                unsigned int m_reserved4;
            };
        }
    }
}

#endif
