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
#ifndef vlinder_rtimdb_outstation_details_transitionqueuetransaction_hpp
#define vlinder_rtimdb_outstation_details_transitionqueuetransaction_hpp

#include "../../details/prologue.hpp"

namespace Vlinder { namespace RTIMDB { namespace Outstation { namespace Details {
	class TransitionQueue;
	struct TransitionTransactionEntry;
	class RTIMDB_API TransitionQueueTransaction
	{
	public :
		~TransitionQueueTransaction();

		TransitionQueueTransaction(TransitionQueueTransaction const&) = default;
		TransitionQueueTransaction& operator=(TransitionQueueTransaction const&) = default;

		TransitionTransactionEntry& operator[](unsigned int index) noexcept;
		unsigned int size() const noexcept;

	private : // friends-only API
		TransitionQueueTransaction(TransitionTransactionEntry *transitions, unsigned int queue_size, unsigned int head, unsigned int tail);

	private :
		TransitionTransactionEntry *transitions_;
		unsigned int queue_size_;
		unsigned int head_;
		unsigned int tail_;
		unsigned int initial_tail_;

		friend class TransitionQueue;
	};
}}}}

#endif

