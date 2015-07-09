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
	auto index(database.insert(Point(PointType::binary_input__, false)) DOT_FIRST);

	auto transaction(database.freeze() DOT_FIRST);
	database.update(index, Point(PointType::binary_input__, true));
	auto frozen_value(database.read(transaction, PointType::binary_input__, index) DOT_FIRST);
	assert(PointType::binary_input__ == frozen_value DEREF type_);
	assert(false == frozen_value DEREF payload_.binary_);
	frozen_value = database.read(PointType::binary_input__, index) DOT_FIRST;
	assert(PointType::binary_input__ == frozen_value DEREF type_);
	assert(true == frozen_value DEREF payload_.binary_);
}