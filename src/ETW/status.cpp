#include <windef.h>
#include <WinBase.h>
#include <etw/status.h>

using ez_etw::status;

status ez_etw::to_status(unsigned int s) {
    status ret;
    switch (s)
    {
    case ERROR_SUCCESS:
        ret = status::success;
        break;
    case ERROR_INVALID_PARAMETER:
    case ERROR_BAD_LENGTH:
    case ERROR_BAD_PATHNAME:
        ret = status::invalid_argument;
        break;
    case ERROR_ALREADY_EXISTS:
        ret = status::already_exist;
        break;
    case ERROR_ACCESS_DENIED:
        ret = status::access_denied;
        break;
    default:
        DebugBreak();
        break;
    }         
    return ret;        
}
