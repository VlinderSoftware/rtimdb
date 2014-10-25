#include "../database.hpp"

using namespace Vlinder::RTIMDB;

#ifdef RTIMDB_ALLOW_EXCEPTIONS
#define FIRST_DEREF
#define DOT_FIRST
#else
#define FIRST_DEREF first->
#define DOT_FIRST	.first
#endif

int main()
{
	Database database;
	unsigned int exp_ai_index(0);
	unsigned int exp_bi_index(0);

	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;

	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;
	assert(exp_bi_index == database.insert(Point(PointType::binary_input__, false))); ++exp_bi_index;

	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;

	auto first_transaction_(database.freeze());
	auto first_transaction(first_transaction_ DOT_FIRST);
#ifndef RTIMDB_ALLOW_EXCEPTIONS
	assert(Errors::no_error__ == first_transaction_.second);
#endif
	assert(Errors::no_error__ == database.update(PointType::analog_input__, 3, 13.24 RTIMDB_NOTHROW_ARG));
	assert(Errors::no_error__ == database.update(PointType::analog_input__, 5, 26.48 RTIMDB_NOTHROW_ARG));
	assert(Errors::no_error__ == database.update(PointType::analog_input__, 7, 52.96 RTIMDB_NOTHROW_ARG));
	assert(Errors::no_error__ == database.update(PointType::analog_input__, 13, 104.192 RTIMDB_NOTHROW_ARG));
	auto second_transaction_(database.freeze());
	auto second_transaction(second_transaction_ DOT_FIRST);
#ifndef RTIMDB_ALLOW_EXCEPTIONS
	assert(Errors::no_error__ == second_transaction_.second);
#endif
	assert(Errors::no_error__ == database.update(PointType::binary_input__, 3, true RTIMDB_NOTHROW_ARG));
	assert(Errors::no_error__ == database.update(PointType::binary_input__, 5, true RTIMDB_NOTHROW_ARG));
	assert(Errors::no_error__ == database.update(PointType::binary_input__, 7, true RTIMDB_NOTHROW_ARG));
	assert(Errors::no_error__ == database.update(PointType::binary_input__, 13, true RTIMDB_NOTHROW_ARG));
	assert(Errors::no_error__ == database.update(PointType::binary_input__, 17, true RTIMDB_NOTHROW_ARG));

	// a read using the first transaction should get the old analogs and the old binaries
#ifdef RTIMDB_ALLOW_EXCEPTIONS
#define CHECK(TRANSACTION, TYPE, INDEX)													\
	{																					\
		auto read_result_1(database.read(TRANSACTION, TYPE, INDEX));					\
		assert(PointType::binary_input__ == read_result_1.FIRST_DEREF type_);			\
		assert(false == read_result_1.FIRST_DEREF payload_.binary_);					\
	}
#else
#define CHECK(EXP, TRANSACTION, TYPE, INDEX, PAYLOAD)									\
	{																					\
		auto read_result_1(database.read(TRANSACTION, TYPE, INDEX));					\
		assert(Errors::no_error__ == read_result_1.second);								\
		assert(TYPE == read_result_1.FIRST_DEREF type_);								\
		assert(EXP == read_result_1.FIRST_DEREF payload_.PAYLOAD);						\
	}
#endif

	CHECK(false, first_transaction, PointType::binary_input__,  3, binary_);
	CHECK(false, first_transaction, PointType::binary_input__,  5, binary_);
	CHECK(false, first_transaction, PointType::binary_input__,  7, binary_);
	CHECK(false, first_transaction, PointType::binary_input__, 13, binary_);
	CHECK(false, first_transaction, PointType::binary_input__, 17, binary_);

	CHECK(0.0, first_transaction, PointType::analog_input__,  3, analog_);
	CHECK(0.0, first_transaction, PointType::analog_input__,  5, analog_);
	CHECK(0.0, first_transaction, PointType::analog_input__,  7, analog_);
	CHECK(0.0, first_transaction, PointType::analog_input__, 13, analog_);
	CHECK(0.0, first_transaction, PointType::analog_input__, 17, analog_);

	// a read using the first transaction should get the new analogs and the old binaries
	CHECK(false, second_transaction, PointType::binary_input__,  3, binary_);
	CHECK(false, second_transaction, PointType::binary_input__,  5, binary_);
	CHECK(false, second_transaction, PointType::binary_input__,  7, binary_);
	CHECK(false, second_transaction, PointType::binary_input__, 13, binary_);
	CHECK(false, second_transaction, PointType::binary_input__, 17, binary_);

	CHECK(  13.24, second_transaction, PointType::analog_input__,  3, analog_);
	CHECK(  26.48, second_transaction, PointType::analog_input__,  5, analog_);
	CHECK(  52.96, second_transaction, PointType::analog_input__,  7, analog_);
	CHECK(104.192, second_transaction, PointType::analog_input__, 13, analog_);
}
