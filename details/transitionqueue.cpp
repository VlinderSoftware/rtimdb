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
#include "transitionqueue.hpp"
#include "exceptions/contract.hpp"

using namespace std;

namespace Vlinder { namespace RTIMDB { namespace Details {
	TransitionQueue::TransitionQueue()
		: head_(0)
		, tail_(0)
		, cached_head_(0)
		, cached_tail_(0)
		, overflow_(false)
	{ /* no-op */ }
	TransitionQueue::~TransitionQueue()
	{ /* no-op */ }

	TransitionQueueTransaction TransitionQueue::beginTransaction(Timestamp const &timestamp) noexcept
	{
		unsigned int tail(tail_.load(memory_order_relaxed));
		unsigned int head(cached_head_ = head_.load(memory_order_acquire));
		if ((tail + 1) % capacity() == head)
		{
			overflow_ = true;
			return TransitionQueueTransaction(nullptr, 0, 0, 0, nullptr);
		}
		else
		{ /* not full */ }
		transitions_[tail] = timestamp;
		return TransitionQueueTransaction(transitions_, capacity(), head, tail + 1, this);
	}
	void TransitionQueue::commit(TransitionQueueTransaction const &transaction) noexcept
	{
		pre_condition(transaction.transitions_ == transitions_);
		tail_.store(transaction.tail_, memory_order_release);
	}
	void TransitionQueue::signalOverflow() noexcept
	{
		overflow_ = true;
	}

	bool TransitionQueue::empty() const noexcept
	{
		// update the cache here so we get the latest info
		cached_tail_ = tail_.load(memory_order_acquire);
		return head_.load(memory_order_relaxed) == cached_tail_;
	}
	unsigned int TransitionQueue::capacity() const noexcept
	{
		return sizeof(transitions_) / sizeof(transitions_[0]);
	}
	unsigned int TransitionQueue::size() const noexcept
	{
		// update the cache here
		unsigned int tail(cached_tail_ = tail_.load(memory_order_acquire));
		unsigned int head(head_.load(memory_order_relaxed));
		if (head > tail) tail += capacity();
		return tail - head;
	}
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	TransitionTransactionEntry TransitionQueue::front() const
	{
		auto result(front(nothrow));
		if (!result.second)
		{
			throw QueueEmpty("Queue empty");
		}
		else
		{ /* all is well */ }

		return move(result.first);
	}
#endif
	pair< TransitionTransactionEntry, bool > TransitionQueue::front(RTIMDB_NOTHROW_PARAM_1) const noexcept
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
			return make_pair(TransitionTransactionEntry(), false);
		}
		else
		{ /* all is well */ }
		return make_pair(transitions_[head], true);
	}
	void TransitionQueue::pop() noexcept
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

	bool TransitionQueue::overflow() const
	{
		return overflow_.exchange(false);
	}
}}}


