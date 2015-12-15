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
#include "iterator.hpp"
#include "locator.hpp"
#include "../datastore.hpp"
#include <utility>

namespace Vlinder { namespace RTIMDB { namespace Core { namespace Details {
    Iterator::Iterator()
		: data_store_(0)
		, at_end_(true)
	{ /* no-op */ }
    Iterator::Iterator(
          DataStore *data_store
		, ROTransaction const &transaction
		, Locator const &locator
		)
		: data_store_(data_store)
		, transaction_(transaction)
		, at_end_(PointType::_type_count__ == locator.first)
		, locator_(locator)
	{ /* no-op */ }
    Iterator::Iterator(
          DataStore *data_store
		, std::pair < ROTransaction, Errors > const &maybe_transaction
		, Locator const &locator
		)
		: data_store_(data_store)
		, transaction_(maybe_transaction.first)
		, at_end_((Errors::no_error__ != maybe_transaction.second) || (PointType::_type_count__ == locator.first))
		, locator_(locator)
	{ /* no-op */ }
	Iterator::~Iterator()
	{ /* no-op */ }

    bool Iterator::operator==(Iterator const &rhs) const
	{
		return at_end_ == rhs.at_end_
			|| (   data_store_ == rhs.data_store_
				&& transaction_ == rhs.transaction_
				&& locator_ == rhs.locator_
			)
			;
	}
    bool Iterator::operator!=(Iterator const &rhs) const
	{
		return !(*this == rhs);
	}

    Iterator& Iterator::operator++()
    {
        if (data_store_)
        {
            locator_ = data_store_->advance(locator_);
            at_end_ = (PointType::_type_count__ == locator_.first);
        }
        else
        { /* nothing we can do here */ }
        return *this;
    }

    Iterator Iterator::operator++(int)
    {
        Iterator retval(*this);
        ++(*this);
        return retval;
    }
}}}}


