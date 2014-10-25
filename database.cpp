#include "database.hpp"
#include <algorithm>
#include <cstring>
#include <atomic>

using namespace std;

namespace Vlinder { namespace RTIMDB {
	Database::Database()
		: next_cell_(0)
		, curr_version_(1)
	{
		memset(start_index_, 0, sizeof(start_index_));
		memset(points_, 0, sizeof(points_));
	}
	Database::~Database()
	{ /* no-op */ }
	
	unsigned int Database::insert(Point const &value)
	{
		using std::begin;
		using std::end;
		unsigned int const type_start_index(start_index_[static_cast< unsigned int >(value.type_)]);
		unsigned int const type_end_index(start_index_[static_cast< unsigned int >(value.type_) + 1]);
		unsigned int const next_index(type_end_index);
		unsigned int const retval(type_end_index - type_start_index);
		unsigned int const last_index(start_index_[static_cast<unsigned int>(PointType::_type_count__)]);
		bool dismiss(false);
		auto rollback = [&](void *){ if (!dismiss) { --next_cell_; move(points_ + next_index + 1, end(points_), points_ + next_index); } };
		unique_ptr< void, decltype(rollback) > scope_guard(&rollback, rollback);

		auto curr_begin(begin(points_) + next_index);
		auto curr_end(begin(points_) + last_index);
		auto target_end(begin(points_) + last_index + 1);
		move_backward(curr_begin, curr_end, target_end);
		points_[next_index] = &cells_[next_cell_++];
		(*points_[next_index]).set(value);
		dismiss = true;
		// as of this, cannot fail
		unsigned int * first = (start_index_ + static_cast<unsigned int>(value.type_) + 1);
		for_each(first, end(start_index_), [](unsigned int &val){ ++val; });
		typedef reverse_iterator< remove_reference< decltype(*points_) >::type * > ReverseIterator;

		return retval;
	}

#ifdef RTIMDB_ALLOW_EXCEPTIONS
	void Database::update(PointType type, unsigned int index, bool value)                   { throwException(update(type, index, value RTIMDB_NOTHROW_ARG)); }
	void Database::update(PointType type, unsigned int index, DoubleBitBinary const &value)	{ throwException(update(type, index, value RTIMDB_NOTHROW_ARG)); }
	void Database::update(PointType type, unsigned int index, uint32_t value)				{ throwException(update(type, index, value RTIMDB_NOTHROW_ARG)); }
	void Database::update(PointType type, unsigned int index, double value)					{ throwException(update(type, index, value RTIMDB_NOTHROW_ARG)); }
	void Database::update(PointType type, unsigned int index, Time value)					{ throwException(update(type, index, value RTIMDB_NOTHROW_ARG)); }
	void Database::update(PointType type, unsigned int index, File *value)					{ throwException(update(type, index, value RTIMDB_NOTHROW_ARG)); }
	void Database::update(PointType type, unsigned int index, Dataset *value)				{ throwException(update(type, index, value RTIMDB_NOTHROW_ARG)); }
	void Database::update(PointType type, unsigned int index, uint16_t value)				{ throwException(update(type, index, value RTIMDB_NOTHROW_ARG)); }
	void Database::update(PointType type, unsigned int index, uint8_t value)				{ throwException(update(type, index, value RTIMDB_NOTHROW_ARG)); }
	void Database::update(PointType type, unsigned int index, String *value)				{ throwException(update(type, index, value RTIMDB_NOTHROW_ARG)); }
#endif

	Errors Database::update(PointType type, unsigned int index, bool value                    RTIMDB_NOTHROW_PARAM) throw() { auto fetch_result(fetch(type, index)); if (Errors::no_error__ == fetch_result.second) { (*fetch_result.first)->set(Point(type, value, ++curr_version_)); return Errors::no_error__; } else { return fetch_result.second; } }
	Errors Database::update(PointType type, unsigned int index, DoubleBitBinary const &value  RTIMDB_NOTHROW_PARAM) throw() { auto fetch_result(fetch(type, index)); if (Errors::no_error__ == fetch_result.second) { (*fetch_result.first)->set(Point(type, value, ++curr_version_)); return Errors::no_error__; } else { return fetch_result.second; } }
	Errors Database::update(PointType type, unsigned int index, uint32_t value                RTIMDB_NOTHROW_PARAM) throw() { auto fetch_result(fetch(type, index)); if (Errors::no_error__ == fetch_result.second) { (*fetch_result.first)->set(Point(type, value, ++curr_version_)); return Errors::no_error__; } else { return fetch_result.second; } }
	Errors Database::update(PointType type, unsigned int index, double value                  RTIMDB_NOTHROW_PARAM) throw() { auto fetch_result(fetch(type, index)); if (Errors::no_error__ == fetch_result.second) { (*fetch_result.first)->set(Point(type, value, ++curr_version_)); return Errors::no_error__; } else { return fetch_result.second; } }
	Errors Database::update(PointType type, unsigned int index, Time value                    RTIMDB_NOTHROW_PARAM) throw() { auto fetch_result(fetch(type, index)); if (Errors::no_error__ == fetch_result.second) { (*fetch_result.first)->set(Point(type, value, ++curr_version_)); return Errors::no_error__; } else { return fetch_result.second; } }
	Errors Database::update(PointType type, unsigned int index, File *value                   RTIMDB_NOTHROW_PARAM) throw() { auto fetch_result(fetch(type, index)); if (Errors::no_error__ == fetch_result.second) { (*fetch_result.first)->set(Point(type, value, ++curr_version_)); return Errors::no_error__; } else { return fetch_result.second; } }
	Errors Database::update(PointType type, unsigned int index, Dataset *value                RTIMDB_NOTHROW_PARAM) throw() { auto fetch_result(fetch(type, index)); if (Errors::no_error__ == fetch_result.second) { (*fetch_result.first)->set(Point(type, value, ++curr_version_)); return Errors::no_error__; } else { return fetch_result.second; } }
	Errors Database::update(PointType type, unsigned int index, uint16_t value                RTIMDB_NOTHROW_PARAM) throw() { auto fetch_result(fetch(type, index)); if (Errors::no_error__ == fetch_result.second) { (*fetch_result.first)->set(Point(type, value, ++curr_version_)); return Errors::no_error__; } else { return fetch_result.second; } }
	Errors Database::update(PointType type, unsigned int index, uint8_t value                 RTIMDB_NOTHROW_PARAM) throw() { auto fetch_result(fetch(type, index)); if (Errors::no_error__ == fetch_result.second) { (*fetch_result.first)->set(Point(type, value, ++curr_version_)); return Errors::no_error__; } else { return fetch_result.second; } }
	Errors Database::update(PointType type, unsigned int index, String *value                 RTIMDB_NOTHROW_PARAM) throw() { auto fetch_result(fetch(type, index)); if (Errors::no_error__ == fetch_result.second) { (*fetch_result.first)->set(Point(type, value, ++curr_version_)); return Errors::no_error__; } else { return fetch_result.second; } }

#ifdef RTIMDB_ALLOW_EXCEPTIONS
	Point Database::read(PointType type, unsigned int index) const
	{
		auto result(read(type, indexRTIMDB_NOTHROW_ARG));
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

	pair< Details::Variant< Point >, Errors > Database::read(PointType type, unsigned int index RTIMDB_NOTHROW_PARAM) const throw()
	{
		auto fetch_result(fetch(type, index));
		return make_pair((Errors::no_error__ == fetch_result.second) ? (*fetch_result.first)->get() : Details::Variant< Point >(), fetch_result.second);
	}

#ifdef RTIMDB_ALLOW_EXCEPTIONS
	Point Database::read(Transaction transaction, PointType type, unsigned int index) const
	{
		auto result(read(transaction, type, indexRTIMDB_NOTHROW_ARG));
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
	std::pair< Details::Variant< Point >, Errors > Database::read(Transaction transaction, PointType type, unsigned int index RTIMDB_NOTHROW_PARAM) const throw()
	{
		auto fetch_result(fetch(type, index));
		return make_pair((Errors::no_error__ == fetch_result.second) ? (*fetch_result.first)->get(*transaction) : Details::Variant< Point >(), fetch_result.second);
	}

#ifdef RTIMDB_ALLOW_EXCEPTIONS
	Database::Transaction Database::freeze()
	{
		auto result(freeze(nothrow));
		if (Errors::no_error__ == result.second)
		{
			return result.first;
		}
		else
		{
			throwException(result.second);
		}
		throw logic_error("Unreachable code");
	}
#endif
	pair < Database::Transaction, Errors > Database::freeze(RTIMDB_NOTHROW_PARAM_1) throw()
	{
		unsigned int frozen_version(curr_version_);
		// find an empty slot in freeze_indices_
		while (true)
		{
			using std::begin;
			using std::end;
			auto which(find(begin(frozen_versions_), end(frozen_versions_), 0));
			if (end(frozen_versions_) == which)
			{
				return make_pair(Transaction(), Errors::cannot_freeze__);
			}
			else
			{ /* try to CAS */ }
			unsigned int exp(0);
			if (which->compare_exchange_strong(exp, frozen_version))
			{
				auto deleter([this, frozen_version](void *p){ thaw(frozen_version); });
				Transaction retval(which, deleter);
				freezeCells(*retval);
				return make_pair(retval, Errors::no_error__);
			}
			else
			{ /* search again */ }
		};
	}

	void Database::thaw(unsigned int frozen_version)
	{
		using std::begin;
		using std::end;

		unsigned int const last_index(start_index_[static_cast<unsigned int>(PointType::_type_count__)]);
		for_each(cells_, cells_ + last_index, [frozen_version](decltype(cells_[0]) cell){ cell.thaw(frozen_version); });
		auto which(find(begin(frozen_versions_), end(frozen_versions_), frozen_version));
		*which = 0;
	}

	std::pair< Cell< RTIMDB_CELL_SIZE > *const*, Errors > Database::fetch(PointType type, unsigned int index) const
	{
		unsigned int const type_start_index_(start_index_[static_cast< unsigned int >(type)]);
		unsigned int const type_end_index_(start_index_[static_cast< unsigned int >(type)+1]);

		if ((type_end_index_ - type_start_index_) < index) return make_pair(nullptr, Errors::unknown_point__);

		index += type_start_index_;
		return make_pair(&points_[index], Errors::no_error__);
	}

	void Database::freezeCells(unsigned int frozen_version)
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
					, [&](Cell< RTIMDB_CELL_SIZE > &c){ c.thaw(frozen_version); }
					);
			}
			else
			{ /* dismissed */ }
		};
		unique_ptr< void, decltype(rollback) > scope_guard(&rollback, rollback);
		unsigned int const last_index(start_index_[static_cast<unsigned int>(PointType::_type_count__)]);
		auto const end(begin(cells_) + last_index);
		for (; cell != end; ++cell)
		{
			cell->freeze(frozen_version);
		}
		dismiss = true;
	}
}}

