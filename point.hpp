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
#include "time.hpp"
#include "doublebitbinary.hpp"

namespace Vlinder { namespace RTIMDB {
	class Dataset;
	class File;
	class String;
	struct Point
	{
		union Payload
		{
			bool binary_;
			DoubleBitBinary double_bit_binary_;
			uint32_t counter_;
			double analog_;
			Time time_and_date_;
			File *file_;
			Dataset *dataset_;
			uint16_t bcd_;
			uint8_t unsigned_integer_;
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
		Point(PointType type, DoubleBitBinary const &double_bit_binary, unsigned int version = 0) throw()
			: type_(type)
			, version_(version)
		{
			payload_.double_bit_binary_ = double_bit_binary;
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
		Point(PointType type, Time time_and_date, unsigned int version = 0) throw()
			: type_(type)
			, version_(version)
		{
			payload_.time_and_date_ = time_and_date;
		}
		Point(PointType type, File *file, unsigned int version = 0) throw()
			: type_(type)
			, version_(version)
		{
			payload_.file_ = file;
		}
		Point(PointType type, Dataset *dataset, unsigned int version = 0) throw()
			: type_(type)
			, version_(version)
		{
			payload_.dataset_ = dataset;
		}
		Point(PointType type, uint16_t bcd, unsigned int version = 0) throw()
			: type_(type)
			, version_(version)
		{
			payload_.bcd_ = bcd;
		}
		Point(PointType type, uint8_t unsigned_integer, unsigned int version = 0) throw()
			: type_(type)
			, version_(version)
		{
			payload_.unsigned_integer_ = unsigned_integer;
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

