#include "../outstation/database.hpp"
#include <stdexcept>

#define assert(x) if (!(x)) { throw std::logic_error("Assertion failed"); }
#ifdef RTIMDB_ALLOW_EXCEPTIONS
#define DOT_FIRST
#define FIRST
#define FIRST_GET
#else
#define FIRST .first
#define FIRST_GET .first.get()
#define DOT_FIRST	.first
#endif

using namespace std;
using namespace Vlinder::RTIMDB::Outstation;

int tryAllocateCommandQueue()
{
	static_assert(RTIMDB_MAX_COMMAND_QUEUE_COUNT == 2, "This test is written for two queues. Please contact support@vlinder.ca if you need help for a different max. number of queues");
	Database database;
	assert(database.allocateCommandQueue()DOT_FIRST == 0);
	assert(database.allocateCommandQueue()DOT_FIRST == 1);
	bool caught(false);
	try
	{
		auto result = database.allocateCommandQueue();
#ifndef RTIMDB_ALLOW_EXCEPTIONS
		caught = !result.second;
#endif

	}
	catch (bad_alloc const &)
	{
		caught = true;
	}
	assert(caught);

	return 0;
}

int tryAllocateFreeAndAllocate()
{
	static_assert(RTIMDB_MAX_COMMAND_QUEUE_COUNT == 2, "This test is written for two queues. Please contact support@vlinder.ca if you need help for a different max. number of queues");
	Database database;
	assert(database.allocateCommandQueue()DOT_FIRST == 0);
	assert(database.allocateCommandQueue()DOT_FIRST == 1);
	database.releaseCommandQueue(0);
	assert(database.allocateCommandQueue()DOT_FIRST == 0);
	database.releaseCommandQueue(1);
	assert(database.allocateCommandQueue()DOT_FIRST == 1);
	database.releaseCommandQueue(1);
	database.releaseCommandQueue(0);
	assert(database.allocateCommandQueue()DOT_FIRST == 0);
	assert(database.allocateCommandQueue()DOT_FIRST == 1);

	return 0;
}

int main()
{
	return 0
		|| tryAllocateCommandQueue()
		|| tryAllocateFreeAndAllocate()
		;
}