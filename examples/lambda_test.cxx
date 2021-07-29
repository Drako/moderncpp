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
	int add(int a, int b) {
		return a + b;
	}
};

TEST(LambdaTest, StoringFunctions) {
	using Adder = s::function<int(int, int)>;

	AdderClass adder{};

	s::vector<Adder> adders{
		&add_function,
		add_functor{},
		add_lambda,
		s::bind(&AdderClass::add, &adder, p::_1, p::_2), // usually avoid bind
	};

	int const expected = 42;
	for (auto const& a : adders) {
		EXPECT_EQ(expected, a(19, 23));
	}
}

namespace workshop {
	// here we do pattern matching over the difference kinds of callables
	template <typename, typename Enable = void>
	struct result_of;

	template <typename R, typename... A>
	struct result_of<R(A...)>
	{
		using type = R;
	};

	template <typename R, typename... A>
	struct result_of<R(*)(A...)>
	{
		using type = R;
	};

	template <typename R, typename C, typename... A>
	struct result_of<R(C::*)(A...)>
	{
		using type = R;
	};

	template <typename R, typename C, typename... A>
	struct result_of<R(C::*)(A...) const>
	{
		using type = R;
	};

	template <typename Functor>
	struct result_of<Functor, s::enable_if_t<s::is_class_v<Functor>>> {
		using type = typename result_of<decltype(&Functor::operator())>::type;
	};

	template <typename T>
	using result_of_t = typename result_of<T>::type;
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
