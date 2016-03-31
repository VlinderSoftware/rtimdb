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
#ifndef vlinder_rtimdb_details_variant_hpp
#define vlinder_rtimdb_details_variant_hpp

#include "meta/typelist.hpp"
#include <cstddef>
#include <stdexcept>

namespace Vlinder { namespace RTIMDB { namespace Details {
	template < typename T1, typename TL >
	struct GetVariantBufferSize;
	template < typename TL >
	struct GetVariantBufferSize< Meta::Nil, TL >
	{
		static const size_t value = 0;
	};
	template < typename T1, typename TL >
	struct GetVariantBufferSize : public GetVariantBufferSize< typename Meta::Head< TL >::type, typename Meta::Tail< TL >::type >
	{
		typedef GetVariantBufferSize< typename Meta::Head< TL >::type, typename Meta::Tail< TL >::type > super;
		static const size_t value = sizeof(T1) > super::value ? sizeof(T1) : super::value;
	};

	template < typename T1, typename TL >
	struct VariantCarrier;
	template < typename TL >
	struct VariantCarrier< Meta::Nil, TL >
	{
		static void destroy(void *ptr, unsigned int index)
		{}
		static bool get(void const *p, unsigned int type)
		{
			return false;
		}
	};
	template < typename T1, typename TL >
	struct VariantCarrier : public VariantCarrier< typename Meta::Head< TL >::type, typename Meta::Tail< TL >::type >
	{
		typedef VariantCarrier< typename Meta::Head< TL >::type, typename Meta::Tail< TL >::type > super;

		static void destroy(void *ptr, unsigned int index)
		{
			if (0 == index)
			{
				((T1*)ptr)->~T1();
			}
			else
			{
				super::destroy(ptr, index - 1);
			}
		}
		template < typename T >
		static bool get(T const *p, unsigned int type)
		{
			if (0 == type) return false;
			else return super::get(p, type - 1);
		}
		static bool get(T1 const *p, unsigned int type)
		{
			if (0 == type) return true;
			else return false;
		}
	};
	template <
		  typename  T1,                      typename  T2 = Vlinder::Meta::Nil, typename  T3 = Vlinder::Meta::Nil, typename  T4 = Vlinder::Meta::Nil
		, typename  T5 = Vlinder::Meta::Nil, typename  T6 = Vlinder::Meta::Nil, typename  T7 = Vlinder::Meta::Nil, typename  T8 = Vlinder::Meta::Nil
		, typename  T9 = Vlinder::Meta::Nil, typename T10 = Vlinder::Meta::Nil, typename T11 = Vlinder::Meta::Nil, typename T12 = Vlinder::Meta::Nil
		, typename T13 = Vlinder::Meta::Nil, typename T14 = Vlinder::Meta::Nil, typename T15 = Vlinder::Meta::Nil, typename T16 = Vlinder::Meta::Nil
		, typename T17 = Vlinder::Meta::Nil, typename T18 = Vlinder::Meta::Nil, typename T19 = Vlinder::Meta::Nil, typename T20 = Vlinder::Meta::Nil
		, typename T21 = Vlinder::Meta::Nil, typename T22 = Vlinder::Meta::Nil, typename T23 = Vlinder::Meta::Nil, typename T24 = Vlinder::Meta::Nil
		, typename T25 = Vlinder::Meta::Nil, typename T26 = Vlinder::Meta::Nil, typename T27 = Vlinder::Meta::Nil, typename T28 = Vlinder::Meta::Nil
		, typename T29 = Vlinder::Meta::Nil, typename T30 = Vlinder::Meta::Nil, typename T31 = Vlinder::Meta::Nil, typename T32 = Vlinder::Meta::Nil
	>
	class Variant : public VariantCarrier< T1, typename Vlinder::Meta::MakeTypeList<
		  T2, T3, T4, T5, T6, T7, T8, T9
		, T10, T11, T12, T13, T14, T15, T16, T17
		, T18, T19, T20, T21, T22, T23, T24, T25
		, T26, T27, T28, T29, T30, T31, T32 >::type >
	{
		typedef VariantCarrier< T1, typename Vlinder::Meta::MakeTypeList<
			  T2, T3, T4, T5, T6, T7, T8, T9
			, T10, T11, T12, T13, T14, T15, T16, T17
			, T18, T19, T20, T21, T22, T23, T24, T25
			, T26, T27, T28, T29, T30, T31, T32 >::type > super;
	public :
		Variant()
			: type_(-1)
		{ /* no-op */ }
		template < typename T >
		explicit Variant(T const &v)
			: type_(Vlinder::Meta::IndexOf< typename Vlinder::Meta::MakeTypeList<
			   T1, T2, T3, T4, T5, T6, T7, T8, T9
			, T10, T11, T12, T13, T14, T15, T16, T17
			, T18, T19, T20, T21, T22, T23, T24, T25
			, T26, T27, T28, T29, T30, T31, T32 >::type, T >::value)
		{
			new (payload_.buffer_) T(v);
		}
		~Variant()
		{
			super::destroy(payload_.buffer_, type_);
		}
		template < typename T > T const& get() const
		{
			T const *p((T const *) payload_.buffer_);
			bool found(super::get(p, type_));
			if (found) return *p;
			else throw std::logic_error("Type error");
		}

		unsigned int type() const noexcept
		{
			return type_;
		}

	private :
		union Payload
		{
			double alignment_;
			unsigned char buffer_[Details::GetVariantBufferSize< T1, typename Vlinder::Meta::MakeTypeList<
				  T2, T3, T4, T5, T6, T7, T8, T9
				, T10, T11, T12, T13, T14, T15, T16, T17
				, T18, T19, T20, T21, T22, T23, T24, T25
				, T26, T27, T28, T29, T30, T31, T32 >::type >::value];
		};
		unsigned int type_;
		Payload payload_;
	};
}}}

#endif

