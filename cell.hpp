#ifndef vlinder_rtimdb_cell_hpp
#define vlinder_rtimdb_cell_hpp

#include "point.hpp"
#include "exceptions.hpp"
#include <algorithm>
#include <atomic>
#include <mutex>
#include <vector>

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
		void set(Point const &point)
		{
			std::unique_lock< decltype(values_lock_) > values_lock(values_lock_);
			auto target(fetchAvailableSlot());
			*target = point;
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

	private:
		Cell(Cell const&) = delete;
		Cell& operator=(Cell const&) = delete;

		Point* fetchAvailableSlot()
		{
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
		std::atomic< unsigned int > frozen_versions_[cell_size__ - 1];
	};
}}

#endif
