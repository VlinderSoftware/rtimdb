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
#include "point.hpp"

namespace Vlinder { namespace RTIMDB {
	template <> bool getValue< PointType::binary_input__ >(Point const &point) { return point.payload_.binary_; }
	template <> bool getValue< PointType::binary_output__ >(Point const &point) { return point.payload_.binary_; }
	template <> unsigned int getValue< PointType::counter__ >(Point const &point) { return point.payload_.counter_; }
	template <> double getValue< PointType::analog_input__ >(Point const &point) { return point.payload_.analog_; }
	template <> double getValue< PointType::analog_output__ >(Point const &point) { return point.payload_.analog_; }
	template <> Dataset* getValue< PointType::dataset__ >(Point const &point) { return point.payload_.dataset_; }
	template <> String* getValue< PointType::octet_string__ >(Point const &point) { return point.payload_.octet_string_; }
}
}
