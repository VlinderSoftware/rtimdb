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
#ifndef vlinder_rtimdb_point_hpp
#define vlinder_rtimdb_point_hpp

#include "pointtype.hpp"
#include "pointvalue.hpp"
#include "timestamp.hpp"
#include "flags.hpp"

namespace Vlinder { namespace RTIMDB { namespace Core {
	struct Point
	{
		Point(
			  PointType type
			, PointValue value
			, Flags flags
			, Timestamp timestamp
			)
			: type_(type)
			, value_(value)
			, flags_(flags)
			, timestamp_(timestamp)
		{ /* no-op */ }
		PointType type_;
		PointValue value_;
		Flags flags_;
		Timestamp timestamp_;
	};
}}}

#endif

