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
#ifndef vlinder_rtimdb_details_eventqueue_hpp
#define vlinder_rtimdb_details_eventqueue_hpp

#include "prologue.hpp"
#include "rtimdb_config.hpp"
#include "event.hpp"
#include "../exceptions.hpp"
#include <stdexcept>
#include <atomic>

namespace Vlinder { namespace RTIMDB { namespace Details {
	class RTIMDB_API EventQueue
	{
	public :
		EventQueue();
		~EventQueue();

		bool empty() const noexcept;
		unsigned int capacity() const noexcept;
		unsigned int size() const noexcept;

		bool overflow() const noexcept;

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		Event&& front() const;
#endif
		std::pair< Event&&, Errors > front(RTIMDB_NOTHROW_PARAM_1) const noexcept;
		void pop() noexcept;
		void clear() noexcept;

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		void push(Event &&event);
#endif
		Errors push(Event &&event RTIMDB_NOTHROW_PARAM) noexcept;

	private :
		EventQueue(EventQueue const&) = delete;
		EventQueue& operator=(EventQueue const&) = delete;

		Event events_[RTIMDB_EVENT_QUEUE_CAPACITY];
		std::atomic< unsigned int > head_;
		std::atomic< unsigned int > tail_;
		unsigned int cached_head_;
		mutable unsigned int cached_tail_;

		mutable std::atomic< bool > overflow_;
	};
}}}

#endif

