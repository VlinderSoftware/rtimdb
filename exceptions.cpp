#include "exceptions.hpp"

using namespace std;

namespace Vlinder { namespace RTIMDB {
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	void throwException(Errors error)
	{
		switch (error)
		{
		case Errors::no_error__ :
			return;
		case Errors::unknown_point__ :
			throw UnknownPoint("Unknown point");
		case Errors::cannot_freeze__:
			throw CannotFreeze("Cannot freeze");
		default :
			throw logic_error("Unknown error type");
		}
	}
#endif
}}

