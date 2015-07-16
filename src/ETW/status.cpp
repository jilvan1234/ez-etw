#include <windef.h>
#include <WinBase.h>
#include <etw/status.h>

using ez_etw::status;

status ez_etw::to_status(unsigned int s) {
    status ret;
    switch (s)
    {
    case ERROR_SUCCESS:
        ret = status::SUCCESS;
        break;
    case ERROR_INVALID_PARAMETER:
    case ERROR_BAD_LENGTH:
    case ERROR_BAD_PATHNAME:
        ret = status::INVALID_ARGUMENT;
        break;
    case ERROR_ALREADY_EXISTS:
        ret = status::ALREADY_EXIST;
        break;
    case ERROR_ACCESS_DENIED:
        ret = status::ACCES_DENIED;
        break;
    default:
        DebugBreak();
        break;
    }         
    return ret;        
}
