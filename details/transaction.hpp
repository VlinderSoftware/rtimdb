#ifndef vlinder_rtimdb_details_transaction_hpp
#define vlinder_rtimdb_details_transaction_hpp

#include <memory>
#include <atomic>

namespace Vlinder { namespace RTIMDB { namespace Details {
	typedef std::shared_ptr< std::atomic< unsigned int > const > Transaction;
}}}

#endif

