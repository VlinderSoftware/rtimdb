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
#include "eventqueue.hpp"
#include "exceptions/contract.hpp"

using namespace std;

namespace Vlinder { namespace RTIMDB { namespace Details {
	EventQueue::EventQueue()
		: head_(0)
		, tail_(0)
		, cached_head_(0)
		, cached_tail_(0)
		, overflow_(false)
	{ /* no-op */ }
	EventQueue::~EventQueue()
	{ /* no-op */ }

	bool EventQueue::empty() const noexcept
	{
		// update the cache here so we get the latest info
		cached_tail_ = tail_.load(memory_order_acquire);
		return head_.load(memory_order_relaxed) == cached_tail_;
	}
	unsigned int EventQueue::capacity() const noexcept
	{
		return sizeof(events_) / sizeof(events_[0]);
	}
	unsigned int EventQueue::size() const noexcept
	{
		// update the cache here
		unsigned int tail(cached_tail_ = tail_.load(memory_order_acquire));
		unsigned int head(head_.load(memory_order_relaxed));
		if (head > tail) tail += capacity();
		return tail - head;
	}
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	Event EventQueue::front() const
	{
		auto result(front(nothrow));
		throwException(result.second);
		return result.first;
	}
#endif
	pair< Event, Errors > EventQueue::front(RTIMDB_NOTHROW_PARAM_1) const noexcept
	{
		unsigned int head(head_.load(memory_order_relaxed));
		unsigned int tail(cached_tail_);
		if (head == tail)
		{
			cached_tail_ = tail = tail_.load(memory_order_acquire);
		}
		else
		{ /* in the typical use-case, our cache is up-to-date when we start getting and popping commands
		     and we really don't want to update the cache unless we have to */ }
		if (head == tail)
		{
			return make_pair(Event(), Errors::queue_full__);
		}
		else
		{ /* all is well */ }
		return make_pair(events_[head], Errors::no_error__);
	}
	void EventQueue::pop() noexcept
	{
		unsigned int head(head_.load(memory_order_relaxed));
		unsigned int tail(cached_tail_);
		/* DO NOT update the cache here: you can lose commands if you do (so pop is a no-op if you don't call
		 * it with either empty(), size() or front() and it appears to be empty when called) */
		if (head != tail)
		{
			head_.store((head + 1) % capacity(), memory_order_release);
		}
		else
		{ /* nothing to pop */ }
	}

	void EventQueue::clear() noexcept
	{
		head_ = tail_.load();
	}

	bool EventQueue::overflow() const noexcept
	{
		return overflow_.exchange(false);
	}

#ifdef RTIMDB_ALLOW_EXCEPTIONS
	void EventQueue::push(Event &&event) { throwException(push(move(event), nothrow)); }
#endif

	Errors EventQueue::push(Event &&event RTIMDB_NOTHROW_PARAM) noexcept
	{
		unsigned int tail(tail_.load(memory_order_relaxed));
		unsigned int head(cached_head_ = head_.load(memory_order_acquire));
		if ((tail + 1) % capacity() == head)
		{
			overflow_ = true;
			return Errors::queue_full__;
		}
		else
		{ /* not full */ }
		events_[tail] = move(event);
		tail_.store((tail + 1) % capacity(), memory_order_release);

		return Errors::no_error__;
	}
}}}


