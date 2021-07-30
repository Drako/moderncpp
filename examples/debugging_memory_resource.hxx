#pragma once

#include <memory_resource>
#include <iostream>

class debugging_memory_resource final : public std::pmr::memory_resource {
public:
	debugging_memory_resource(std::pmr::memory_resource * base = nullptr)
		: base{base ? base : std::pmr::get_default_resource()}
	{}

private:
	virtual void* do_allocate(std::size_t bytes, std::size_t alignment) override {
		std::cerr << "Allocating " << bytes << " Bytes." << std::endl;
		return base->allocate(bytes, alignment);
	}

	virtual void do_deallocate(void * p, std::size_t bytes, std::size_t alignment) override {
		std::cerr << "Releasing " << bytes << " Bytes." << std::endl;
		base->deallocate(p, bytes, alignment);
	}

	virtual bool do_is_equal(std::pmr::memory_resource const& other) const noexcept override {
		return base->is_equal(other);
	}

	std::pmr::memory_resource * base;
};
