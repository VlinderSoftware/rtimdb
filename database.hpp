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
#ifndef vlinder_rtimdb_database_hpp
#define vlinder_rtimdb_database_hpp

#include "details/prologue.hpp"
#include "rtimdb_config.hpp"
#include "cell.hpp"
#include "exceptions.hpp"
#include "details/iterator.hpp"
#include "details/variant.hpp"
#include <memory>
#include <utility>

namespace Vlinder { namespace RTIMDB {
	class RTIMDB_API Database
	{
	public :
		typedef Details::Iterator const_iterator;
		
		Database();
		~Database();

		unsigned int insert(Point const &value);

		const_iterator begin();
		const_iterator end();

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		void update(PointType type, unsigned int index, bool value);
		void update(PointType type, unsigned int index, DoubleBitBinary const &value);
		void update(PointType type, unsigned int index, uint32_t value);
		void update(PointType type, unsigned int index, double value);
		void update(PointType type, unsigned int index, Time value);
		void update(PointType type, unsigned int index, File *value);
		void update(PointType type, unsigned int index, Dataset *value);
		void update(PointType type, unsigned int index, uint16_t value);
		void update(PointType type, unsigned int index, uint8_t value);
		void update(PointType type, unsigned int index, String *value);
#endif
		Errors update(PointType type, unsigned int index, bool value RTIMDB_NOTHROW_PARAM) throw();
		Errors update(PointType type, unsigned int index, DoubleBitBinary const &value RTIMDB_NOTHROW_PARAM) throw();
		Errors update(PointType type, unsigned int index, uint32_t value RTIMDB_NOTHROW_PARAM) throw();
		Errors update(PointType type, unsigned int index, double value RTIMDB_NOTHROW_PARAM) throw();
		Errors update(PointType type, unsigned int index, Time value RTIMDB_NOTHROW_PARAM) throw();
		Errors update(PointType type, unsigned int index, File *value RTIMDB_NOTHROW_PARAM) throw();
		Errors update(PointType type, unsigned int index, Dataset *value RTIMDB_NOTHROW_PARAM) throw();
		Errors update(PointType type, unsigned int index, uint16_t value RTIMDB_NOTHROW_PARAM) throw();
		Errors update(PointType type, unsigned int index, uint8_t value RTIMDB_NOTHROW_PARAM) throw();
		Errors update(PointType type, unsigned int index, String *value RTIMDB_NOTHROW_PARAM) throw();

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		Point read(PointType type, unsigned int index) const;
#endif
		std::pair< Details::Variant< Point >, Errors > read(PointType type, unsigned int index RTIMDB_NOTHROW_PARAM) const throw();
#ifdef RTIMDB_ALLOW_EXCEPTIONS
		Point read(Details::Transaction transaction, PointType type, unsigned int index) const;
#endif
		std::pair< Details::Variant< Point >, Errors > read(Details::Transaction transaction, PointType type, unsigned int index RTIMDB_NOTHROW_PARAM) const throw();

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		Details::Transaction freeze();
#endif
		std::pair< Details::Transaction, Errors > freeze(RTIMDB_NOTHROW_PARAM_1) throw();

	private :
		Database(Database const&) = delete;
		Database& operator=(Database const&) = delete;

		void thaw(unsigned int frozen_version);

		std::pair< Cell< RTIMDB_CELL_SIZE > *const*, Errors > fetch(PointType type, unsigned int index) const;
		void freezeCells(unsigned int frozen_version);

		unsigned int getPointCount(PointType point_type) const;
        unsigned int getPointOffset(PointType point_type, unsigned int index) const;
        PointType getPointTypeAtOffset(unsigned int offset) const;
		Details::Locator advance(Details::Locator const &curr_location) const;

		std::atomic< unsigned int > frozen_versions_[RTIMDB_MAX_CONCURRENT_TRANSACTIONS];
		unsigned int start_index_[static_cast< unsigned int >(PointType::_type_count__) + 1];
		std::atomic< unsigned int > curr_version_;
		Cell< RTIMDB_CELL_SIZE > *points_[RTIMDB_POINT_COUNT];
		Cell< RTIMDB_CELL_SIZE > cells_[RTIMDB_POINT_COUNT];
		unsigned int next_cell_;

		friend class Details::Iterator;
	};
}}

#endif

