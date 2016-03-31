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
#include "eventiterator.hpp"
#include "events.hpp"
#include <limits>

using namespace std;

namespace Vlinder { namespace RTIMDB { namespace Details {
	EventIterator::EventIterator(Events const *events, unsigned int size)
		: events_(events)
		, size_(size)
		, offset_(0)
		, at_end_(false)
	{ /* no-op */ }

	Event const &EventIterator::operator*() const
	{
		return (*events_)[offset_];
	}
	Event const *EventIterator::operator->() const
	{
		return &(*events_)[offset_];
	}

	EventIterator& EventIterator::operator++()
	{
		++offset_;
		return *this;
	}
	EventIterator EventIterator::operator++(int)
	{
		EventIterator retval(*this);
		++(*this);
		return retval;
	}

	EventIterator& EventIterator::operator--()
	{
		--offset_;
		return *this;
	}
	EventIterator EventIterator::operator--(int)
	{
		EventIterator retval(*this);
		--(*this);
		return retval;
	}

	EventIterator& EventIterator::operator+=(int n)
	{
		offset_ += n;
		return *this;
	}

	EventIterator& EventIterator::operator-=(int n)
	{
		offset_ += n;
		return *this;
	}

	int EventIterator::compare(EventIterator const &other) const
	{
		// note there is a slight chance of a false positive if one of the iterators is invalid i.e. if any event was confirmed since its creation)
		if (false
			|| (at_end_ == other.at_end_)
			|| (at_end_ == (other.size_ == other.offset_))
			|| ((size_ == offset_) == (other.at_end_))
			|| ((events_ == other.events_) && (size_ == other.size_) && (offset_ == other.offset_))
			) return 0;
		else return (at_end_ ? numeric_limits< int >::max() : offset_) - (other.at_end_ ? numeric_limits< int >::max() : other.offset_);
	}
	EventIterator::difference_type EventIterator::distance(EventIterator const &other) const
	{
		return offset_ - other.offset_;
	}

	/*RTIMDB_API */bool operator==(EventIterator const &lhs, EventIterator const &rhs) { return lhs.compare(rhs) == 0; }
	/*RTIMDB_API */bool operator!=(EventIterator const &lhs, EventIterator const &rhs) { return lhs.compare(rhs) != 0; }
	/*RTIMDB_API */bool operator<(EventIterator const &lhs, EventIterator const &rhs)  { return lhs.compare(rhs) < 0; }
	/*RTIMDB_API */bool operator<=(EventIterator const &lhs, EventIterator const &rhs) { return lhs.compare(rhs) <= 0; }
	/*RTIMDB_API */bool operator>(EventIterator const &lhs, EventIterator const &rhs)  { return lhs.compare(rhs) > 0; }
	/*RTIMDB_API */bool operator>=(EventIterator const &lhs, EventIterator const &rhs) { return lhs.compare(rhs) >= 0; }
	/*RTIMDB_API */EventIterator operator+(int n, EventIterator it) { return it += n; } 
	/*RTIMDB_API */EventIterator operator+(EventIterator it, int n) { return it += n; } 
	/*RTIMDB_API */EventIterator operator-(int n, EventIterator it) { return it -= n; }
	/*RTIMDB_API */EventIterator operator-(EventIterator it, int n) { return it -= n; }
	/*RTIMDB_API */std::iterator_traits< EventIterator >::difference_type operator-(EventIterator lhs, EventIterator rhs) { return lhs.distance(rhs); }
}}}


