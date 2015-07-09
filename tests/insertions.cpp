#include "../core/database.hpp"

using namespace Vlinder::RTIMDB;
using namespace Vlinder::RTIMDB::Core;

#ifdef RTIMDB_ALLOW_EXCEPTIONS
template < typename T >
void fillToTheBrim(Database &database, PointType point_type, T value)
{
	for (int i(0); i < RTIMDB_POINT_COUNT; ++i)
		database.insert(Point(point_type, value));
}
#else
template < typename T >
Errors fillToTheBrim(Database &database, PointType point_type, T value)
{
	for (int i(0); i < RTIMDB_POINT_COUNT; ++i)
	{
		auto insert_result(database.insert(Point(point_type, value)));
		if (Errors::no_error__ != insert_result.second) return insert_result.second;
	}
	return Errors::no_error__;
}
#endif

template < PointType point_type__, typename T >
int tryInsertTooMany()
{
	T const value__(static_cast< T >(0));
	Database database;
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	fillToTheBrim(database, point_type__, value__);
	bool caught(false);
	try
	{
		database.insert(Point(point_type__, value__));
	}
	catch (DatabaseFull const &)
	{
		caught = true;
	}
	if (!caught) return 1;
#else
	if (Errors::no_error__ != fillToTheBrim(database, point_type__, value__)) return 1;
	auto insert_result(database.insert(Point(point_type__, value__)));
	if (Errors::database_full__ != insert_result.second) return 1;
#endif

	return 0;
}

template < PointType point_type__, typename T >
int tryInsertUpdateInsert()
{
	T const value__(static_cast< T >(0));
	Database database;
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	database.insert(Point(point_type__, value__));
	database.update(0, Point(point_type__, static_cast< T >(value__ + 1)));
	database.insert(Point(point_type__, value__));
#else
	auto insert_result(database.insert(Point(point_type__, value__)));
	if (Errors::no_error__ != insert_result.second) return 1;
	auto update_result(database.update(0, Point(point_type__, static_cast< T >(value__ + 1))));
	if (Errors::no_error__ != update_result) return 1;
	auto insert_result2(database.insert(Point(point_type__, value__)));
	if (Errors::no_error__ != insert_result2.second) return 1;
#endif

	return 0;
}
template < PointType point_type__, typename T >
int tryInsertReadInsert()
{
	T const value__(static_cast< T >(0));
	Database database;
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	database.insert(Point(point_type__, value__));
	if (value__ != getValue< point_type__ >(database.read(point_type__, 0))) return 1;
	database.insert(Point(point_type__, value__));
#else
	auto insert_result(database.insert(Point(point_type__, value__)));
	if (Errors::no_error__ != insert_result.second) return 1;
	auto read_result(database.read(point_type__, 0));
	if (Errors::no_error__ != read_result.second) return 1;
	if (value__ != getValue< point_type__ >(*read_result.first)) return 1;
	auto insert_result2(database.insert(Point(point_type__, value__)));
	if (Errors::no_error__ != insert_result2.second) return 1;
#endif

	return 0;
}

int main()
{
	return 0
		|| tryInsertTooMany< PointType::binary_input__, bool >()
		|| tryInsertTooMany< PointType::binary_output__, bool >()
		|| tryInsertTooMany< PointType::counter__, uint32_t >()
		|| tryInsertTooMany< PointType::dataset__, Dataset* >()
		|| tryInsertTooMany< PointType::octet_string__, String* >()
		|| tryInsertTooMany< PointType::analog_input__, double >()
		|| tryInsertTooMany< PointType::analog_output__, double >()
		|| tryInsertUpdateInsert< PointType::binary_input__, bool >()
		|| tryInsertUpdateInsert< PointType::binary_output__, bool >()
		|| tryInsertUpdateInsert< PointType::counter__, uint32_t >()
		// can't test this for datasets and strings
		//|| tryInsertUpdateInsert< PointType::dataset__, Dataset* >()
		//|| tryInsertUpdateInsert< PointType::octet_string__, String* >()
		|| tryInsertUpdateInsert< PointType::analog_input__, double >()
		|| tryInsertUpdateInsert< PointType::analog_output__, double >()
		|| tryInsertReadInsert< PointType::binary_input__, bool >()
		|| tryInsertReadInsert< PointType::binary_output__, bool >()
		|| tryInsertReadInsert< PointType::counter__, uint32_t >()
		|| tryInsertReadInsert< PointType::dataset__, Dataset* >()
		|| tryInsertReadInsert< PointType::octet_string__, String* >()
		|| tryInsertReadInsert< PointType::analog_input__, double >()
		|| tryInsertReadInsert< PointType::analog_output__, double >()
		;
}

