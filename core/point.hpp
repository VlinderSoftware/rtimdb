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
#ifndef vlinder_rtimdb_core_point_hpp
#define vlinder_rtimdb_core_point_hpp

#include "details/prologue.hpp"
#include <cstdint>
#include <atomic>
#include "../pointtype.hpp"

namespace Vlinder { namespace RTIMDB { namespace Core {
	class Dataset;
	class String;
	namespace Details {
		template < PointType point_type__ > struct PointTraits;
		template <> struct PointTraits< PointType::binary_input__ >
		{
			typedef bool cpp_type;
		};
		template <> struct PointTraits< PointType::binary_output__ >
		{
			typedef bool cpp_type;
		};
		template <> struct PointTraits< PointType::counter__ >
		{
			typedef unsigned int cpp_type;
		};
		template <> struct PointTraits< PointType::analog_input__ >
		{
			typedef double cpp_type;
		};
		template <> struct PointTraits< PointType::analog_output__ >
		{
			typedef double cpp_type;
		};
		template <> struct PointTraits< PointType::dataset__ >
		{
			typedef Dataset* cpp_type;
		};
		template <> struct PointTraits< PointType::octet_string__ >
		{
			typedef String* cpp_type;
		};
	}
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
	template < PointType point_type__ > typename Details::PointTraits< point_type__ >::cpp_type getValue(Point const &point);
	template <> RTIMDB_API bool getValue< PointType::binary_input__ >(Point const &point);
	template <> RTIMDB_API bool getValue< PointType::binary_output__ >(Point const &point);
	template <> RTIMDB_API unsigned int getValue< PointType::counter__ >(Point const &point);
	template <> RTIMDB_API double getValue< PointType::analog_input__ >(Point const &point);
	template <> RTIMDB_API double getValue< PointType::analog_output__ >(Point const &point);
	template <> RTIMDB_API Dataset* getValue< PointType::dataset__ >(Point const &point);
	template <> RTIMDB_API String* getValue< PointType::octet_string__ >(Point const &point);
}}}

#endif

