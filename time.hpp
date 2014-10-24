#ifndef vlinder_rtimdb_time_hpp
#define vlinder_rtimdb_time_hpp

#include <cstdint>

namespace Vlinder { namespace RTIMDB {
	struct Time
	{
		uint16_t high_part_;
		uint32_t low_part_;
	};
}}

#endif

