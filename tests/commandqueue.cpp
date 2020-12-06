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
#include "catch.hpp"

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

TEST_CASE( "Try to create an instance (of a command queue)", "[commandqueue]") {
	static_assert(RTIMDB_COMMAND_QUEUE_CAPACITY == 16, "This test is written for queues with a capacity of 16. Please contact support@vlinder.ca if you need help for a different queue capacity");
	CommandQueue command_queue;
}

TEST_CASE("Try push and pop", "[commandqueue]") {
	CommandQueue queue;
	Details::CROB crob(Details::CROB::operate_pulse_on__, false, Details::CROB::close__, 1, 0, 0);
	REQUIRE( Private::CommandQueueTestAttorney::push(&queue, crob) );
	REQUIRE( !queue.empty() );
	REQUIRE( queue.size() == 1 );
	auto queue_front(queue.front(RTIMDB_NOTHROW_ARG_1));
	REQUIRE( queue_front.second );
	REQUIRE( Command::crob__ == queue_front.first.type_ );
	auto da_crob(queue_front.first.get< Details::CROB >());
	REQUIRE( crob.clear_ == da_crob.clear_ );
	REQUIRE( crob.count_ == da_crob.count_ );
	REQUIRE( crob.off_time_ == da_crob.off_time_ );
	REQUIRE( crob.on_time_ == da_crob.on_time_ );
	REQUIRE( crob.op_type_ == da_crob.op_type_ );
	REQUIRE( crob.tcc_ == da_crob.tcc_ );
	queue.pop();
	REQUIRE( queue.empty() );
	REQUIRE( queue.size() == 0 );
}

