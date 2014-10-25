#ifndef vlinder_rtimdb_details_iterator_hpp
#define vlinder_rtimdb_details_iterator_hpp

#include "prologue.hpp"
#include "transaction.hpp"
#include "locator.hpp"
#include "../exceptions.hpp"

namespace Vlinder { namespace RTIMDB { 
	class Database;
	struct Point;
	namespace Details {
	class RTIMDB_API Iterator : public std::iterator< std::input_iterator_tag, Point >
	{
	public :
		Iterator();
		Iterator(
			  Database *database
			, Transaction const &transaction
			, Locator const &locator
			);
		Iterator(
			  Database *database
			, std::pair < Transaction, Errors > const &maybe_transaction
			, Locator const &locator
			);

		bool operator==(Iterator const &rhs) const;
		bool operator!=(Iterator const &rhs) const;

        Iterator& operator++();
        Iterator operator++(int);

	private:
		Database *database_;
		Transaction transaction_;
		bool at_end_;
		Locator locator_;
	};
}}}

#endif

