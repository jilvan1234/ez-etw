#ifndef EZETW_ETWSESSION_H
#define EZETW_ETWSESSION_H

#include <string>
#include <memory>
#include <array>

namespace ez_etw {
    struct session {
        using string_type = std::wstring;
        using props_type = std::shared_ptr<unsigned char>;
        static const unsigned long PROPERTIES_SIZE;
        session(const string_type& logger);
        props_type get_properties();
    private:
        static const unsigned long MAX_SESSION_NAME;
        static const unsigned long MAX_LOGFILE_NAME;      
        const string_type m_logger;
        unsigned long m_handle;
        props_type m_props;
    };    
}

#endif
