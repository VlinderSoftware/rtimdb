#include "../database.hpp"
#include "exceptions/contract.hpp"
#include "catch.hpp"

using namespace Vlinder::RTIMDB;
using namespace Vlinder::RTIMDB::Core;
using namespace Vlinder::RTIMDB::Details;

#define USER_MAPPING_BI 1
#define USER_MAPPING_AI 2
#define USER_MAPPING_CI 3
#define USER_MAPPING_BO 4
#define USER_MAPPING_AO 5

#define USER_MAPPING(T, ID) (((((uintptr_t)(T)) & 0x0000000FU) << 28) | (((uintptr_t)(ID)) & 0x0FFFFFFFU))

TEST_CASE("Try to register a consumer", "[consumer]") {
	Database database;

	auto consumer(database.registerConsumer(RTIMDB_NOTHROW_ARG_1));
	REQUIRE( consumer.second == Errors::no_error__ );
}

TEST_CASE("create a mapping", "[consumer]") {
	Database database;

	auto consumer(database.registerConsumer(RTIMDB_NOTHROW_ARG_1));
	REQUIRE( consumer.second == Errors::no_error__ );
	consumer.first->mapPoint(USER_MAPPING(USER_MAPPING_BI, 1), Core::PointType::binary_input__, 3);
}

TEST_CASE("set event class", "[consumer]") {
	Database database;

	auto consumer(database.registerConsumer(RTIMDB_NOTHROW_ARG_1));
	REQUIRE( consumer.second == Errors::no_error__ );
	consumer.first->mapPoint(USER_MAPPING(USER_MAPPING_BI, 1), Core::PointType::binary_input__, 3);
	consumer.first->setEventClass(USER_MAPPING(USER_MAPPING_BI, 1), EventClass::class_1__);
}

TEST_CASE("Poll event presence", "[consumer]") {
	Database database;

	auto consumer(database.registerConsumer(RTIMDB_NOTHROW_ARG_1));
	REQUIRE( consumer.second == Errors::no_error__ );
	consumer.first->mapPoint(USER_MAPPING(USER_MAPPING_BI, 1), Core::PointType::binary_input__, 3);
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_0__) ); // can never be true
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_1__) );
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_2__) );
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_3__) );
}

TEST_CASE("Update the database", "[consumer]") {
	Database database;

	/* initialization phase */
	auto producer(database.registerProducer(RTIMDB_NOTHROW_ARG_1));
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);

	auto consumer(database.registerConsumer(RTIMDB_NOTHROW_ARG_1));
	REQUIRE( consumer.second == Errors::no_error__ );
	consumer.first->mapPoint(USER_MAPPING(USER_MAPPING_BI, 72), Core::PointType::binary_input__, 3);

	// first consumer poll
	database.update();
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_0__) ); // can never be true
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_1__) );
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_2__) );
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_3__) );

	// producer creates transitions -- one point changes (which happens to be the one mapped by the consumer)
	auto transaction(database.beginTransaction(producer.first, Timestamp(1, 2)));
	transaction.add(Transition(Core::PointType::binary_input__, 0, false));
	transaction.add(Transition(Core::PointType::binary_input__, 1, false));
	transaction.add(Transition(Core::PointType::binary_input__, 2, false));
	transaction.add(Transition(Core::PointType::binary_input__, 3, true));
	transaction.add(Transition(Core::PointType::binary_input__, 4, false));
	transaction.add(Transition(Core::PointType::binary_input__, 5, false));
	transaction.add(Transition(Core::PointType::binary_input__, 6, false));
	transaction.add(Transition(Core::PointType::binary_input__, 7, false));
	transaction.commit();

	// second consumer poll
	database.update();
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_0__) ); // can never be true
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_1__) );
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_2__) );
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_3__) );

	consumer.first->setEventClass(USER_MAPPING(USER_MAPPING_BI, 72), EventClass::class_1__);
	// producer creates transitions -- one point changes (which happens to be the one mapped by the consumer)
	transaction = database.beginTransaction(producer.first, Timestamp(1, 3));
	transaction.add(Transition(Core::PointType::binary_input__, 0, false));
	transaction.add(Transition(Core::PointType::binary_input__, 1, false));
	transaction.add(Transition(Core::PointType::binary_input__, 2, false));
	transaction.add(Transition(Core::PointType::binary_input__, 3, false));
	transaction.add(Transition(Core::PointType::binary_input__, 4, false));
	transaction.add(Transition(Core::PointType::binary_input__, 5, false));
	transaction.add(Transition(Core::PointType::binary_input__, 6, false));
	transaction.add(Transition(Core::PointType::binary_input__, 7, false));
	transaction.commit();

	// third consumer poll
	database.update();
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_0__) ); // can never be true
	REQUIRE( consumer.first->eventsAvailable(EventClass::class_1__) );
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_2__) );
	REQUIRE( !consumer.first->eventsAvailable(EventClass::class_3__) );

	// get class 1 events
	REQUIRE( 1 == consumer.first->getEventCount(EventClass::class_1__) );
	auto events(consumer.first->getEvents(EventClass::class_1__));
	REQUIRE( events.size() == 1 );
	auto event = events[0];
	// pretend we sent it over the wire and got a confirmation back
	events.confirm(1);
	REQUIRE( event.timestamp_.high_ == 1 );
	REQUIRE( event.timestamp_.low_ == 3 );
	REQUIRE( event.type_ == Core::PointType::binary_input__ );
	REQUIRE( event.id_ == 3 );
	REQUIRE( event.tag_ == USER_MAPPING(USER_MAPPING_BI, 72) );
	REQUIRE( event.value_.cpp_type_ == decltype(event.value_.cpp_type_)::bool__ );
	REQUIRE( event.value_.payload_.bool_ == false );
}

TEST_CASE("Class poll", "[consumer]") {
	Database database;

	/* initialization phase */
	auto producer(database.registerProducer(RTIMDB_NOTHROW_ARG_1));
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);
	database.createPoint(producer.first, Core::PointType::binary_input__, false);

	auto consumer(database.registerConsumer(RTIMDB_NOTHROW_ARG_1));
	REQUIRE( consumer.second == Errors::no_error__ );
	consumer.first->mapPoint(USER_MAPPING(USER_MAPPING_BI, 72), Core::PointType::binary_input__, 3);

	// first consumer poll - an integrity poll
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__, EventClass::class_0__) RTIMDB_NOTHROW_ARG));
		REQUIRE( Errors::no_error__ == poll_result.second );
		REQUIRE( poll_result.first.size() == 1 );
	}

	// producer creates transitions -- one point changes (which happens to be the one mapped by the consumer)
	auto transaction(database.beginTransaction(producer.first, Timestamp(1, 2)));
	transaction.add(Transition(Core::PointType::binary_input__, 0, false));
	transaction.add(Transition(Core::PointType::binary_input__, 1, false));
	transaction.add(Transition(Core::PointType::binary_input__, 2, false));
	transaction.add(Transition(Core::PointType::binary_input__, 3, true));
	transaction.add(Transition(Core::PointType::binary_input__, 4, false));
	transaction.add(Transition(Core::PointType::binary_input__, 5, false));
	transaction.add(Transition(Core::PointType::binary_input__, 6, false));
	transaction.add(Transition(Core::PointType::binary_input__, 7, false));
	transaction.commit();
	
	database.update(); // always update the database before polling
	// event poll - class not set yet, so no events
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__) RTIMDB_NOTHROW_ARG));
		REQUIRE( Errors::no_error__ == poll_result.second );
		REQUIRE( poll_result.first.size() == 0 );
	}
	// an integrity poll
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__, EventClass::class_0__) RTIMDB_NOTHROW_ARG));
		REQUIRE( Errors::no_error__ == poll_result.second );
		REQUIRE( poll_result.first.size() == 1 );
	}

	consumer.first->setEventClass(USER_MAPPING(USER_MAPPING_BI, 72), EventClass::class_1__);
	// producer creates transitions -- one point changes (which happens to be the one mapped by the consumer)
	transaction = database.beginTransaction(producer.first, Timestamp(1, 3));
	transaction.add(Transition(Core::PointType::binary_input__, 0, false));
	transaction.add(Transition(Core::PointType::binary_input__, 1, false));
	transaction.add(Transition(Core::PointType::binary_input__, 2, false));
	transaction.add(Transition(Core::PointType::binary_input__, 3, false));
	transaction.add(Transition(Core::PointType::binary_input__, 4, false));
	transaction.add(Transition(Core::PointType::binary_input__, 5, false));
	transaction.add(Transition(Core::PointType::binary_input__, 6, false));
	transaction.add(Transition(Core::PointType::binary_input__, 7, false));
	transaction.commit();

	database.update(); // always update the database before polling
	// event poll
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__) RTIMDB_NOTHROW_ARG));
		REQUIRE(Errors::no_error__ == poll_result.second );
		REQUIRE(poll_result.first.size() == 1 );
	}
	// an integrity poll
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__, EventClass::class_0__) RTIMDB_NOTHROW_ARG));
		REQUIRE( Errors::no_error__ == poll_result.second );
		REQUIRE( poll_result.first.size() == 2 );
	}

	// add another event without confirming - a new poll should get the new events
	transaction = database.beginTransaction(producer.first, Timestamp(1, 4));
	transaction.add(Transition(Core::PointType::binary_input__, 0, false));
	transaction.add(Transition(Core::PointType::binary_input__, 1, false));
	transaction.add(Transition(Core::PointType::binary_input__, 2, false));
	transaction.add(Transition(Core::PointType::binary_input__, 3, true));
	transaction.add(Transition(Core::PointType::binary_input__, 4, false));
	transaction.add(Transition(Core::PointType::binary_input__, 5, false));
	transaction.add(Transition(Core::PointType::binary_input__, 6, false));
	transaction.add(Transition(Core::PointType::binary_input__, 7, false));
	transaction.commit();

	database.update(); // always update the database before polling
	// event poll
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__) RTIMDB_NOTHROW_ARG));
		REQUIRE( Errors::no_error__ == poll_result.second );
		REQUIRE( poll_result.first.size() == 2 );
		// read the events, but don't confirm them
		auto event1(poll_result.first[0]);
		auto event2(poll_result.first[1]);
		REQUIRE( event1.type() == 0 );
		REQUIRE( event1.get< Event >().id_ == 3 );
		REQUIRE( event1.get< Event >().timestamp_.high_ == 1 );
		REQUIRE( event1.get< Event >().timestamp_.low_ == 3 );
		REQUIRE( event1.get< Event >().type_ == Core::PointType::binary_input__ );
		REQUIRE( !event1.get< Event >().value_.payload_.bool_ );
		REQUIRE( event2.type() == 0 );
		REQUIRE( event2.get< Event >().id_ == 3 );
		REQUIRE( event2.get< Event >().timestamp_.high_ == 1 );
		REQUIRE( event2.get< Event >().timestamp_.low_ == 4 );
		REQUIRE( event2.get< Event >().type_ == Core::PointType::binary_input__ );
		REQUIRE( event2.get< Event >().value_.payload_.bool_ );
	}
	// an integrity poll
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__, EventClass::class_0__) RTIMDB_NOTHROW_ARG));
		REQUIRE( Errors::no_error__ == poll_result.second );
		REQUIRE( poll_result.first.size() == 3 );
		auto event1(poll_result.first[0]);
		auto event2(poll_result.first[1]);
		auto static3(poll_result.first[2]);
		REQUIRE( event1.type() == 0 );
		REQUIRE( event1.get< Event >().id_ == 3 );
		REQUIRE( event1.get< Event >().timestamp_.high_ == 1 );
		REQUIRE( event1.get< Event >().timestamp_.low_ == 3 );
		REQUIRE( event1.get< Event >().type_ == Core::PointType::binary_input__ );
		REQUIRE( !event1.get< Event >().value_.payload_.bool_ );
		REQUIRE( event2.type() == 0 );
		REQUIRE( event2.get< Event >().id_ == 3 );
		REQUIRE( event2.get< Event >().timestamp_.high_ == 1 );
		REQUIRE( event2.get< Event >().timestamp_.low_ == 4 );
		REQUIRE( event2.get< Event >().type_ == Core::PointType::binary_input__ );
		REQUIRE( event2.get< Event >().value_.payload_.bool_ );
		REQUIRE( static3.type() == 1 );
		REQUIRE( static3.get< Core::Point >().value_.payload_.bool_ );
		poll_result.first.confirm();
	}
}

