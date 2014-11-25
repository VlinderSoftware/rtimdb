#include "../database.hpp"

using namespace Vlinder::RTIMDB;

int tryIterateEmptyDB()
{
	Database db;
	if (db.begin() != db.end()) return 1;
	if (db.end() != db.begin()) return 1;
	if (!(db.begin() == db.end())) return 1;
	if (!(db.end() == db.begin())) return 1;
	return 0;
}

int tryReadFromEmptyDB()
{
	Database db;
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	bool caught(false);
	try
	{
		db.read(db.freeze(), PointType::binary_input__, 0);
	}
	catch (UnknownPoint const &)
	{
		caught = true;
	}
	return caught ? 0 : 1;
#else
	auto transaction(db.freeze());
	if (transaction.second != Errors::no_error__) return 1;
	auto read_result(db.read(transaction.first, PointType::binary_input__, 0));
	return read_result.second != Errors::unknown_point__ ? 0 : 1;
#endif
}

int main()
{
	return 0
		|| tryIterateEmptyDB()
		|| tryReadFromEmptyDB()
		;
}

