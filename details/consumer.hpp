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
#ifndef vlinder_rtimdb_details_consumer_hpp
#define vlinder_rtimdb_details_consumer_hpp

#include "eventclass.hpp"
#include "../exceptions.hpp"
#include "../core/pointtype.hpp"
#include <utility>
#include <mutex>
#include <condition_variable>
#include "eventqueue.hpp"
#include "events.hpp"
#include "pollresult.hpp"

namespace Vlinder { namespace RTIMDB { 
	class Database;
namespace Details {
	class RTIMDB_API Consumer
	{
	public :
		Consumer();

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		void mapPoint(uintptr_t tag, Core::PointType point_type, unsigned int system_id);
		void setEventClass(uintptr_t tag, EventClass event_class);
		EventClass getEventClass(uintptr_t tag) const;
		EventClass getEventClass(Core::PointType point_type, unsigned int system_id) const;
#endif
		Errors mapPoint(uintptr_t tag, Core::PointType point_type, unsigned int system_id RTIMDB_NOTHROW_PARAM) noexcept;
		Errors setEventClass(uintptr_t tag, EventClass event_class RTIMDB_NOTHROW_PARAM) noexcept;
		std::pair< EventClass, Errors > getEventClass(uintptr_t tag RTIMDB_NOTHROW_PARAM) const noexcept;
		std::pair< EventClass, Errors > getEventClass(Core::PointType point_type, unsigned int system_id RTIMDB_NOTHROW_PARAM) const noexcept;

		bool eventsAvailable(EventClass event_class) const noexcept;
		unsigned int getEventCount(EventClass event_class) const noexcept;
		Events getEvents(EventClass event_class) noexcept;

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		PollResult poll(PollDescriptor const &poll_descriptor);
#endif
		std::pair< PollResult, Errors > poll(PollDescriptor const &poll_descriptor RTIMDB_NOTHROW_PARAM) noexcept;

		unsigned int getMappingSize() const noexcept { return next_mapping_entry_; }

	private : // friend-only API
		struct Mapping
		{
			Mapping()
				: event_class_(EventClass::class_0__)
			{ /* no-op */ }
			Mapping(uintptr_t tag, Core::PointType point_type, unsigned int system_id)
				: tag_(tag)
				, point_type_(point_type)
				, system_id_(system_id)
				, event_class_(EventClass::class_0__)
			{ /* no-op */ }

			uintptr_t tag_;
			Core::PointType point_type_;
			unsigned int system_id_;
			EventClass event_class_;
		};
		
		void reset(Database *database);

		Mapping const* findMapping(Core::PointType point_type, unsigned int system_id) const noexcept;
		Mapping* findMapping(Core::PointType point_type, unsigned int system_id) noexcept;

		void setCommitted(unsigned int committed_version) noexcept;
		void awaitTransactionDone(unsigned int transaction_version) const noexcept;

		Core::PointValue getPointByIndex(Core::Details::ROTransaction const &transaction, unsigned int index) const noexcept;

	private :
		Mapping const* findMapping(uintptr_t tag) const noexcept;
		Mapping* findMapping(uintptr_t tag) noexcept;

		Mapping mappings_[RTIMDB_POINT_COUNT];
		unsigned int next_mapping_entry_;
		EventQueue event_queues_[static_cast< unsigned int >(EventClass::class_count__)];
		Database *database_;
		unsigned int committed_version_;
		mutable std::mutex committed_version_sem_mutex_;
		mutable std::condition_variable committed_version_sem_cv_;

		friend class RTIMDB::Database;
		friend class PollResult;
	};
}}}

#endif


