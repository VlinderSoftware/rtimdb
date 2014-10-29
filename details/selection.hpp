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
#ifndef vlinder_rtimdb_details_selection_hpp
#define vlinder_rtimdb_details_selection_hpp

#include <memory>
#include <atomic>

namespace Vlinder { namespace RTIMDB { namespace Details {
	struct Selection_
	{
		Selection_()
			: id_(0)
		{ /* no-op */ }

		std::atomic< unsigned int > id_; // atomic for asynchronous release
	};
	typedef std::pair< std::shared_ptr< Selection_ const >, unsigned int > Selection;
}}}

#endif
