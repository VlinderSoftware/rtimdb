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

TEST_CASE("Try to create a data store", "[datastore]") {
	DataStore data_store;
	data_store.insert(PointType::binary_output__, PointValue(true), Flags(), Timestamp());
	data_store.insert(PointType::analog_input__, PointValue(25.35), Flags(), Timestamp());
	data_store.insert(PointType::counter__, PointValue(static_cast< uint32_t >(463)), Flags(), Timestamp());
	data_store.insert(PointType::analog_output__, PointValue(58.24), Flags(), Timestamp());
	data_store.insert(PointType::binary_input__, PointValue(true), Flags(), Timestamp());
	data_store.insert(PointType::binary_input__, PointValue(true), Flags(), Timestamp());
	data_store.insert(PointType::binary_output__, PointValue(true), Flags(), Timestamp());
	data_store.insert(PointType::analog_input__, PointValue(3.623632346), Flags(), Timestamp());
	data_store.insert(PointType::counter__, PointValue(static_cast< uint32_t >(2587546342)), Flags(), Timestamp());
	data_store.insert(PointType::analog_output__, PointValue(72.435), Flags(), Timestamp());
	data_store.insert(PointType::binary_input__, PointValue(true), Flags(), Timestamp());

	data_store.update(PointType::binary_input__, 1, PointValue(false), Flags(Flag::online__), Timestamp());
}
