/* Copyright 2014  Vlinder Software
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */
 #include "../core/datastore.hpp"
 #include "catch.hpp"

using namespace Vlinder::RTIMDB;
using namespace Vlinder::RTIMDB::Core;

TEST_CASE("Try to iterate on an empty database", "[empty database]") {
	DataStore db;
	REQUIRE( !(db.begin() != db.end()) );
	REQUIRE( !(db.end() != db.begin()) );
	REQUIRE( (db.begin() == db.end()) );
	REQUIRE( (db.end() == db.begin()) );
}

TEST_CASE("Try to read from an empty database", "[empty database]") {
	DataStore db;
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	bool caught(false);
	try
	{
		db.read(db.startROTransaction(), PointType::binary_input__, 0);
	}
	catch (UnknownPoint const &)
	{
		caught = true;
	}
	REQUIRE( caught );
#else
	auto transaction(db.startROTransaction());
	REQUIRE( transaction.second == Errors::no_error__ );
	auto read_result(db.read(transaction.first, PointType::binary_input__, 0));
	REQUIRE( read_result.second == Errors::unknown_point__ );
#endif
}
