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
#include "exceptions/exception.h"
#endif
namespace Vlinder {	namespace RTIMDB {
	enum struct Errors {
		  no_error__
		, unknown_point__
		, cannot_freeze__
		};
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::unknown_point__ > UnknownPoint;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::cannot_freeze__ > CannotFreeze;

	RTIMDB_API void throwException(Errors error);
#endif
}}

#endif

