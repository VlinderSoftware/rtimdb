#include "../core/database.hpp"
#include <vector>

using namespace std;
using namespace Vlinder::RTIMDB;
using namespace Vlinder::RTIMDB::Core;

int tryTooManyFreezes()
{
	Database database;
	vector< decltype(database.freeze()) > transactions;

	bool caught(false);
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	try
	{
#endif
		while (!caught)
		{
			transactions.push_back(database.freeze());
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

	return caught ? 0 : 1;
}

int main()
{
	return 0
		|| tryTooManyFreezes()
		;
}
