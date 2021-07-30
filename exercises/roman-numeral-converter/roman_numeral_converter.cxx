#include "roman_numeral_converter.hxx"

#include <cassert>
#include <array>
#include <memory_resource>
#include <tuple>
#include <unordered_map>

#include "../../examples/debugging_memory_resource.hxx"

std::array<std::byte, 1000000000L> static_mem;
std::pmr::monotonic_buffer_resource static_resource{
	static_mem.data(),
	static_mem.size(),
	std::pmr::null_memory_resource()
};

/*
int main() {
	std::pmr::set_default_resource(&static_resource);
}
*/

namespace {
	using namespace std;

	array<tuple<string_view, int>, 13> const mapping{
		tuple { "M", 1000 },
		tuple { "CM", 900 },
		tuple { "D", 500 },
		tuple { "CD", 400 },
		tuple { "C", 100 },
		tuple { "XC", 90 },
		tuple { "L", 50 },
		tuple { "XL", 40 },
		tuple { "X", 10 },
		tuple { "IX", 9 },
		tuple { "V", 5 },
		tuple { "IV", 4 },
		tuple { "I", 1 },
	};

	string operator * (string_view part, int count) {
		assert(count >= 0 && count <= 3);
		string result{};
		while (count--) {
			result += part;
		}
		return result;
	}

	int prefixCount(string_view roman, string_view prefix) {
		if (prefix.size() == 1) {
			int count{ 0 };
			for (char const c : roman) {
				if (c == prefix[0] && count < 3)
					++count;
				else
					break;
			}
			return count;
		}
		else if (roman.substr(0, 2) == prefix) {
			return 1;
		}
		else {
			return 0;
		}
	}

	// works on all good cases, but fails to detect certain invalid inputs
	int from_roman_faulty(string_view roman) {
		int result{ 0 };
		for (auto const[prefix, arabic] : mapping) {
			int const factor = prefixCount(roman, prefix);
			roman.remove_prefix(factor * prefix.size());
			result += arabic * factor;

			// optimization as it makes no sense to check the remaining mappings
			if (roman.empty())
				break;
		}

		if (!roman.empty() || result == 0) {
			throw invalid_argument{ "value is not a valid Roman numeral" };
		}

		return result;
	}

	int from_roman_with_reverse_mapping(std::string_view value) {
		// this is only an estimate
		constexpr std::size_t const BUFFER_SIZE
			= 3999
			* sizeof(std::pmr::unordered_map<std::string, int>::node_type)
			* 6;
		static_assert(sizeof(std::pmr::unordered_map<std::string, int>::node_type) == 16);

		static std::array<std::byte, BUFFER_SIZE> buffer;
		// static debugging_memory_resource base_resource{};
		static std::pmr::monotonic_buffer_resource resource{ buffer.data(), buffer.size() , std::pmr::null_memory_resource() };

		// IIFE - Immediatly Invoked Function Expression
		static std::pmr::unordered_map<std::string, int> const reverse_mapping = [](std::pmr::memory_resource * resource) {
			std::pmr::unordered_map<std::string, int> rmap{ resource };
			for (int n = 1; n <= 3999; ++n) {
				rmap.emplace(workshop::to_roman(n), n);
			}
			return rmap;
		}(&resource);

		auto const it = reverse_mapping.find(std::string{ value });
		if (it == reverse_mapping.end()) {
			throw std::invalid_argument{ "value is not a valid Roman numeral" };
		}

		return it->second;
	}
}

std::string operator * (std::string_view part, int count) {
	assert(count >= 0 && count <= 3);
	std::string result{};
	while (count--) {
		result += part;
	}
	return result;
}

std::string workshop::to_roman(int value)
{
	using ::operator*;

	if (value < 1 || value > 3999) {
		throw std::invalid_argument{ "value is not in the range of 1 to 3999" };
	}

	std::string result{};
	for (auto const[roman, arabic] : mapping) {
		int const count = value / arabic;
		value %= arabic;
		result += roman * count;

		// without the custom operator*
		/*
		while (value >= arabic) {
			value -= arabic;
			result += roman;
		}
		*/

		// optimization as it makes no sense to check the remaining mappings
		if (value == 0) {
			break;
		}
	}

	return result;
}

int workshop::from_roman(std::string_view const value)
{
	return from_roman_with_reverse_mapping(value);
}
