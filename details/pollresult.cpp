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
#include "pollresult.hpp"
#include "consumer.hpp"
#include <algorithm>
#include <type_traits>

using namespace std;

namespace Vlinder { namespace RTIMDB { namespace Details {
	unsigned int PollResult::size() const noexcept
	{
		if (!size_known_)
		{
			awaitTransactionDone();
			// class 0 returns the values of all the mapped points. We do not allow 
			// (by design, not enforced) the mapping to change after initialization,
			// so the size of the mapping is the size of the class 0 response.
			class_0_size_ = descriptor_.poll_class_0_ ? consumer_->getMappingSize() : 0;
			// for classes 1, 2 and 3, it's slightly more complicated: we return any  
			// event that has occurred to lead to the version of the database
			// we're polling, so we can't just count the total number of events:
			// we need to count the events for which the version <= ours 
			// (DataStore::startTransaction does a post-increment on the version)
			class_1_size_ = descriptor_.poll_class_1_ ? getEventCount(EventClass::class_1__) : 0;
			class_2_size_ = descriptor_.poll_class_1_ ? getEventCount(EventClass::class_2__) : 0;
			class_3_size_ = descriptor_.poll_class_1_ ? getEventCount(EventClass::class_3__) : 0;
			result_size_ = class_0_size_ + class_1_size_ + class_2_size_ + class_3_size_;
			size_known_ = true;
		}
		else
		{ /* memo-ized: we already know the answer */ }

		return result_size_;
	}

	Variant< Event, Core::Point > PollResult::operator[](unsigned int index) const noexcept
	{
		size();
		if (index < class_1_size_) return Variant< Event, Core::Point >(getClassEvent(EventClass::class_1__, index));
		else if (index < (class_1_size_ + class_2_size_)) return Variant< Event, Core::Point >(getClassEvent(EventClass::class_2__, index - class_1_size_));
		else if (index < (class_1_size_ + class_2_size_ + class_3_size_)) return Variant< Event, Core::Point >(getClassEvent(EventClass::class_3__, index - (class_1_size_ + class_2_size_)));
		else return Variant< Event, Core::Point >(getStatic(index - (class_1_size_ + class_2_size_ + class_2_size_)));
	}

	void PollResult::confirm() noexcept
	{
		size();
		consumer_->getEvents(EventClass::class_1__).confirm(class_1_size_);
		consumer_->getEvents(EventClass::class_2__).confirm(class_2_size_);
		consumer_->getEvents(EventClass::class_3__).confirm(class_3_size_);
	}

	Event PollResult::getClassEvent(EventClass event_class, unsigned int index) const noexcept
	{
		auto events(consumer_->getEvents(event_class));
		return events[index];
	}
	Core::Point PollResult::getStatic(unsigned int index) const noexcept
	{
		return consumer_->getPointByIndex(transaction_, index);
	}

	void PollResult::awaitTransactionDone() const noexcept
	{
		// In a system in which more than one thread can call update (e.g. 
		// a system with a comm task and a logic task) there is a chance 
		// that the version of the database we're looking at is not complete 
		// yet. Hence, we need to check and wait for the transaction to 
		// complete.
		consumer_->awaitTransactionDone(transaction_.getVersion());
	}

	unsigned int PollResult::getEventCount(EventClass event_class) const noexcept
	{
		auto events(consumer_->getEvents(event_class));
		auto count(count_if(
			  events.begin()
			, events.end()
			, [=](typename std::remove_reference< decltype(*events.begin()) >::type const &event)
				{
					return event.value_.version_ <= transaction_.getVersion(); 
				})
			);
		return count;
	}
}}}


