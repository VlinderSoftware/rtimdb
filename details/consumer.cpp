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
#include "consumer.hpp"
#include <algorithm>

using namespace std;

namespace Vlinder { namespace RTIMDB { namespace Details {
	Consumer::Consumer()
		: next_mapping_entry_(0)
	{ /* no-op */ }
	
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	void Consumer::mapPoint(uintptr_t tag, PointType point_type, unsigned int system_id) { throwException(mapPoint(tag, point_type, system_id, nothrow)); }
	void Consumer::setEventClass(uintptr_t tag, EventClass event_class) { throwException(setEventClass(tag, event_class, nothrow)); }
	EventClass Consumer::getEventClass(uintptr_t tag) const { auto result(getEventClass(tag, nothrow)); throwException(result.second); return result.first; }
	EventClass Consumer::getEventClass(PointType point_type, unsigned int system_id) const { auto result(getEventClass(point_type, system_id, nothrow)); throwException(result.second); return result.first; }
#endif

	Errors Consumer::mapPoint(uintptr_t tag, PointType point_type, unsigned int system_id RTIMDB_NOTHROW_PARAM) noexcept
	{
		if ((sizeof(mappings_) / sizeof(mappings_[0])) == next_mapping_entry_) return Errors::mapping_full__;
		mappings_[next_mapping_entry_++] = Mapping(tag, point_type, system_id);
		return Errors::no_error__;
	}
	
	Errors Consumer::setEventClass(uintptr_t tag, EventClass event_class RTIMDB_NOTHROW_PARAM) noexcept
	{
		auto which(findMapping(tag));
		if (end(mappings_) == which) return Errors::unknown_point__;
		which->event_class_ = event_class;
		return Errors::no_error__;
	}
	
	std::pair< EventClass, Errors > Consumer::getEventClass(uintptr_t tag RTIMDB_NOTHROW_PARAM) const noexcept
	{
		auto which(findMapping(tag));
		if (end(mappings_) == which) return make_pair(EventClass::class_0__, Errors::unknown_point__);
		return make_pair(which->event_class_, Errors::no_error__);
	}
	std::pair< EventClass, Errors > Consumer::getEventClass(PointType point_type, unsigned int system_id RTIMDB_NOTHROW_PARAM) const noexcept
	{
		auto which(findMapping(point_type, system_id));
		if (end(mappings_) == which) return make_pair(EventClass::class_0__, Errors::unknown_point__);
		return make_pair(which->event_class_, Errors::no_error__);
	}

	bool Consumer::eventsAvailable(EventClass event_class) const noexcept
	{
		if (event_class == EventClass::class_0__) return false;
		static_assert(EventClass::class_3__ == EventClass::class_count__, "unexpected value for EventClass enumerators");
		return !event_queues_[static_cast< unsigned int >(event_class) - 1/* offset for class 0 */].empty();
	}

	unsigned int Consumer::getEventCount(EventClass event_class) const noexcept
	{
		if (event_class == EventClass::class_0__) 0;
		static_assert(EventClass::class_3__ == EventClass::class_count__, "unexpected value for EventClass enumerators");
		return event_queues_[static_cast< unsigned int >(event_class) - 1/* offset for class 0 */].size();
	}
	Events Consumer::getEvents(EventClass event_class) noexcept
	{
		return Events(event_queues_[static_cast< unsigned int >(event_class) - 1/* offset for class 0 */]);
	}

	void Consumer::reset()
	{
		next_mapping_entry_ = 0;
		for_each(begin(event_queues_), end(event_queues_), [=](remove_reference< decltype(event_queues_[0]) >::type &q){ q.clear(); });
	}

	Consumer::Mapping const* Consumer::findMapping(uintptr_t tag) const noexcept
	{
		auto which(find_if(
			  begin(mappings_)
			, end(mappings_)
			, [=](remove_reference< decltype(*begin(mappings_)) >::type &mapping) -> bool {
				return (mapping.tag_ == tag);
			  }
			));
		return which;
	}

	Consumer::Mapping const* Consumer::findMapping(PointType point_type, unsigned int system_id) const noexcept
	{
		auto which(find_if(
			  begin(mappings_)
			, end(mappings_)
			, [=](remove_reference< decltype(*begin(mappings_)) >::type &mapping) -> bool {
				return ((mapping.point_type_ == point_type) && (mapping.system_id_ == system_id));
			  }
			));
		return which;
	}

	Consumer::Mapping* Consumer::findMapping(uintptr_t tag) noexcept
	{
		auto which(find_if(
			  begin(mappings_)
			, end(mappings_)
			, [=](remove_reference< decltype(*begin(mappings_)) >::type &mapping) -> bool {
				return (mapping.tag_ == tag);
			  }
			));
		return which;
	}

	Consumer::Mapping* Consumer::findMapping(PointType point_type, unsigned int system_id) noexcept
	{
		auto which(find_if(
			  begin(mappings_)
			, end(mappings_)
			, [=](remove_reference< decltype(*begin(mappings_)) >::type &mapping) -> bool {
				return ((mapping.point_type_ == point_type) && (mapping.system_id_ == system_id));
			  }
			));
		return which;
	}
}}}


