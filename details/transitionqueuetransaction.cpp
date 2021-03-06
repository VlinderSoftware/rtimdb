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
 #include "transitionqueuetransaction.hpp"
 #include "exceptions/contract.hpp"
 #include "transitiontransactionentry.hpp"
 #include "../database.hpp"

namespace Vlinder { namespace RTIMDB { namespace Details {
	TransitionQueueTransaction::~TransitionQueueTransaction()
	{ /* no-op */ }

	TransitionTransactionEntry& TransitionQueueTransaction::operator[](unsigned int index) noexcept
	{
		pre_condition(index < size());
		index = (index + initial_tail_) % queue_size_;
		pre_condition(0
			|| (head_ <= tail_ && index >= initial_tail_ && index <= tail_)
			|| (head_ > tail_ && head_ <= initial_tail_ && ((index >= initial_tail_ && index <= (tail_ + queue_size_)) || (index <= tail_)))
			|| (head_ > tail_ && head_ > initial_tail_ && index >= initial_tail_ && index <= tail_)
			);
		tail_ = (index + 1) % queue_size_;
		return transitions_[index];
	}
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	void TransitionQueueTransaction::add(Transition &&transition) { throwException(add(std::move(transition), std::nothrow)); }
#endif
	Errors TransitionQueueTransaction::add(Transition &&transition RTIMDB_NOTHROW_PARAM) noexcept
	{
		if (size() == next_index_) return Errors::too_many_transitions__;
		auto index(next_index_++);
		index = (index + initial_tail_) % queue_size_;
		tail_ = (index + 1) % queue_size_;
		transitions_[index] = std::move(transition);
		return Errors::no_error__;
	}
	void TransitionQueueTransaction::commit()
	{
		queue_->commit(*this);
	}

	unsigned int TransitionQueueTransaction::size() const noexcept
	{
		unsigned int head(head_);
		unsigned int tail(tail_);
		if (head < tail) head += queue_size_;
		return (head - tail) - 1;
	}

	TransitionQueueTransaction::TransitionQueueTransaction(TransitionTransactionEntry *transitions, unsigned int queue_size, unsigned int head, unsigned int tail, TransitionQueue *queue)
		: transitions_(transitions)
		, queue_size_(queue_size)
		, head_(head)
		, tail_(tail)
		, initial_tail_(tail)
		, next_index_(0)
		, queue_(queue)
	{ /* no-op */ }
}}}


