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
	{
		for_each(begin(command_queue_allocations_), end(command_queue_allocations_), [](decltype(command_queue_allocations_[0]) &allocation){ allocation = false; });
	}
	Database::~Database()
	{ /* no-op */ }

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
}}}


