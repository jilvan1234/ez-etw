#include <etw/guid_comparator.h>

using ez_etw::guid_comparator;

bool guid_comparator::operator()(const GUID& left, const GUID& right) {
    return left.Data1 < right.Data1 &&
        left.Data2 < right.Data2 &&
        left.Data3 < right.Data3 &&
        left.Data4 < right.Data4;
}
