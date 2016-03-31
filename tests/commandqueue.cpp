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
#include "../commandqueue.hpp"
#include <stdexcept>
#include "exceptions/contract.hpp"

#ifdef RTIMDB_ALLOW_EXCEPTIONS
#define DOT_FIRST
#define FIRST
#define FIRST_GET
#else
#define FIRST .first
#define FIRST_GET .first.get()
#define DOT_FIRST .first
#endif

using namespace std;
using namespace Vlinder::RTIMDB;

namespace Vlinder { namespace RTIMDB { namespace Private {
class CommandQueueTestAttorney
{
public :
	static bool push(CommandQueue *queue, Command const &command) noexcept
	{
		return queue->push(command RTIMDB_NOTHROW_ARG);
	}
};
}}}

int tryCreateInstance()
{
	static_assert(RTIMDB_COMMAND_QUEUE_CAPACITY == 16, "This test is written for queues with a capacity of 16. Please contact support@vlinder.ca if you need help for a different queue capacity");
	CommandQueue command_queue;

	return 0;
}

int tryPushAndPop()
{
	CommandQueue queue;
	Details::CROB crob(Details::CROB::operate_pulse_on__, false, Details::CROB::close__, 1, 0, 0);
	assert(Private::CommandQueueTestAttorney::push(&queue, crob));
	assert(!queue.empty());
	assert(queue.size() == 1);
	auto queue_front(queue.front(RTIMDB_NOTHROW_ARG_1));
	assert(queue_front.second);
	assert(Command::crob__ == queue_front.first.type_);
	auto da_crob(queue_front.first.get< Details::CROB >());
	assert(crob.clear_ == da_crob.clear_);
	assert(crob.count_ == da_crob.count_);
	assert(crob.off_time_ == da_crob.off_time_);
	assert(crob.on_time_ == da_crob.on_time_);
	assert(crob.op_type_ == da_crob.op_type_);
	assert(crob.tcc_ == da_crob.tcc_);
	queue.pop();
	assert(queue.empty());
	assert(queue.size() == 0);
	
	return 0;
}

int main()
{
	return 0
		|| tryCreateInstance()
		|| tryPushAndPop()
		;
}