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
#include "../outstation/commandqueue.hpp"
#include <stdexcept>

#define assert(x) if (!(x)) { throw std::logic_error("Assertion failed"); }
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
using namespace Vlinder::RTIMDB::Outstation;

int tryCreateInstance()
{
	static_assert(RTIMDB_COMMAND_QUEUE_CAPACITY == 16, "This test is written for queues with a capacity of 16. Please contact support@vlinder.ca if you need help for a different queue capacity");
	CommandQueue command_queue;

	return 0;
}

int main()
{
	return 0
		|| tryCreateInstance()
		;
}