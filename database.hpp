#ifndef vlinder_rtimdb_database_hpp
#define vlinder_rtimdb_database_hpp

#include "details/prologue.hpp"
#include "rtimdb_config.hpp"
#include "cell.hpp"
#include "exceptions.hpp"
#include "details/variant.hpp"
#include <memory>
#include <utility>

namespace Vlinder { namespace RTIMDB {
	class RTIMDB_API Database
	{
	public :
		typedef std::shared_ptr< std::atomic< unsigned int > const > Transaction;
		
		Database();
		~Database();

		unsigned int insert(Point const &value);

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

		Point read(PointType type, unsigned int index) const;
		std::pair< Details::Variant< Point >, Errors > read(PointType type, unsigned int index, std::nothrow_t const&) const throw();
		Point read(Transaction transaction, PointType type, unsigned int index) const;
		std::pair< Details::Variant< Point >, Errors > read(Transaction transaction, PointType type, unsigned int index, std::nothrow_t const&) const throw();

		Transaction freeze();
		std::pair< Transaction, Errors > freeze(std::nothrow_t const&) throw();

	private :
		Database(Database const&) = delete;
		Database& operator=(Database const&) = delete;

		void thaw(unsigned int frozen_version);

		std::pair< Cell< RTIMDB_CELL_SIZE > *const*, Errors > fetch(PointType type, unsigned int index) const;
		void freezeCells(unsigned int frozen_version);

		std::atomic< unsigned int > frozen_versions_[RTIMDB_MAX_CONCURRENT_TRANSACTIONS];
		unsigned int start_index_[static_cast< unsigned int >(PointType::_type_count__) + 1];
		std::atomic< unsigned int > curr_version_;
		Cell< RTIMDB_CELL_SIZE > *points_[RTIMDB_POINT_COUNT];
		Cell< RTIMDB_CELL_SIZE > cells_[RTIMDB_POINT_COUNT];
		unsigned int next_cell_;
	};
}}

#endif

