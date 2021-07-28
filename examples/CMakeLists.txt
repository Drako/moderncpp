add_executable(example_tests
	container_test.cxx
	smart_pointer_test.cxx
	string_test.cxx
)
target_link_libraries(example_tests PRIVATE
	CONAN_PKG::gtest
)

add_test(NAME example_tests COMMAND example_tests)
