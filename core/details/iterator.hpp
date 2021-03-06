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
#ifndef vlinder_rtimdb_core_details_iterator_hpp
#define vlinder_rtimdb_core_details_iterator_hpp

#include "../../details/prologue.hpp"
#include "transaction.hpp"
#include "locator.hpp"
#include "../../exceptions.hpp"

namespace Vlinder { namespace RTIMDB { namespace Core { 
	class DataStore;
	struct Point;
	namespace Details {
	class RTIMDB_API Iterator : public std::iterator< std::input_iterator_tag, Point >
	{
	public :
		Iterator();
		Iterator(
			  DataStore *data_store
			, ROTransaction const &transaction
			, Locator const &locator
			);
		Iterator(
			  DataStore *data_store
			, std::pair < ROTransaction, Errors > const &maybe_transaction
			, Locator const &locator
			);
		Iterator(Iterator const &) = default;
		~Iterator();

		Iterator& operator=(Iterator const &) = default;

		bool operator==(Iterator const &rhs) const;
		bool operator!=(Iterator const &rhs) const;

        Iterator& operator++();
        Iterator operator++(int);

	private:
		DataStore *data_store_;
		ROTransaction transaction_;
		bool at_end_;
		Locator locator_;
	};
}}}}

#endif

