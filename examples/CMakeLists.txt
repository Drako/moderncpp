add_executable(example_tests
	container_test.cxx
	lambda_test.cxx
	smart_pointer_test.cxx
	string_test.cxx

	allocators.cxx
	lambdas.cxx
)
target_link_libraries(example_tests PRIVATE
	CONAN_PKG::gtest
)

add_test(NAME example_tests COMMAND example_tests)

add_executable(intializer_lists_are_broken
	intializer_lists_are_broken.cxx
	debugging_memory_resource.hxx
)

find_package(Threads REQUIRED)
add_executable(threading
	threading.cxx
)
target_link_libraries(threading PRIVATE Threads::Threads)
