cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)
project(rtimdb)

enable_testing()

set(RTIMDB_MAX_CONCURRENT_TRANSACTIONS	  5	CACHE STRING "Maximum number of concurrent read transactions (for freezes during polls)")
set(RTIMDB_POINT_COUNT					200	CACHE STRING "Total number of points in the system (maximum)")
set(RTIMDB_ALLOW_EXCEPTIONS              ON	CACHE BOOL   "Should exceptions be thrown from the library, on error")

include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(atomic __atomic_load CMAKE_LIBRARY_PATH HAVE_ATOMIC_LIB)
if (HAVE_ATOMIC_LIB)
    list(APPEND EXTRA_LIBS atomic)
endif()

if (${PROJECT_BINARY_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    add_subdirectory(exceptions)
endif()

set(RTIMDB_SOURCES
	cell.hpp
	database.cpp
	database.hpp
	details/iterator.cpp
	details/iterator.hpp
	details/locator.hpp
	details/prologue.hpp
	details/transaction.hpp
	details/variant.hpp
	doublebitbinary.hpp
	exceptions.cpp
	exceptions.hpp
	file.hpp
	rtimdb_config.hpp
	point.cpp
	point.hpp
	pointtype.hpp
	time.hpp
	)

add_library(rtimdb SHARED
	${RTIMDB_SOURCES}
	)
target_link_libraries(rtimdb exceptions ${EXTRA_LIBS})
target_link_libraries(rtimdb exceptions ${EXTRA_LIBS})

foreach (test
	create_database
	try_concurrent_transactions
	try_freeze
	try_iterate
	)
	add_executable(test_${test} tests/${test}.cpp)
	target_link_libraries(test_${test} rtimdb)
	add_test(test_${test} ${EXECUTABLE_OUTPUT_PATH}/test_${test})
endforeach (test)

configure_file(rtimdb_config.hpp.in rtimdb_config.hpp)
