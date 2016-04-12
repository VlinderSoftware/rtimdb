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
#ifndef vlinder_rtimdb_details_mapping_hpp
#define vlinder_rtimdb_details_mapping_hpp

#include "eventclass.hpp"

namespace Vlinder { namespace RTIMDB { namespace Details {
	struct Mapping
	{
		Mapping()
			: event_class_(EventClass::class_0__)
		{ /* no-op */ }
		Mapping(uintptr_t tag, Core::PointType point_type, unsigned int system_id)
			: tag_(tag)
			, point_type_(point_type)
			, system_id_(system_id)
			, event_class_(EventClass::class_0__)
		{ /* no-op */ }

		uintptr_t tag_;
		Core::PointType point_type_;
		unsigned int system_id_;
		EventClass event_class_;
	};
}}}

#endif


