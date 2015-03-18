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

