#include "../database.hpp"
#include "exceptions/contract.hpp"

using namespace Vlinder::RTIMDB;
using namespace Vlinder::RTIMDB::Details;

#define USER_MAPPING_BI 1
#define USER_MAPPING_AI 2
#define USER_MAPPING_CI 3
#define USER_MAPPING_BO 4
#define USER_MAPPING_AO 5

#define USER_MAPPING(T, ID) (((((uintptr_t)(T)) & 0x0000000FU) << 28) | (((uintptr_t)(ID)) & 0x0FFFFFFFU))

int registerConsumer()
{
	Database database;

	auto consumer(database.registerConsumer(RTIMDB_NOTHROW_ARG_1));
	assert(consumer.second == Errors::no_error__);
	return 0;
}

int createMapping()
{
	Database database;

	auto consumer(database.registerConsumer(RTIMDB_NOTHROW_ARG_1));
	assert(consumer.second == Errors::no_error__);
	consumer.first->mapPoint(USER_MAPPING(USER_MAPPING_BI, 1), PointType::binary_input__, 3);

	return 0;
}

int setEventClass()
{
	Database database;

	auto consumer(database.registerConsumer(RTIMDB_NOTHROW_ARG_1));
	assert(consumer.second == Errors::no_error__);
	consumer.first->mapPoint(USER_MAPPING(USER_MAPPING_BI, 1), PointType::binary_input__, 3);
	consumer.first->setEventClass(USER_MAPPING(USER_MAPPING_BI, 1), EventClass::class_1__);
	return 0;
}

int pollEventPresence()
{
	Database database;

	auto consumer(database.registerConsumer(RTIMDB_NOTHROW_ARG_1));
	assert(consumer.second == Errors::no_error__);
	consumer.first->mapPoint(USER_MAPPING(USER_MAPPING_BI, 1), PointType::binary_input__, 3);
	assert(!consumer.first->eventsAvailable(EventClass::class_0__)); // can never be true
	assert(!consumer.first->eventsAvailable(EventClass::class_1__));
	assert(!consumer.first->eventsAvailable(EventClass::class_2__));
	assert(!consumer.first->eventsAvailable(EventClass::class_3__));
	return 0;
}

int updateDatabase()
{
	Database database;

	/* initialization phase */
	auto producer(database.registerProducer(RTIMDB_NOTHROW_ARG_1));
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);

	auto consumer(database.registerConsumer(RTIMDB_NOTHROW_ARG_1));
	assert(consumer.second == Errors::no_error__);
	consumer.first->mapPoint(USER_MAPPING(USER_MAPPING_BI, 72), PointType::binary_input__, 3);

	// first consumer poll
	database.update();
	assert(!consumer.first->eventsAvailable(EventClass::class_0__)); // can never be true
	assert(!consumer.first->eventsAvailable(EventClass::class_1__));
	assert(!consumer.first->eventsAvailable(EventClass::class_2__));
	assert(!consumer.first->eventsAvailable(EventClass::class_3__));

	// producer creates transitions -- one point changes (which happens to be the one mapped by the consumer)
	auto transaction(database.beginTransaction(producer.first, Timestamp(1, 2)));
	transaction.add(Transition(0, PointType::binary_input__, false));
	transaction.add(Transition(1, PointType::binary_input__, false));
	transaction.add(Transition(2, PointType::binary_input__, false));
	transaction.add(Transition(3, PointType::binary_input__, true));
	transaction.add(Transition(4, PointType::binary_input__, false));
	transaction.add(Transition(5, PointType::binary_input__, false));
	transaction.add(Transition(6, PointType::binary_input__, false));
	transaction.add(Transition(7, PointType::binary_input__, false));
	transaction.commit();

	// second consumer poll
	database.update();
	assert(!consumer.first->eventsAvailable(EventClass::class_0__)); // can never be true
	assert(!consumer.first->eventsAvailable(EventClass::class_1__));
	assert(!consumer.first->eventsAvailable(EventClass::class_2__));
	assert(!consumer.first->eventsAvailable(EventClass::class_3__));

	consumer.first->setEventClass(USER_MAPPING(USER_MAPPING_BI, 72), EventClass::class_1__);
	// producer creates transitions -- one point changes (which happens to be the one mapped by the consumer)
	transaction = database.beginTransaction(producer.first, Timestamp(1, 3));
	transaction.add(Transition(0, PointType::binary_input__, false));
	transaction.add(Transition(1, PointType::binary_input__, false));
	transaction.add(Transition(2, PointType::binary_input__, false));
	transaction.add(Transition(3, PointType::binary_input__, false));
	transaction.add(Transition(4, PointType::binary_input__, false));
	transaction.add(Transition(5, PointType::binary_input__, false));
	transaction.add(Transition(6, PointType::binary_input__, false));
	transaction.add(Transition(7, PointType::binary_input__, false));
	transaction.commit();

	// third consumer poll
	database.update();
	assert(!consumer.first->eventsAvailable(EventClass::class_0__)); // can never be true
	assert(consumer.first->eventsAvailable(EventClass::class_1__));
	assert(!consumer.first->eventsAvailable(EventClass::class_2__));
	assert(!consumer.first->eventsAvailable(EventClass::class_3__));

	// get class 1 events
	assert(1 == consumer.first->getEventCount(EventClass::class_1__));
	auto events(consumer.first->getEvents(EventClass::class_1__));
	assert(events.size() == 1);
	auto event = events[0];
	// pretend we sent it over the wire and got a confirmation back
	events.confirm(1);
	assert(event.timestamp_.high_ == 1);
	assert(event.timestamp_.low_ == 3);
	assert(event.value_.type_ == PointType::binary_input__);
	assert(event.id_ == 3);
	assert(event.tag_ == USER_MAPPING(USER_MAPPING_BI, 72));
	assert(event.value_.cpp_type_ == decltype(event.value_.cpp_type_)::bool__);
	assert(event.value_.payload_.bool_ == false);

	return 0;
}

int classPoll()
{
	Database database;

	/* initialization phase */
	auto producer(database.registerProducer(RTIMDB_NOTHROW_ARG_1));
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);
	database.createPoint(producer.first, PointType::binary_input__, false);

	auto consumer(database.registerConsumer(RTIMDB_NOTHROW_ARG_1));
	assert(consumer.second == Errors::no_error__);
	consumer.first->mapPoint(USER_MAPPING(USER_MAPPING_BI, 72), PointType::binary_input__, 3);

	// first consumer poll - an integrity poll
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__, EventClass::class_0__) RTIMDB_NOTHROW_ARG));
		assert(Errors::no_error__ == poll_result.second);
		assert(poll_result.first.size() == 1);
	}

	// producer creates transitions -- one point changes (which happens to be the one mapped by the consumer)
	auto transaction(database.beginTransaction(producer.first, Timestamp(1, 2)));
	transaction.add(Transition(0, PointType::binary_input__, false));
	transaction.add(Transition(1, PointType::binary_input__, false));
	transaction.add(Transition(2, PointType::binary_input__, false));
	transaction.add(Transition(3, PointType::binary_input__, true));
	transaction.add(Transition(4, PointType::binary_input__, false));
	transaction.add(Transition(5, PointType::binary_input__, false));
	transaction.add(Transition(6, PointType::binary_input__, false));
	transaction.add(Transition(7, PointType::binary_input__, false));
	transaction.commit();
	
	database.update(); // always update the database before polling
	// event poll - class not set yet, so no events
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__) RTIMDB_NOTHROW_ARG));
		assert(Errors::no_error__ == poll_result.second);
		assert(poll_result.first.size() == 0);
	}
	// an integrity poll
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__, EventClass::class_0__) RTIMDB_NOTHROW_ARG));
		assert(Errors::no_error__ == poll_result.second);
		assert(poll_result.first.size() == 1);
	}

	consumer.first->setEventClass(USER_MAPPING(USER_MAPPING_BI, 72), EventClass::class_1__);
	// producer creates transitions -- one point changes (which happens to be the one mapped by the consumer)
	transaction = database.beginTransaction(producer.first, Timestamp(1, 3));
	transaction.add(Transition(0, PointType::binary_input__, false));
	transaction.add(Transition(1, PointType::binary_input__, false));
	transaction.add(Transition(2, PointType::binary_input__, false));
	transaction.add(Transition(3, PointType::binary_input__, false));
	transaction.add(Transition(4, PointType::binary_input__, false));
	transaction.add(Transition(5, PointType::binary_input__, false));
	transaction.add(Transition(6, PointType::binary_input__, false));
	transaction.add(Transition(7, PointType::binary_input__, false));
	transaction.commit();

	database.update(); // always update the database before polling
	// event poll
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__) RTIMDB_NOTHROW_ARG));
		assert(Errors::no_error__ == poll_result.second);
		assert(poll_result.first.size() == 1);
	}
	// an integrity poll
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__, EventClass::class_0__) RTIMDB_NOTHROW_ARG));
		assert(Errors::no_error__ == poll_result.second);
		assert(poll_result.first.size() == 2);
	}

	// add another event without confirming - a new poll should get the new events
	transaction = database.beginTransaction(producer.first, Timestamp(1, 4));
	transaction.add(Transition(0, PointType::binary_input__, false));
	transaction.add(Transition(1, PointType::binary_input__, false));
	transaction.add(Transition(2, PointType::binary_input__, false));
	transaction.add(Transition(3, PointType::binary_input__, true));
	transaction.add(Transition(4, PointType::binary_input__, false));
	transaction.add(Transition(5, PointType::binary_input__, false));
	transaction.add(Transition(6, PointType::binary_input__, false));
	transaction.add(Transition(7, PointType::binary_input__, false));
	transaction.commit();

	database.update(); // always update the database before polling
	// event poll
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__) RTIMDB_NOTHROW_ARG));
		assert(Errors::no_error__ == poll_result.second);
		assert(poll_result.first.size() == 2);
		// read the events, but don't confirm them
		auto event1(poll_result.first[0]);
		auto event2(poll_result.first[1]);
		assert(event1.type() == 0);
		assert(event1.get< Event >().id_ == 3);
		assert(event1.get< Event >().timestamp_.high_ == 1);
		assert(event1.get< Event >().timestamp_.low_ == 3);
		assert(event1.get< Event >().value_.type_ == PointType::binary_input__);
		assert(!event1.get< Event >().value_.payload_.bool_);
		assert(event2.type() == 0);
		assert(event2.get< Event >().id_ == 3);
		assert(event2.get< Event >().timestamp_.high_ == 1);
		assert(event2.get< Event >().timestamp_.low_ == 4);
		assert(event2.get< Event >().value_.type_ == PointType::binary_input__);
		assert(event2.get< Event >().value_.payload_.bool_);
	}
	// an integrity poll
	{
		auto poll_result(consumer.first->poll(PollDescriptor(EventClass::class_1__, EventClass::class_2__, EventClass::class_3__, EventClass::class_0__) RTIMDB_NOTHROW_ARG));
		assert(Errors::no_error__ == poll_result.second);
		assert(poll_result.first.size() == 3);
		auto event1(poll_result.first[0]);
		auto event2(poll_result.first[1]);
		auto static3(poll_result.first[2]);
		assert(event1.type() == 0);
		assert(event1.get< Event >().id_ == 3);
		assert(event1.get< Event >().timestamp_.high_ == 1);
		assert(event1.get< Event >().timestamp_.low_ == 3);
		assert(event1.get< Event >().value_.type_ == PointType::binary_input__);
		assert(!event1.get< Event >().value_.payload_.bool_);
		assert(event2.type() == 0);
		assert(event2.get< Event >().id_ == 3);
		assert(event2.get< Event >().timestamp_.high_ == 1);
		assert(event2.get< Event >().timestamp_.low_ == 4);
		assert(event2.get< Event >().value_.type_ == PointType::binary_input__);
		assert(event2.get< Event >().value_.payload_.bool_);
		assert(static3.type() == 1);
		assert(static3.get< Core::Point >().type_ == PointType::binary_input__);
		assert(static3.get< Core::Point >().payload_.bool_);
		poll_result.first.confirm();
	}


	return 0;
}

int main()
{
	return 0
		|| registerConsumer()
		|| createMapping()
		|| setEventClass()
		|| updateDatabase()
		|| classPoll()
		;
}