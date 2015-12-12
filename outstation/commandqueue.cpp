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
#include "commandqueue.hpp"

using namespace std;

namespace Vlinder { namespace RTIMDB { namespace Outstation {
	CommandQueue::CommandQueue()
		: head_(0)
		, tail_(0)
		, cached_head_(0)
		, cached_tail_(0)
		, overflow_(false)
	{ /* no-op */ }
	CommandQueue::~CommandQueue()
	{ /* no-op */ }
	bool CommandQueue::empty() const noexcept
	{
		// update the cache here so we get the latest info
		cached_tail_ = tail_.load(memory_order_acquire);
		return head_.load(memory_order_relaxed) == cached_tail_;
	}
	unsigned int CommandQueue::capacity() const noexcept
	{
		return sizeof(commands_) / sizeof(commands_[0]);
	}
	unsigned int CommandQueue::size() const noexcept
	{
		// update the cache here
		unsigned int tail(cached_tail_ = tail_.load(memory_order_acquire));
		unsigned int head(head_.load(memory_order_relaxed));
		if (head > tail) tail += capacity();
		return tail - head;
	}
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	Command&& CommandQueue::front() const
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
	pair< Command&&, bool > CommandQueue::front(RTIMDB_NOTHROW_PARAM_1) const noexcept
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
			return make_pair(Command(), false);
		}
		else
		{ /* all is well */ }
		return make_pair(commands_[head], true);
	}
	void CommandQueue::pop() noexcept
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

	bool CommandQueue::overflow() const
	{
		return overflow_.exchange(false);
	}

#ifdef RTIMDB_ALLOW_EXCEPTIONS
	void CommandQueue::push(Command const &command)
	{
		if (!push(command, nothrow))
		{
			throw QueueFull("Push failed - queue full");
		}
	}
#endif
	bool CommandQueue::push(Command const &command RTIMDB_NOTHROW_PARAM) noexcept
	{
		unsigned int tail(tail_.load(memory_order_relaxed));
		unsigned int head(cached_head_);
		if (((tail + 1) % capacity()) == head)
		{
			cached_head_ = head = head_.load(memory_order_acquire);
		}
		else
		{ /* front can only increase since the last time we read it, which means we can only get
		   * more space to push into.
		   * If we still have space left from the last time we read, we don't have to read again. */
		}
		if ((tail + 1) % capacity() == head)
		{
			overflow_ = true;
			return false;
		}
		else
		{ /* not full */ }
		commands_[tail] = command;
		tail_.store((tail + 1) % capacity(), memory_order_release);

		return true;
	}
}}}


