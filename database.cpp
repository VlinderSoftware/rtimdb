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
#include "database.hpp"
#include <algorithm>
#include <stdexcept>
#include "exceptions/contract.hpp"

using namespace std;

namespace Vlinder { namespace RTIMDB {
	Database::Database()
		: next_point_id_(0)
	{
		for_each(begin(producer_allocations_), end(producer_allocations_), [](decltype(producer_allocations_[0]) &allocation) { allocation = false; });
	}
	Database::~Database()
	{ /* no-op */ }

#ifdef RTIMDB_ALLOW_EXCEPTIONS
	Details::Producer const * Database::registerProducer() { auto result(registerProducer(nothrow)); throwException(result.second); return result.first; }

	unsigned int Database::createPoint(Details::Producer const *producer, PointType point_type, bool initial_value)		{ auto result(createPoint(producer, point_type, initial_value, nothrow)); throwException(result.second); return result.first; }
	unsigned int Database::createPoint(Details::Producer const *producer, PointType point_type, int16_t initial_value)	{ auto result(createPoint(producer, point_type, initial_value, nothrow)); throwException(result.second); return result.first; }
	unsigned int Database::createPoint(Details::Producer const *producer, PointType point_type, int32_t initial_value)	{ auto result(createPoint(producer, point_type, initial_value, nothrow)); throwException(result.second); return result.first; }
	unsigned int Database::createPoint(Details::Producer const *producer, PointType point_type, uint16_t initial_value)	{ auto result(createPoint(producer, point_type, initial_value, nothrow)); throwException(result.second); return result.first; }
	unsigned int Database::createPoint(Details::Producer const *producer, PointType point_type, uint32_t initial_value)	{ auto result(createPoint(producer, point_type, initial_value, nothrow)); throwException(result.second); return result.first; }
	unsigned int Database::createPoint(Details::Producer const *producer, PointType point_type, float initial_value)	{ auto result(createPoint(producer, point_type, initial_value, nothrow)); throwException(result.second); return result.first; }
	unsigned int Database::createPoint(Details::Producer const *producer, PointType point_type, double initial_value)   { auto result(createPoint(producer, point_type, initial_value, nothrow)); throwException(result.second); return result.first; }
#endif

	std::pair< Details::Producer const *, Errors > Database::registerProducer(RTIMDB_NOTHROW_PARAM_1) noexcept
	{
		for (auto curr(begin(producer_allocations_)); curr != end(producer_allocations_); ++curr)
		{
			if (!curr->exchange(true))
			{
				producers_[distance(producer_allocations_, curr)] = Details::Producer(distance(producer_allocations_, curr), distance(producer_allocations_, curr));
				return make_pair(producers_ + distance(producer_allocations_, curr), Errors::no_error__);
			}
			else
			{ /* already set */ }
		}
		return make_pair(nullptr, Errors::allocation_error__);
	}
	void Database::unregisterProducer(Details::Producer const *producer) noexcept
	{
		static_assert((sizeof(command_queues_) / sizeof(command_queues_[0])) == (sizeof(producer_allocations_) / sizeof(producer_allocations_[0])), "Different amount of allocations vs producers allocatable");
		static_assert((sizeof(transition_queues_) / sizeof(transition_queues_[0])) == (sizeof(producer_allocations_) / sizeof(producer_allocations_[0])), "Different amount of allocations vs producers allocatable");
		pre_condition((producer >= producers_) && (producer < (producers_ + (sizeof(producers_) / sizeof(producers_[0])))));
		unsigned int const producer_id(producer - producers_);
		pre_condition(producer_allocations_[producer_id]);
		producer_allocations_[producer_id] = false;
	}
	CommandQueue& Database::getCommandQueue(Details::Producer const *producer) noexcept
	{
		pre_condition((producer >= producers_) && (producer < (producers_ + (sizeof(producers_) / sizeof(producers_[0])))));
		static_assert((sizeof(command_queues_) / sizeof(command_queues_[0])) == (sizeof(producer_allocations_) / sizeof(producer_allocations_[0])), "Different amount of allocations vs queues allocatable");
		pre_condition(producer->command_queue_id_ < (sizeof(command_queues_) / sizeof(command_queues_[0])));
		pre_condition(producer_allocations_[producer->command_queue_id_]);
		return command_queues_[producer->command_queue_id_];
	}
	Details::TransitionQueue& Database::getTransitionQueue(Details::Producer const *producer) noexcept
	{
		pre_condition((producer >= producers_) && (producer < (producers_ + (sizeof(producers_) / sizeof(producers_[0])))));
		static_assert((sizeof(transition_queues_) / sizeof(transition_queues_[0])) == (sizeof(producer_allocations_) / sizeof(producer_allocations_[0])), "Different amount of allocations vs queues allocatable");
		pre_condition(producer->transition_queue_id_ < (sizeof(transition_queues_) / sizeof(transition_queues_[0])));
		pre_condition(producer_allocations_[producer->transition_queue_id_]);
		return transition_queues_[producer->transition_queue_id_];
	}

	std::pair< unsigned int, Errors > Database::createPoint(Details::Producer const *producer, PointType point_type, bool initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		pre_condition((producer >= producers_) && (producer < producers_ + (sizeof(producers_) / sizeof(producers_[0]))));
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(producer, point_type, result.first);
	}
	std::pair< unsigned int, Errors > Database::createPoint(Details::Producer const *producer, PointType point_type, int16_t initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		pre_condition((producer >= producers_) && (producer < producers_ + (sizeof(producers_) / sizeof(producers_[0]))));
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(producer, point_type, result.first);
	}
	std::pair< unsigned int, Errors > Database::createPoint(Details::Producer const *producer, PointType point_type, int32_t initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		pre_condition((producer >= producers_) && (producer < producers_ + (sizeof(producers_) / sizeof(producers_[0]))));
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(producer, point_type, result.first);
	}
	std::pair< unsigned int, Errors > Database::createPoint(Details::Producer const *producer, PointType point_type, uint16_t initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		pre_condition((producer >= producers_) && (producer < producers_ + (sizeof(producers_) / sizeof(producers_[0]))));
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(producer, point_type, result.first);
	}
	std::pair< unsigned int, Errors > Database::createPoint(Details::Producer const *producer, PointType point_type, uint32_t initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		pre_condition((producer >= producers_) && (producer < producers_ + (sizeof(producers_) / sizeof(producers_[0]))));
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(producer, point_type, result.first);
	}
	std::pair< unsigned int, Errors > Database::createPoint(Details::Producer const *producer, PointType point_type, float initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		pre_condition((producer >= producers_) && (producer < producers_ + (sizeof(producers_) / sizeof(producers_[0]))));
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(producer, point_type, result.first);
	}
	std::pair< unsigned int, Errors > Database::createPoint(Details::Producer const *producer, PointType point_type, double initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		pre_condition((producer >= producers_) && (producer < producers_ + (sizeof(producers_) / sizeof(producers_[0]))));
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(producer, point_type, result.first);
	}

	Errors Database::sendCommand(unsigned int point_id, Details::CROB const &crob RTIMDB_NOTHROW_PARAM)
	{
		pre_condition(point_id < next_point_id_);
		auto point(point_descriptors_[point_id]);
		pre_condition(point.producer_id_ < RTIMDB_MAX_PRODUCER_COUNT);
		static_assert(RTIMDB_MAX_PRODUCER_COUNT == (sizeof(command_queues_) / sizeof(command_queues_[0])), "unexpected mismatch of sizes");
		return command_queues_[point.producer_id_].push(Command(crob) RTIMDB_NOTHROW_ARG) ? Errors::no_error__ : Errors::command_queue_full__;
	}

	Details::TransitionQueueTransaction Database::beginTransaction(Details::Producer const *producer, Details::Timestamp const &timestamp) noexcept
	{
		return getTransitionQueue(producer).beginTransaction(timestamp);
	}
	void Database::signalOverflow(Details::Producer const *producer) noexcept
	{
		getTransitionQueue(producer).signalOverflow();
	}
	void Database::commit(Details::Producer const *producer, Details::TransitionQueueTransaction const &transaction)
	{
		getTransitionQueue(producer).commit(transaction);
	}

	Errors Database::update(RTIMDB_NOTHROW_PARAM_1) noexcept
	{
		Errors retval(Errors::no_error__);
		auto transaction(data_store_.startTransaction(RTIMDB_NOTHROW_ARG_1));
		for (auto transition_queue(begin(transition_queues_)); transition_queue != end(transition_queues_); ++transition_queue)
		{
			bool first(true);
			for (auto transition_count(transition_queue->size()); (retval == Errors::no_error__) && transition_count; --transition_count)
			{
				auto transition(transition_queue->front(RTIMDB_NOTHROW_ARG_1));
				post_condition(transition.second);
				if (0 == transition.first.type()) // entry is a time stamp
				{
					latest_timestamp_[transition_queue - transition_queues_] = transition.first.get< Details::Timestamp >();
					if (!first)
					{
						Errors commit_result(data_store_.commit(transaction.first RTIMDB_NOTHROW_ARG));
						//TODO check commit result and create event queue entries
						transaction = std::move(data_store_.startTransaction(RTIMDB_NOTHROW_ARG_1));
					}
					else
					{ /* this is the first transition */ }
				}
				else
				{
					first = false;
					invariant(1 == transition.first.type());
					auto value(transition.first.get< Transition >());
					Errors update_result(data_store_.update(transaction.first, value.system_id_, value.point_value_ RTIMDB_NOTHROW_ARG));
					if (Errors::no_error__ != update_result)
					{
						transition_queue->pop(); // we'll have to pop it because there's no other way to get rid of it
						return update_result;
					}
					else
					{ /* all is well */ }
				}
				transition_queue->pop();
			}
		}

		return retval;
	}

	std::pair< unsigned int, Errors > Database::createPoint_(Details::Producer const *producer, PointType point_type, unsigned int data_store_id) noexcept
	{
		Details::PointDescriptor descriptor(point_type, data_store_id, producer - producers_);
		unsigned int point_id(next_point_id_++);
		point_descriptors_[point_id] = descriptor;
		return make_pair(point_id, Errors::no_error__);
	}
}}


