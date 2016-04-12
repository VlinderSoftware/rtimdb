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
#include "sortedmappinghelper.hpp"
#include "exceptions/contract.hpp"
#include <algorithm>

using namespace std;

namespace Vlinder { namespace RTIMDB { namespace Details {
	SortedMappingHelper::SortedMappingHelper()
		: mapping_entry_count_(0)
	{ /* no-op */ }
	/*virtual */SortedMappingHelper::~SortedMappingHelper()
	{ /* no-op */ }

	/*virtual */void SortedMappingHelper::clear() noexcept/* override final*/
	{
		mapping_entry_count_ = 0;
	}

	/*virtual */Errors SortedMappingHelper::insert(uintptr_t tag, Core::PointType point_type, unsigned int system_id)/* override final*/
	{
		if (size() == capacity()) return Errors::mapping_full__;
		Mapping new_mapping(tag, point_type, system_id);
		mappings_[mapping_entry_count_] = new_mapping;
		{
			auto where(upper_bound(begin(tag_to_id__), end(tag_to_id__), &mappings_[mapping_entry_count_], compareByTag));
			move_backward(where, end(tag_to_id__), end(tag_to_id__) + 1);
			*where = &mappings_[mapping_entry_count_];
		}
		{
			auto where(upper_bound(begin(id_to_tag__), end(id_to_tag__), &mappings_[mapping_entry_count_], compareBySysID));
			move_backward(where, end(id_to_tag__), end(id_to_tag__) + 1);
			*where = &mappings_[mapping_entry_count_];
		}
		++mapping_entry_count_;
		return Errors::no_error__;
	}

	/*virtual */Mapping const *SortedMappingHelper::at(unsigned int index) const noexcept/* override final*/
	{
		pre_condition(index < size());
		return tag_to_id_[index];
	}

	/*virtual */Mapping const* SortedMappingHelper::find(Core::PointType point_type, unsigned int system_id) const noexcept/* override final*/
	{
		Mapping key(0, point_type, system_id);
		auto where(lower_bound(begin(id_to_tag__), end(id_to_tag__), &key, compareBySysID));
		if ((where != end(id_to_tag__)) && ((*where)->point_type_ == point_type) && ((*where)->system_id_ == system_id)) 
		{
			return *where;
		}
		else
		{
			return nullptr;
		}
	}
	/*virtual */Mapping* SortedMappingHelper::find(Core::PointType point_type, unsigned int system_id) noexcept/* override final*/
	{
		Mapping key(0, point_type, system_id);
		auto where(lower_bound(begin(id_to_tag__), end(id_to_tag__), &key, compareBySysID));
		if ((where != end(id_to_tag__)) && ((*where)->point_type_ == point_type) && ((*where)->system_id_ == system_id)) 
		{
			return *where;
		}
		else
		{
			return nullptr;
		}
	}
	/*virtual */Mapping const* SortedMappingHelper::find(uintptr_t tag) const noexcept/* override final*/
	{
		Mapping key(tag, Core::PointType::_type_count__, 0);
		auto where(lower_bound(begin(tag_to_id__), end(tag_to_id__), &key, compareByTag));
		if ((where != end(tag_to_id__)) && ((*where)->tag_ == tag)) 
		{
			return *where;
		}
		else
		{
			return nullptr;
		}
	}
	/*virtual */Mapping* SortedMappingHelper::find(uintptr_t tag) noexcept/* override final*/
	{
		Mapping key(tag, Core::PointType::_type_count__, 0);
		auto where(lower_bound(begin(tag_to_id__), end(tag_to_id__), &key, compareByTag));
		if ((where != end(tag_to_id__)) && ((*where)->tag_ == tag)) 
		{
			return *where;
		}
		else
		{
			return nullptr;
		}
	}
	
	/*virtual */unsigned int SortedMappingHelper::size() const noexcept/* override final*/
	{
		return mapping_entry_count_;
	}

	unsigned int SortedMappingHelper::capacity() const noexcept
	{
		static_assert((sizeof(mappings_) / sizeof(mappings_[0])) == (sizeof(id_to_tag_) / sizeof(id_to_tag_[0])), "Sizes of mapping arrays do not match");
		static_assert((sizeof(tag_to_id_)/ sizeof(tag_to_id_[0])) == (sizeof(id_to_tag_) / sizeof(id_to_tag_[0])), "Sizes of mapping arrays do not match");
		return (sizeof(mappings_)/ sizeof(mappings_[0]));
	}
	Mapping** SortedMappingHelper::begin(Which which) noexcept
	{
		switch (which)
		{
		case tag_to_id__:
			return tag_to_id_;
		case id_to_tag__:
			return id_to_tag_;
		default:
			invariant(which == tag_to_id__ || which == id_to_tag__);
		}
		return nullptr;
	}

	Mapping const* const* SortedMappingHelper::begin(Which which) const noexcept
	{
		switch (which)
		{
		case tag_to_id__:
			return tag_to_id_;
		case id_to_tag__:
			return id_to_tag_;
		default:
			invariant(which == tag_to_id__ || which == id_to_tag__);
		}
		return nullptr;
	}

	Mapping** SortedMappingHelper::end(Which which) noexcept
	{
		switch (which)
		{
		case tag_to_id__ :
			return tag_to_id_ + mapping_entry_count_;
		case id_to_tag__ :
			return id_to_tag_ + mapping_entry_count_;
		default :
			invariant(which == tag_to_id__ || which == id_to_tag__);
		}
		return nullptr;
	}

	Mapping const* const* SortedMappingHelper::end(Which which) const noexcept
	{
		switch (which)
		{
		case tag_to_id__:
			return tag_to_id_ + mapping_entry_count_;
		case id_to_tag__:
			return id_to_tag_ + mapping_entry_count_;
		default:
			invariant(which == tag_to_id__ || which == id_to_tag__);
		}
		return nullptr;
	}

	/*static */bool SortedMappingHelper::compareByTag(Mapping const *lhs, Mapping const *rhs) noexcept
	{
		return lhs->tag_ < rhs->tag_;
	}
	/*static */bool SortedMappingHelper::compareBySysID(Mapping const *lhs, Mapping const *rhs) noexcept
	{
		return false
			|| static_cast< unsigned int >(lhs->point_type_) < static_cast< unsigned int >(rhs->point_type_)
			|| (lhs->point_type_ == rhs->point_type_) && (lhs->system_id_ < rhs->system_id_)
			;
	}
}}}




 
