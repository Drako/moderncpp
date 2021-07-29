#include "game_of_life.hxx"

#include <array>
#include <stdexcept>
#include <tuple>

namespace workshop {
	static std::array<std::tuple<int, int>, 8> const neighbors{{
		{-1, -1}, {0, -1}, {1, -1},
		{-1, 0},           {1, 0},
		{-1, 1},  {0, 1},  {1, 1},
	}};

	GameOfLife::CellReference::CellReference(int const x, int const y, GameOfLife& game)
		: m_x{x}
		, m_y{y}
		, m_game{game}
	{}

	GameOfLife::CellReference & GameOfLife::CellReference::operator=(CellState const state) {
		if (m_game.isInField(m_x, m_y))
			m_game.m_cells[m_y * m_game.m_width + m_x] = state;
		return *this;
	}

	GameOfLife::CellReference::operator CellState() const {
		return m_game.isInField(m_x, m_y)
			? m_game.m_cells[m_y * m_game.m_width + m_x]
			: CellState::Dead;
	}

	GameOfLife::GameOfLife(int const width, int const height)
		: m_width{width}
		, m_height{height}
		, m_cells{width * height, CellState::Dead}
	{}

	CellState GameOfLife::operator() (int const x, int const y) const {
		if (!isInField(x, y))
			return CellState::Dead;

		return m_cells[y * m_width + x];
	}

	GameOfLife::CellReference GameOfLife::operator() (int const x, int const y) {
		return CellReference{ x, y, *this };
	}

	bool GameOfLife::isInField(int const x, int const y) const
	{
		return x >= 0 && y >= 0  && x < m_width && y < m_height;
	}

	int GameOfLife::width() const {
		return m_width;
	}

	int GameOfLife::height() const {
		return m_height;
	}

	void GameOfLife::step() {
		GameOfLife old{ *this };
		for (int y = 0; y < m_height; ++y) {
			for (int x = 0; x < m_width; ++x) {
				int const neighbors = old.livingNeighborsOf(x, y);
				if (neighbors == 3)
					operator()(x, y) = CellState::Alive;
				else if (neighbors < 2 || neighbors > 3)
					operator()(x, y) = CellState::Dead;
			}
		}
	}

	int GameOfLife::livingNeighborsOf(int const x, int const y) const {
		int count{ 0 };

		// pattern matching, destructuring, in C++: structured binding
		// range-for
		for (auto const[dx, dy] : neighbors) {
			int const nx = x + dx;
			int const ny = y + dy;

			if (operator()(nx, ny) == CellState::Alive)
				++count;
		}
		return count;
	}
}
