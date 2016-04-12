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
#ifndef vlinder_rtimdb_core_details_cell_hpp
#define vlinder_rtimdb_core_details_cell_hpp

#include "../../details/action.hpp"
#include "observer.hpp"
#include "../pointvalue.hpp"
#include "../../exceptions.hpp"
#include "exceptions/contract.hpp"
#include "optional.hpp"
#include <algorithm>
#include <atomic>
#include <mutex>
#include <vector>

namespace Vlinder { namespace RTIMDB { namespace Core { namespace Details {
	template < unsigned int cell_size__ >
	class Cell
	{
	public :
		Cell()
			: type_(PointType::_type_count__)
			, lock_flag_(false)
		{
			std::for_each(std::begin(frozen_versions_), std::end(frozen_versions_), [](decltype(frozen_versions_[0]) &frozen_version){ frozen_version = 0; });
		}
		~Cell()
		{ /* no-op */ }

		Cell(Cell &&cell)
			: type_(cell.type_)
			, lock_flag_(std::move(cell.lock_flag_))
		{
			cell.type_ = PointType::_type_count__;
			std::move(begin(cell.values_), end(cell.values_), begin(values_));
		}
		Cell& operator=(Cell &&cell)
		{
			type_ = cell.type_;
			cell.type_ = PointType::_type_count__;
			lock_flag_ = std::move(cell.lock_flag_);
			std::move(begin(cell.values_), end(cell.values_), begin(values_));
			return *this;
		}

		PointType getType() const noexcept
		{
			std::unique_lock< decltype(values_lock_) > values_lock(values_lock_);
			return type_;
		}
		PointValue get(unsigned int max_version = std::numeric_limits< unsigned int >::max()) const
		{
			std::unique_lock< decltype(values_lock_) > values_lock(values_lock_);
			return get_(max_version);
		}
		Errors setType(PointType point_type) noexcept
		{
			std::unique_lock< decltype(values_lock_) > values_lock(values_lock_);
			if ((type_ != PointType::_type_count__) && (type_ != point_type))
			{
				return Errors::not_allowed__;
			}
			else
			{
				type_ = point_type;
				return Errors::no_error__;
			}
		}
		Errors set(PointType type, PointValue const &point_value)
		{
			std::unique_lock< decltype(values_lock_) > values_lock(values_lock_);
			auto target(fetchAvailableSlot());
			if ((type == type_) || (PointType::_type_count__ == type_))
			{
				type_ = type;
				*target = point_value;
				return Errors::no_error__;
			}
			else
			{
				return Errors::not_allowed__;
			}
		}

		Errors freeze(unsigned int frozen_version)
		{
			// find an empty slot in freeze_indices_
			while (true)
			{
				auto which(find(begin(frozen_versions_), end(frozen_versions_), 0));
				if (end(frozen_versions_) == which)
				{
					return Errors::cannot_freeze__;
				}
				else
				{ /* try to CAS */ }
				unsigned int exp(0);
				if (which->compare_exchange_strong(exp, frozen_version))
				{
					if (getCurrentVersion() > frozen_version)
					{
						return Errors::expired_while_freezing__;
					}
					else
					{ /* all is well */ }
					break;
				}
				else
				{ /* search again */ }
			}
			return Errors::no_error__;
		}

		void thaw(unsigned int frozen_version) throw()
		{
			auto which(find(begin(frozen_versions_), end(frozen_versions_), frozen_version));
			*which = 0;
		}

		bool lock(unsigned int version)
		{
			if (lock_flag_.exchange(true)) return false;
			if (getCurrentVersion() > version)
			{
				lock_flag_ = false;
				return false;
			}
			else
			{ /* succeeded */ }

			return true;
		}

		void unlock()
		{
			lock_flag_ = false;
		}

	private:
		Cell(Cell const&) = delete;
		Cell& operator=(Cell const&) = delete;

		Optional< PointValue >* fetchAvailableSlot()
		{
			using std::begin;
			using std::end;

			// find the highest version that's frozen
			unsigned int highest_frozen_version(0);
			std::for_each(
				  begin(frozen_versions_)
				, end(frozen_versions_)
				, [&](std::atomic< unsigned int > const &frozen_version)
					{
						if (frozen_version > highest_frozen_version)
						{
							highest_frozen_version = frozen_version;
						}
						else
						{ /* not the highest one */ }
					}
				);
			// find a version of the point that's higher
			auto where(
				  std::find_if(
					  begin(values_)
					, end(values_)
					, [=](Optional< PointValue > const &point_value)->bool{ return point_value.empty() || point_value.get().version_ > highest_frozen_version; }
					)
				);
			// replace it if it exists
			if (where != end(values_)) return where;
			// use an empty slot if it doesn't
			return findEmptySlot();
		}
		Optional< PointValue >* findEmptySlot()
		{
			using std::begin;
			using std::end;

			// find out which slots are frozen, which for each frozen version are the slot with the highest version number lower than or equal to the frozen version
			std::vector< bool > frozen(cell_size__);
			std::for_each(
				  begin(frozen_versions_)
				, end(frozen_versions_)
				, [&](std::atomic< unsigned int > const &frozen_version)
					{
						if (0 != frozen_version)
						{
							auto value_with_highest_version_lower_than_frozen(begin(values_));
							for (auto curr(begin(values_)); curr != end(values_); ++curr)
							{
								if (curr->empty())
								{ /* can't freeze an empty value */ }
								else
								{
									if (curr->get().version_ <= frozen_version)
									{
										if (value_with_highest_version_lower_than_frozen->empty())
										{
											value_with_highest_version_lower_than_frozen = curr;
										}
										else if (value_with_highest_version_lower_than_frozen->get().version_ < curr->get().version_)
										{
											value_with_highest_version_lower_than_frozen = curr;
										}
										else
										{ /* curr is not the value with the highest version lower than the one that was frozen */ }
									}
									else
									{ /* This version didn't exist yet when the freeze occurred */ }
								}
							}
							if (
								   !value_with_highest_version_lower_than_frozen->empty()
								&& value_with_highest_version_lower_than_frozen != end(values_)
								)
							{
								frozen[value_with_highest_version_lower_than_frozen - values_] = true;
							}
							else
							{ /* nothing found (this point was not set at the time of the freeze) */ }
						}
						else
						{ /* not frozen */ }
					}
				);
			auto which(find(frozen.begin(), frozen.end(), false));
			assert(which != frozen.end());
			return values_ + std::distance(frozen.begin(), which);
		}
		unsigned int getCurrentVersion() const
		{
			unsigned int current_version(0);
			std::unique_lock< decltype(values_lock_) > values_lock(values_lock_);
			std::for_each(
				  std::begin(values_)
				, std::end(values_)
				, [&](decltype(*values_) point)
					{
						if (!point.empty() && (current_version < point.get().version_)) current_version = point.get().version_; 
					}
				);
			return current_version;
		}
		PointValue get_(unsigned int max_version = std::numeric_limits< unsigned int >::max()) const
		{
			using std::begin;
			using std::end;

			PointValue retval;
			unsigned int highest_version_so_far(0);
			std::for_each(
				  begin(values_)
				, end(values_)
				, [&](Optional< PointValue > const &point_value)
					{
						if (point_value.empty())
						{ /* this value is not set */ }
						else
						{
							PointValue p(point_value.get());
							if ((highest_version_so_far <= p.version_) && (p.version_ <= max_version))
							{
								highest_version_so_far = p.version_;
								retval = p;
							}
							else
							{ /* not the latest version */ }
						}
					}
				);
			return retval;
		}
	
		mutable std::mutex values_lock_;
		PointType type_;
		Optional< PointValue > values_[cell_size__];
		static_assert(RTIMDB_MAX_CONCURRENT_TRANSACTIONS < cell_size__, "There must be more points in a cell than the maximum number of transactions");
		std::atomic< unsigned int > frozen_versions_[RTIMDB_MAX_CONCURRENT_TRANSACTIONS];
		std::atomic< bool > lock_flag_;
	};
}}}}

#endif
