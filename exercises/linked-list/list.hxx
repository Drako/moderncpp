#pragma once

#include <cstddef>

namespace workshop {
	template <typename T>
	class List {
	public:
		inline List(...) {}

		inline bool empty() const {
			return true;
		}

		inline std::size_t size() const {
			return 0u;
		}

		inline void push_front(T const&) {
		}

		inline void pop_front() {
		}

		inline T front() const {
			return {};
		}
	};
}
