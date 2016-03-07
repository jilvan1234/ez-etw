#include <etw/image_v3.h>

using ez_etw::event;
using ez_etw::parsed_events::images::v3;
using std::string;
using std::stringstream;

/*
[dynamic: ToInstance, EventType{10, 2, 3, 4}]
class Image_Load : Image
{
[WmiDataId(1), pointer, read] uint32 ImageBase;
[WmiDataId(2), pointer, read] uint32 ImageSize;
[WmiDataId(3), read] uint32 ProcessId;
[WmiDataId(4), read] uint32 ImageChecksum;
[WmiDataId(5), read] uint32 TimeDateStamp;
[WmiDataId(6), read] uint8 SignatureLevel;
[WmiDataId(7), read] uint8 SignatureType;
[WmiDataId(8), read] uint16 Reserved0;
[WmiDataId(9), pointer, read] uint32 DefaultBase;
[WmiDataId(10), read] uint32 Reserved1;
[WmiDataId(11), read] uint32 Reserved2;
[WmiDataId(12), read] uint32 Reserved3;
[WmiDataId(13), read] uint32 Reserved4;
[WmiDataId(14), StringTermination("NullTerminated"), format("w"), read] string FileName;
};
*/

//#include <iostream>

v3::v3(const event& evt, unsigned long pointer_size)
:image(evt, pointer_size) {
    if(evt.get_type() == event::type::load ||
        evt.get_type() == event::type::end || // unload
        evt.get_type() == event::type::data_collection_start ||
        evt.get_type() == event::type::data_collection_end) {
        string& buffer_str = *evt.get_buffer();
        stringstream ss(buffer_str);
        if(ss.read(reinterpret_cast<char*>(&m_base_address), sizeof(m_base_address)).good() &&
            ss.read(reinterpret_cast<char*>(&m_module_size), sizeof(m_module_size)).good() &&
            ss.read(reinterpret_cast<char*>(&m_process_id), sizeof(m_process_id)).good() &&
            ss.read(reinterpret_cast<char*>(&m_checksum), sizeof(m_checksum)).good() &&
            ss.read(reinterpret_cast<char*>(&m_timestamp), sizeof(m_timestamp)).good() &&
            ss.read(reinterpret_cast<char*>(&m_sig_level), sizeof(m_sig_level)).good() &&
            ss.read(reinterpret_cast<char*>(&m_sig_type), sizeof(m_sig_type)).good() &&
            ss.read(reinterpret_cast<char*>(&m_reserved0), sizeof(m_reserved0)).good() &&
            ss.read(reinterpret_cast<char*>(&m_default_base), sizeof(m_default_base)).good() &&
            ss.read(reinterpret_cast<char*>(&m_reserved1), sizeof(m_reserved1)).good() &&
            ss.read(reinterpret_cast<char*>(&m_reserved2), sizeof(m_reserved2)).good() &&
            ss.read(reinterpret_cast<char*>(&m_reserved3), sizeof(m_reserved3)).good() &&
            ss.read(reinterpret_cast<char*>(&m_reserved4), sizeof(m_reserved4)).good() &&
            set_image_filename(ss, buffer_str)) {
            //std::cout << m_process_id << " ";
			//std::wcout << m_image_filename << std::endl;
            m_status = success;
        }
    } else {
        m_status = status::unsupported_type;
    }
}
