#ifndef EZETW_PROPERTIES_H
#define EZETW_PROPERTIES_H

#include <memory>

namespace ez_etw {
    union properties_buffer;

    struct properties {
        properties();
    private:
        std::unique_ptr<properties_buffer> m_buff;
    };
}

#endif
