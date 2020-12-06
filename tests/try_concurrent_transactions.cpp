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
#include "exceptions/contract.hpp"
#include "catch.hpp"

using namespace Vlinder::RTIMDB;
using namespace Vlinder::RTIMDB::Core;

#ifdef RTIMDB_ALLOW_EXCEPTIONS
#define FIRST_DEREF
#define DOT_FIRST
#else
#define FIRST_DEREF first->
#define DOT_FIRST	.first
#endif

TEST_CASE("Try concurrent transactions", "[transactions]") {
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

	auto first_transaction_(database.startROTransaction());
	auto first_transaction(first_transaction_ DOT_FIRST);
#ifndef RTIMDB_ALLOW_EXCEPTIONS
	REQUIRE( Errors::no_error__ == first_transaction_.second );
#endif
	REQUIRE( Errors::no_error__ == database.update(PointType::analog_input__,  3,   PointValue(13.24), Flags(Flag::online__), Timestamp() RTIMDB_NOTHROW_ARG) );
	REQUIRE( Errors::no_error__ == database.update(PointType::analog_input__,  5,   PointValue(26.48), Flags(Flag::online__), Timestamp() RTIMDB_NOTHROW_ARG) );
	REQUIRE( Errors::no_error__ == database.update(PointType::analog_input__,  7,   PointValue(52.96), Flags(Flag::online__), Timestamp() RTIMDB_NOTHROW_ARG) );
	REQUIRE( Errors::no_error__ == database.update(PointType::analog_input__, 13, PointValue(104.192), Flags(Flag::online__), Timestamp() RTIMDB_NOTHROW_ARG) );
	auto second_transaction_(database.startROTransaction());
	auto second_transaction(second_transaction_ DOT_FIRST);
#ifndef RTIMDB_ALLOW_EXCEPTIONS
	REQUIRE( Errors::no_error__ == second_transaction_.second );
#endif
	REQUIRE( Errors::no_error__ == database.update(PointType::binary_input__,  3, PointValue(true), Flags(Flag::online__), Timestamp() RTIMDB_NOTHROW_ARG) );
	REQUIRE( Errors::no_error__ == database.update(PointType::binary_input__,  5, PointValue(true), Flags(Flag::online__), Timestamp() RTIMDB_NOTHROW_ARG) );
	REQUIRE( Errors::no_error__ == database.update(PointType::binary_input__,  7, PointValue(true), Flags(Flag::online__), Timestamp() RTIMDB_NOTHROW_ARG) );
	REQUIRE( Errors::no_error__ == database.update(PointType::binary_input__, 13, PointValue(true), Flags(Flag::online__), Timestamp() RTIMDB_NOTHROW_ARG) );
	REQUIRE( Errors::no_error__ == database.update(PointType::binary_input__, 17, PointValue(true), Flags(Flag::online__), Timestamp() RTIMDB_NOTHROW_ARG) );

	// a read using the first transaction should get the old analogs and the old binaries
#ifdef RTIMDB_ALLOW_EXCEPTIONS
#define VERIFY(EXP, TRANSACTION, TYPE, INDEX, PAYLOAD)									\
	{																					\
		auto read_result_1(database.read(TRANSACTION, TYPE, INDEX));					\
		REQUIRE( EXP == read_result_1.FIRST_DEREF value_.payload_.PAYLOAD );			\
	}
#else
#define VERIFY(EXP, TRANSACTION, TYPE, INDEX, PAYLOAD)									\
	{																					\
		auto read_result_1(database.read(TRANSACTION, TYPE, INDEX));					\
		REQUIRE( Errors::no_error__ == read_result_1.second );							\
		REQUIRE( EXP == read_result_1.FIRST_DEREF value_.payload_.PAYLOAD );			\
	}
#endif

	VERIFY(false, first_transaction, PointType::binary_input__,  3, bool_);
	VERIFY(false, first_transaction, PointType::binary_input__,  5, bool_);
	VERIFY(false, first_transaction, PointType::binary_input__,  7, bool_);
	VERIFY(false, first_transaction, PointType::binary_input__, 13, bool_);
	VERIFY(false, first_transaction, PointType::binary_input__, 17, bool_);

	VERIFY(0.0, first_transaction, PointType::analog_input__,  3, double_);
	VERIFY(0.0, first_transaction, PointType::analog_input__,  5, double_);
	VERIFY(0.0, first_transaction, PointType::analog_input__,  7, double_);
	VERIFY(0.0, first_transaction, PointType::analog_input__, 13, double_);
	VERIFY(0.0, first_transaction, PointType::analog_input__, 17, double_);

	// a read using the first transaction should get the new analogs and the old binaries
	VERIFY(false, second_transaction, PointType::binary_input__,  3, bool_);
	VERIFY(false, second_transaction, PointType::binary_input__,  5, bool_);
	VERIFY(false, second_transaction, PointType::binary_input__,  7, bool_);
	VERIFY(false, second_transaction, PointType::binary_input__, 13, bool_);
	VERIFY(false, second_transaction, PointType::binary_input__, 17, bool_);

	VERIFY(  13.24, second_transaction, PointType::analog_input__,  3, double_);
	VERIFY(  26.48, second_transaction, PointType::analog_input__,  5, double_);
	VERIFY(  52.96, second_transaction, PointType::analog_input__,  7, double_);
	VERIFY(104.192, second_transaction, PointType::analog_input__, 13, double_);
} 
