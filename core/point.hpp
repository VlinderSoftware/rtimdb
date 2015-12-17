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
#include <stdexcept>
#include "../pointtype.hpp"

namespace Vlinder { namespace RTIMDB { namespace Core {
	struct Point;
	template < typename T > T getValue(Point const &point);

	struct Point
	{
		enum CPPType {
			  none__
			, bool__
			, u16__
			, s16__
			, u32__
			, s32__
			, float__
			, double__
		};
		union Payload
		{
			bool bool_;
			int16_t s16_;
			int32_t s32_;
			uint16_t u16_;
			uint32_t u32_;
			float float_;
			double double_;
		};

		Point() throw()
			: type_(PointType::_type_count__)
			, cpp_type_(none__)
			, version_(0)
		{ /* no-op */ }
		Point(PointType type, bool value, unsigned int version = 0) throw()
			: type_(type)
			, cpp_type_(bool__)
			, version_(version)
		{
			payload_.bool_ = value;
		}
		Point(PointType type, int16_t value, unsigned int version = 0) throw()
			: type_(type)
			, cpp_type_(s16__)
			, version_(version)
		{
			payload_.s16_ = value;
		}
		Point(PointType type, int32_t value, unsigned int version = 0) throw()
			: type_(type)
			, cpp_type_(s32__)
			, version_(version)
		{
			payload_.s32_ = value;
		}
		Point(PointType type, uint16_t value, unsigned int version = 0) throw()
			: type_(type)
			, cpp_type_(u16__)
			, version_(version)
		{
			payload_.u16_ = value;
		}
		Point(PointType type, uint32_t value, unsigned int version = 0) throw()
			: type_(type)
			, cpp_type_(u32__)
			, version_(version)
		{
			payload_.u32_ = value;
		}
		Point(PointType type, float value, unsigned int version = 0) throw()
			: type_(type)
			, cpp_type_(float__)
			, version_(version)
		{
			payload_.float_ = value;
		}
		Point(PointType type, double value, unsigned int version = 0) throw()
			: type_(type)
			, cpp_type_(double__)
			, version_(version)
		{
			payload_.double_ = value;
		}

		bool operator==(Point const &other) const
		{
			return true
				&& type_ == other.type_
				&& cpp_type_ == other.cpp_type_
				&& (false
					|| ((cpp_type_ == bool__) && (getValue< bool >(*this) == getValue< bool >(other)))
					|| ((cpp_type_ == u16__) && (getValue< uint16_t >(*this) == getValue< uint16_t >(other)))
					|| ((cpp_type_ == s16__) && (getValue< int16_t >(*this) == getValue< int16_t >(other)))
					|| ((cpp_type_ == u32__) && (getValue< uint32_t >(*this) == getValue< uint32_t >(other)))
					|| ((cpp_type_ == s32__) && (getValue< int32_t >(*this) == getValue< int32_t >(other)))
					|| ((cpp_type_ == float__) && (getValue< float >(*this) == getValue< float >(other)))
					|| ((cpp_type_ == double__) && (getValue< double >(*this) == getValue< double >(other)))
					)
				;
		}
		bool operator!=(Point const &other) const
		{
			return !(*this == other);
		}

		PointType type_;
		CPPType cpp_type_;
		unsigned int version_;
		Payload payload_;
	};

	template < typename T > T getValue(Point const &point)
	{
		switch (point.cpp_type_)
		{
		case Point::bool__:
			return T(point.payload_.bool_);
		case Point::u16__:
			return T(point.payload_.u16_);
		case Point::s16__:
			return T(point.payload_.s16_);
		case Point::u32__:
			return T(point.payload_.u32_);
		case Point::s32__:
			return T(point.payload_.s32_);
		case Point::float__:
			return T(point.payload_.float_);
		case Point::double__:
			return T(point.payload_.double_);
		case Point::none__ :
			throw std::logic_error("Cannot get the value of a non-assigned point");
		}
		return T(0);
	}
}}}

#endif

