#ifndef EZETW_PRIVATE_SESSION_GLOBAL_H
#define EZETW_PRIVATE_SESSION_GLOBAL_H

#include <map>
#include <Guiddef.h>
#include <memory>
#include <etw/guid_comparator.h>
#include <etw/event_parser.h>

namespace ez_etw {
	namespace session_global {
		enum event_parser_action {
			query,
			insert,
			remove
		};
		using container_value_type = std::shared_ptr<ez_etw::event_parser>;
		using container_key_type = GUID;
		using container_type = std::map<container_key_type, container_value_type, ez_etw::guid_comparator>;
		container_type& event_parser_do(event_parser_action action, container_value_type value, std::shared_ptr<unsigned long> trace_pointer_size, bool* success);
	}
}

#endif
