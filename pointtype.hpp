#ifndef vlinder_rtimdb_pointtype_hpp
#define vlinder_rtimdb_pointtype_hpp

#include <cstdint>

namespace Vlinder { namespace RTIMDB {
	enum struct PointType {
		  binary_input__				// g1, g2
		, double_bit_binary_input__		// g3, g4
		, binary_output__				// g10, g11, g12, g13
		, counter__						// g20, g21, g22, g23 -- needs an extra slot in the cell for freezing
		, analog_input__				// g30, g31, g32, g33 -- needs an extra slot in the cell for freezing
		, analog_output__				// g40, g41, g42, g43
		, time_and_date__				// g50, g51, g52
		, file__						// g70
		, dataset__						// g83, g85, g86, g87, g88
		, bcd__	// 16-bit				// g101
		, unsigned_integer__ // 8-bit	// g102
		, octet_string__				// g110, g111, g114, g115
		, _type_count__
		};
}}

#endif

