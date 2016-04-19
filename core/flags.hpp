/* Copyright 2016  Vlinder Software
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
#ifndef vlinder_rtimdb_core_flags_hpp
#define vlinder_rtimdb_core_flags_hpp

#include "flag.hpp"

namespace Vlinder { namespace RTIMDB { namespace Core {
	struct Flags
	{
		Flags()
			: flags_(0)
		{ /* no-op */ }
		Flags(uint8_t flags)
			: flags_(flags)
		{ /* no-op */}
		Flags(Flag flag)
			: flags_(static_cast< uint8_t >(flag))
		{ /* no-op */
		}

		uint8_t flags_;
	};

	bool operator==(Flags const &lhs, Flags const &rhs);
	bool operator!=(Flags const &lhs, Flags const &rhs);
}}}

#endif

