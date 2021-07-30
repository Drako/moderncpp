#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstddef>
#include <utility>
#include <vector>

template <typename T>
struct SmallObjectAllocator {
	SmallObjectAllocator(std::size_t maxObjects) {
		data.reserve(maxObjects);
	}

	template <typename... Args>
	T* allocate(Args&&... args) {
		data.emplace_back(std::forward<Args>(args)...);
		return std::addressof(data.back());
	}

private:
	std::vector<T> data;
};

template <typename T>
struct FixedMemoryAllocator {
	FixedMemoryAllocator(std::size_t sizeInBytes)
		: data(sizeInBytes)
	{}

	//template <typename... Args>
	T* allocate(std::size_t n/* Args&&... args */) {
		auto const size = n * sizeof(T);
		if (next + size >= data.size()) {
			return nullptr;
		}
		auto const ptr = data.data() + next;
		// new((void*)(ptr)) T{ std::forward<Args>(args)... };
		next += size;
		return ptr;
	}

	void deallocate(T*) {
		// do nothing
	}

private:
	std::vector<std::byte> data;
	std::size_t next = 0u;
};

template <typename T>
struct GlobalAllocator {
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using propagate_on_container_move_assignment = std::true_type;
	using is_always_equal = std::true_type;

	template <typename U>
	struct rebind {
		using other = GlobalAllocator<U>;
	};

	template <typename U>
	using rebind_t = typename rebind<U>::other;

	constexpr GlobalAllocator() noexcept = default;

	template <typename U>
	constexpr GlobalAllocator(GlobalAllocator<U> const&) noexcept {};

	value_type * allocate(size_type count) {
		return reinterpret_cast<value_type*>(
			GlobalAlloc(GMEM_FIXED, sizeof(T) * count)
		);
	}

	void deallocate(value_type * p, size_type) {
		GlobalFree(p);
	}

	template <typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		new((void*)p) U{ std::forward<Args>(args)... };
	}

	template <typename U>
	void destroy(U* p) {
		p->~U();
	}
};

// rebinding template arguments
template <typename T, typename Allocator = GlobalAllocator<T>>
struct List {
	struct Item {
		T data;
		Item* next;
	};

	using ItemAllocator = typename Allocator::template rebind_t<Item>;
	using ValueAllocator = typename Allocator::template rebind_t<T>;

	T* p;
	Allocator alloc;

	void foo() {
		p = alloc.allocate(1);
	}

};

#include <gtest/gtest.h>

#include <vector>

TEST(AllocatorTest, testGlobalAllocator) {
	GlobalAllocator<int> allocator;
	std::vector<int, GlobalAllocator<int>> numbers{ { 1, 2, 3, 4, 5, 6 }, allocator };
	EXPECT_EQ(1, numbers[0]);
}
