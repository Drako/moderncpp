#include <gtest/gtest.h>
namespace t = testing;

#include <functional>
#include <type_traits>
#include <vector>
namespace s = std;
namespace p = std::placeholders;

int add_function(int a, int b) {
	return a + b;
}

struct add_functor final {
	int operator() (int a, int b) const {
		return a + b;
	}
};

auto const add_lambda = [](int a, int b) {
	return a + b;
};

struct AdderClass final {
	// non-static member function
	int add(int a, int b) const {
		return a + b;
	}

	auto adder() {
		return [*this](int a, int b) { return add(a, b); };
	}
};

auto add_curry(AdderClass* _this) {
	return [_this](int a, int b) { return a + b; };
}

void test() {
	add_curry(new AdderClass{})(23, 42);
}

template <typename Fn>
auto reduce(int begin, int end, Fn&& reducer)
-> std::enable_if_t<std::is_same_v<int, decltype(reducer(begin, end))>, int>
{
	int result{};
	for (int n = begin; n < end; ++n) {
		result = reducer(result, n);
	}
	return result;
}

template <typename T, typename... Args>
T construct(Args&&... args) {
// (Arg1&& arg1, Arg2&& arg2)
	return T{std::forward<Args>(args)...};
	// forward<Arg1>(arg1), forward<Arg2>(arg2)
}

// evil old C style variadic function
#include <cstdarg>
int vararg_sum(int count, ...) {
	int result{};
	va_list argList;
	va_start(argList, count);
	for (int n = 0; n < count; ++n) {
		result += va_arg(argList, int);
	}
	va_end(argList);
	return result;
}

template <typename... Args>
auto variadic_sum(Args... args)
// Arg1 == int && Arg2 == int && true
-> std::enable_if_t<(std::is_same_v<int, Args> && ... && true), int>
{
	return (0 + ... + args);
}

// function template
template <typename... Args>
auto generic_sum(Args&&... args) {
	return (std::forward<Args>(args) + ...);
}

/*
template <typename Class>
int reduce(int begin, int end, Class* inst, int (Class::* reducer)(int, int)) {
	int result{};
	for (int n = begin; n < end; ++n) {
		result = (*inst)->reducer(result, n);
	}
	return result;
}

template <typename Class>
int reduce(int begin, int end, Class* inst, int (Class::* reducer)(int, int) const) {
	int result{};
	for (int n = begin; n < end; ++n) {
		result = (*inst)->reducer(result, n);
	}
	return result;
}
*/

#include <string>
using namespace std::literals::string_literals;

TEST(LambdaTest, StoringFunctions) {
	using Adder = s::function<int(int, int)>;

	double added = generic_sum(23.0, 42.0);
	EXPECT_EQ(65.0, added);

	std::string catted = generic_sum("foo"s, "bar"s, "baz"s);
	EXPECT_EQ("foobarbaz"s, catted);

	AdderClass adder{};

	s::vector<Adder> adders{
		&add_function,
		add_functor{},
		add_lambda,
		// s::bind(&AdderClass::add, &adder, p::_1, p::_2), // usually avoid bind
		[&adder](int a, int b) { return adder.add(a, b); },
	};

	int const expected = 42;
	for (auto const& a : adders) {
		EXPECT_EQ(expected, a(19, 23));
	}
}

template <typename T>
struct Point final {
	T x{}, y{};

	constexpr Point(T x, T y) : x{x}, y{y}
	{}
};

template <typename T>
constexpr Point<T> operator + (Point<T> const& lhs, Point<T> const& rhs) {
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

#include <array>
std::array<int, (Point{ 23, 42 } +Point{ 1337, 0 }).x > arr;

namespace workshop {
	// here we do pattern matching over the difference kinds of callables
	template <typename, typename Enable = void>
	struct result_of;

	template <typename R, typename... A>
	struct result_of<R(A...)>
	{
		using type = R;
	};

	// free standing function pointer
	// static member function pointer
	template <typename R, typename... A>
	struct result_of<R(*)(A...)>
	{
		using type = R;
	};

	// non-static non-const member function pointer
	template <typename R, typename C, typename... A>
	struct result_of<R(C::*)(A...)>
	{
		using type = R;
	};

	// non-static const member function pointer
	template <typename R, typename C, typename... A>
	struct result_of<R(C::*)(A...) const>
	{
		using type = R;
	};

	// Functors and Lambdas
	template <typename Functor>
	struct result_of<Functor, s::enable_if_t<s::is_class_v<Functor>>> {
		using type = typename result_of<decltype(&Functor::operator())>::type;
	};

	template <typename T>
	using result_of_t = typename result_of<T>::type;


	template <typename A, typename B>
	struct is_same {
		constexpr static bool const value = false;
	};

	template <typename T>
	struct is_same<T, T> {
		constexpr static bool const value = true;
	};

	template <typename A, typename B>
	constexpr static bool const is_same_v = is_same<A, B>::value;
}

TEST(LambdaTest, determiningResult) {
	// uses the R(A...) specialization
	static_assert(std::is_same_v<int, workshop::result_of_t<decltype(add_function)>>);
	
	// uses the R(*)(A...) specialization
	static_assert(std::is_same_v<int, workshop::result_of_t<decltype(&add_function)>>);
	
	// uses the R(C::*)(A...) specialization
	static_assert(std::is_same_v<int, workshop::result_of_t<decltype(&AdderClass::add)>>);

	// uses the specialization for Functors
	// and afterwards the "R(C::*)(A...) const" one.
	static_assert(std::is_same_v<int, workshop::result_of_t<add_functor>>);

	static_assert(std::is_same_v<int, workshop::result_of_t<decltype(add_lambda)>>);
}
