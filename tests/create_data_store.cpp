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

using namespace Vlinder::RTIMDB;
using namespace Vlinder::RTIMDB::Core;

int main()
{
	DataStore data_store;
	data_store.insert(Point(PointType::binary_output__, true));
	data_store.insert(Point(PointType::analog_input__, 25.35));
	data_store.insert(Point(PointType::counter__, static_cast< uint32_t >(463)));
	data_store.insert(Point(PointType::analog_output__, 58.24));
	data_store.insert(Point(PointType::binary_input__, true));
	data_store.insert(Point(PointType::binary_input__, true));
	data_store.insert(Point(PointType::binary_output__, true));
	data_store.insert(Point(PointType::analog_input__, 3.623632346));
	data_store.insert(Point(PointType::counter__, static_cast< uint32_t >(2587546342)));
	data_store.insert(Point(PointType::analog_output__, 72.435));
	data_store.insert(Point(PointType::binary_input__, true));

	data_store.update(1, Point(PointType::binary_input__, false));
}
