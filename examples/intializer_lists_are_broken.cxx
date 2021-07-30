#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "debugging_memory_resource.hxx"

std::array<std::byte, 1'000'000L> static_mem;
std::pmr::monotonic_buffer_resource static_resource{
	static_mem.data(),
	static_mem.size(),
	std::pmr::null_memory_resource()
};
bool initialized = [] {
	std::pmr::set_default_resource(&static_resource);
	return true;
}();

using std::pmr::string;
using std::pmr::vector;

int main() {
	debugging_memory_resource debugger{};
	std::pmr::set_default_resource(&debugger);

	std::cout << "using initializer list" << std::endl;

	string __data[3]{ "alpha", "bravo", "a very long long long string" };
	// std::initializer_list<pmr::string>

	vector<string> strings{ "alpha", "bravo", "charlie" };

	std::cout << "using emplace" << std::endl;
	vector<string> strings2;
	strings2.reserve(3);
	for (char const* txt : { "alpha", "bravo", "a very long long long string" }) {
		strings2.emplace_back(txt);
	}

	std::cin.get();
	return 0;
}
