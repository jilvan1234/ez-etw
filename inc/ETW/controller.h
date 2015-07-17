#ifndef EZETW_CONTROLLER_H
#define EZETW_CONTROLLER_H

#include <memory>
#include <string>
#include <etw/status.h>
#include <etw/properties.h>

namespace ez_etw {
    struct controller_ctx;

    struct controller {
        static const std::wstring& KERNEL_NAME;
        controller(const std::wstring& name);
        ~controller();
        status start(const properties& props);
        status stop();
        bool is_kernel_logger() const;
        bool is_running() const;
    private:
        using ptr_type = std::unique_ptr<controller_ctx>;
        ptr_type m_ctx;
    };
}

#endif
