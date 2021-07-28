add_executable(example_tests
	container_test.cxx
	string_test.cxx
)
target_link_libraries(example_tests PRIVATE
	gtest gmock gmock_main
	spdlog
)

add_test(NAME example_tests COMMAND example_tests)
