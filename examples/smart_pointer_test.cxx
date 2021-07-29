#include <gtest/gtest.h>
namespace t = testing;

#include <memory>
#include <string>

// never raw delete
// avoid raw new

// TR1 "smart" pointer
// std::auto_ptr

// boost::scoped_ptr
// boost::shared_ptr

struct Person {
	std::string name;
	int age;

	Person(std::string, int){}
};

char my_memory[1'000'000'000];

struct RelationShip {
	std::unique_ptr<Person> partnerA;
	std::unique_ptr<Person> partnerB;

	RelationShip(std::unique_ptr<Person>, std::unique_ptr<Person>){}
};

template <typename T>
struct MyUniquePtr {
	T* data;

	MyUniquePtr(MyUniquePtr const&) = delete;
	MyUniquePtr& operator=(MyUniquePtr const&) = delete;

	MyUniquePtr(MyUniquePtr&&src) : data{ src.data } {
		src.data = nullptr;
	}

	MyUniquePtr& operator =(MyUniquePtr && src) {
		if (this == std::addressof(src))
			return *this;
		delete data;
		data = src.data;
		src.data = nullptr;
		return *this;
	}

	~MyUniquePtr() {
		delete data;
	}
};

// std::unique_ptr (std::auto_ptr but better)
TEST(SmartPointerTest, testUniquePtr) {
	// roughly the same, but prefer make_unique
	auto ptr = std::make_unique<Person>("Max Mustermann", 1337);
	std::unique_ptr<Person> ptr2{ new Person{"Max Mustermann", 1337} };

	// bad: if the Person constructor for B throws, A could leak... and a lot more
	// order of parameter evaluation is not guaranteed in optimized builds
	std::unique_ptr<RelationShip> r{new RelationShip{
		std::unique_ptr<Person>{new Person{"A", 23}},
		std::unique_ptr<Person>{new Person{"B", 42}},
	} };

	// good
	auto r2 = std::make_unique<RelationShip>(
		std::make_unique<Person>("A", 23),
		std::make_unique<Person>("B", 42)
	);

	std::unique_ptr<Person> ptr3{ std::move(ptr) };
	Person* non_owning = ptr3.get(); // raw pointer is the weak_ptr to unique_ptr
}

#include <array>
#include <atomic>
#include <mutex>

template <typename T>
struct MySharedPtr {
	struct Holder {
		std::mutex mutex;
		T* data;
		int refCount;
		int weakCount;
	};

	Holder * ptr;

	explicit MySharedPtr(T* p = nullptr)
		: ptr{p ? new Holder{{}, p, 1} : nullptr}
	{}

	MySharedPtr(MySharedPtr const& src) : ptr{ src.ptr } {
		std::lock_guard<std::mutex> lock{ ptr->mutex };
		++ptr->refCount;
	}

	MySharedPtr(MySharedPtr && src) : ptr{ src.ptr } {
		std::lock_guard<std::mutex> lock{ ptr->mutex };
		src.ptr = nullptr;
	}

	MySharedPtr& operator = (MySharedPtr const& src) {
		if (this == std::addressof(src))
			return *this;
		release();
		std::lock_guard<std::mutex> lock{ src.ptr->mutex };
		ptr = src.ptr;
		++ptr->refCount;
		return *this;
	}

	MySharedPtr& operator = (MySharedPtr && src) {
		if (this == std::addressof(src))
			return *this;
		release();
		std::lock_guard<std::mutex> lock{ src.ptr->mutex };
		ptr = src.ptr;
		return *this;
	}

	T* operator& () const {
		return ptr ? ptr->data : nullptr;
	}

	~MySharedPtr() {
		release();
	}

	void release() {
		std::lock_guard<std::mutex> lock{ ptr->mutex };
		--ptr->refCount;
		if (ptr->refCount == 0) {
			delete ptr->data;
			if (ptr->weakCount == 0) {
				delete ptr;
			}
		}
		ptr = nullptr;
	}
};

// std::shared_ptr (basically boost::shared_ptr)
TEST(SmartPointerTest, testSharedPtr) {
	/*
	MySharedPtr<int> p{ new int{42} };
	MySharedPtr<int>& p2 = p;
	MySharedPtr<int>& p3 = p;
	p2 = p3;
	*/

	// roughly the same, but prefer make_unique
	auto ptr = std::make_shared<Person>("Max Mustermann", 1337);
	std::shared_ptr<Person> ptr2{ new Person{"Max Mustermann", 1337} };

	// see unique_ptr and make_unique regarding make_shared
	auto ptr3 = ptr;

	std::weak_ptr<Person> non_owning{ ptr };
	std::shared_ptr<Person> owning = non_owning.lock();
}

/*
malloc -> VirtualAlloc
new -> VirtualAlloc

C++ - direct pointers     [ptr] -> memory
JVM/.NET - indirect pointers (relocatable pointers)    [ptr] -> [ptr table] -> memory
*/
