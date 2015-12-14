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

namespace Vlinder { namespace RTIMDB { namespace Core { 
	class DataStore;
	namespace Details {
	class Transaction
	{
	private : // friend-only API
		template < typename Deleter >
		Transaction(std::atomic< unsigned int > *version_to_guard, Deleter &&rollback)
			: version_guard_(version_to_guard, std::move(rollback))
		{ /* no-op */ }

	public :
		Transaction()
		{ /* no-op */ }

		bool operator==(Transaction const &other) const
		{
			return true
				&& version_guard_ == other.version_guard_
				;
		}

		unsigned int getVersion() const
		{
			return *version_guard_;
		}

	private :
		typedef std::shared_ptr< std::atomic< unsigned int > const > VersionGuard;

		VersionGuard version_guard_;
		friend class DataStore;
	};
}}}}

#endif

