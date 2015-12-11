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
#ifndef vlinder_rtimdb_pointtype_hpp
#define vlinder_rtimdb_pointtype_hpp

#include <cstdint>

namespace Vlinder { namespace RTIMDB {
	enum struct PointType {
		  binary_input__				// g1, g2
		, binary_output__				// g10, g11, g12, g13
		, counter__						// g20, g21, g22, g23
		, analog_input__				// g30, g31, g32, g33
		, analog_output__				// g40, g41, g42, g43
		, _type_count__
		};
}}

#endif

