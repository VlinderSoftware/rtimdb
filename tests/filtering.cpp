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

template < PointType first__, PointType second__ >
int pointTypeShallNotSilentlyChange()
{
    typedef Details::PointTraits< first__ >::cpp_type FirstCPPType;
    typedef Details::PointTraits< second__ >::cpp_type SecondCPPType;
    FirstCPPType first_value(0);
    SecondCPPType second_value(0);
    Point first_point_value(first__, first_value);
    Point second_point_value(second__, second_value);
    Database database;
    
    database.insert(first__, first_point_value);
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
}

int main()
{
    return 0
        || pointTypeShallNotSilentlyChange()
        ;
}
