#include "../core/datastore.hpp"
#include "catch.hpp"

using namespace Vlinder::RTIMDB;
using namespace Vlinder::RTIMDB::Core;

#ifdef RTIMDB_ALLOW_EXCEPTIONS
template < typename T >
void fillToTheBrim(DataStore &data_store, PointType point_type, T value)
{
	for (int i(0); i < RTIMDB_POINT_COUNT; ++i)
	{
		data_store.insert(point_type, PointValue(value), Flags(), Timestamp());
	}
}
#else
template < typename T >
Errors fillToTheBrim(DataStore &data_store, PointType point_type, T value)
{
	for (int i(0); i < RTIMDB_POINT_COUNT; ++i)
	{
		auto insert_result(data_store.insert(point_type, PointValue(value), Flags(), Timestamp()));
		if (Errors::no_error__ != insert_result.second) return insert_result.second;
	}
	return Errors::no_error__;
}
#endif

template < PointType point_type__, typename T >
int tryInsertTooMany()
{
	T const value__(static_cast< T >(0));
	DataStore data_store;
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	fillToTheBrim(data_store, point_type__, value__);
	bool caught(false);
	try
	{
		data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp());
	}
	catch (DatabaseFull const &)
	{
		caught = true;
	}
	if (!caught) return 1;
#else
	if (Errors::no_error__ != fillToTheBrim(data_store, point_type__, value__)) return 1;
	auto insert_result(data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp()));
	if (Errors::database_full__ != insert_result.second) return 1;
#endif

	return 0;
}

template < PointType point_type__, typename T >
int tryInsertUpdateInsert()
{
	T const value__(static_cast< T >(0));
	DataStore data_store;
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp());
	data_store.update(point_type__, 0, PointValue(static_cast< T >(value__ + 1)), Flags(Flag::online__), Timestamp());
	data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp());
#else
	auto insert_result(data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp()));
	if (Errors::no_error__ != insert_result.second) return 1;
	auto update_result(data_store.update(point_type__, 0, PointValue(static_cast< T >(value__ + 1)), Flags(Flag::online__), Timestamp()));
	if (Errors::no_error__ != update_result) return 1;
	auto insert_result2(data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp()));
	if (Errors::no_error__ != insert_result2.second) return 1;
#endif

	return 0;
}
template < PointType point_type__, typename T >
int tryInsertReadInsert()
{
	T const value__(static_cast< T >(0));
	DataStore data_store;
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp());
	if (value__ != getValue< T >(data_store.read(point_type__, 0).value_)) return 1;
	data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp());
#else
	auto insert_result(data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp()));
	if (Errors::no_error__ != insert_result.second) return 1;
	auto read_result(data_store.read(point_type__, 0));
	if (Errors::no_error__ != read_result.second) return 1;
	if (value__ != getValue< T >((*read_result.first).value_)) return 1;
	auto insert_result2(data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp()));
	if (Errors::no_error__ != insert_result2.second) return 1;
#endif

	return 0;
}
template < PointType point_type__, typename T >
int tryInsertFreezeInsert()
{
	T const value__(static_cast< T >(0));
	DataStore data_store;
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp());
	auto freeze_result(data_store.startTransaction());
	data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp());
#else
	auto insert_result(data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp()));
	if (Errors::no_error__ != insert_result.second) return 1;
	auto freeze_result(data_store.startTransaction());
	if (Errors::no_error__ != freeze_result.second) return 1;
	auto insert_result2(data_store.insert(point_type__, PointValue(value__), Flags(), Timestamp()));
	if (Errors::no_error__ != insert_result2.second) return 1;
#endif

	return 0;
}

TEST_CASE("insertions", "insertions") {
	REQUIRE( 0 == tryInsertTooMany< PointType::binary_input__, bool >() );
	REQUIRE( 0 == tryInsertTooMany< PointType::binary_output__, bool >() );
	REQUIRE( 0 == tryInsertTooMany< PointType::counter__, uint32_t >() );
	REQUIRE( 0 == tryInsertTooMany< PointType::analog_input__, double >() );
	REQUIRE( 0 == tryInsertTooMany< PointType::analog_output__, double >() );
	REQUIRE( 0 == tryInsertUpdateInsert< PointType::binary_input__, bool >() );
	REQUIRE( 0 == tryInsertUpdateInsert< PointType::binary_output__, bool >() );
	REQUIRE( 0 == tryInsertUpdateInsert< PointType::counter__, uint32_t >() );
	REQUIRE( 0 == tryInsertUpdateInsert< PointType::analog_input__, double >() );
	REQUIRE( 0 == tryInsertUpdateInsert< PointType::analog_output__, double >() );
	REQUIRE( 0 == tryInsertReadInsert< PointType::binary_input__, bool >() );
	REQUIRE( 0 == tryInsertReadInsert< PointType::binary_output__, bool >() );
	REQUIRE( 0 == tryInsertReadInsert< PointType::counter__, uint32_t >() );
	REQUIRE( 0 == tryInsertReadInsert< PointType::analog_input__, double >() );
	REQUIRE( 0 == tryInsertReadInsert< PointType::analog_output__, double >() );
	REQUIRE( 0 == tryInsertFreezeInsert< PointType::binary_input__, bool >() );
	REQUIRE( 0 == tryInsertFreezeInsert< PointType::binary_output__, bool >() );
	REQUIRE( 0 == tryInsertFreezeInsert< PointType::counter__, uint32_t >() );
	REQUIRE( 0 == tryInsertFreezeInsert< PointType::analog_input__, double >() );
	REQUIRE( 0 == tryInsertFreezeInsert< PointType::analog_output__, double >() );
}

