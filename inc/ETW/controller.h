#ifndef EZETW_CONTROLLER_H
#define EZETW_CONTROLLER_H

#include <string>
#include <etw/properties.h>
#include <etw/status.h>

namespace ez_etw {
    struct controller {
        static const std::wstring kernel_name;
        controller(const std::wstring& name);
        ez_etw::status start(const properties& props);
        ez_etw::status stop();
        bool is_running() const;
        bool is_kernel_logger() const;
		ez_etw::status system_stop(const std::wstring& name);
    private:
        const std::wstring& m_name;
        const bool m_is_kernel_logger;
        bool m_is_running;
    };
}

#endif
