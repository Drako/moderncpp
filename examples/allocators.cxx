#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstddef>
#include <utility>

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

#include <gtest/gtest.h>

#include <vector>

TEST(AllocatorTest, testGlobalAllocator) {
	std::vector<int, GlobalAllocator<int>> numbers{ 1, 2, 3, 4, 5, 6 };
	EXPECT_EQ(1, numbers[0]);
}
