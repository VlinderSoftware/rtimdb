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
#ifndef vlinder_rtimdb_core_details_bubblesort_hpp
#define vlinder_rtimdb_core_details_bubblesort_hpp

namespace Vlinder { namespace RTIMDB { namespace Core { namespace Details {
	template < typename It, typename Cmp >
	void bubbleSort(It beg, It end, Cmp cmp)
	{
		using std::swap;
		bool swapped(false);
		if (beg == end) return;
		do
		{
			swapped = false;
			It curr(beg);
			It prev(curr++);
			while (curr != end)
			{
				// the predicate is less-than, but we want a stable sort, so we only swap if rhs < lhs
				if (cmp(*curr, *prev))
				{
					swap(*prev, *curr);
					swapped = true;
				}
				else
				{ /* not out of order */ }
				prev = curr++;
			}
		} while (swapped);
	}
}}}}

#endif
