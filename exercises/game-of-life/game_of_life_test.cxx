#include <gtest/gtest.h>
namespace t = testing;

#include "game_of_life.hxx"
namespace w = workshop;

#include <string>

w::GameOfLife operator"" _g(char const* field, std::size_t length) {
	int lines = 0, rows = 0, maxRows = 0;

	for (char const * cp = field; cp != field + length; ++cp) {
		if (*cp == '\n') {
			if (rows > maxRows)
				maxRows = rows;
			++lines;
			rows = 0;
		}
		else {
			++rows;
		}
	}

	w::GameOfLife game{ maxRows, lines };

	int x = 0, y = 0;
	for (char const * cp = field; cp != field + length; ++cp) {
		switch (*cp) {
		case 'X':
			game(x, y) = w::CellState::Alive;
			[[fallthrough]];
		default:
			++x;
			break;
		case '\n':
			++y;
			x = 0;
			break;
		}
	}

	return game;
}

struct GameOfLifeTest : t::Test {
	[[nodiscard]] std::string stringify(w::GameOfLife const& game) {
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
	
	stringify(game);

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

TEST_F(GameOfLifeTest, userDefinedLiteral) {
	EXPECT_EQ(
		"   \n"
		"   \n"
		"   \n",
		stringify(
			"   \n"
			"   \n"
			"   \n"_g
		)
	);
}

struct StepParams final {
	char const* name;
	w::GameOfLife game;
	char const* expected;
};

std::ostream& operator << (std::ostream& os, StepParams const& sp) {
	return os << sp.name;
}


// using Name = char const*;
// using Expected = char const*;
// using StepParams = std::tuple<Name, w::GameOfLife, Expected>;
struct StepTests
	: GameOfLifeTest
	, t::WithParamInterface<StepParams>
{};

TEST_P(StepTests, rules) {
	auto const param = GetParam();
	w::GameOfLife game{ param.game };

	game.step();

	std::string const actual = stringify(game);

	EXPECT_EQ(param.expected, actual);
}

INSTANTIATE_TEST_SUITE_P(
	GameOfLifeTest,
	StepTests,
	t::Values(
		StepParams{
			"Horizontal line to vertical",
			"   \n"
			"XXX\n"
			"   \n"_g,
			" X \n"
			" X \n"
			" X \n"
		},
		StepParams{
			"Vertical line to horizontal",
			" X \n"
			" X \n"
			" X \n"_g,
			"   \n"
			"XXX\n"
			"   \n"
		},
		StepParams{
			"2x2 square",
			" XX\n"
			" XX\n"
			"   \n"_g,
			" XX\n"
			" XX\n"
			"   \n"
		}
	)
);
