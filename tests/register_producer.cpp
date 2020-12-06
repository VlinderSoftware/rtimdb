#include "../database.hpp"
#include "catch.hpp"

using namespace Vlinder::RTIMDB;

TEST_CASE("Try to register a producer", "[producer]") {
	Database database;

	auto registration(database.registerProducer());
}