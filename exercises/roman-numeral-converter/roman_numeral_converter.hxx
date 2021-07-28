#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

namespace workshop {
	/**
	 * @brief to_roman converts the given value to its Roman numeral representation.
	 * @param value An integer in the range of 1 to 3999.
	 * @return A string containing the Roman numeral.
	 * @throws std::invalid_argument if the value is not in the expected range.
	 */
	std::string to_roman(int value);

	/**
	 * @brief from_roman parses a Roman numeral to get its integer value.
	 * @param value The Roman numeral (letters must be all uppercase).
	 * @return An integer in the range of 1 to 3999.
	 * @throws std::invalid_argument if the input is not a valid Roman numeral.
	 */
	int from_roman(std::string_view value);
}
