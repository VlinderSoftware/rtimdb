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

#ifdef RTIMDB_ALLOW_EXCEPTIONS
#define FIRST_DEREF
#define DOT_FIRST
#define DEREF .
#else
#define FIRST_DEREF first->
#define DOT_FIRST	.first
#define DEREF ->
#endif

TEST_CASE("Try to freeze", "[freeze]") {
	DataStore data_store;
	auto index(data_store.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST);

	auto transaction(data_store.startROTransaction() DOT_FIRST);
	data_store.update(PointType::binary_input__, index, PointValue(true), Flags(0), Timestamp());
	auto frozen_value(data_store.read(transaction, PointType::binary_input__, index) DOT_FIRST);
	//REQUIRE( PointType::binary_input__ == frozen_value DEREF type_ );
	REQUIRE( false == frozen_value DEREF value_.payload_.bool_ );
	frozen_value = data_store.read(PointType::binary_input__, index) DOT_FIRST;
	//REQUIRE( PointType::binary_input__ == frozen_value DEREF type_ );
	REQUIRE( true == frozen_value DEREF value_.payload_.bool_ );
}
