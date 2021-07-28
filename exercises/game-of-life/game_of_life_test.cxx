#include <gtest/gtest.h>
namespace t = testing;

#include "game_of_life.hxx"
namespace w = workshop;

#include <string>

struct GameOfLifeTest : t::Test {
	std::string stringify(w::GameOfLife const& game) {
		std::string result{};
		for (int y = 0; y < game.height(); ++y) {
			for (int x = 0; x < game.width(); ++x) {
				result += (game(x, y) == w::CellState::Alive) ? "X" : " ";
			}
			result += "\n";
		}
		return result;
	}
};

TEST_F(GameOfLifeTest, canCreateGameWithSize) {
	w::GameOfLife game{ 5, 5 };
	EXPECT_EQ(5, game.width());
	EXPECT_EQ(5, game.height());
	auto const actual = stringify(game);
	EXPECT_EQ(
		"     \n"
		"     \n"
		"     \n"
		"     \n"
		"     \n",
		actual
	);

	game(2, 2) = w::CellState::Alive;

	EXPECT_EQ(
		"     \n"
		"     \n"
		"  X  \n"
		"     \n"
		"     \n",
		stringify(game)
	);
}
