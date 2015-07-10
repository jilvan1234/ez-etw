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
    case ERROR_BAD_LENGTH:
        ret = status::INVALID_ARGUMENT;
        break;
    case ERROR_ALREADY_EXISTS:
        ret = status::ALREADY_EXIST;
        break;
    default:
        DebugBreak();
        break;
    }         
    return ret;        
}
