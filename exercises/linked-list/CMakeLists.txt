add_library(linked_list_impl INTERFACE)
target_sources(linked_list_impl INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/list.hxx")

add_executable(linked_list_tests
	list_test.cxx
	list2_test.cxx
)
target_link_libraries(linked_list_tests PRIVATE
	CONAN_PKG::gtest
	linked_list_impl
)

add_test(NAME linked_list_tests COMMAND linked_list_tests)
