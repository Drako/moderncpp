add_library(game_of_life_impl STATIC
	game_of_life.cxx game_of_life.hxx
)

add_executable(game_of_life_tests
	game_of_life_test.cxx
)
target_link_libraries(game_of_life_tests PRIVATE
	gtest gmock gmock_main
	game_of_life_impl
)

add_test(NAME game_of_life_tests COMMAND game_of_life_tests)
