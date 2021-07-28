#include <gtest/gtest.h>
namespace t = testing;

#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;

// Fira Code, JetBrains Mono, Hasklig
// evtl Ligaturen aktivieren beim Editor

namespace initialization {
	struct Point {
		int x, y;
	};

	void _() {
		// Copy-Initialization
		int a = 42;
		std::string pi = "3.1415926";

		// Aggregatinitialisierung
		int arr[10] = { 1, 2, 3, 4, 5 };
		Point p = { 10, 10 };

		// Konstruktorinitialisierung
		std::string foo("foo");
		std::wstring bar(L"bar");

		// Uniform-Initialization
		int n{ 42 };
		double arr2d2[]{ 13.37, 42.0, 23.0 };
		std::string s{ "test" };
	}
}

// char const*

// Zeiger auf einzelnen char? Zeiger auf Anfang eines char-Arrays?
// Allokation? Freigeben?

TEST(StringTest, testStringStream) {
	std::ostringstream out;
	out << std::hex << 23 << " + " << 42 << " = " << (23 + 42);
	auto s = out.str();
}

#include <algorithm>
#include <cctype>

TEST(StringTest, testString) {
	// User-defined literal
	// "Hello world!"s;

	std::string const greeting{ "Hello, "s };
	std::string const message = greeting + "World";

	char const * part1 = "foo";
	char const * part2 = "bar";
	auto combined = part1 + ""s + part2;

	// short string optimization
	std::string shrt{ "123456789" }; // no allocation happening

	std::transform(combined.begin(), combined.end(), combined.begin(), [](char c) {
		return std::toupper(c);
	});

	// shrt now has undefined content
	std::string target{ std::move(shrt) };
}

void greet(std::string_view whom) {
	
}

TEST(StringTest, testStringView) {
	std::string name = "Benedict Cumberbatch";
	greet(name);
}
