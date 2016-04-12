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
#ifndef vlinder_rtimdb_details_sortedmappinghelper_hpp
#define vlinder_rtimdb_details_sortedmappinghelper_hpp

#include "mappinghelper.hpp"

namespace Vlinder { namespace RTIMDB { namespace Details {
	class SortedMappingHelper : public MappingHelper
	{
	public :
		SortedMappingHelper();
		virtual ~SortedMappingHelper();

		virtual void clear() noexcept override final;

		virtual Errors insert(uintptr_t tag, Core::PointType point_type, unsigned int system_id) override final;
		virtual Mapping const *at(unsigned int index) const noexcept override final;

		virtual Mapping const* find(Core::PointType point_type, unsigned int system_id) const noexcept override final;
		virtual Mapping* find(Core::PointType point_type, unsigned int system_id) noexcept override final;
		virtual Mapping const* find(uintptr_t tag) const noexcept override final;
		virtual Mapping* find(uintptr_t tag) noexcept override final;
		
		virtual unsigned int size() const noexcept override final;
		unsigned int capacity() const noexcept;

	private :
		enum Which {tag_to_id__, id_to_tag__};

		Mapping** begin(Which which) noexcept;
		Mapping const* const* begin(Which which) const noexcept;
		Mapping** end(Which which) noexcept;
		Mapping const* const* end(Which which) const noexcept;

		static bool compareByTag(Mapping const *lhs, Mapping const *rhs) noexcept;
		static bool compareBySysID(Mapping const *lhs, Mapping const *rhs) noexcept;

		Mapping mappings_[RTIMDB_POINT_COUNT];
		Mapping *tag_to_id_[RTIMDB_POINT_COUNT];
		Mapping *id_to_tag_[RTIMDB_POINT_COUNT];
		unsigned int mapping_entry_count_;
	};
}}}

#endif



