#include "../database.hpp"
#include "../details/crob.hpp"
#include "../exceptions.hpp"
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
#define DOT_FIRST	.first
#endif

using namespace std;
using namespace Vlinder::RTIMDB;

TEST_CASE("Try to allocate a command queue", "[commandqueuemanagement]") {
	static_assert(RTIMDB_MAX_PRODUCER_COUNT == 2, "This test is written for two queues. Please contact support@vlinder.ca if you need help for a different max. number of queues");
	Database database;
	REQUIRE( database.registerProducer()DOT_FIRST != nullptr );
	REQUIRE( database.registerProducer()DOT_FIRST != nullptr );
	bool caught(false);
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	try
	{
#endif
		auto result = database.registerProducer();
		(void)result;
#ifndef RTIMDB_ALLOW_EXCEPTIONS
		caught = result.second != Errors::no_error__;
#else
	}
	catch (Vlinder::RTIMDB::AllocationError const &)
	{
		caught = true;
	}
#endif
	REQUIRE( caught );
}

TEST_CASE("Try to allocate, them free, the allocate a command queue", "[commandqueuemanagement]") {
	static_assert(RTIMDB_MAX_PRODUCER_COUNT == 2, "This test is written for two queues. Please contact support@vlinder.ca if you need help for a different max. number of queues");
	Database database;
	auto producer1(database.registerProducer()DOT_FIRST); REQUIRE( producer1 != nullptr );
	auto producer2(database.registerProducer()DOT_FIRST); REQUIRE( producer2 != nullptr );
	database.unregisterProducer(producer1);
	REQUIRE( database.registerProducer()DOT_FIRST == producer1 );
	database.unregisterProducer(producer2);
	REQUIRE( database.registerProducer()DOT_FIRST == producer2 );
	database.unregisterProducer(producer2);
	database.unregisterProducer(producer1);
	REQUIRE( database.registerProducer()DOT_FIRST == producer1 );
	REQUIRE( database.registerProducer()DOT_FIRST == producer2 );
}

TEST_CASE("Try to send a command", "[commandqueuemanagement]") {
	Database database;

	// set up the simili scan task
	auto allocation_result(database.registerProducer(RTIMDB_NOTHROW_ARG_1));
	REQUIRE( allocation_result.second == Errors::no_error__ );
	auto point(database.createPoint(allocation_result.first, Core::PointType::binary_output__, false RTIMDB_NOTHROW_ARG));
	REQUIRE( point.second == Errors::no_error__ );

	// protocol rx a crob for this point (for now we'll pretend mapping is done by magic)
	Details::CROB crob(Details::CROB::operate_pulse_on__, false, Details::CROB::close__, 1, 0, 0);
	auto result(database.sendCommand(point.first, crob RTIMDB_NOTHROW_ARG));
	REQUIRE( Errors::no_error__ == result );

	// now fetch what we just pushed
	auto &queue(database.getCommandQueue(allocation_result.first));
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
