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
#ifndef vlinder_rtimdb_core_details_optional_hpp
#define vlinder_rtimdb_core_details_optional_hpp

#include <new>
#include <utility>
#include <stdexcept>

namespace Vlinder {	namespace RTIMDB { namespace Core { namespace Details {
	template < typename T >
	class Optional
	{
	public :
		Optional()
			: empty_(true)
			, p_(nullptr)
		{ /* no-op */ }
		~Optional()
		{
			clear();
		}

		Optional(T const &v)
			: empty_(true)
			, p_(nullptr)
		{
			p_ = new (value_.buffer_) T(v);
			empty_ = false;
		}

		Optional(Optional const &other)
			: empty_(other.empty_)
			, p_(nullptr)
		{
			if (!empty_)
			{
				p_ = new (value_.buffer_) T(other.get());
			}
			else
			{ /* nothing more to do */ }
		}

		Optional& operator=(Optional other)
		{
			return swap(other);
		}

		Optional& swap(Optional &other)
		{
			using std::swap;
			if (empty_ && other.empty_)
			{
				/* nothing to do */
			}
			else if (!empty_ && !other.empty_)
			{
				swap(*p_, *other.p_);
			}
			else if (empty_ && !other.empty_)
			{
				p_ = new (value_.buffer_) T(other.get());
				empty_ = false;
				other.clear();
			}
			else if (!empty_ && other.empty_)
			{
				other.p_ = new (other.value_.buffer_) T(get());
				other.empty_ = false;
				clear();
			}
			else
			{
				throw std::logic_error("Unreachable code");
			}

			return *this;
		}

		T const &get() const { return *p_; }
		T const &operator*() const { return get(); }
		T const *operator->() const { return p_; }
		bool empty() const { return empty_; }
		void clear()
		{
			if (!empty_)
			{
				p_->~T();
			}
			empty_ = true;
		}
	private :
		bool empty_;
		union
		{
			double alignment_;
			unsigned char buffer_[sizeof(T)];
		} value_;
		T *p_;
	};
}}}}


#endif

