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
#include "../core/database.hpp"

using namespace Vlinder::RTIMDB;
using namespace Vlinder::RTIMDB::Core;

template < PointType first__, PointType second__ >
int pointTypeShallNotSilentlyChange()
{
    typedef typename Core::Details::PointTraits< first__ >::cpp_type FirstCPPType;
    typedef typename Core::Details::PointTraits< second__ >::cpp_type SecondCPPType;
    FirstCPPType first_value(0);
    SecondCPPType second_value(0);
    Point first_point_value(first__, first_value);
    Point second_point_value(second__, second_value);
    Database database;
    
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	database.insert(first_point_value);
    auto selection(database.select(first__, 0));
    bool caught(false);
    try
    {
        database.operate(selection, first__, 0, second_point_value);
    }
    catch (NotAllowed const &)
    {
        caught = true;
    }

	return caught ? 0 : 1;
#else
	auto insert_result(database.insert(first_point_value));
	if (Errors::no_error__ != insert_result.second) return 1;
	auto selection(database.select(first__, 0));
	if (Errors::no_error__ != selection.second) return 1;
	bool caught(false);
	auto operate_result(database.operate(selection.first, first__, 0, second_point_value));
	if (Errors::not_allowed__ != operate_result) return 1;

	return 0;

#endif
}

int main()
{
    return 0
		|| pointTypeShallNotSilentlyChange< PointType::binary_input__, PointType::binary_output__  >()
		|| pointTypeShallNotSilentlyChange< PointType::binary_input__, PointType::counter__  >()
		|| pointTypeShallNotSilentlyChange< PointType::binary_input__, PointType::analog_input__  >()
		|| pointTypeShallNotSilentlyChange< PointType::binary_input__, PointType::analog_output__  >()
		|| pointTypeShallNotSilentlyChange< PointType::binary_input__, PointType::dataset__  >()
		|| pointTypeShallNotSilentlyChange< PointType::binary_input__, PointType::octet_string__  >()
		|| pointTypeShallNotSilentlyChange< PointType::binary_output__, PointType::binary_input__  >()
		|| pointTypeShallNotSilentlyChange< PointType::binary_output__, PointType::counter__  >()
		|| pointTypeShallNotSilentlyChange< PointType::binary_output__, PointType::analog_input__  >()
		|| pointTypeShallNotSilentlyChange< PointType::binary_output__, PointType::analog_output__  >()
		|| pointTypeShallNotSilentlyChange< PointType::binary_output__, PointType::dataset__  >()
		|| pointTypeShallNotSilentlyChange< PointType::binary_output__, PointType::octet_string__  >()
		|| pointTypeShallNotSilentlyChange< PointType::counter__, PointType::binary_input__  >()
		|| pointTypeShallNotSilentlyChange< PointType::counter__, PointType::binary_output__  >()
		|| pointTypeShallNotSilentlyChange< PointType::counter__, PointType::analog_input__  >()
		|| pointTypeShallNotSilentlyChange< PointType::counter__, PointType::analog_output__  >()
		|| pointTypeShallNotSilentlyChange< PointType::counter__, PointType::dataset__  >()
		|| pointTypeShallNotSilentlyChange< PointType::counter__, PointType::octet_string__  >()
		|| pointTypeShallNotSilentlyChange< PointType::analog_input__, PointType::binary_input__  >()
		|| pointTypeShallNotSilentlyChange< PointType::analog_input__, PointType::binary_output__  >()
		|| pointTypeShallNotSilentlyChange< PointType::analog_input__, PointType::counter__  >()
		|| pointTypeShallNotSilentlyChange< PointType::analog_input__, PointType::analog_output__  >()
		|| pointTypeShallNotSilentlyChange< PointType::analog_input__, PointType::dataset__  >()
		|| pointTypeShallNotSilentlyChange< PointType::analog_input__, PointType::octet_string__  >()
		|| pointTypeShallNotSilentlyChange< PointType::analog_output__, PointType::binary_input__  >()
		|| pointTypeShallNotSilentlyChange< PointType::analog_output__, PointType::binary_output__  >()
		|| pointTypeShallNotSilentlyChange< PointType::analog_output__, PointType::counter__  >()
		|| pointTypeShallNotSilentlyChange< PointType::analog_output__, PointType::analog_input__  >()
		|| pointTypeShallNotSilentlyChange< PointType::analog_output__, PointType::dataset__  >()
		|| pointTypeShallNotSilentlyChange< PointType::analog_output__, PointType::octet_string__  >()
		|| pointTypeShallNotSilentlyChange< PointType::dataset__, PointType::binary_input__  >()
		|| pointTypeShallNotSilentlyChange< PointType::dataset__, PointType::binary_output__  >()
		|| pointTypeShallNotSilentlyChange< PointType::dataset__, PointType::counter__  >()
		|| pointTypeShallNotSilentlyChange< PointType::dataset__, PointType::analog_input__  >()
		|| pointTypeShallNotSilentlyChange< PointType::dataset__, PointType::analog_output__  >()
		|| pointTypeShallNotSilentlyChange< PointType::dataset__, PointType::octet_string__  >()
		|| pointTypeShallNotSilentlyChange< PointType::octet_string__, PointType::binary_input__  >()
		|| pointTypeShallNotSilentlyChange< PointType::octet_string__, PointType::binary_output__  >()
		|| pointTypeShallNotSilentlyChange< PointType::octet_string__, PointType::counter__  >()
		|| pointTypeShallNotSilentlyChange< PointType::octet_string__, PointType::analog_input__  >()
		|| pointTypeShallNotSilentlyChange< PointType::octet_string__, PointType::analog_output__  >()
		|| pointTypeShallNotSilentlyChange< PointType::octet_string__, PointType::dataset__  >()
		;
}
