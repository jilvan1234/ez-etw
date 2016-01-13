#ifndef EZETW_STATUS_H
#define EZETW_STATUS_H

namespace ez_etw {
    enum status {
        success,
        invalid_argument,
        already_exist,
        not_running,
        access_denied
    };
    status to_status(unsigned int status);
}

#endif
