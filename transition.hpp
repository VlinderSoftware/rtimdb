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
#ifndef vlinder_rtimdb_transition_hpp
#define vlinder_rtimdb_transition_hpp

#include "details/variant.hpp"
#include "core/point.hpp"
#include <limits>

namespace Vlinder { namespace RTIMDB {
	struct Transition
	{
		Transition()
			: system_id_(std::numeric_limits< decltype(system_id_) >::max())
		{ /* no-op */ }
		Transition(unsigned int system_id, PointType point_type, bool value) : system_id_(system_id), point_value_(point_type, value) { /* no-op */ }
		Transition(unsigned int system_id, PointType point_type, int16_t value) : system_id_(system_id), point_value_(point_type, value) { /* no-op */ }
		Transition(unsigned int system_id, PointType point_type, int32_t value) : system_id_(system_id), point_value_(point_type, value) { /* no-op */ }
		Transition(unsigned int system_id, PointType point_type, uint16_t value) : system_id_(system_id), point_value_(point_type, value) { /* no-op */ }
		Transition(unsigned int system_id, PointType point_type, uint32_t value) : system_id_(system_id), point_value_(point_type, value) { /* no-op */ }
		Transition(unsigned int system_id, PointType point_type, float value) : system_id_(system_id), point_value_(point_type, value) { /* no-op */ }
		Transition(unsigned int system_id, PointType point_type, double value) : system_id_(system_id), point_value_(point_type, value) { /* no-op */ }

		unsigned int system_id_;
		Core::Point point_value_;
	};
}}

#endif

