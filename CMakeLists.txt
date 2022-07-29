cmake_minimum_required(VERSION 3.14)

find_package(doctest)

project(conveyor
	LANGUAGES CXX
)

add_library(conveyor
	src/dummy.cpp
	include/conveyor/conveyor.h
	include/conveyor/enumerate
	include/conveyor/views/enumerate.h
)

target_compile_features(conveyor
	PUBLIC cxx_std_20
)

target_include_directories(conveyor
	PUBLIC include
)

if(doctest_FOUND)
	add_executable(tests
		src/tests/tests.cpp
	)

    target_link_libraries(tests
		PUBLIC doctest::doctest
		PUBLIC conveyor
	)
endif()
