add_library(boost_di INTERFACE)
target_sources(boost_di INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include/boost/di.hpp"
)
target_include_directories(boost_di INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_library(Boost::DI ALIAS boost_di)
