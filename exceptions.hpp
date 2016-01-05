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
#ifndef vlinder_rtimdb_exceptions_hpp
#define vlinder_rtimdb_exceptions_hpp

#include "details/prologue.hpp"
#include "rtimdb_config.hpp"
#ifdef RTIMDB_ALLOW_EXCEPTIONS
#include "exceptions/exception.hpp"
#endif
namespace Vlinder {	namespace RTIMDB {
	enum struct Errors {
		  no_error__
		, unknown_point__
		, cannot_freeze__
		, not_allowed__
		, duplicate_selection__
		, operate_without_select__
		, not_yet_implemented__
		, expired_while_freezing__
		, database_full__
		, command_queue_full__
		, too_many_transitions__
		, transaction_failed__
		, allocation_error__
		, mapping_full__
		, queue_full__
		};
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::unknown_point__ > UnknownPoint;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::cannot_freeze__ > CannotFreeze;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::not_allowed__ > NotAllowed;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::duplicate_selection__ > DuplicateSelection;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::operate_without_select__ > OperateWithoutSelect;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::not_yet_implemented__ > NotYetImplemented;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::expired_while_freezing__ > ExpiredWhileFreezing;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::database_full__ > DatabaseFull;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::command_queue_full__ > CommandQueueFull;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::too_many_transitions__ > TooManyTransitions;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::transaction_failed__ > TransactionFailed;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::allocation_error__ > AllocationError;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::mapping_full__ > MappingFull;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::queue_full__ > QueueFull;

	RTIMDB_API void throwException(Errors error);
#endif
}}

#endif

