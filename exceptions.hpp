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

