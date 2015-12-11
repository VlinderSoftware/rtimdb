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

using namespace Vlinder;
using namespace Vlinder::RTIMDB;
using namespace Vlinder::RTIMDB::Core;
using namespace std;

#ifdef RTIMDB_ALLOW_EXCEPTIONS
#define DOT_FIRST
#define FIRST
#define FIRST_GET
#else
#define FIRST .first
#define FIRST_GET .first.get()
#define DOT_FIRST	.first
#endif

int main()
{
	DataStore database;
	unsigned int exp_ai_index(0);
	unsigned int exp_bo_index(0);

	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0)) DOT_FIRST); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0)) DOT_FIRST); ++exp_ai_index;

	assert(exp_bo_index == database.insert(Point(PointType::binary_output__, false)) DOT_FIRST); ++exp_bo_index;
	assert(exp_bo_index == database.insert(Point(PointType::binary_output__, true)) DOT_FIRST); ++exp_bo_index;

	assert(4 == distance(database.begin(), database.end()));

	database.registerObserver(
		  PointType::binary_output__
		, 0
		, [&](RTIMDB::Details::Action action, Point new_val, Point old_val)
			{
				database.update(1, Point(PointType::binary_output__, !new_val.payload_.bool_));
			}
		);
	auto selection(database.select(PointType::binary_output__, 0));
	database.operate(selection FIRST, PointType::binary_output__, 0, Point(PointType::binary_output__, true));
	assert(!database.read(PointType::binary_output__, 1) FIRST_GET .payload_.bool_);
	database.directOperate(0, Point(PointType::binary_output__, false));
	assert(database.read(PointType::binary_output__, 1) FIRST_GET .payload_.bool_);
}
