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

namespace Vlinder {	namespace RTIMDB { namespace Details {
	template < typename T >
	class Variant
	{
	public :
		Variant()
			: empty_(true)
			, p_(nullptr)
		{ /* no-op */ }

		Variant(T const &v)
			: empty_(true)
			, p_(nullptr)
		{
			p_ = new (value_.buffer_) T(v);
			empty_ = false;
		}

		T const &get() const { return *p_; }
		T const &operator*() const { return get(); }
		T const *operator->() const { return p_; }
		bool empty() const { return empty_; }

	private :
		bool empty_;
		union
		{
			double alignment_;
			unsigned char buffer_[sizeof(T)];
		} value_;
		T *p_;
	};
}}}


#endif

