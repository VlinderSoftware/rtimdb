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
#include "events.hpp"
#include "eventqueue.hpp"

namespace Vlinder { namespace RTIMDB { namespace Details {
	Events::Events(EventQueue &queue)
		: events_(queue.events_)
		, capacity_(queue.capacity())
		, head_(queue.head_)
		, tail_(queue.tail_.load(std::memory_order_acquire))
	{ /* no-op */ }

	unsigned int Events::size() const noexcept
	{
		unsigned int tail(tail_);
		unsigned int head(head_.load(std::memory_order_relaxed));
		if (head > tail) tail += capacity_;
		return tail - head;
	}

	Event const &Events::operator[](unsigned int index) const noexcept
	{
		index = (index + head_) % capacity_;
		return events_[index];
	}
	void Events::confirm(unsigned int count/* = std::numeric_limits< unsigned int >::max()*/) noexcept
	{
		unsigned int tail(tail_);
		unsigned int head(head_.load(std::memory_order_relaxed));
		if (head > tail) tail += capacity_;
		unsigned int const curr_size(tail - head);
		if (count > curr_size) count = curr_size;
		head = (head + count) % capacity_;
		head_.store(head, std::memory_order_release);
	}
	Events::const_iterator Events::begin() const noexcept
	{
		return Events::const_iterator(this, size());
	}
	Events::const_iterator Events::end() const noexcept
	{
		return Events::const_iterator();
	}
}}}


