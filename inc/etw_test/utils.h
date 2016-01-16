#ifndef EZETW_UTILS_H
#define EZETW_UTILS_H

#include <string>

namespace test_utils {
	namespace process {
		bool launch(const std::string& filename, uintptr_t& process_handle);
		bool terminate(const uintptr_t& process_handle);
	}
}

#endif
