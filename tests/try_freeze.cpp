#include "../database.hpp"
using namespace Vlinder::RTIMDB;

#ifdef RTIMDB_ALLOW_EXCEPTIONS
#define FIRST_DEREF
#define DOT_FIRST
#define DEREF .
#else
#define FIRST_DEREF first->
#define DOT_FIRST	.first
#define DEREF ->
#endif

int main()
{
	Database database;
	auto index(database.insert(Point(PointType::binary_input__, false)));

	auto transaction(database.freeze() DOT_FIRST);
	database.update(PointType::binary_input__, index, true);
	auto frozen_value(database.read(transaction, PointType::binary_input__, index RTIMDB_NOTHROW_ARG) DOT_FIRST);
	assert(PointType::binary_input__ == frozen_value DEREF type_);
	assert(false == frozen_value DEREF payload_.binary_);
	frozen_value = database.read(PointType::binary_input__, index) DOT_FIRST;
	assert(PointType::binary_input__ == frozen_value DEREF type_);
	assert(true == frozen_value DEREF payload_.binary_);
}