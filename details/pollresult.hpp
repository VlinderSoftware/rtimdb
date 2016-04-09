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
#ifndef vlinder_rtimdb_details_pollresult_hpp
#define vlinder_rtimdb_details_pollresult_hpp

#include "prologue.hpp"
#include "polldescriptor.hpp"
#include "variant.hpp"
#include "event.hpp"
#include "../core/details/transaction.hpp"
#include "../core/pointvalue.hpp"

namespace Vlinder { namespace RTIMDB { namespace Details {
	class Consumer;
	class RTIMDB_API PollResult
	{
	public :
		PollResult()
			: consumer_(0)
			, size_known_(false)
		{ /* no-op */ }
		PollResult(PollDescriptor const &descriptor, Consumer *consumer, Core::Details::ROTransaction &&transaction)
			: descriptor_(descriptor)
			, consumer_(consumer)
			, transaction_(transaction)
			, size_known_(false)
		{ /* no-op */ }
		PollResult(PollResult const &) = delete;
		PollResult& operator=(PollResult const &) = delete;
		PollResult(PollResult &&) = default;
		PollResult& operator=(PollResult &&) = default;

		unsigned int size() const noexcept;

		Variant< Event, Core::PointValue > operator[](unsigned int index) const noexcept;

		void confirm() noexcept;

	private :
		void awaitTransactionDone() const noexcept;
		unsigned int getEventCount(EventClass event_class) const noexcept;
		Event getClassEvent(EventClass event_class, unsigned int index) const noexcept;
		Core::PointValue getStatic(unsigned int index) const noexcept;

		PollDescriptor descriptor_;
		Consumer *consumer_;
		Core::Details::ROTransaction transaction_;
		mutable bool size_known_;
		mutable unsigned int result_size_;
		mutable unsigned int class_0_size_;
		mutable unsigned int class_1_size_;
		mutable unsigned int class_2_size_;
		mutable unsigned int class_3_size_;
	};
}}}

#endif

