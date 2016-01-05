#include "../database.hpp"

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
	transaction.add(Transition(3, PointType::binary_input__,  true));
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
	transaction = database.beginTransaction(producer.first, Timestamp(1, 2));
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

	return 0;
}

int main()
{
	return 0
		|| registerConsumer()
		|| createMapping()
		|| setEventClass()
		|| updateDatabase()
		;
}