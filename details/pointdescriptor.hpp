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
#ifndef vlinder_rtimdb_details_pointdescriptor_hpp
#define vlinder_rtimdb_details_pointdescriptor_hpp

#include "../pointtype.hpp"

namespace Vlinder { namespace RTIMDB { namespace Details {
	struct PointDescriptor
	{
		PointDescriptor() noexcept
			: point_type_(PointType::_type_count__)
		{ /* no-op */ }
		PointDescriptor(PointType point_type, unsigned int data_store_id, unsigned int producer_id) noexcept
			: point_type_(point_type)
			, data_store_id_(data_store_id)
			, producer_id_(producer_id)
		{ /* no-op */ }
		PointDescriptor(PointDescriptor const &other) = default;
		PointDescriptor& operator=(PointDescriptor const &other) = default;
		PointDescriptor(PointDescriptor &&other) = default;
		PointDescriptor& operator=(PointDescriptor &&other) = default;

		PointType point_type_;
		unsigned int data_store_id_;
		unsigned int producer_id_;
	};
}}}

#endif

