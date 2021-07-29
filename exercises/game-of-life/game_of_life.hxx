#pragma once

#include <cstdint>
#include <random>
#include <type_traits>

/* These collide
enum Chromosome {
	X, Y
};

enum Coordinate {
	X, Y, Z
};
*/

#include <vector>

namespace workshop {
	enum class CellState : std::uint8_t {
		Dead = 0,
		Alive = 1,
		// Schrodinger = 3,
	};

	/*
	CellState operator|(CellState lhs, CellState rhs) {
		return static_cast<CellState>(
			static_cast<std::uint8_t>(lhs) | static_cast<std::uint8_t>(rhs);
		)
	}

	void foo(std::uint8_t) {

	}

	void bar() {
		foo(CellState::Dead | CellState::Alive);
	}

	using CellStateT = std::underlying_type_t<CellState>;
	*/

	class GameOfLife final {
	public:
		// this is what vector<bool> does when using the non-const index operator
		class CellReference final {
		public:
			CellReference(int x, int y, GameOfLife& game);
			/*CellReference(CellReference const&) = default;
			CellReference(CellReference&&) = default;
			CellReference& operator =(CellReference const&) = delete;
			CellReference& operator =(CellReference&&) = delete;*/

			operator CellState() const;

			CellReference& operator = (CellState state);

		private:
			int m_x;
			int m_y;
			GameOfLife& m_game;
		};

		GameOfLife(int width, int height);

		CellState operator() (int x, int y) const;
		CellReference operator() (int x, int y);

		int width() const;
		int height() const;

		void step();

	private:
		int m_width;
		int m_height;
		std::vector<CellState> m_cells;

		void randomize() {

			std::random_device dev{};
			std::mt19937 gen{ dev() };
			// std::uniform_int_distribution<int> dice{ 1, 6 };
			std::discrete_distribution<int> dist{ {70.0, 30.0} };

			auto const nextCellState = [&] {
				return static_cast<CellState>(dist(gen));
			};

			for (int y = 0; y < m_height; ++y) {
				for (int x = 0; x < m_width; ++x) {
					operator()(x, y) = nextCellState();
				}
			}
		}

		int livingNeighborsOf(int x, int y) const;
		bool isInField(int const x, int const y) const;
	};
}
