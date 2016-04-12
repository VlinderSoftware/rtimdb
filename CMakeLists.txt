cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)
project(rtimdb)

if (${PROJECT_BINARY_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	enable_testing()
	include(CTest)
endif()

set(VLINDER_USING_CXX11 1)
include(cmake/settings.cmake)

include_directories(${CMAKE_SOURCE_DIR})

set(RTIMDB_MAX_CONCURRENT_TRANSACTIONS				  5	CACHE STRING "Maximum number of concurrent read transactions (for freezes during polls)")
set(RTIMDB_POINT_COUNT								200	CACHE STRING "Total number of points in the system (maximum)")
set(RTIMDB_ALLOW_EXCEPTIONS							 ON	CACHE BOOL   "Should exceptions be thrown from the library, on error")
set(RTIMDB_COMMAND_QUEUE_CAPACITY					 16 CACHE STRING "Number of commands that can be queued at any given time, per queue")
set(RTIMDB_MAX_PRODUCER_COUNT						  2	CACHE STRING "Maximum number of producers in the application")
set(RTIMDB_MAX_CONSUMER_COUNT						  2	CACHE STRING "Maximum number of consumer in the application")
set(RTIMDB_TRANSITION_QUEUE_CAPACITY				256 CACHE STRING "Number of transitions that can be queued at any given time, per queue")
set(RTIMDB_MAX_TRANSITIONS_PER_TRANSACTION			200 CACHE STRING "Maximum number of transitions that can be expected to occur in a single transaction.")
set(RTIMDB_EVENT_QUEUE_CAPACITY						 64 CACHE STRING "Maximum number of events, per event class")
set(RTIMDB_MAPPING							  "Default" CACHE STRING "Choose the way mappings are handled")
set_property(CACHE RTIMDB_MAPPING PROPERTY STRINGS Default Sorted)

include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(atomic __atomic_load CMAKE_LIBRARY_PATH HAVE_ATOMIC_LIB)
if (HAVE_ATOMIC_LIB)
    list(APPEND EXTRA_LIBS atomic)
endif()

if (${PROJECT_BINARY_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    add_subdirectory(exceptions)
    add_subdirectory(meta)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

set(RTIMDB_CORE_SOURCES
	core/datastore.cpp
	core/datastore.hpp
	core/details/bubblesort.hpp
	core/details/cell.hpp
	core/details/iterator.cpp
	core/details/iterator.hpp
	core/details/locator.hpp
	core/details/observer.hpp
	core/details/selection.hpp
	core/details/transaction.hpp
	core/details/optional.hpp
	core/pointtype.hpp
	core/pointvalue.cpp
	core/pointvalue.hpp
	)
set(RTIMDB_OUTSTATION_SOURCES
	command.hpp
	commandqueue.cpp
	commandqueue.hpp
	database.cpp
	database.hpp
	details/consumer.cpp
	details/consumer.hpp
	details/crob.hpp
	details/defaultmappinghelper.cpp
	details/defaultmappinghelper.hpp
	details/event.hpp
	details/eventclass.hpp
	details/eventiterator.cpp
	details/eventiterator.hpp
	details/eventqueue.cpp
	details/eventqueue.hpp
	details/events.cpp
	details/events.hpp
	details/mapping.hpp
	details/mappinghelper.cpp
	details/mappinghelper.hpp
	details/pointdescriptor.hpp
	details/polldescriptor.hpp
	details/pollresult.cpp
	details/pollresult.hpp
	details/producer.hpp
	details/sortedmappinghelper.cpp
	details/sortedmappinghelper.hpp
	details/timestamp.hpp
	details/transitionqueue.cpp
	details/transitionqueue.hpp
	details/transitionqueuetransaction.cpp
	details/transitionqueuetransaction.hpp
	details/transitiontransactionentry.hpp
	details/variant.hpp
	)
set(RTIMDB_SOURCES
	details/action.hpp
	details/prologue.hpp
	exceptions.cpp
	exceptions.hpp
	rtimdb_config.hpp
	)
add_library(rtimdb SHARED
	${RTIMDB_CORE_SOURCES}
	${RTIMDB_SOURCES}
	${RTIMDB_OUTSTATION_SOURCES}
	)
target_link_libraries(rtimdb exceptions ${EXTRA_LIBS})

set(TESTS
	bubblesort
	commandqueue
	commandqueuemanagement
	consumer
	create_data_store
	insertions
	freezing
	optional
	produce_transitions
	queries_on_empty_db
	register_producer
	try_concurrent_transactions
	try_freeze
	try_iterate
	variant
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
		${RTIMDB_CORE_SOURCES}
		${RTIMDB_SOURCES}
		${RTIMDB_OUTSTATION_SOURCES}
		)
	target_link_libraries(rtimdb_cfg_${test_config} exceptions ${EXTRA_LIBS})
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

