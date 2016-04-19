/* Copyright 2016  Vlinder Software
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
#ifndef vlinder_rtimdb_core_flag_hpp
#define vlinder_rtimdb_core_flag_hpp

#include "flag.hpp"
#include <cstdint>

namespace Vlinder { namespace RTIMDB { namespace Core {
	enum Flag : uint8_t
	{
		  online__				= 0x01
		, restart__				= 0x02
		, communications_lost__ = 0x04
		, remote_forced__		= 0x08
		, local_forced__		= 0x10
		, rollover__			= 0x20
		, over_range__			= 0x40
		, discontinuity__		= 0x40
		, reference_error__		= 0x80
	};
}}}

#endif

