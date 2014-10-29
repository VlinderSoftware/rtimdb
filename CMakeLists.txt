cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)
project(rtimdb)

enable_testing()

set(RTIMDB_MAX_CONCURRENT_TRANSACTIONS				  5	CACHE STRING "Maximum number of concurrent read transactions (for freezes during polls)")
set(RTIMDB_POINT_COUNT								200	CACHE STRING "Total number of points in the system (maximum)")
set(RTIMDB_ALLOW_EXCEPTIONS							 ON	CACHE BOOL   "Should exceptions be thrown from the library, on error")
set(RTIMDB_ABORT_SELECTION_ON_DUPLICATE_SELECTION	 ON CACHE BOOL	 "On duplicate selection (i.e. select on a point already selected) abort selection (true) or replace it (false)")

include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(atomic __atomic_load CMAKE_LIBRARY_PATH HAVE_ATOMIC_LIB)
if (HAVE_ATOMIC_LIB)
    list(APPEND EXTRA_LIBS atomic)
endif()

if (${UNIX})
    # Initialize CXXFLAGS.
    set(CMAKE_CXX_FLAGS                "-Wall -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
    
    # Compiler-specific C++11 activation.
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        execute_process(
		COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
        if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
            message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
        endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    else ()
        message(FATAL_ERROR "Your C++ compiler does not support C++11.")
    endif ()
endif()

if (${PROJECT_BINARY_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    add_subdirectory(exceptions)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

set(RTIMDB_SOURCES
	cell.hpp
	database.cpp
	database.hpp
	details/action.hpp
	details/duplicateselectionpolicy.hpp
	details/iterator.cpp
	details/iterator.hpp
	details/locator.hpp
	details/prologue.hpp
	details/selection.hpp
	details/transaction.hpp
	details/variant.hpp
	exceptions.cpp
	exceptions.hpp
	rtimdb_config.hpp
	point.cpp
	point.hpp
	pointtype.hpp
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
