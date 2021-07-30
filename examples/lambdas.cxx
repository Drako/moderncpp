/*
() = > { ... }
{ args -> ... }
() -> {}

auto const fn = []() {}

f(x)
g(x)

h(f, g) = f >> g
*/

#include <gtest/gtest.h>

#include <algorithm>
#include <functional>

// Closure = lambda that captures stuff

template <typename T>
std::function<T(T)> chain(std::function<T(T)> a, std::function<T(T)> b) {
	return [a, b](T x) -> T {
		return b(a(x));
	};
}

void foo() {
	/***********
	 Minimal lambda
	************/
	auto const min_lambda = [] {}; // void()

	struct anynomous_struct {
		void operator()() const {}
	} min_functor;

	// lambdas without capture can be used as simple functions
	void(*ptr)() = min_lambda;

	min_lambda();
	min_functor();

	/***********
	 Lambda with parameters
	************/
	auto const add_lambda = [](int a, int b) {
		return a + b;
	};

	struct anon2 {
		int operator()(int a, int b) const {
			return impl(a, b);
		}

		static int impl(int a, int b) {
			return a + b;
		}

		using Pointer = int(*)(int, int);

		explicit operator Pointer() const {
			return &impl;
		}
	} add_functor;

	int(*ptr2)(int, int) = add_lambda;
	anon2::Pointer ptr3 = static_cast<anon2::Pointer>(add_functor);

	/***********
	 Lambda with capture
	************/

	// currying - when a function is called with less parameters than it needs
	// it returns another function that takes the remaining parameters
	auto const multiply_lambda = [](int a) {
		return [a](int b) {
			return a + b;
		};
	};

	multiply_lambda(23)(42);
	auto const multiply_by_2 = multiply_lambda(2);
	multiply_by_2(10);

	class inner {
		int a;

	public:
		inner(int a) : a{a} {}

		int operator()(int b) const { return a + b; }
	};

	struct {
		inner operator()(int a) const {
			return inner{ a };
		}
	} multiply_functor;

	multiply_functor(23)(42);
	auto const multiply_by_3 = multiply_functor(3);
	multiply_functor(10);
	

}

/***********
 Lambda with mutable capture
************/
TEST(LambdaTests, sequenceGenerator) {
	auto const make_sequence_generator = [](int start) {
		int current = start;
		return [current]() mutable {
			return current++;
		};
	};

	auto gen = make_sequence_generator(23);
	EXPECT_EQ(23, gen());
	EXPECT_EQ(24, gen());
	EXPECT_EQ(25, gen());

	class generator {
		int current;

	public:
		generator(int current) : current{current} {}

		int operator()() {
			return current++;
		}
	};

	auto gen2 = generator{ 42 };
	EXPECT_EQ(42, gen2());
	EXPECT_EQ(43, gen2());
	EXPECT_EQ(44, gen2());

	// since C++14:
	// we can capture stuff that doesn't exist in the outer scope
	auto gen3 = [current = 1337]()mutable{
		return current++;
	};
	EXPECT_EQ(1337, gen3());
	EXPECT_EQ(1338, gen3());
	EXPECT_EQ(1339, gen3());
}

struct {
	template <typename T>
	T operator() (T const& value) {
		return value;
	}
} identity_functor;

TEST(LambdaTests, genericLambda) {
	// generic lambda (since C++14)
	auto const identity_lambda = [](auto&& value) {
		return std::forward<decltype(value)>(value);
	};

	EXPECT_EQ(42, identity_lambda(6 * 7));
	EXPECT_EQ(23, identity_functor(19 + 4));

	// C++20 -> explicit template parameters
	/*
		auto const identity = [] <typename T> (T&& value) {
			return std::forward<T>(value);
		};
	*/
}

auto evil() {
	int n = 0;
	return [&n] {
		return n++;
	};
}

TEST(LambdaTest, automaticCapture) {
	int a = 1, b = 2, c = 3;
	int*p = &a;
	
	// = and & make everything in the surrounding scope available
	// but only capture what is actually used in the lambda
	auto const fn = [=] {
		*p = 23;

		return a + b;
	};

	auto const fn2 = [&, b]() mutable {
		p = &b;// int*&

		// even though lambda state itself is const here,
		// we can modify external mutable state via reference
		c = 42;
	};
}

struct opaque;
int init(opaque** op);

void x() {
	opaque* op = nullptr;
	if (init(&op) == -1) {
		// error
	}
}

/*
// same -> constant memory
int const
const int

// variable pointer to variable memory
int* p
p = (int*)0xAFFE; // variable pointer
*p = 42; // variable memory

// variable pointer to constant memory
const char* // west const
char const* // east const

// constant pointer to variable memory
// somewhat equivalent to a reference
char* const

// constant pointer to constant memory
const char* const
char const* const

char // single character
char* // pointer to single character,
      // pointer to beginning of a string,
      // pointer to beginning of a character array
using cstring = char*;

char**
cstring* // pointer to single string
         // pointer to beginning of a string array
            -> int main(int argc, cstring* argv)
using cstrings = cstring*;
*/
