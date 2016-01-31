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
#ifndef vlinder_rtimdb_details_events_hpp
#define vlinder_rtimdb_details_events_hpp

#include "prologue.hpp"
#include "../core/point.hpp"
#include "timestamp.hpp"
#include <limits>
#include "eventiterator.hpp"

namespace Vlinder { namespace RTIMDB { namespace Details {
	struct Event;
	class EventQueue;
	class RTIMDB_API Events
	{
	public :
		typedef EventIterator const_iterator;

		Events(EventQueue &queue);

		unsigned int size() const noexcept;
		Event const &operator[](unsigned int index) const noexcept;
		void confirm(unsigned int count = std::numeric_limits< unsigned int >::max()) noexcept;

		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;

	private :
		Event *events_;
		unsigned int capacity_;
		std::atomic< unsigned int > &head_;
		unsigned int tail_;
	};
}}}

#endif

