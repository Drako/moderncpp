#include <gtest/gtest.h>
namespace t = testing;

#include <array>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int array_size(int arr[4]) {
	return sizeof(arr) / sizeof(arr[0]);
}

TEST(ContainerTest, testArray) {
	int arr[3];
	EXPECT_EQ(2, array_size(arr));

	int size = sizeof(arr) / (sizeof(arr[0]));
	std::array<int, 3> foo;
	EXPECT_EQ(3, foo.size());
}

struct Point {
	int x, y;

	Point(int x, int y)
		: x{x}, y{y}
	{}
};

TEST(ContainerTest, testDeque) {
	// double ended queue
	std::deque<Point> stack;
	stack.push_back(Point{ 23, 42 });
	stack.emplace_back(23, 42);
}

#include <initializer_list>

int sum(std::initializer_list<int> numbers) {
	int result = 0;
	for (auto n : numbers) {
		result += n;
	}
	return result;
}

#include <any>
#include <variant>

struct rectangle {
	double width, height;
};

// normally never put stuff into the std namespace
// extending std::hash is one of the rare cases,
// where it is officially allowed
namespace std {
	template <>
	struct hash<rectangle> {
		size_t operator() (rectangle const& r) {
			return hash<double>{}(r.width)
				^ (hash<double>{}(r.height) << 10);
		}
	};
}

struct circle {
	double radius;
};

using shape = std::variant<rectangle, circle>;

struct Lambda {

	template<typename T>
	double operator()(T const&);

	// double operator()(rectangle const&);
	// double operator()(circle const&);
};

enum class ErrorCode {
	DivisionByZero
};

template <typename T>
using Result = std::variant<T, ErrorCode>;

Result<double> divide(double x, double y) {
	if (y == 0.0)
		return ErrorCode::DivisionByZero;
	else
		return x / y;
}

double area(shape const& s) {
	return std::visit([](auto const& s) {
		using Type = decltype(s);
		if constexpr (std::is_same_v<Type, rectangle>) {
			return s.width * s.height;
		}
		else if constexpr (std::is_same_v<Type, circle>) {
			return s.radius * s.radius * 3.1415926;
		}
		else {
			return 0.0;
		}
	}, s);
}

TEST(ContainerTest, testList) {
	int answer = sum({ 19, 23 });

	// class type deduction
	std::list<std::any> numbers{23, 42.0, "1337"};

	

	int illuminati = std::any_cast<int>(numbers.front());
}

// std::unordered_map is not actually unordered
// it is a hash map, so the hashes of the keys are used for ordering
// which makes the actual order non-obvious
// same for std::unordered_set
TEST(ContainerTest, testMap) {
	std::map<char const*, int> const specialNumbers{
		{"answer", 42},
		{"leet", 1337},
	};

	// map["answer"] -> geht nur bei non-const map

	try {
		specialNumbers.at("answer");
	}
	catch (std::out_of_range const& ex) {

	}

	auto const it = specialNumbers.find("leet");
	if (it == specialNumbers.end()) {
		// not found
		return;
	}
	auto const leet = it->second;
}

TEST(ContainerTest, testQueue) {
	std::queue<int> numbers;
	numbers.push(23);
	numbers.pop();
}

TEST(ContainerTest, testSet) {
	// similar to map, but with key and value being the same
}

TEST(ContainerTest, testTuple) {
	std::tuple<int, char, std::string, std::vector<int>>;
}

/*

list
new -> teuer
A <-> B <-> C <-> D <-> E

vector
ABCDEFGHI0000000000
*/

TEST(ContainerTest, testVector) {

	std::vector<int> v{};
	v.reserve(v.size() + 3);
	for (int n = 0; n < 3; ++n)
		v.push_back(n);

	// v.shrink_to_fit();
	std::vector<int> shrinked{ v.begin(), v.end() };
	v = shrinked;

	// v.capacity() vs v.size()
}
