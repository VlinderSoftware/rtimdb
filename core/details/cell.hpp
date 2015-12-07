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
#include "../../details/duplicateselectionpolicy.hpp"
#include "../point.hpp"
#include "../exceptions.hpp"
#include <algorithm>
#include <atomic>
#include <mutex>
#include <vector>
#include <cassert>

namespace Vlinder { namespace RTIMDB { namespace Core { namespace Details {
	template < unsigned int cell_size__, RTIMDB::Details::DuplicateSelectionPolicy duplicate_selection_policy__ = RTIMDB_DUPLICATE_SELECTION_POLICY >
	class Cell
	{
	public :
		Cell()
			: next_selection_id_(0)
			, observer_([](RTIMDB::Details::Action, Point, Point){})
		{
			std::for_each(std::begin(frozen_versions_), std::end(frozen_versions_), [](decltype(frozen_versions_[0]) &frozen_version){ frozen_version = 0; });
		}
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
			std::unique_lock< decltype(values_lock_) > values_lock(values_lock_);
			return get_(max_version);
		}
		Errors set(RTIMDB::Details::Action action, Point const &point)
		{
			using RTIMDB::Details::Action;
			std::unique_lock< decltype(values_lock_) > values_lock(values_lock_);
			auto target(fetchAvailableSlot());
			Point current(get_());
			if (((point.type_ == current.type_) || (PointType::_type_count__ == current.type_)) && ((action == Action::update__) || filter_(action, point, current)))
			{
				auto prev_value(get_());
				*target = point;
				values_lock.unlock();
				std::unique_lock< decltype(observer_lock_) > observer_lock(observer_lock_);
				observer_(action, point, prev_value);
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

		void registerFilter(std::function< bool(RTIMDB::Details::Action, Point, Point) > &&filter)
		{
			filter_ = std::move(filter);
		}

		std::pair< Details::Selection, Errors > select()
		{
			std::unique_lock< decltype(select_operate_lock_) > select_operate_lock(select_operate_lock_);
			auto makeRelease([](unsigned int select_id) -> std::function< void(Details::Selection_*) >{ return [select_id](Details::Selection_ *selection) -> void{ if (selection) { unsigned int exp(select_id); selection->id_.compare_exchange_strong(exp, 0); }};});
			unsigned int exp(0);
			unsigned int selection_id(next_selection_id_++);
			if (selection_.id_.compare_exchange_strong(exp, selection_id))
			{
				return std::make_pair(std::make_pair(Details::Selection::first_type(&selection_, makeRelease(selection_id)), selection_id), Errors::no_error__);
			}
			else if (duplicate_selection_policy__ == RTIMDB::Details::DuplicateSelectionPolicy::replace_selection__)
			{
				selection_.id_ = selection_id;
				return std::make_pair(std::make_pair(Details::Selection::first_type(&selection_, makeRelease(selection_id)), selection_id), Errors::duplicate_selection__);
			}
			else
			{
				return std::make_pair(Details::Selection(), Errors::duplicate_selection__);
			}
		}
		Errors operate(Details::Selection const &selection, Point new_value)
		{
			std::unique_lock< decltype(select_operate_lock_) > select_operate_lock(select_operate_lock_);
			if ((selection.first.get() == &selection_) && (selection.second == selection.first->id_))
			{
				auto set_result(set(RTIMDB::Details::Action::operate__, new_value));
				return set_result;
			}
			return Errors::operate_without_select__;
		}
		Errors freeze()
		{
			std::unique_lock< decltype(values_lock_) > values_lock(values_lock_);
			auto current_value(get_());
			if (filter_(RTIMDB::Details::Action::freeze__, current_value, current_value))
			{
				return freeze(getCurrentVersion());
			}
			else
			{
				return Errors::not_allowed__;
			}
		}
		Errors freezeAndClear()
		{
			std::unique_lock< decltype(values_lock_) > values_lock(values_lock_);
			auto current_value(get_());
			if (filter_(RTIMDB::Details::Action::freeze_and_clear__, default_clear_value_, current_value))
			{
				Errors retval(freeze(getCurrentVersion()));
				if (Errors::no_error__ == retval)
				{
					auto target(fetchAvailableSlot());
					*target = default_clear_value_;
				}
				else
				{ /* failed to freeze */ }
				return retval;
			}
			else
			{
				return Errors::not_allowed__;
			}
		}

		void setDefaultClearValue(Point &&default_clear_value)
		{
			default_clear_value_ = std::move(default_clear_value);
		}

		Errors registerObserver(Observer const &observer)
		{
			std::unique_lock< decltype(observer_lock_) > observer_lock(observer_lock_);
			observer_= observer;
			return Errors::no_error__;
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
		unsigned int getCurrentVersion() const
		{
			unsigned int current_version(0);
			std::unique_lock< decltype(values_lock_) > values_lock(values_lock_);
			std::for_each(std::begin(values_), std::end(values_), [&](decltype(*values_) point){ if (current_version < point.version_) current_version = point.version_; });
			return current_version;
		}
		Point get_(unsigned int max_version = std::numeric_limits< unsigned int >::max()) const
		{
			using std::begin;
			using std::end;

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
		
		mutable std::mutex values_lock_;
		Point values_[cell_size__];
		static_assert(RTIMDB_MAX_CONCURRENT_TRANSACTIONS < cell_size__, "There must be more points in a cell than the maximum number of transactions");
		std::atomic< unsigned int > frozen_versions_[RTIMDB_MAX_CONCURRENT_TRANSACTIONS];
		std::function< bool(RTIMDB::Details::Action, Point, Point) > filter_;
		Details::Selection_ selection_;
		std::atomic< unsigned int > next_selection_id_;
		std::mutex select_operate_lock_;
		Point default_clear_value_;
		Observer observer_;
		std::mutex observer_lock_;
	};
}}}}

#endif
