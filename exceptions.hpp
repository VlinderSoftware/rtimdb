#ifndef vlinder_rtimdb_exceptions_hpp
#define vlinder_rtimdb_exceptions_hpp

#include "details/prologue.hpp"
#include "exceptions/exception.h"

namespace Vlinder {	namespace RTIMDB {
	enum struct Errors {
		  no_error__
		, unknown_point__
		, cannot_freeze__
		};
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::unknown_point__ > UnknownPoint;
	typedef Exceptions::Exception< std::runtime_error, Errors, Errors::cannot_freeze__ > CannotFreeze;

	RTIMDB_API void throwException(Errors error);
}}

#endif

