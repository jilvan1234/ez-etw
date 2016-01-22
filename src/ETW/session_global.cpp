#include "inc/session_global.h"

using namespace ez_etw::session_global;
using std::end;

static bool action_insert(container_type& container, container_value_type value, std::shared_ptr<unsigned long> trace_pointer_size) {
	bool success = false;
	if(nullptr != value && nullptr != trace_pointer_size) {
		GUID& key(value->get_event_type());
		bool found = container.find(key) != end(container);
		if(!found) {
			auto itt_inserted = container.emplace(key, value);
			success = itt_inserted.second;
			if(success) {
				itt_inserted.first->second->set_pointer_size(trace_pointer_size);
			}
		}
	}
	return success;
}

static bool action_remove(container_type& container, container_key_type& key) {
	bool success = false;
	const auto itt_find = container.find(key);
	if(itt_find != end(container)) {
		container.erase(itt_find);
		success = container.find(key) == end(container);
	}
	return success;
}

container_type& ez_etw::session_global::event_parser_do(event_parser_action action, container_value_type value, std::shared_ptr<unsigned long> trace_pointer_size, bool* success) {
	static container_type parsables;
	bool action_success = false;
	switch(action) {
		case event_parser_action::insert:
		action_success = action_insert(parsables, value, trace_pointer_size);
		break;
		case event_parser_action::remove:
		action_success = action_remove(parsables, value->get_event_type());
		break;
	}
	if(nullptr != success) {
		*success = action_success;
	}
	return parsables;
}
