#ifndef EZETW_ETWCONTROLLER_H
#define EZETW_ETWCONTROLLER_H

#include <memory>

namespace ez_etw {
    struct controller_impl;

    struct controller {
        bool start(bool force_start = false);
        void stop();
        unsigned long get_last_status();
    private:
        std::unique_ptr<controller_impl> m_pimpl;
    };
}

#endif
