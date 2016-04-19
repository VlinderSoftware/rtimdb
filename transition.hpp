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
#include "core/pointvalue.hpp"
#include "core/flags.hpp"
#include "core/details/optional.hpp"
#include <limits>

namespace Vlinder { namespace RTIMDB {
	struct no_value_change_tag {};
	extern no_value_change_tag no_value_change;
	struct Transition
	{
		Transition()
			: system_id_(std::numeric_limits< decltype(system_id_) >::max())
		{ /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, bool value)                                     : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, int16_t value)                                  : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, int32_t value)                                  : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, uint16_t value)                                 : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, uint32_t value)                                 : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, float value)                                    : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, double value)                                   : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, bool value, Core::Flags flags)                  : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)), flags_(flags) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, int16_t value, Core::Flags flags)               : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)), flags_(flags) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, int32_t value, Core::Flags flags)               : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)), flags_(flags) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, uint16_t value, Core::Flags flags)              : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)), flags_(flags) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, uint32_t value, Core::Flags flags)              : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)), flags_(flags) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, float value, Core::Flags flags)                 : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)), flags_(flags) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, double value, Core::Flags flags)                : point_type_(point_type), system_id_(system_id), point_value_(Core::PointValue(value)), flags_(flags) { /* no-op */ }
		Transition(Core::PointType point_type, unsigned int system_id, no_value_change_tag const &, Core::Flags flags) : point_type_(point_type), system_id_(system_id), flags_(flags) { /* no-op */ }

		Core::PointType point_type_;
		unsigned int system_id_;
		Core::Details::Optional< Core::PointValue > point_value_;
		Core::Details::Optional< Core::Flags > flags_;
	};
}}

#endif

