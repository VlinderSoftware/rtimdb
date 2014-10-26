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
#ifndef vlinder_rtimdb_cell_hpp
#define vlinder_rtimdb_cell_hpp

#include "point.hpp"
#include "details/action.hpp"
#include "exceptions.hpp"
#include <algorithm>
#include <atomic>
#include <mutex>
#include <vector>
#include <cassert>

namespace Vlinder { namespace RTIMDB {
	template < unsigned int cell_size__ >
	class Cell
	{
	public :
		Cell()
		{ /* no-op */ }
		~Cell()
		{ /* no-op */ }

		Cell(Cell &&cell)
		{
			std::move(begin(cell.values_), end(cell.values_), begin(values_));
		}
		Cell& operator=(Cell &&cell)
		{
			std::move(begin(cell.values_), end(cell.values_), begin(values_));
			return *this;
		}

		Point get(unsigned int max_version = std::numeric_limits< unsigned int >::max()) const
		{
			using std::begin;
			using std::end;

			std::unique_lock< decltype(values_lock_) > values_lock(values_lock_);
			Point retval;
			unsigned int highest_version_so_far(0);
			std::for_each(
				  begin(values_)
				, end(values_)
				, [&](std::atomic< Point > const &point)
					{
						Point p(point);
						if ((PointType::_type_count__ != p.type_) && (highest_version_so_far <= p.version_) && (p.version_ <= max_version))
						{
							highest_version_so_far = p.version_;
							retval = p;
						}
						else
						{ /* not the latest version */ }
					}
				);
			return retval;
		}
		Errors set(Details::Action action, Point const &point)
		{
			using Details::Action;
			std::unique_lock< decltype(values_lock_) > values_lock(values_lock_);
			auto target(fetchAvailableSlot());
			if ((action == Action::update__) || filter_(action, point, *target))
			{
				*target = point;
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

		void registerFilter(std::function< bool(Details::Action, Point, Point) > &&filter)
		{
			filter_ = std::move(filter);
		}
	private:
		Cell(Cell const&) = delete;
		Cell& operator=(Cell const&) = delete;

		Point* fetchAvailableSlot()
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
			auto where(std::find_if(begin(values_), end(values_), [=](Point const &point)->bool{ return point.version_ > highest_frozen_version; }));
			// replace it if it exists
			if (where != end(values_)) return where;
			// use an empty slot if it doesn't
			return findEmptySlot();
		}
		Point* findEmptySlot()
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
							Point *value(values_);
							std::for_each(
								  begin(values_)
								, end(values_)
								, [&](Point &point)
									{
										if ((point.version_ > value->version_) && (point.version_ <= frozen_version)) value = &point;
									}
								);
							if (value != end(values_)) frozen[value - values_] = true;
						}
						else
						{ /* not frozen */ }
					}
				);
			auto which(find(frozen.begin(), frozen.end(), false));
			assert(which != frozen.end());
			return values_ + std::distance(frozen.begin(), which);
		}

		mutable std::mutex values_lock_;
		Point values_[cell_size__];
		static_assert(RTIMDB_MAX_CONCURRENT_TRANSACTIONS < cell_size__, "There must be more points in a cell than the maximum number of transactions");
		std::atomic< unsigned int > frozen_versions_[RTIMDB_MAX_CONCURRENT_TRANSACTIONS];
		std::function< bool(Details::Action, Point, Point) > filter_;
	};
}}

#endif
