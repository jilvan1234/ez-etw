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
    enum kernel_flags : unsigned long {
        none = 0,
        process = 0x00000001,
        image = 0x00000004
    };

    struct properties {
        using ptr_type = std::unique_ptr<properties_buffer>;
        properties(const log_mode& mode, const kernel_flags& flags = kernel_flags::none);
        ~properties();
        void set_resolution_speed(const resolution_speed& speed);
        ptr_type::element_type* const get_ptr() const;
    private:
        ptr_type m_buff;
    };
}

#endif
