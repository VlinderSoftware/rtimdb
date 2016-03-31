/* Copyright 2016  Vlinder Software
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
#ifndef vlinder_rtimdb_details_eventiterator_hpp
#define vlinder_rtimdb_details_eventiterator_hpp

#include "prologue.hpp"
#include <iterator>
#include "event.hpp"

namespace Vlinder { namespace RTIMDB { namespace Details {
	class Events;
	class RTIMDB_API EventIterator : public std::iterator< std::random_access_iterator_tag, Event const >
	{
	public :
		EventIterator(Events const *events, unsigned int size);
		constexpr EventIterator() : events_(0), size_(0), offset_(0), at_end_(true) {}
		EventIterator(EventIterator const &) = default;
		EventIterator(EventIterator &&) = default;
		EventIterator& operator=(EventIterator const &) = default;
		EventIterator& operator=(EventIterator &&) = default;
		
		Event const &operator*() const;
		Event const *operator->() const;

		EventIterator& operator++();
		EventIterator operator++(int);

		EventIterator& operator--();
		EventIterator operator--(int);

		EventIterator& operator+=(int n);

		EventIterator& operator-=(int n);

		int compare(EventIterator const &other) const;
		difference_type distance(EventIterator const &other) const;

	private :
		Events const *events_;
		unsigned int size_;
		unsigned int offset_;
		bool at_end_;
	};

	RTIMDB_API bool operator==(EventIterator const &lhs, EventIterator const &rhs);
	RTIMDB_API bool operator!=(EventIterator const &lhs, EventIterator const &rhs);
	RTIMDB_API bool operator<(EventIterator const &lhs, EventIterator const &rhs);
	RTIMDB_API bool operator<=(EventIterator const &lhs, EventIterator const &rhs);
	RTIMDB_API bool operator>(EventIterator const &lhs, EventIterator const &rhs);
	RTIMDB_API bool operator>=(EventIterator const &lhs, EventIterator const &rhs);
	RTIMDB_API EventIterator operator+(int n, EventIterator it);
	RTIMDB_API EventIterator operator+(EventIterator it, int n);
	RTIMDB_API EventIterator operator-(int n, EventIterator it);
	RTIMDB_API EventIterator operator-(EventIterator it, int n);
	RTIMDB_API std::iterator_traits< EventIterator >::difference_type operator-(EventIterator lhs, EventIterator rhs);
}}}

#endif

