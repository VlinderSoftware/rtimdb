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
