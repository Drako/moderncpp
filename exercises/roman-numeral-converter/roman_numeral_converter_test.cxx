#include "roman_numeral_converter.hxx"
namespace w = workshop;

#include <gtest/gtest.h>
namespace t = testing;

#include <tuple>
namespace s = std;

// Test Funktionen
using InvalidRomanNumeralFixture = t::TestWithParam<char const*>;
TEST_P(InvalidRomanNumeralFixture, throws_invalid_argument)
{
	ASSERT_THROW(w::from_roman(GetParam()), s::invalid_argument);
}

using InvalidIntegerFixture = t::TestWithParam<int>;
TEST_P(InvalidIntegerFixture, throws_invalid_argument)
{
	ASSERT_THROW(w::to_roman(GetParam()), s::invalid_argument);
}

using ConversionParams = s::tuple<int, char const*>;
using RomanNumeralConversionFixture = t::TestWithParam<ConversionParams>;

TEST_P(RomanNumeralConversionFixture, converts_to_roman)
{
	auto const& [value, roman] = GetParam();
	ASSERT_EQ(roman, w::to_roman(value));
}

TEST_P(RomanNumeralConversionFixture, converts_from_roman)
{
	auto const& [value, roman] = GetParam();
	ASSERT_EQ(value, w::from_roman(roman));
}

// Test Daten
INSTANTIATE_TEST_SUITE_P(
	RomanNumeralConverterTest,
	InvalidRomanNumeralFixture,
	t::Values("", "A", "ABC", "alpha", "mx", "23", "M!", "XF", "MMS", "IIX", "IXI", "IM", "IIM", "IIIM", "IIII", "IIIII", "IIIIIIII", "LC", "LL", "CCCC", "MMMM", "MIM", "XM", "MXM", "XXC", "CXLICMIM", "XCX", "MXCX", "XCXCXC", "MIMIMI", "XCXL"));

INSTANTIATE_TEST_SUITE_P(
	RomanNumeralConverterTest,
	InvalidIntegerFixture,
	t::Values(0, 4000, 65536, -100));

constexpr ConversionParams cp(int const value, char const* const roman) {
	return { value, roman };
}
INSTANTIATE_TEST_SUITE_P(
	RomanNumeralConverterTest,
	RomanNumeralConversionFixture,
	t::Values(
		cp(1, "I"),
		cp(2, "II"),
		cp(4, "IV"),
		cp(5, "V"),
		cp(6, "VI"),
		cp(9, "IX"),
		cp(10, "X"),
		cp(11, "XI"),
		cp(12, "XII"),
		cp(14, "XIV"),
		cp(15, "XV"),
		cp(16, "XVI"),
		cp(23, "XXIII"),
		cp(49, "XLIX"),
		cp(50, "L"),
		cp(51, "LI"),
		cp(99, "XCIX"),
		cp(100, "C"),
		cp(101, "CI"),
		cp(149, "CXLIX"),
		cp(500, "D"),
		cp(999, "CMXCIX"),
		cp(1000, "M"),
		cp(1001, "MI"),
		cp(1400, "MCD"),
		cp(1500, "MD"),
		cp(1990, "MCMXC"),
		cp(3999, "MMMCMXCIX")));
