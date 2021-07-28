#pragma once

namespace workshop {
	enum class CellState {
		Dead,
		Alive
	};

	class GameOfLife final {
	public:
		GameOfLife(int width, int height);

		CellState operator() (int x, int y) const;

		int width() const;
		int height() const;
	};
}
