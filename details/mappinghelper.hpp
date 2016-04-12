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
#ifndef vlinder_rtimdb_details_mappinghelper_hpp
#define vlinder_rtimdb_details_mappinghelper_hpp

#include "../exceptions.hpp"
#include "../core/pointtype.hpp"
#include "mapping.hpp"

namespace Vlinder { namespace RTIMDB { namespace Details {
	class MappingHelper
	{
	public :
		
		MappingHelper();
		virtual ~MappingHelper();

		virtual void clear() noexcept = 0;

		virtual Errors insert(uintptr_t tag, Core::PointType point_type, unsigned int system_id) = 0;

		virtual Mapping const* at(unsigned int index) const noexcept = 0;

		virtual Mapping const* find(Core::PointType point_type, unsigned int system_id) const noexcept = 0;
		virtual Mapping* find(Core::PointType point_type, unsigned int system_id) noexcept = 0;
		virtual Mapping const* find(uintptr_t tag) const noexcept = 0;
		virtual Mapping* find(uintptr_t tag) noexcept = 0;

		virtual unsigned int size() const noexcept = 0;

	protected :
	};
}}}

#endif


