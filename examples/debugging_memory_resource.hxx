#pragma once

#include <memory_resource>
#include <iostream>

class debugging_memory_resource final : public std::pmr::memory_resource {
	virtual void* do_allocate(std::size_t bytes, std::size_t alignment) override {
		std::cerr << "Allocating " << bytes << " Bytes." << std::endl;
		return std::pmr::new_delete_resource()->allocate(bytes, alignment);
	}

	virtual void do_deallocate(void * p, std::size_t bytes, std::size_t alignment) override {
		std::cerr << "Releasing " << bytes << " Bytes." << std::endl;
		std::pmr::new_delete_resource()->deallocate(p, bytes, alignment);
	}

	virtual bool do_is_equal(std::pmr::memory_resource const& other) const noexcept override {
		return std::pmr::new_delete_resource()->is_equal(other);
	}
};
