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
#include "../core/details/optional.hpp"
#include "../core/pointvalue.hpp"
#include "exceptions/contract.hpp"
#include "catch.hpp"

using namespace Vlinder::RTIMDB::Core;

TEST_CASE("Try to create an instance (of an optional)", "[optional]") {
	Details::Optional< PointValue > v;
	REQUIRE( v.empty() );
}
TEST_CASE("Try to create a non-empty instance", "[optional]") {
	PointValue p;
	Details::Optional< PointValue > v(p);

	REQUIRE( !v.empty() );
}

TEST_CASE("Try to copy", "[optional]") {
	struct T
	{
		T(bool &f)
			: f_(f)
		{}
		T(T const &t)
			: f_(t.f_)
		{
			f_ = true;
		}

		bool &f_;
	};
	bool f(false);
	T t(f);
	Details::Optional< T > v1(t);
	REQUIRE( f );
	f = false;
	Details::Optional< T > v2(t);
	REQUIRE( f );
}

TEST_CASE("Try to assign", "[optional]") {
	struct T
	{
		T(bool &f)
			: f_(&f)
		{}
		T(T const &t)
			: f_(t.f_)
		{
			*f_ = true;
		}

		bool *f_;
	};
	bool f(false);
	T t(f);
	Details::Optional< T > v1(t);
	REQUIRE( f );
	f = false;
	Details::Optional< T > v2;
	v2 = v1;
	REQUIRE( f );
}
