#include <clocale>


class LocaleInitializer {
public:
	static LocaleInitializer& instance() {
		static LocaleInitializer inst;
		return inst;
	}

private:
	LocaleInitializer() {
		std::setlocale(LC_ALL, "de_DE.UTF-8");
	}
};


void initializeLocaleOnce() {
	// IIFE - Immediatly Invoked Function Expression
	static auto const initialized = [] {
		return std::setlocale(LC_ALL, "de_DE.UTF-8");
	}();
}


int foo() {
	LocaleInitializer::instance();


}
