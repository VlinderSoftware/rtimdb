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
#ifndef vlinder_rtimdb_outstation_details_pointdescriptor_hpp
#define vlinder_rtimdb_outstation_details_pointdescriptor_hpp

#include "../../pointtype.hpp"

namespace Vlinder { namespace RTIMDB { namespace Outstation { namespace Details {
	struct PointDescriptor
	{
		PointDescriptor()
			: point_type_(PointType::_type_count__)
		{ /* no-op */ }
		PointDescriptor(uintptr_t tag, PointType point_type, unsigned int data_store_id)
			: tag_(tag)
			, point_type_(point_type)
			, data_store_id_(data_store_id)
			, associated_command_queue_(std::numeric_limits< unsigned int >::max())
		{ /* no-op */ }
		PointDescriptor(PointDescriptor const &other)
			: tag_(other.tag_)
			, point_type_(other.point_type_)
			, data_store_id_(other.data_store_id_)
			, associated_command_queue_(other.associated_command_queue_.load())
		{ /* no-op */ }
		PointDescriptor& operator=(PointDescriptor other)
		{
			return swap(other);
		}
		PointDescriptor& swap(PointDescriptor &other)
		{
			std::swap(tag_, other.tag_);
			std::swap(point_type_, other.point_type_);
			std::swap(data_store_id_, other.data_store_id_);
			associated_command_queue_.store(other.associated_command_queue_.exchange(associated_command_queue_.load()));
			return *this;
		}

		uintptr_t tag_;
		PointType point_type_;
		unsigned int data_store_id_;
		std::atomic< unsigned int > associated_command_queue_;
	};
}}}}

#endif

