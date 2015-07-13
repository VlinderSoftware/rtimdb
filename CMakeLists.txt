cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)
project(rtimdb)

enable_testing()
include(CTest)

include_directories(${CMAKE_SOURCE_DIR})

set(RTIMDB_MAX_CONCURRENT_TRANSACTIONS			  5	CACHE STRING "Maximum number of concurrent read transactions (for freezes during polls)")
set(RTIMDB_POINT_COUNT					200	CACHE STRING "Total number of points in the system (maximum)")
set(RTIMDB_ALLOW_EXCEPTIONS				 ON	CACHE BOOL   "Should exceptions be thrown from the library, on error")
set(RTIMDB_ABORT_SELECTION_ON_DUPLICATE_SELECTION	 ON 	CACHE BOOL   "On duplicate selection (i.e. select on a point already selected) abort selection (true) or replace it (false)")

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

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996 /wd4251")
endif()

if (${PROJECT_BINARY_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    add_subdirectory(exceptions)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

set(RTIMDB_CORE_SOURCES
	core/database.cpp
	core/database.hpp
	core/details/cell.hpp
	core/details/iterator.cpp
	core/details/iterator.hpp
	core/details/locator.hpp
	core/details/observer.hpp
	core/details/selection.hpp
	core/details/transaction.hpp
	core/details/variant.hpp
	core/point.cpp
	core/point.hpp
	)
set(RTIMDB_SOURCES
	details/action.hpp
	details/duplicateselectionpolicy.hpp
	details/prologue.hpp
	exceptions.cpp
	exceptions.hpp
	rtimdb_config.hpp
	pointtype.hpp
	)

add_library(rtimdb SHARED
	${RTIMDB_SOURCES}
	${RTIMDB_CORE_SOURCES}
	)
target_link_libraries(rtimdb exceptions ${EXTRA_LIBS})

set(TESTS
	create_database
	insertions
	filtering
	try_concurrent_transactions
	try_freeze
	try_iterate
	try_set_filter
	try_register_observer
	try_variant
	try_update_through_observer
	queries_on_empty_db
	)

foreach (test
	${TESTS}
	)
	add_executable(test_${test} tests/${test}.cpp)
	target_link_libraries(test_${test} rtimdb)
	add_test(test_${test} ${EXECUTABLE_OUTPUT_PATH}/test_${test})
endforeach (test)
foreach (test_config 1 2 3 4)
	add_library(rtimdb_cfg_${test_config} SHARED
		${RTIMDB_SOURCES}
		${RTIMDB_CORE_SOURCES}
		)
	target_link_libraries(rtimdb_cfg_${test_config} exceptions ${EXTRA_LIBS})
	if (MSVC)
		set_target_properties(rtimdb_cfg_${test_config} PROPERTIES COMPILE_FLAGS "/FI\"${CMAKE_SOURCE_DIR}/tests/test_config_${test_config}.hpp\"")
	else()
		set_target_properties(rtimdb_cfg_${test_config} PROPERTIES COMPILE_FLAGS "-include ${CMAKE_SOURCE_DIR}/tests/test_config_${test_config}.hpp")
	endif()
	set_target_properties(rtimdb_cfg_${test_config} PROPERTIES COMPILE_DEFINITIONS "rtimdb_EXPORTS")

	foreach (test
		${TESTS}
		)
		add_executable(test_cfg${test_config}_${test} tests/${test}.cpp)
		if (MSVC)
			set_target_properties(test_cfg${test_config}_${test} PROPERTIES COMPILE_FLAGS "/FI\"${CMAKE_SOURCE_DIR}/tests/test_config_${test_config}.hpp\"")
		else()
			set_target_properties(test_cfg${test_config}_${test} PROPERTIES COMPILE_FLAGS "-include ${CMAKE_SOURCE_DIR}/tests/test_config_${test_config}.hpp")
		endif()
		target_link_libraries(test_cfg${test_config}_${test} rtimdb_cfg_${test_config})
		add_test(test_cfg${test_config}_${test} ${EXECUTABLE_OUTPUT_PATH}/test_cfg${test_config}_${test})
	endforeach (test)
endforeach(test_config)

configure_file(rtimdb_config.hpp.in rtimdb_config.hpp)

