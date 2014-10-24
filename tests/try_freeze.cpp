#include "../database.hpp"
using namespace Vlinder::RTIMDB;

int main()
{
	Database database;
	auto index(database.insert(Point(PointType::binary_input__, false)));

	auto transaction(database.freeze());
	database.update(PointType::binary_input__, index, true);
	auto frozen_value(database.read(transaction, PointType::binary_input__, index));
	assert(PointType::binary_input__ == frozen_value.type_);
	assert(false == frozen_value.payload_.binary_);
	frozen_value = database.read(PointType::binary_input__, index);
	assert(PointType::binary_input__ == frozen_value.type_);
	assert(true == frozen_value.payload_.binary_);
}