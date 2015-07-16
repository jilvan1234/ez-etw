#ifndef EZETW_STATUS_H
#define EZETW_STATUS_H

namespace ez_etw {
    enum status {
        SUCCESS,
        INVALID_ARGUMENT,
        ALREADY_EXIST,
        NOT_RUNNING,
        ACCES_DENIED
    };
    status to_status(unsigned int status);
}

#endif
