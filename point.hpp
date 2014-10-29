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
#ifndef vlinder_rtimdb_point_hpp
#define vlinder_rtimdb_point_hpp

#include <cstdint>
#include <atomic>
#include "pointtype.hpp"

namespace Vlinder { namespace RTIMDB {
	class Dataset;
	class String;
	struct Point
	{
		union Payload
		{
			bool binary_;
			uint32_t counter_;
			double analog_;
			Dataset *dataset_;
			String *octet_string_;
		};

		Point() throw()
			: type_(PointType::_type_count__)
			, version_(0)
		{ /* no-op */ }
		Point(PointType type, bool binary, unsigned int version = 0) throw()
			: type_(type)
			, version_(version)
		{
			payload_.binary_ = binary;
		}
		Point(PointType type, uint32_t counter, unsigned int version = 0) throw()
			: type_(type)
			, version_(version)
		{
			payload_.counter_ = counter;
		}
		Point(PointType type, double analog, unsigned int version = 0) throw()
			: type_(type)
			, version_(version)
		{
			payload_.analog_ = analog;
		}
		Point(PointType type, Dataset *dataset, unsigned int version = 0) throw()
			: type_(type)
			, version_(version)
		{
			payload_.dataset_ = dataset;
		}
		Point(PointType type, String *octet_string, unsigned int version = 0) throw()
			: type_(type)
			, version_(version)
		{
			payload_.octet_string_ = octet_string;
		}

		PointType type_;
		Payload payload_;
		unsigned int version_;
	};

}}

#endif

