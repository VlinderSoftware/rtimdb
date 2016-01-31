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
#ifndef vlinder_rtimdb_details_polldescriptor_hpp
#define vlinder_rtimdb_details_polldescriptor_hpp

#include "eventclass.hpp"

namespace Vlinder { namespace RTIMDB { namespace Details {
	class PollResult;
	class PollDescriptor
	{
	public :
		PollDescriptor()
			: poll_class_0_(false)
			, poll_class_1_(false)
			, poll_class_2_(false)
			, poll_class_3_(false)
		{ /* no-op */ }
		PollDescriptor(EventClass class_to_poll)
			: poll_class_0_(class_to_poll == EventClass::class_0__)
			, poll_class_1_(class_to_poll == EventClass::class_1__)
			, poll_class_2_(class_to_poll == EventClass::class_2__)
			, poll_class_3_(class_to_poll == EventClass::class_3__)
		{ /* no-op */ }
		PollDescriptor(EventClass first_class_to_poll, EventClass second_class_to_poll)
			: poll_class_0_((first_class_to_poll == EventClass::class_0__) || (second_class_to_poll == EventClass::class_0__))
			, poll_class_1_((first_class_to_poll == EventClass::class_1__) || (second_class_to_poll == EventClass::class_1__))
			, poll_class_2_((first_class_to_poll == EventClass::class_2__) || (second_class_to_poll == EventClass::class_2__))
			, poll_class_3_((first_class_to_poll == EventClass::class_3__) || (second_class_to_poll == EventClass::class_3__))
		{ /* no-op */ }
		PollDescriptor(EventClass first_class_to_poll, EventClass second_class_to_poll, EventClass third_class_to_poll)
			: poll_class_0_((first_class_to_poll == EventClass::class_0__) || (second_class_to_poll == EventClass::class_0__) || (third_class_to_poll == EventClass::class_0__))
			, poll_class_1_((first_class_to_poll == EventClass::class_1__) || (second_class_to_poll == EventClass::class_1__) || (third_class_to_poll == EventClass::class_1__))
			, poll_class_2_((first_class_to_poll == EventClass::class_2__) || (second_class_to_poll == EventClass::class_2__) || (third_class_to_poll == EventClass::class_2__))
			, poll_class_3_((first_class_to_poll == EventClass::class_3__) || (second_class_to_poll == EventClass::class_3__) || (third_class_to_poll == EventClass::class_3__))
		{ /* no-op */ }
		PollDescriptor(EventClass first_class_to_poll, EventClass second_class_to_poll, EventClass third_class_to_poll, EventClass fourth_class_to_poll)
			: poll_class_0_((first_class_to_poll == EventClass::class_0__) || (second_class_to_poll == EventClass::class_0__) || (third_class_to_poll == EventClass::class_0__) || (fourth_class_to_poll == EventClass::class_0__))
			, poll_class_1_((first_class_to_poll == EventClass::class_1__) || (second_class_to_poll == EventClass::class_1__) || (third_class_to_poll == EventClass::class_1__) || (fourth_class_to_poll == EventClass::class_1__))
			, poll_class_2_((first_class_to_poll == EventClass::class_2__) || (second_class_to_poll == EventClass::class_2__) || (third_class_to_poll == EventClass::class_2__) || (fourth_class_to_poll == EventClass::class_2__))
			, poll_class_3_((first_class_to_poll == EventClass::class_3__) || (second_class_to_poll == EventClass::class_3__) || (third_class_to_poll == EventClass::class_3__) || (fourth_class_to_poll == EventClass::class_3__))
		{ /* no-op */ }

	private :
		bool poll_class_0_;
		bool poll_class_1_;
		bool poll_class_2_;
		bool poll_class_3_;

		friend class PollResult; // this object is opaque, except for PollResult
	};
}}}

#endif

