#ifndef EZETW_PROPERTIES_H
#define EZETW_PROPERTIES_H

#include <memory>

namespace ez_etw {
    struct properties_buffer;
    
    enum resolution_speed {
        low = 2,
        high = 1,
        highest = 3       
    };
    enum log_mode {
        real_time = 0x00000100
    };
    enum kernel_flags {
        process = 0x00000001
    };

    struct properties {
        using properties_buffer_ptr = std::unique_ptr<properties_buffer>;
        properties(const resolution_speed& speed, const log_mode& mode, const kernel_flags& flags);
        const properties_buffer_ptr& get_buffer() const;
    private:
        properties_buffer_ptr m_buff;
    };
}

#endif
