#include "../database.hpp"

using namespace Vlinder::RTIMDB;

int main()
{
	Database database;
	database.insert(Point(PointType::binary_output__, true));
	database.insert(Point(PointType::analog_input__, 25.35));
	database.insert(Point(PointType::counter__, static_cast< uint32_t >(463)));
	database.insert(Point(PointType::analog_output__, 58.24));
	database.insert(Point(PointType::binary_input__, true));
	database.insert(Point(PointType::binary_input__, true));
	database.insert(Point(PointType::binary_output__, true));
	database.insert(Point(PointType::analog_input__, 3.623632346));
	database.insert(Point(PointType::counter__, static_cast< uint32_t >(2587546342)));
	database.insert(Point(PointType::analog_output__, 72.435));
	database.insert(Point(PointType::binary_input__, true));

	database.update(PointType::binary_input__, 1, false);
}
