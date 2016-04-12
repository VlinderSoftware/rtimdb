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
#include "defaultmappinghelper.hpp"
#include "exceptions/contract.hpp"
#include <algorithm>

using namespace std;

namespace Vlinder { namespace RTIMDB { namespace Details {
	DefaultMappingHelper::DefaultMappingHelper()
		: next_mapping_entry_(0)
	{ /* no-op */ }
	/*virtual */DefaultMappingHelper::~DefaultMappingHelper()
	{ /* no-op */ }

	/*virtual */void DefaultMappingHelper::clear() noexcept/* override final*/
	{
		next_mapping_entry_ = 0;
	}

	/*virtual */Errors DefaultMappingHelper::insert(uintptr_t tag, Core::PointType point_type, unsigned int system_id)/* override final*/
	{
		if ((sizeof(mappings_) / sizeof(mappings_[0])) == next_mapping_entry_) return Errors::mapping_full__;
		mappings_[next_mapping_entry_++] = Mapping(tag, point_type, system_id);
		return Errors::no_error__;
	}

	/*virtual */Mapping const *DefaultMappingHelper::at(unsigned int index) const noexcept/* override final*/
	{
		pre_condition(index < next_mapping_entry_);
		auto mapping(mappings_ + index);
		return mapping;
	}
	/*virtual */Mapping const* DefaultMappingHelper::find(Core::PointType point_type, unsigned int system_id) const noexcept/* override final*/
	{
		auto which(find_if(
			  begin(mappings_)
			, end(mappings_)
			, [=](remove_reference< decltype(*begin(mappings_)) >::type &mapping) -> bool {
				return ((mapping.point_type_ == point_type) && (mapping.system_id_ == system_id));
			  }
			));
		return which == end(mappings_) ? nullptr : which;
	}

	/*virtual */Mapping* DefaultMappingHelper::find(Core::PointType point_type, unsigned int system_id) noexcept/* override final*/
	{
		auto which(find_if(
			  begin(mappings_)
			, end(mappings_)
			, [=](remove_reference< decltype(*begin(mappings_)) >::type &mapping) -> bool {
				return ((mapping.point_type_ == point_type) && (mapping.system_id_ == system_id));
			  }
			));
		return which == end(mappings_) ? nullptr : which;
	}

	/*virtual */Mapping const* DefaultMappingHelper::find(uintptr_t tag) const noexcept/* override final*/
	{
		auto which(find_if(
			  begin(mappings_)
			, end(mappings_)
			, [=](remove_reference< decltype(*begin(mappings_)) >::type &mapping) -> bool {
				return (mapping.tag_ == tag);
			  }
			));
		return which == end(mappings_) ? nullptr : which;
	}

	/*virtual */Mapping* DefaultMappingHelper::find(uintptr_t tag) noexcept/* override final*/
	{
		auto which(find_if(
			  begin(mappings_)
			, end(mappings_)
			, [=](remove_reference< decltype(*begin(mappings_)) >::type &mapping) -> bool {
				return (mapping.tag_ == tag);
			  }
			));
		return which == end(mappings_) ? nullptr : which;
	}
	/* virtual */unsigned int DefaultMappingHelper::size() const noexcept/* override final*/
	{
		return next_mapping_entry_;
	}
}}}





