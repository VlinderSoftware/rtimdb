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
#include "pointvalue.hpp"
#include <limits>

namespace Vlinder { namespace RTIMDB { namespace Core {
	template < > bool getValue< bool >(PointValue const &point_value)
	{
		switch (point_value.cpp_type_)
		{
		case PointValue::bool__:
			return point_value.payload_.bool_;
		case PointValue::u16__:
			return point_value.payload_.u16_ != 0;
		case PointValue::s16__:
			return point_value.payload_.s16_ != 0;
		case PointValue::u32__:
			return point_value.payload_.u32_ != 0;
		case PointValue::s32__:
			return point_value.payload_.s32_ != 0;
		case PointValue::float__:
			return point_value.payload_.float_ != 0;
		case PointValue::double__:
			return point_value.payload_.double_ != 0;
		case PointValue::none__:
			throw std::logic_error("Cannot get the value of a non-assigned point");
		}
		return false;
	}
}}}
