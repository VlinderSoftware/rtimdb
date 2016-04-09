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
#ifndef vlinder_rtimdb_core_details_transaction_hpp
#define vlinder_rtimdb_core_details_transaction_hpp

#include <memory>
#include <atomic>
#include "../../exceptions.hpp"
#include "../pointvalue.hpp"

namespace Vlinder { namespace RTIMDB { 
	class Database;
	namespace Details { class PollResult; }
	namespace Core { 
	class DataStore;
	namespace Details {
	class Iterator;
	class ROTransaction
	{
	public :
		ROTransaction()
		{ /* no-op */ }

		ROTransaction(ROTransaction const &other) = default;
		ROTransaction& operator=(ROTransaction const &other) = default;

		bool operator==(ROTransaction const &other) const
		{
			return true
				&& version_guard_ == other.version_guard_
				;
		}
	protected :
		template < typename Deleter >
		ROTransaction(std::atomic< unsigned int > *version_to_guard, Deleter &&rollback)
			: version_guard_(version_to_guard, std::move(rollback))
		{ /* no-op */ }
		
		unsigned int getVersion() const
		{
			return *version_guard_;
		}

	private :
		typedef std::shared_ptr< std::atomic< unsigned int > const > VersionGuard;

		VersionGuard version_guard_;
		friend class Core::DataStore;
		friend class RTIMDB::Details::PollResult; // needs access to getVersion
	};
	class Transaction : public ROTransaction
	{
	public:
		Transaction()
			: next_entry_(0)
		{ /* no-op */ }
		Transaction(Transaction &&other) = default;
		Transaction& operator=(Transaction &&other) = default;
		Transaction(Transaction const &other) = delete;
		Transaction& operator=(Transaction const &other) = delete;

		ROTransaction getROTransaction() const noexcept
		{
			return *this;
		}

	private : // friend-only API
		struct Entry
		{
			enum struct TransactState {
				  initial__
				, unchanged__
				, value_changed__
				, pended__
				, transacted__
			};
			Entry()
				: point_id_(-1)
			{ /* no-op */ }
			Entry(PointType type, unsigned int point_id, PointValue value)
				: type_(type)
				, point_id_(point_id)
				, value_(value)
				, transact_state_(TransactState::initial__)
			{ /* no-op */ }

			PointType type_;
			unsigned int point_id_;
			PointValue value_;
			TransactState transact_state_;
		};

		template < typename Deleter >
		Transaction(std::atomic< unsigned int > *version_to_guard, Deleter &&rollback)
			: ROTransaction(version_to_guard, std::move(rollback))
			, next_entry_(0)
		{ /* no-op */ }

		Errors push(PointType point_type, unsigned int point_id, PointValue const &new_value)
		{
			if (next_entry_ == sizeof(entries_) / sizeof(entries_[0])) return Errors::too_many_transitions__;
			entries_[next_entry_++] = std::move(Entry(point_type, point_id, new_value));
			return Errors::no_error__;
		}

		Entry* begin() { return entries_; }
		Entry* end() { return entries_ + next_entry_; }
		std::reverse_iterator< Entry* > rbegin() { return std::reverse_iterator< Entry* >(entries_ + next_entry_); }
		std::reverse_iterator< Entry* > rend() { return std::reverse_iterator< Entry* >(entries_); }

	private :
		Entry entries_[RTIMDB_MAX_TRANSITIONS_PER_TRANSACTION];
		unsigned int next_entry_;
		
		friend class RTIMDB::Database;
		friend class Core::DataStore;
	};
}}}}

#endif

