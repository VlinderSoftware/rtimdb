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
#include "datastore.hpp"
#include <algorithm>
#include <cstring>
#include <atomic>
#include "exceptions/contract.hpp"
#include "details/bubblesort.hpp"
using namespace std;

namespace Vlinder { namespace RTIMDB { namespace Core {
	DataStore::DataStore()
		: curr_version_(1) // start at 1 so we don't start frozen
		, next_cell_(0)
	{
		for_each(std::begin(frozen_versions_), std::end(frozen_versions_), [](decltype(frozen_versions_[0]) &frozen_version){ frozen_version = 0; });
		memset(start_index_, 0, sizeof(start_index_));
		memset(points_, 0, sizeof(points_));
	}
	DataStore::~DataStore()
	{ /* no-op */ }
	
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	unsigned int DataStore::insert(PointType type, PointValue const &value, Flags const &flags, Timestamp const &timestamp)
	{
		auto insert_result(insert(type, value, flags, timestamp, nothrow));
		throwException(insert_result.second);
		return insert_result.first;
	}
#endif
	std::pair< unsigned int, Errors > DataStore::insert(PointType type, PointValue const &value, Flags const &flags, Timestamp const &timestamp RTIMDB_NOTHROW_PARAM) throw()
	{
		using std::begin;
		using std::end;
		unsigned int const type_start_index(start_index_[static_cast< unsigned int >(type)]);
		unsigned int const type_end_index(start_index_[static_cast< unsigned int >(type) + 1]);
		unsigned int const next_index(type_end_index);
		unsigned int const retval(type_end_index - type_start_index);
		unsigned int const last_index(start_index_[static_cast<unsigned int>(PointType::_type_count__)]);
		if (last_index == (sizeof(points_) / sizeof(points_[0]))) return make_pair(-1, Errors::database_full__);
		bool dismiss(false);
		auto rollback = [&](void *){ if (!dismiss) { --next_cell_; move(points_ + next_index + 1, end(points_), points_ + next_index); } };
		unique_ptr< void, decltype(rollback) > scope_guard(&rollback, rollback);

		auto curr_begin(begin(points_) + next_index);
		auto curr_end(begin(points_) + last_index);
		auto target_end(begin(points_) + last_index + 1);
		move_backward(curr_begin, curr_end, target_end);
		points_[next_index] = &cells_[next_cell_++];
		auto set_result((*points_[next_index]).set(type, value, flags, timestamp));
		if (Errors::no_error__ != set_result) return make_pair(-1, set_result);

		dismiss = true;
		// as of this, cannot fail
		unsigned int * first = (start_index_ + static_cast<unsigned int>(type) + 1);
		for_each(first, end(start_index_), [](unsigned int &val){ ++val; });

		return make_pair((unsigned int)retval, Errors::no_error__);
	}

	DataStore::const_iterator DataStore::begin()
	{
		return Details::Iterator(this, startROTransaction(), Details::Locator(getPointTypeAtOffset(0), 0));
	}

	DataStore::const_iterator DataStore::end()
	{
		return Details::Iterator();
	}

#ifdef RTIMDB_ALLOW_EXCEPTIONS
	void DataStore::update(PointType point_type, unsigned int index, Details::Optional< PointValue > const &new_value, Details::Optional< Flags > const &new_flags, Timestamp const &timestamp)										{ throwException(update(point_type, index, new_value, new_flags, timestamp, nothrow)); }
	void DataStore::update(Details::Transaction &transaction, PointType point_type, unsigned int index, Details::Optional< PointValue > const &new_value, Details::Optional< Flags > const &new_flags, Timestamp const &timestamp)	{ throwException(update(transaction, point_type, index, new_value, new_flags, timestamp, nothrow)); }
#endif

	Errors DataStore::update(PointType point_type, unsigned int index, Details::Optional< PointValue > const &new_value, Details::Optional< Flags > const &new_flags, Timestamp const &timestamp RTIMDB_NOTHROW_PARAM) throw()
	{
		auto transaction(startTransaction(RTIMDB_NOTHROW_ARG_1));
		if (transaction.second != Errors::no_error__) return transaction.second;
		auto result(update(transaction.first, point_type, index, new_value, new_flags, timestamp RTIMDB_NOTHROW_ARG));
		if (result != Errors::no_error__) return result;
		return commit(transaction.first RTIMDB_NOTHROW_ARG);
	}
	Errors DataStore::update(Details::Transaction &transaction, PointType point_type, unsigned int index, Details::Optional< PointValue > const &new_value, Details::Optional< Flags > const &new_flags, Timestamp const &timestamp RTIMDB_NOTHROW_PARAM) throw()
	{
		return transaction.push(point_type, index, new_value, new_flags, timestamp);
	}

#ifdef RTIMDB_ALLOW_EXCEPTIONS
	Point DataStore::read(PointType type, unsigned int index) const
	{
		auto result(read(type, index RTIMDB_NOTHROW_ARG));
		if (Errors::no_error__ == result.second)
		{
			return result.first.get();
		}
		else
		{
			throwException(result.second);
		}
		throw logic_error("Unreachable code");
	}
#endif

	pair< Details::Optional< Point >, Errors > DataStore::read(PointType type, unsigned int index RTIMDB_NOTHROW_PARAM) const throw()
	{
		auto fetch_result(fetch(type, index));
		return make_pair((Errors::no_error__ == fetch_result.second) ? (*fetch_result.first)->get() : Details::Optional< Point >(), fetch_result.second);
	}

#ifdef RTIMDB_ALLOW_EXCEPTIONS
	Point DataStore::read(Details::ROTransaction const &transaction, PointType type, unsigned int index) const
	{
		auto result(read(transaction, type, index RTIMDB_NOTHROW_ARG));
		if (Errors::no_error__ == result.second)
		{
			return result.first.get();
		}
		else
		{
			throwException(result.second);
		}
		throw logic_error("Unreachable code");
	}
#endif
	std::pair< Details::Optional< Point >, Errors > DataStore::read(Details::ROTransaction const &transaction, PointType type, unsigned int index RTIMDB_NOTHROW_PARAM) const throw()
	{
		auto fetch_result(fetch(type, index));
		return make_pair((Errors::no_error__ == fetch_result.second) ? (*fetch_result.first)->get(transaction.getVersion()) : Details::Optional< Point >(), fetch_result.second);
	}

#ifdef RTIMDB_ALLOW_EXCEPTIONS
	Details::Transaction DataStore::startTransaction()
	{
		auto result(startTransaction(nothrow));
		if (Errors::no_error__ == result.second)
		{
			return std::move(result.first);
		}
		else
		{
			throwException(result.second);
		}
		throw logic_error("Unreachable code");
	}
	void DataStore::commit(Details::Transaction &transaction)
	{
		auto result(commit(transaction, nothrow));
		throwException(result);
	}
	Details::ROTransaction DataStore::startROTransaction()
	{
		auto result(startROTransaction(nothrow));
		if (Errors::no_error__ == result.second)
		{
			return std::move(result.first);
		}
		else
		{
			throwException(result.second);
		}
		throw logic_error("Unreachable code");
	}
#endif
	pair < Details::Transaction, Errors > DataStore::startTransaction(RTIMDB_NOTHROW_PARAM_1) throw()
	{
		return startTransaction_(true);
	}
	Errors DataStore::commit(Details::Transaction &transaction RTIMDB_NOTHROW_PARAM)
	{
		using namespace std;
		// three phases:
		// phase   I: sort out which of the transitions in the transaction actually change something
		// phase  II: sort the transitions (in-place) so the locks are always in the same order
		// phase III: lock affected cells; fail if version changed or already locked
		// phase  IV: change the affected cells
		// phase   V: unlock the affected cells
		// let's go

		// phase I
		Errors retval(tagTransitions(transaction));
		// phase II
		if (Errors::no_error__ == retval)
		{
			sortTransitions(transaction);
			// phase III
			retval = lockCells(transaction);
			// as of here, this function cannot fail (though any failure that occurs before this may still be returned)
			// phase IV
			if (Errors::no_error__ == retval)
			{
				applyChanges(transaction);
			}
			else
			{ /* something went wrong */ }
			// phase V
			unlockCells(transaction);
		}
		else
		{ /* something went wrong */ }

		return Errors::no_error__;
	}
	pair < Details::ROTransaction, Errors > DataStore::startROTransaction(RTIMDB_NOTHROW_PARAM_1) throw()
	{
		auto result(startTransaction_(false));
		return std::make_pair(std::move(result.first.getROTransaction()), result.second);
	}

	void DataStore::thaw(unsigned int frozen_version)
	{
		using std::begin;
		using std::end;

		unsigned int const last_index(start_index_[static_cast<unsigned int>(PointType::_type_count__)]);
		for_each(cells_, cells_ + last_index, [frozen_version](decltype(cells_[0]) cell){ cell.thaw(frozen_version); });
		auto which(find(begin(frozen_versions_), end(frozen_versions_), frozen_version));
		*which = 0;
	}

	std::pair< Details::Cell< RTIMDB_CELL_SIZE > *const*, Errors > DataStore::fetch(PointType type, unsigned int index) const
	{
		unsigned int const type_start_index(start_index_[static_cast< unsigned int >(type)]);
		unsigned int const type_end_index(start_index_[static_cast< unsigned int >(type)+1]);

		if (
			   (type_end_index == type_start_index)
			|| ((type_end_index - type_start_index) < index)
			)
		{
			return make_pair(nullptr, Errors::unknown_point__);
		}
		else
		{ /* we have the point */ }

		index += type_start_index;
		return make_pair(&points_[index], Errors::no_error__);
	}

	void DataStore::freezeCells(unsigned int frozen_version)
	{
		using std::begin;
		using std::end;
		bool dismiss(false);
		auto cell(begin(cells_));
		auto rollback = [&](void*)
		{
			if (!dismiss)
			{
				for_each(
					  reverse_iterator< remove_reference< decltype(cell) >::type >(cell)
					, reverse_iterator< remove_reference< decltype(cell) >::type >(cells_)
					, [&](Details::Cell< RTIMDB_CELL_SIZE > &c){ c.thaw(frozen_version); }
					);
			}
			else
			{ /* dismissed */ }
		};
		unique_ptr< void, decltype(rollback) > scope_guard(&rollback, rollback);
		unsigned int const last_index(start_index_[static_cast<unsigned int>(PointType::_type_count__)]);
		auto const cells_end(begin(cells_) + last_index);
		for (; cell != cells_end; ++cell)
		{
			cell->freeze(frozen_version);
		}
		dismiss = true;
	}

	unsigned int DataStore::getPointCount(PointType point_type) const
	{
        unsigned int const start_index(start_index_[static_cast< unsigned int >(point_type)]);
        unsigned int const end_index(start_index_[static_cast< unsigned int >(point_type) + 1]);
        return end_index - start_index;
    }

    unsigned int DataStore::getPointOffset(PointType point_type, unsigned int index) const
    {
        unsigned int const type_start_index(start_index_[static_cast< unsigned int >(point_type)]);

        return type_start_index + index;
    }
    PointType DataStore::getPointTypeAtOffset(unsigned int offset) const
    {
        using std::begin;
        using std::end;
        // find the first point we have
        auto const rbound(
            find_if(
                  begin(start_index_)
                , end(start_index_)
                , [offset](decltype(start_index_[0]) start_index)
                    {
                        return offset < start_index;
                    }
                )
            );
        auto const where(rbound - 1);
        auto const point_type_index(distance(begin(start_index_), where));
        invariant(point_type_index <= static_cast< unsigned int >(PointType::_type_count__));
        invariant(point_type_index >= 0);
        auto const point_type(static_cast< PointType >(point_type_index));

        return point_type;
    }

	Details::Locator DataStore::advance(
		  Details::Locator const &curr_location
		) const
	{
		Details::Locator new_location(curr_location);
		++new_location.second;
        if (new_location.second >= getPointCount(new_location.first))
        {
            new_location.first = getPointTypeAtOffset(getPointOffset(new_location.first, 0) + new_location.second);
            new_location.second = 0;
        }
        else
        { /* nothing more to be done */ }
        return new_location;
    }

	pair < Details::Transaction, Errors > DataStore::startTransaction_(bool write_enable) throw()
	{
		unsigned int frozen_version;
		if (write_enable) frozen_version = ++curr_version_;
		else frozen_version = curr_version_;
		// find an empty slot in freeze_indices_
		while (true)
		{
			using std::begin;
			using std::end;
			auto which(find(begin(frozen_versions_), end(frozen_versions_), 0));
			if (end(frozen_versions_) == which)
			{
				return make_pair(Details::Transaction(), Errors::cannot_freeze__);
			}
			else
			{ /* try to CAS */ }
			unsigned int exp(0);
			if (which->compare_exchange_strong(exp, frozen_version))
			{
				auto deleter([this, frozen_version](void *p) { thaw(frozen_version); });
				Details::Transaction retval(which, std::move(deleter));
				freezeCells(retval.getVersion());
				return make_pair(std::move(retval), Errors::no_error__);
			}
			else
			{ /* search again */ }
		}
	}

	Errors DataStore::tagTransitions(Details::Transaction &transaction) noexcept
	{
		using namespace std;
		Errors retval(Errors::no_error__);
		for_each(
			  transaction.begin()
			, transaction.end()
			, [&](decltype(transaction.entries_[0]) &entry){
				if (Errors::no_error__ == retval)
				{
					auto read_result(read(transaction, entry.type_, entry.point_id_ RTIMDB_NOTHROW_ARG));
					retval = read_result.second;
					if (Errors::no_error__ != retval) return;
					//TODO this is where we need to check for flag changes
					entry.transact_state_ = Details::Transaction::Entry::TransactState::unchanged__;
					if (entry.flags_.empty())
					{
						entry.flags_ = read_result.first.get().flags_;
					}
					else
					{ /* flags are set, and should therefore be compared */ }
					if (entry.value_.empty())
					{
						entry.value_ = read_result.first.get().value_;
					}
					else
					{ /* value is set, and should therefore be compared */ }
					if ((entry.flags_.get() == read_result.first.get().flags_) && (entry.value_.get() == read_result.first.get().value_))
					{
						entry.transact_state_ = decltype(entry.transact_state_)::unchanged__;
					}
					else if (entry.flags_.get() != read_result.first.get().flags_)
					{
						entry.transact_state_ = decltype(entry.transact_state_)::flags_changed__;
					}
					else
					{
						entry.transact_state_ = decltype(entry.transact_state_)::value_changed__;
					}
				}
				else
				{ /* some error occurred, can't continue */ }
			  }
			);
		return retval;
	}

	void DataStore::sortTransitions(Details::Transaction &transaction) noexcept
	{
		// why bubble sort, you ask? Bubble sort is very efficient (linear time) if everything is already sorted, which is likely to be the case most of the time
		Details::bubbleSort(
			  transaction.begin()
			, transaction.end()
			, [](std::remove_reference< decltype(transaction.entries_[0]) >::type const &lhs, std::remove_reference< decltype(transaction.entries_[0]) >::type const &rhs) -> bool {
				if (lhs.type_ < rhs.type_) return true;
				if ((lhs.type_ == rhs.type_) && (lhs.point_id_ < rhs.point_id_)) return true;
				return false;
				}
			);
	}

	Errors DataStore::lockCells(Details::Transaction &transaction) noexcept
	{
		using namespace std;
		Errors retval(Errors::no_error__);
		for_each(
			  transaction.begin()
			, transaction.end()
			, [&](decltype(transaction.entries_[0]) &entry){
				if (Errors::no_error__ == retval)
				{
					if (entry.transact_state_ == decltype(entry.transact_state_)::value_changed__)
					{
						auto fetch_result(fetch(entry.type_, entry.point_id_));
						if (Errors::no_error__ == fetch_result.second)
						{
							if ((*fetch_result.first)->lock(transaction.getVersion()))
							{
								entry.transact_state_ = decltype(entry.transact_state_)::pended__;
							}
							else
							{
								retval = Errors::transaction_failed__;
							}
						}
						else
						{
							retval = fetch_result.second;
						}
					}
					else
					{ /* don't transact this one */ }
				}
				else
				{ /* some error occurred, can't continue */ }
			  }
			);

		return retval;
	}

	void DataStore::applyChanges(Details::Transaction &transaction) noexcept
	{
		using namespace std;
		for_each(
			  transaction.begin()
			, transaction.end()
			, [&](decltype(transaction.entries_[0]) &entry){
				auto fetch_result(fetch(entry.type_, entry.point_id_));
				assert(Errors::no_error__ == fetch_result.second);
				entry.value_.get().version_ = transaction.getVersion();
				auto set_result((*fetch_result.first)->set(entry.type_, *entry.value_, *entry.flags_, entry.timestamp_));
				assert(Errors::no_error__ == set_result);
			  }
			);
	}

	void DataStore::unlockCells(Details::Transaction &transaction) noexcept
	{
		for_each(
			  transaction.rbegin()
			, transaction.rend()
			, [&](decltype(transaction.entries_[0]) &entry){
				if (entry.transact_state_ == decltype(entry.transact_state_)::pended__)
				{
					auto fetch_result(fetch(entry.type_, entry.point_id_));
					assert(Errors::no_error__ == fetch_result.second);
					(*fetch_result.first)->unlock();
					entry.transact_state_ = decltype(entry.transact_state_)::transacted__;
				}
				else
				{ /* didn't transact this one */ }
			  }
			);
	}
}}}

