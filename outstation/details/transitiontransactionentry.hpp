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
#ifndef vlinder_rtimdb_outstation_details_transitiontransactionentry_hpp
#define vlinder_rtimdb_outstation_details_transitiontransactionentry_hpp

#include "prologue.hpp"
#include "variant.hpp"
#include "../transition.hpp"
#include "timestamp.hpp"

namespace Vlinder { namespace RTIMDB { namespace Outstation { namespace Details {
	struct RTIMDB_OUTSTATION_API TransitionTransactionEntry : Variant< Timestamp, Transition >
	{
		typedef Variant< Timestamp, Transition > super;

		TransitionTransactionEntry()
		{ /* no-op */ }
		template < typename T >
		TransitionTransactionEntry(T const &v)
			: super(v)
		{ /* no-op */ }
	};
}}}}

#endif

