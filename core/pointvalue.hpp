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
#ifndef vlinder_rtimdb_core_pointvalue_hpp
#define vlinder_rtimdb_core_pointvalue_hpp

#include "details/prologue.hpp"
#include <cstdint>
#include <atomic>
#include <stdexcept>
#include "pointtype.hpp"

namespace Vlinder { namespace RTIMDB { namespace Core {
	struct PointValue;
	template < typename T > T getValue(PointValue const &point_value);
	template < > RTIMDB_API bool getValue< bool >(PointValue const &point_value);

	struct PointValue
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

		PointValue() throw()
			: cpp_type_(none__)
			, version_(0)
		{ /* no-op */ }
		PointValue(bool value, unsigned int version = 0) throw()
			: cpp_type_(bool__)
			, version_(version)
		{
			payload_.bool_ = value;
		}
		PointValue(int16_t value, unsigned int version = 0) throw()
			: cpp_type_(s16__)
			, version_(version)
		{
			payload_.s16_ = value;
		}
		PointValue(int32_t value, unsigned int version = 0) throw()
			: cpp_type_(s32__)
			, version_(version)
		{
			payload_.s32_ = value;
		}
		PointValue(uint16_t value, unsigned int version = 0) throw()
			: cpp_type_(u16__)
			, version_(version)
		{
			payload_.u16_ = value;
		}
		PointValue(uint32_t value, unsigned int version = 0) throw()
			: cpp_type_(u32__)
			, version_(version)
		{
			payload_.u32_ = value;
		}
		PointValue(float value, unsigned int version = 0) throw()
			: cpp_type_(float__)
			, version_(version)
		{
			payload_.float_ = value;
		}
		PointValue(double value, unsigned int version = 0) throw()
			: cpp_type_(double__)
			, version_(version)
		{
			payload_.double_ = value;
		}

		bool operator==(PointValue const &other) const
		{
			return true
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
		bool operator!=(PointValue const &other) const
		{
			return !(*this == other);
		}

		CPPType cpp_type_;
		unsigned int version_;
		Payload payload_;
	};

	template < typename T > T getValue(PointValue const &point_value)
	{
		switch (point_value.cpp_type_)
		{
		case PointValue::bool__:
			return T(point_value.payload_.bool_);
		case PointValue::u16__:
			return T(point_value.payload_.u16_);
		case PointValue::s16__:
			return T(point_value.payload_.s16_);
		case PointValue::u32__:
			return T(point_value.payload_.u32_);
		case PointValue::s32__:
			return T(point_value.payload_.s32_);
		case PointValue::float__:
			return T(point_value.payload_.float_);
		case PointValue::double__:
			return T(point_value.payload_.double_);
		case PointValue::none__ :
			throw std::logic_error("Cannot get the value of a non-assigned point");
		}
		return T(0);
	}

}}}

#endif

