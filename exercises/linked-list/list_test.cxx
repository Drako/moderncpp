#include <gtest/gtest.h>

#include "list.hxx"

using workshop::List;

TEST(ListTest, empty_list)
{
	List<int> const empty{};
	try {
		EXPECT_TRUE(empty.empty());
		EXPECT_EQ(0u, empty.size());
	}
	catch (std::runtime_error const& ex) {

	}
}

TEST(ListTest, single_value_list)
{
	List<double> const pi{ 3.1415926 };
	EXPECT_FALSE(pi.empty());
	EXPECT_EQ(1u, pi.size());
}

TEST(ListTest, multi_value_list)
{
	List<int> const numbers{ 23, 42, 1337 };
	EXPECT_FALSE(numbers.empty());
	EXPECT_EQ(numbers.size(), 3u);
}

TEST(ListTest, front)
{
	List<char> letters{};
	EXPECT_TRUE(letters.empty());
	EXPECT_EQ(letters.size(), 0u);

	letters.push_front('@');
	EXPECT_FALSE(letters.empty());
	EXPECT_EQ(letters.size(), 1u);
	EXPECT_EQ(letters.front(), '@');

	letters.pop_front();
	EXPECT_TRUE(letters.empty());
	EXPECT_EQ(letters.size(), 0u);

	EXPECT_THROW(letters.pop_front(), std::out_of_range);
	EXPECT_THROW(letters.front(), std::out_of_range);
}

class ListEmptySizeCorelationTestFixture : public ::testing::TestWithParam<std::size_t> {
};

TEST_P(ListEmptySizeCorelationTestFixture, only_size_0_is_empty)
{
	std::size_t const size = GetParam();
	List<char> letters;

	for (std::size_t n = size; n--;) {
		letters.push_front('%');
	}

	EXPECT_EQ(letters.size(), size);
	EXPECT_EQ(letters.empty(), size == 0u);
}

INSTANTIATE_TEST_SUITE_P(
	ListTest,
	ListEmptySizeCorelationTestFixture,
	::testing::Values(0u, 1u, 3u, 23u, 42u)
);
