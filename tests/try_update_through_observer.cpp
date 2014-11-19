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
using namespace std;

int main()
{
	Database database;
	unsigned int exp_ai_index(0);
	unsigned int exp_bo_index(0);

	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;
	assert(exp_ai_index == database.insert(Point(PointType::analog_input__, 0.0))); ++exp_ai_index;

	assert(exp_bo_index == database.insert(Point(PointType::binary_output__, false))); ++exp_bo_index;
	assert(exp_bo_index == database.insert(Point(PointType::binary_output__, true))); ++exp_bo_index;

	assert(4 == distance(database.begin(), database.end()));

	database.registerObserver(
		  PointType::binary_output__
		, 0
		, [&](Details::Action action, Point new_val, Point old_val)
			{
				database.update(1, Point(PointType::binary_output__, !new_val.payload_.binary_));
			}
		);
	auto selection(database.select(PointType::binary_output__, 0));
	database.operate(selection, PointType::binary_output__, 0, Point(PointType::binary_output__, true));
	assert(!database.read(PointType::binary_output__, 1).payload_.binary_);
	database.directOperate(0, Point(PointType::binary_output__, false));
	assert(database.read(PointType::binary_output__, 1).payload_.binary_);
}
