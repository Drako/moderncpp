#include <clocale>

// Singleton Design Pattern
// CAUTION: This pattern is mostly considered an anti pattern
// as it comes with a lot of downsides. This is a rare case, where it could be okay.
class LocaleInitializer {
public:
	static LocaleInitializer& instance() {
		// function local statics are initialized exactly once
		// even across multiple threads
		// this happends lazily when the first threads calls the
		// surrounding function
		static LocaleInitializer inst;
		return inst;
	}

private:
	LocaleInitializer() {
		std::setlocale(LC_ALL, "de_DE.UTF-8");
	}
};

int foo() {
	LocaleInitializer::instance();
}

void initializeLocaleOnce() {
	// IIFE - Immediatly Invoked Function Expression
	static auto const initialized = [] {
		return std::setlocale(LC_ALL, "de_DE.UTF-8");
	}();
}
