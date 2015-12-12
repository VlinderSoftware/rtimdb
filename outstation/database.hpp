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
#ifndef vlinder_rtimdb_outstation_database_hpp
#define vlinder_rtimdb_outstation_database_hpp

#include "details/prologue.hpp"
#include "rtimdb_config.hpp"
#include "commandqueue.hpp"
#include "details/pointdescriptor.hpp"
#include "../pointtype.hpp"
#include "../core/datastore.hpp"
#include "../exceptions.hpp"
#include <atomic>

namespace Vlinder { namespace RTIMDB { namespace Outstation {
	class RTIMDB_OUTSTATION_API Database
	{
	public :
		Database();
		~Database();

		std::pair< unsigned int, Errors > createPoint(uintptr_t tag, PointType point_type, bool initial_value RTIMDB_NOTHROW_PARAM) noexcept;
		std::pair< unsigned int, Errors > createPoint(uintptr_t tag, PointType point_type, int16_t initial_value RTIMDB_NOTHROW_PARAM) noexcept;
		std::pair< unsigned int, Errors > createPoint(uintptr_t tag, PointType point_type, int32_t initial_value RTIMDB_NOTHROW_PARAM) noexcept;
		std::pair< unsigned int, Errors > createPoint(uintptr_t tag, PointType point_type, uint16_t initial_value RTIMDB_NOTHROW_PARAM) noexcept;
		std::pair< unsigned int, Errors > createPoint(uintptr_t tag, PointType point_type, uint32_t initial_value RTIMDB_NOTHROW_PARAM) noexcept;
		std::pair< unsigned int, Errors > createPoint(uintptr_t tag, PointType point_type, float initial_value RTIMDB_NOTHROW_PARAM) noexcept;
		std::pair< unsigned int, Errors > createPoint(uintptr_t tag, PointType point_type, double initial_value RTIMDB_NOTHROW_PARAM) noexcept;

		void associate(unsigned int point_id, unsigned int command_queue_id);
		Errors sendCommand(unsigned int point_id, Details::CROB const &crob RTIMDB_NOTHROW_PARAM);

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		unsigned int allocateCommandQueue();
#endif
		std::pair< unsigned int, bool > allocateCommandQueue(RTIMDB_NOTHROW_PARAM_1);
		CommandQueue& getCommandQueue(unsigned int command_queue_id);
		void releaseCommandQueue(unsigned int command_queue_id) noexcept;

	private :
		Database(Database const&) = delete;
		Database& operator=(Database const&) = delete;

		std::pair< unsigned int, Errors > createPoint_(uintptr_t tag, PointType point_type, unsigned int data_store_id) noexcept;

		Details::PointDescriptor point_descriptors_[RTIMDB_POINT_COUNT];
		std::atomic< unsigned int > next_point_id_;
		CommandQueue command_queues_[RTIMDB_MAX_COMMAND_QUEUE_COUNT];
		std::atomic< bool > command_queue_allocations_[RTIMDB_MAX_COMMAND_QUEUE_COUNT];
		Core::DataStore data_store_;
	};
}}}

#endif

