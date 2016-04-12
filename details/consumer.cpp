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
#include "database.hpp"
#include "exceptions/contract.hpp"
#include "rtimdb_config.hpp"
#include "defaultmappinghelper.hpp"
#include "sortedmappinghelper.hpp"
#include <algorithm>

using namespace std;

namespace Vlinder { namespace RTIMDB { namespace Details {
	Consumer::Consumer()
		: mapping_helper_(new RTIMDB_MappingHelper)
		, database_(0)
		, committed_version_(1) // datastore starts at version 1
	{ /* no-op */ }
	Consumer::~Consumer()
	{
		delete mapping_helper_;
	}
	
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	void Consumer::mapPoint(uintptr_t tag, Core::PointType point_type, unsigned int system_id) { throwException(mapPoint(tag, point_type, system_id, nothrow)); }
	void Consumer::setEventClass(uintptr_t tag, EventClass event_class) { throwException(setEventClass(tag, event_class, nothrow)); }
	EventClass Consumer::getEventClass(uintptr_t tag) const { auto result(getEventClass(tag, nothrow)); throwException(result.second); return result.first; }
	EventClass Consumer::getEventClass(Core::PointType point_type, unsigned int system_id) const { auto result(getEventClass(point_type, system_id, nothrow)); throwException(result.second); return result.first; }
#endif

	Errors Consumer::mapPoint(uintptr_t tag, Core::PointType point_type, unsigned int system_id RTIMDB_NOTHROW_PARAM) noexcept
	{
		return mapping_helper_->insert(tag, point_type, system_id);
	}
	
	Errors Consumer::setEventClass(uintptr_t tag, EventClass event_class RTIMDB_NOTHROW_PARAM) noexcept
	{
		auto which(mapping_helper_->find(tag));
		if (nullptr == which) return Errors::unknown_point__;
		which->event_class_ = event_class;
		return Errors::no_error__;
	}
	
	std::pair< EventClass, Errors > Consumer::getEventClass(uintptr_t tag RTIMDB_NOTHROW_PARAM) const noexcept
	{
		auto which(mapping_helper_->find(tag));
		if (nullptr == which) return make_pair(EventClass::class_0__, Errors::unknown_point__);
		return make_pair(which->event_class_, Errors::no_error__);
	}
	std::pair< EventClass, Errors > Consumer::getEventClass(Core::PointType point_type, unsigned int system_id RTIMDB_NOTHROW_PARAM) const noexcept
	{
		auto which(mapping_helper_->find(point_type, system_id));
		if (nullptr == which) return make_pair(EventClass::class_0__, Errors::unknown_point__);
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
		if (event_class == EventClass::class_0__) return 0;
		static_assert(EventClass::class_3__ == EventClass::class_count__, "unexpected value for EventClass enumerators");
		return event_queues_[static_cast< unsigned int >(event_class) - 1/* offset for class 0 */].size();
	}
	Events Consumer::getEvents(EventClass event_class) noexcept
	{
		return Events(event_queues_[static_cast< unsigned int >(event_class) - 1/* offset for class 0 */]);
	}

#ifdef RTIMDB_ALLOW_EXCEPTIONS
	PollResult Consumer::poll(PollDescriptor const &poll_descriptor) { auto result(poll(poll_descriptor, nothrow)); throwException(result.second); return move(result.first); }
#endif
	pair< PollResult, Errors > Consumer::poll(PollDescriptor const &poll_descriptor RTIMDB_NOTHROW_PARAM) noexcept
	{
		// start a read-only transaction on the data store. This transaction will give us the current 
		// version of the data store which will tell us which events we can, or cannot, report in this poll
		auto transaction(database_->beginTransaction(this RTIMDB_NOTHROW_ARG));
		if (Errors::no_error__ != transaction.second) return make_pair(PollResult(), transaction.second);
		return make_pair(PollResult(poll_descriptor, this, move(transaction.first)), Errors::no_error__);
	}

	void Consumer::reset(Database *database)
	{
		//NOTE: we do not reset committed_version_!
		database_ = database;
		mapping_helper_->clear();
		for_each(begin(event_queues_), end(event_queues_), [=](remove_reference< decltype(event_queues_[0]) >::type &q){ q.clear(); });
	}

	void Consumer::setCommitted(unsigned int committed_version) noexcept
	{
		committed_version_ = committed_version;
		committed_version_sem_cv_.notify_all();
	}
	void Consumer::awaitTransactionDone(unsigned int transaction_version) const noexcept
	{
		unique_lock< decltype(committed_version_sem_mutex_) > committed_version_sem_mutex(committed_version_sem_mutex_);
		while (committed_version_ < transaction_version)
		{
			committed_version_sem_cv_.wait(committed_version_sem_mutex);
		}
	}

	Mapping const* Consumer::findMapping(Core::PointType point_type, unsigned int system_id) const noexcept
	{
		return mapping_helper_->find(point_type, system_id);
	}
	Mapping* Consumer::findMapping(Core::PointType point_type, unsigned int system_id) noexcept
	{
		return mapping_helper_->find(point_type, system_id);
	}
	Mapping const* Consumer::findMapping(uintptr_t tag) const noexcept
	{
		return mapping_helper_->find(tag);
	}
	Mapping* Consumer::findMapping(uintptr_t tag) noexcept
	{
		return mapping_helper_->find(tag);
	}

	Core::PointValue Consumer::getPointByIndex(Core::Details::ROTransaction const &transaction, unsigned int index) const noexcept
	{
		pre_condition(index < mapping_helper_->size());
		auto mapping(mapping_helper_->at(index));
		post_condition(mapping);
		auto read_result(database_->read(transaction, mapping->point_type_, mapping->system_id_));
		// the only way for the read of the database to fail is if the point doesn't exist, in which case the mapping should have failed. Hence, this cannot fail without it being a bug
		post_condition(Errors::no_error__ == read_result.second);
		post_condition(!read_result.first.empty());
		return *(read_result.first);
	}
	unsigned int Consumer::getMappingSize() const noexcept
	{
		return mapping_helper_->size();
	}
}}}


