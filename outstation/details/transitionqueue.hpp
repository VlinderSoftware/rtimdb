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
#ifndef vlinder_rtimdb_outstation_details_transitionqueue_hpp
#define vlinder_rtimdb_outstation_details_transitionqueue_hpp

#include "details/prologue.hpp"
#include "exceptions/exception.hpp"
#include "../transition.hpp"
#include "rtimdb_config.hpp"
#include <stdexcept>
#include <atomic>
#include "transitionqueuetransaction.hpp"
#include "timestamp.hpp"
#include "variant.hpp"
#include "transitiontransactionentry.hpp"

namespace Vlinder { namespace RTIMDB { namespace Outstation { namespace Details {
	class RTIMDB_OUTSTATION_API TransitionQueue
	{
	public :
		enum TransitionQueueErrors {
			  queue_empty__
			, queue_full__
		};
		typedef Vlinder::Exceptions::Exception< std::runtime_error, TransitionQueueErrors, queue_empty__ > QueueEmpty;
		typedef Vlinder::Exceptions::Exception< std::runtime_error, TransitionQueueErrors, queue_full__ > QueueFull;

		TransitionQueue();
		~TransitionQueue();

		TransitionQueueTransaction beginTransaction(Timestamp const &timestamp) noexcept;
		void commit(TransitionQueueTransaction const &transaction) noexcept;
		void signalOverflow() noexcept;

		bool empty() const noexcept;
		unsigned int capacity() const noexcept;
		unsigned int size() const noexcept;

		bool overflow() const;

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		TransitionTransactionEntry&& front() const;
#endif
		std::pair< TransitionTransactionEntry&&, bool > front(RTIMDB_NOTHROW_PARAM_1) const noexcept;
		void pop() noexcept;

	private :
		TransitionQueue(TransitionQueue const&) = delete;
		TransitionQueue& operator=(TransitionQueue const&) = delete;

		TransitionTransactionEntry transitions_[RTIMDB_TRANSITION_QUEUE_CAPACITY];
		std::atomic< unsigned int > head_;
		std::atomic< unsigned int > tail_;
		unsigned int cached_head_;
		mutable unsigned int cached_tail_;

		mutable std::atomic< bool > overflow_;
	};
}}}}

#endif

