#include "../database.hpp"
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

using namespace Vlinder::RTIMDB;
using Vlinder::RTIMDB::Core::PointType;
using Vlinder::RTIMDB::Errors;

TEST_CASE("produce transitions", "[transitions]") {
	Database database;

	auto producer(database.registerProducer());

	/* allocate 32 BIs, 8 BOs and 8 BIs */
	for (unsigned int bi_index(0); bi_index < 32; ++bi_index)
	{
		auto result(database.createPoint(producer DOT_FIRST, PointType::binary_input__, false RTIMDB_NOTHROW_ARG));
		REQUIRE( Errors::no_error__ == result.second );
		REQUIRE( result.first == bi_index );
	}
	for (unsigned int bo_index(0); bo_index < 8; ++bo_index)
	{
		auto result(database.createPoint(producer DOT_FIRST, PointType::binary_output__, false RTIMDB_NOTHROW_ARG));
		REQUIRE( Errors::no_error__ == result.second );
		REQUIRE( result.first == bo_index + 32 );
	}
	for (unsigned int ai_index(0); ai_index < 8; ++ai_index)
	{
		auto result(database.createPoint(producer DOT_FIRST, PointType::analog_input__, false RTIMDB_NOTHROW_ARG));
		REQUIRE( Errors::no_error__ == result.second );
		REQUIRE( result.first == ai_index + 40 );
	}

	Core::Timestamp timestamp;
	auto transaction(database.beginTransaction(producer DOT_FIRST, timestamp));
	static_assert(RTIMDB_TRANSITION_QUEUE_CAPACITY == 256, "This test is written for a transition queue of 256 entries. If you need help with other sizes, please contact support@vlinder.ca");
	REQUIRE( transaction.size() == 254 );

	/* pretend we are a typical device with 32 BIs, 8 BOs and 8 AIs. Our scanning task should 
	 * not be worried about what the previous values were, so it'll just read everything and 
	 * put the current values, as transitions, into the database in a single transaction. If 
	 * anything did change, the database will see the change and create the appropriate events. */
	if (transaction.size() < 48) // not enough space for all of our points - signal an overflow
	{
		database.signalOverflow(producer DOT_FIRST);
	}
	else
	{
		for (unsigned int bi_index(0); bi_index < 32; ++bi_index)
		{
			transaction[bi_index] = Transition(PointType::binary_input__, bi_index, false); // we're pretending - so we don't care about the value much for now
		}
		for (unsigned int bo_index(0); bo_index < 8; ++bo_index)
		{
			transaction[bo_index + 32/* offset for the BIs */] = Transition(PointType::binary_output__, bo_index, false); // we're pretending - so we don't care about the value much for now
		}
		for (unsigned int ai_index(0); ai_index < 8; ++ai_index)
		{
			transaction[ai_index + 40/* offset for the BOs */] = Transition(PointType::analog_input__, ai_index, 0.0f); // we're pretending - so we don't care about the value much for now
		}
		database.commit(producer DOT_FIRST, transaction);
	}

	/* now, let's pretend we're a communications task. Typically, we'll want to update the status of
	 * any event queue (normally there are three for us: one for each event class), then see if there
	 * are any events to report.
	 * The same thing is true for a PLC-type logic task: that would typically run every 100 ms or so, 
	 * and start by making sure its view of the system is up to date. In that case, there is only one
	 * event queue -- but it may well be subscribed to all points in the system. */
	Errors update_result(database.update(RTIMDB_NOTHROW_ARG_1));
	REQUIRE( Errors::no_error__ == update_result );
}