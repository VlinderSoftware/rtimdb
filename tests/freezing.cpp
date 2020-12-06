#include "../core/datastore.hpp"
#include <vector>
#include "catch.hpp"

using namespace std;
using namespace Vlinder::RTIMDB;
using namespace Vlinder::RTIMDB::Core;

TEST_CASE("Too many freezes", "[freezing]") {
	DataStore data_store;
	vector< decltype(data_store.startTransaction()) > transactions;

	bool caught(false);
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	try
	{
#endif
		while (!caught)
		{
			transactions.push_back(data_store.startTransaction());
#ifndef RTIMDB_ALLOW_EXCEPTIONS
			caught = transactions.back().second == Errors::cannot_freeze__;
#endif
		}
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	}
	catch (CannotFreeze const&)
	{
		caught = true;
	}
#endif

	REQUIRE( caught );
}
