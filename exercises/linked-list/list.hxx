#pragma once

#include <cstddef>

#include <initializer_list>
#include <memory>
#include <type_traits>
#include <stdexcept>
#include <iostream>

// ODR - One Definition Rule
// apparently in classes inline is not necessary

struct Printer {
	virtual ~Printer() noexcept = default;

	virtual void print(char const * message) = 0;
};

struct ConsolePrinter final : Printer {
	void print(char const* message) override {
		std::cout << message << std::endl;
	}
};


inline void foo() {
	std::shared_ptr<ConsolePrinter> p = std::make_shared<ConsolePrinter>();
	std::shared_ptr<Printer> p2 = std::static_pointer_cast<Printer>(p);
	std::shared_ptr<ConsolePrinter> p3 = std::dynamic_pointer_cast<ConsolePrinter>(p2);


}

namespace workshop {
	template <typename T>
	class List final {
		struct Item final {
			T data;
			std::unique_ptr<Item> next;

			Item(T const& data, std::unique_ptr<Item> next)
				noexcept(std::is_nothrow_copy_constructible_v<T>)
				: data{data}
				, next{std::move(next)}
			{}
		};

	public:
		inline List() noexcept = default;

		List(std::initializer_list<T> init) {
			for (auto n = init.size(); n--;) {
				push_front(*(init.begin() + n));
			}
		}

		bool empty() const noexcept {
			return m_size == 0u;
		}

		std::size_t size() const noexcept {
			return m_size;
		}

		void push_front(T const& value) {
			m_head = std::make_unique<Item>(value, std::move(m_head));
			++m_size;
		}

		void pop_front() {
			if (m_head) {
				m_head = std::move(m_head->next);
				--m_size;
			}
			else {
				throw std::out_of_range{ "popping from empty list" };
			}
		}

		T front() const {
			if (m_head)
				return m_head->data;
			throw std::out_of_range{ "accessing front of empty list" };
		}

	private:
		std::unique_ptr<Item> m_head{};
		std::size_t m_size{ 0u };
	};
}
