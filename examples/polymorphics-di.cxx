#include <iostream>
#include <string>
#include <string_view>

// std::ostream
struct OutputDevice {
	virtual ~OutputDevice() = default;

	virtual void printLine(std::string_view message) = 0;
};

// std::istream
struct InputDevice {
	virtual ~InputDevice() = default;

	virtual std::string readLine() = 0;
};

// std::iostream
struct IODevice
	: InputDevice
	, OutputDevice
{
};

class ConsoleDevice : public IODevice {
public:
	void printLine(std::string_view message) override {
		std::cout << message << std::endl;
	}

	std::string readLine() override {
		std::string result;
		std::getline(std::cin, result);
		return result;
	}
};

class Greeter {
public:
	Greeter(OutputDevice& out) : out{out} {}

	void greet(std::string_view whom) {
		out.printLine("Hello, " + std::string{ whom });
	}

private:
	OutputDevice& out;
};

int main() {
	ConsoleDevice console{};
	Greeter app{ console };
	app.greet("Felix");
	return 0;
}
