// RAII - Resource Acquisition Is Initialization

#include <string>

int a = 0;
std::string foo; // equivalent to "std::string foo{};"

// POD - Plain Old Data
struct Point final {
	int x;
	int y;
};

Point p1; // x and y not initialized - not RAII
Point p2{}; // x and y initialized with 0 - RAII

// After the end of the constructor an object should be fully initialized

// bad example, only half RAII - std::ifstream
class ifstream {
	// doesn't throw if opening did not work
	ifstream(std::string const& filename);

	// not RAII
	void open(std::string const& filename);

	bool is_open() const;
};

// in the destructor everything should be cleaned up:

// streaming classes
// container
// threading (mutex, condition_variable, future)
// smart pointer





// Rule of 0/3/5

// 0: if there is no custom destructor, no additional constructors are needed (preferred)
// 3: if there is a custom destructor, at least copy constructor and copy assignment are also needed
// 5: since C++11 -> additionally add move constructor and move assignment
