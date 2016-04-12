/* Copyright 2014  Vlinder Software
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */
#ifndef vlinder_rtimdb_config_hpp
#define vlinder_rtimdb_config_hpp

#define RTIMDB_MAX_CONCURRENT_TRANSACTIONS 5
#define RTIMDB_CELL_SIZE RTIMDB_MAX_CONCURRENT_TRANSACTIONS + 1
#define RTIMDB_POINT_COUNT 200
#define RTIMDB_ALLOW_EXCEPTIONS
#undef RTIMDB_ABORT_SELECTION_ON_DUPLICATE_SELECTION
#define RTIMDB_MAX_PRODUCER_COUNT 2
#define RTIMDB_MAX_CONSUMER_COUNT 2
#define RTIMDB_COMMAND_QUEUE_CAPACITY 16
#define RTIMDB_TRANSITION_QUEUE_CAPACITY 256
#define RTIMDB_EVENT_QUEUE_CAPACITY 64
#define RTIMDB_MAX_TRANSITIONS_PER_TRANSACTION 200
#define RTIMDB_MAPPING Default

#define RTIMDB_MappingHelper DefaultMappingHelper

#ifdef RTIMDB_ALLOW_EXCEPTIONS
#	define RTIMDB_NOTHROW_PARAM_1 std::nothrow_t const&
#	define RTIMDB_NOTHROW_PARAM , std::nothrow_t const&
#	define RTIMDB_NOTHROW_ARG_1   std::nothrow
#	define RTIMDB_NOTHROW_ARG   , std::nothrow
#else
#	define RTIMDB_NOTHROW_PARAM_1
#	define RTIMDB_NOTHROW_PARAM
#	define RTIMDB_NOTHROW_ARG_1
#	define RTIMDB_NOTHROW_ARG
#endif
#ifdef RTIMDB_ABORT_SELECTION_ON_DUPLICATE_SELECTION
#	define RTIMDB_DUPLICATE_SELECTION_POLICY RTIMDB::Details::DuplicateSelectionPolicy::abort_selection__
#else
#	define RTIMDB_DUPLICATE_SELECTION_POLICY RTIMDB::Details::DuplicateSelectionPolicy::replace_selection__
#endif

#endif

