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
#ifndef vlinder_rtimdb_core_datastore_hpp
#define vlinder_rtimdb_core_datastore_hpp

#include "../details/prologue.hpp"
#include "../details/action.hpp"
#include "details/selection.hpp"
#include "rtimdb_config.hpp"
#include "details/cell.hpp"
#include "../exceptions.hpp"
#include "details/iterator.hpp"
#include "details/optional.hpp"
#include <memory>
#include <utility>

namespace Vlinder { namespace RTIMDB { namespace Core {
	class RTIMDB_API DataStore
	{
	public :
		typedef Details::Iterator const_iterator;

		DataStore();
		~DataStore();

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		unsigned int insert(Point const &value);
#endif
		std::pair< unsigned int, Errors > insert(Point value RTIMDB_NOTHROW_PARAM) throw();

		const_iterator begin();
		const_iterator end();

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		void update(unsigned int index, Point new_value);
		void update(Details::Transaction &transaction, unsigned int index, Point new_value);
#endif
		Errors update(unsigned int index, Point new_value RTIMDB_NOTHROW_PARAM) throw();
		Errors update(Details::Transaction &transaction, unsigned int index, Point new_value RTIMDB_NOTHROW_PARAM) throw();

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		Point read(PointType type, unsigned int index) const;
#endif
		std::pair< Details::Optional< Point >, Errors > read(PointType type, unsigned int index RTIMDB_NOTHROW_PARAM) const throw();
#ifdef RTIMDB_ALLOW_EXCEPTIONS
		Point read(Details::ROTransaction const &transaction, PointType type, unsigned int index) const;
#endif
		std::pair< Details::Optional< Point >, Errors > read(Details::ROTransaction const &transaction, PointType type, unsigned int index RTIMDB_NOTHROW_PARAM) const throw();

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		Details::Transaction startTransaction();
		Details::ROTransaction startROTransaction();
		void commit(Details::Transaction &transaction);
#endif
		std::pair< Details::Transaction, Errors > startTransaction(RTIMDB_NOTHROW_PARAM_1) throw();
		Errors commit(Details::Transaction &transaction RTIMDB_NOTHROW_PARAM);
		std::pair< Details::ROTransaction, Errors > startROTransaction(RTIMDB_NOTHROW_PARAM_1) throw();

	private :
		DataStore(DataStore const&) = delete;
		DataStore& operator=(DataStore const&) = delete;

		void thaw(unsigned int frozen_version);

		std::pair< Details::Cell< RTIMDB_CELL_SIZE > *const*, Errors > fetch(PointType type, unsigned int index) const;
		void freezeCells(unsigned int frozen_version);

		unsigned int getPointCount(PointType point_type) const;
        unsigned int getPointOffset(PointType point_type, unsigned int index) const;
        PointType getPointTypeAtOffset(unsigned int offset) const;
		Details::Locator advance(Details::Locator const &curr_location) const;

		std::pair< Details::Transaction, Errors > startTransaction_(bool write_enable) throw();

		// commit phases
		Errors tagTransitions(Details::Transaction &transaction) noexcept;
		void sortTransitions(Details::Transaction &transaction) noexcept;
		Errors lockCells(Details::Transaction &transaction) noexcept;
		void applyChanges(Details::Transaction &transaction) noexcept;
		void unlockCells(Details::Transaction &transaction) noexcept;

		std::atomic< unsigned int > frozen_versions_[RTIMDB_MAX_CONCURRENT_TRANSACTIONS];
		unsigned int start_index_[static_cast< unsigned int >(PointType::_type_count__) + 1];
		std::atomic< unsigned int > curr_version_;
		Details::Cell< RTIMDB_CELL_SIZE > *points_[RTIMDB_POINT_COUNT];
		Details::Cell< RTIMDB_CELL_SIZE > cells_[RTIMDB_POINT_COUNT];
		unsigned int next_cell_;

		friend class Details::Iterator;
	};
}}}

#endif

