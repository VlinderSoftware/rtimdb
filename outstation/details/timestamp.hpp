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
#ifndef vlinder_rtimdb_outstation_details_timestamp_hpp
#define vlinder_rtimdb_outstation_details_timestamp_hpp

#include "details/prologue.hpp"
#include <cstdint>

namespace Vlinder { namespace RTIMDB { namespace Outstation { namespace Details {
	struct Timestamp
	{
		Timestamp()
			: high_(0)
			, low_(0)
		{ /* no-op */ }
		Timestamp(uint16_t high, uint32_t low)
			: high_(high)
			, low_(low)
		{ /* no-op */ }

		uint16_t high_;
		uint32_t low_;
	};
}}}}

#endif

