#include "iterator.hpp"
#include "locator.hpp"
#include "../database.hpp"
#include <utility>

namespace Vlinder { namespace RTIMDB { namespace Details {
    Iterator::Iterator()
		: database_(0)
		, at_end_(true)
	{ /* no-op */ }
    Iterator::Iterator(
			Database *database
		, Transaction const &transaction
		, Locator const &locator
		)
		: database_(database)
		, transaction_(transaction)
		, at_end_(false)
	{ /* no-op */ }
    Iterator::Iterator(
          Database *database
		, std::pair < Transaction, Errors > const &maybe_transaction
		, Locator const &locator
		)
		: database_(database)
		, transaction_(maybe_transaction.first)
		, at_end_((Errors::no_error__ != maybe_transaction.second) && (PointType::_type_count__ != locator.first))
	{ /* no-op */ }

    bool Iterator::operator==(Iterator const &rhs) const
	{
		return at_end_ == rhs.at_end_
			|| (   database_ == rhs.database_
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
        if (database_)
        {
            locator_ = database_->advance(locator_);
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
}}}


