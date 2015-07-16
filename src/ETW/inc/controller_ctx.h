#ifndef EZETW_PRIVATE_CONTROLLER_CTX_H
#define EZETW_PRIVATE_CONTROLLER_CTX_H

#include <windef.h>
#include <winbase.h>
#include <wmistr.h>
#include <evntrace.h>
#include <string>
#include <etw/properties.h>
#include <etw/status.h>

namespace ez_etw {
    struct controller_ctx {
        controller_ctx(const std::wstring& name);
        ez_etw::status start(const properties& props);
        ez_etw::status stop();
        bool is_running() const;
        bool is_kernel_logger() const;
    private:
        //TRACEHANDLE get_platform_handle_error();
        TRACEHANDLE m_handle;
        const std::wstring& m_name;
        const bool m_is_kernel_logger;
        bool m_is_running;

        static const TRACEHANDLE HANDLE_DEFAULT_VALUE;
    };
}

#endif
