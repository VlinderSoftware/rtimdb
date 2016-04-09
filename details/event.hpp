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
#ifndef vlinder_rtimdb_details_event_hpp
#define vlinder_rtimdb_details_event_hpp

#include "../core/pointtype.hpp"
#include "../core/pointvalue.hpp"
#include "timestamp.hpp"
#include <limits>

namespace Vlinder { namespace RTIMDB { namespace Details {
	struct Event
	{
		Event()
			: id_(std::numeric_limits< decltype(id_) >::max())
		{ /* no-op */ }
		Event(Timestamp const &timestamp, Core::PointType type, unsigned int id, Core::PointValue const &value, uintptr_t tag)
			: timestamp_(timestamp)
			, type_(type)
			, id_(id)
			, value_(value)
			, tag_(tag)
		{ /* no-op */ }
		Timestamp timestamp_;
		Core::PointType type_;
		unsigned int id_;
		Core::PointValue value_;
		uintptr_t tag_;
	};
}}}

#endif

