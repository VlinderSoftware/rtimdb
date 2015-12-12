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

namespace Vlinder { namespace RTIMDB { namespace Outstation {
	Database::Database()
		: next_point_id_(0)
	{
		for_each(begin(command_queue_allocations_), end(command_queue_allocations_), [](decltype(command_queue_allocations_[0]) &allocation){ allocation = false; });
	}
	Database::~Database()
	{ /* no-op */ }

	std::pair< unsigned int, Errors > Database::createPoint(uintptr_t tag, PointType point_type, bool initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(tag, point_type, result.first);
	}
	std::pair< unsigned int, Errors > Database::createPoint(uintptr_t tag, PointType point_type, int16_t initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(tag, point_type, result.first);
	}
	std::pair< unsigned int, Errors > Database::createPoint(uintptr_t tag, PointType point_type, int32_t initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(tag, point_type, result.first);
	}
	std::pair< unsigned int, Errors > Database::createPoint(uintptr_t tag, PointType point_type, uint16_t initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(tag, point_type, result.first);
	}
	std::pair< unsigned int, Errors > Database::createPoint(uintptr_t tag, PointType point_type, uint32_t initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(tag, point_type, result.first);
	}
	std::pair< unsigned int, Errors > Database::createPoint(uintptr_t tag, PointType point_type, float initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(tag, point_type, result.first);
	}
	std::pair< unsigned int, Errors > Database::createPoint(uintptr_t tag, PointType point_type, double initial_value RTIMDB_NOTHROW_PARAM) noexcept
	{
		Core::Point point(point_type, initial_value);
		auto result(data_store_.insert(point RTIMDB_NOTHROW_ARG));
		if (result.second != Errors::no_error__) return std::make_pair(0, result.second);
		return createPoint_(tag, point_type, result.first);
	}

	void Database::associate(unsigned int point_id, unsigned int command_queue_id)
	{
		pre_condition(point_id < next_point_id_);
		pre_condition(command_queue_id < RTIMDB_MAX_COMMAND_QUEUE_COUNT);
		static_assert(RTIMDB_MAX_COMMAND_QUEUE_COUNT == (sizeof(command_queues_) / sizeof(command_queues_[0])), "unexpected mismatch of sizes");
		point_descriptors_[point_id].associated_command_queue_ = command_queue_id;
	}
	Errors Database::sendCommand(unsigned int point_id, Details::CROB const &crob RTIMDB_NOTHROW_PARAM)
	{
		pre_condition(point_id < next_point_id_);
		auto point(point_descriptors_[point_id]);
		pre_condition(point.associated_command_queue_ < RTIMDB_MAX_COMMAND_QUEUE_COUNT);
		static_assert(RTIMDB_MAX_COMMAND_QUEUE_COUNT == (sizeof(command_queues_) / sizeof(command_queues_[0])), "unexpected mismatch of sizes");
		return command_queues_[point.associated_command_queue_].push(Command(crob) RTIMDB_NOTHROW_ARG) ? Errors::no_error__ : Errors::command_queue_full__;
	}

#ifdef RTIMDB_ALLOW_EXCEPTIONS
	unsigned int Database::allocateCommandQueue()
	{
		auto result(allocateCommandQueue(nothrow));
		if (!result.second)
		{
			throw bad_alloc();
		}
		else
		{ /* no-op */ }
		return result.first;
	}
#endif

	std::pair< unsigned int, bool > Database::allocateCommandQueue(RTIMDB_NOTHROW_PARAM_1)
	{
		for (auto curr(begin(command_queue_allocations_)); curr != end(command_queue_allocations_); ++curr)
		{
			if (!curr->exchange(true))
			{
				return make_pair(distance(begin(command_queue_allocations_), curr), true);
			}
			else
			{ /* already set */ }
		}
		return make_pair(0, false);
	}
	void Database::releaseCommandQueue(unsigned int command_queue_id) noexcept
	{
		static_assert((sizeof(command_queues_) / sizeof(command_queues_[0])) == (sizeof(command_queue_allocations_) / sizeof(command_queue_allocations_[0])), "Different amount of allocations vs queues allocatable");
		pre_condition(command_queue_id < (sizeof(command_queues_) / sizeof(command_queues_[0])));
		pre_condition(command_queue_allocations_[command_queue_id]);
		command_queue_allocations_[command_queue_id] = false;
	}
	CommandQueue& Database::getCommandQueue(unsigned int command_queue_id)
	{
		static_assert((sizeof(command_queues_) / sizeof(command_queues_[0])) == (sizeof(command_queue_allocations_) / sizeof(command_queue_allocations_[0])), "Different amount of allocations vs queues allocatable");
		pre_condition(command_queue_id < (sizeof(command_queues_) / sizeof(command_queues_[0])));
		pre_condition(command_queue_allocations_[command_queue_id]);
		return command_queues_[command_queue_id];
	}
	std::pair< unsigned int, Errors > Database::createPoint_(uintptr_t tag, PointType point_type, unsigned int data_store_id) noexcept
	{
		Details::PointDescriptor descriptor(tag, point_type, data_store_id);
		unsigned int point_id(next_point_id_++);
		point_descriptors_[point_id] = descriptor;
		return make_pair(point_id, Errors::no_error__);
	}
}}}


