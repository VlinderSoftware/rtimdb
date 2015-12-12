#include "../outstation/database.hpp"
#include "../outstation/details/crob.hpp"
#include <stdexcept>

#undef assert
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
using namespace Vlinder::RTIMDB;
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

int trySendCommand()
{
	Database database;

	// set up the simili scan task
	auto allocation_result(database.allocateCommandQueue(RTIMDB_NOTHROW_ARG_1));
	assert(allocation_result.second);
	auto point(database.createPoint(1/*tag*/, PointType::binary_output__, false RTIMDB_NOTHROW_ARG));
	assert(point.second == Errors::no_error__);
	database.associate(point.first, allocation_result.first);

	// protocol rx a crob for this point (for now we'll pretend mapping is done by magic)
	Outstation::Details::CROB crob(Outstation::Details::CROB::operate_pulse_on__, false, Outstation::Details::CROB::close__, 1, 0, 0);
	auto result(database.sendCommand(point.first, crob RTIMDB_NOTHROW_ARG));
	assert(Errors::no_error__ == result);

	// now fetch what we just pushed
	auto &queue(database.getCommandQueue(allocation_result.first));
	assert(!queue.empty());
	assert(queue.size() == 1);
	auto queue_front(queue.front(RTIMDB_NOTHROW_ARG_1));
	assert(queue_front.second);
	assert(Command::crob__ == queue_front.first.type_);
	auto da_crob(queue_front.first.get< Outstation::Details::CROB >());
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
		|| tryAllocateCommandQueue()
		|| tryAllocateFreeAndAllocate()
		|| trySendCommand()
		;
}