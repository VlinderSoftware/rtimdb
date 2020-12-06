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
using namespace std;

#ifdef RTIMDB_ALLOW_EXCEPTIONS
#define FIRST_DEREF
#define DOT_FIRST
#else
#define FIRST_DEREF first->
#define DOT_FIRST	.first
#endif

TEST_CASE("Try to iterate", "[iterator]") {
	DataStore database;
	unsigned int exp_ai_index(0);
	unsigned int exp_bi_index(0);

	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;

	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;
	REQUIRE( exp_bi_index == database.insert(PointType::binary_input__, PointValue(false), Flags(), Timestamp()) DOT_FIRST ); ++exp_bi_index;

	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;
	REQUIRE( exp_ai_index == database.insert(PointType::analog_input__, PointValue(0.0), Flags(), Timestamp()) DOT_FIRST ); ++exp_ai_index;

	REQUIRE( 96 == distance(database.begin(), database.end()) );
}

