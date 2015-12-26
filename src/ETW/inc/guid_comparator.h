#ifndef EZETW_PRIVATE_GUID_COMPARATOR_H
#define EZETW_PRIVATE_GUID_COMPARATOR_H

#include <guiddef.h>

namespace ez_etw {
    struct guid_comparator {
        bool operator()(const GUID& left, const GUID& right);
    };
}

#endif
