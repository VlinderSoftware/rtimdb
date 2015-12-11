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
#include <atomic>

namespace Vlinder { namespace RTIMDB { namespace Outstation {
	class RTIMDB_OUTSTATION_API Database
	{
	public :
		Database();
		~Database();

		std::pair< Details::PointDescriptor, bool > createPoint(uintptr_t tag, PointType point_type, bool initial_value RTIMDB_NOTHROW_PARAM);
		std::pair< Details::PointDescriptor, bool > createPoint(uintptr_t tag, PointType point_type, uint16_t initial_value RTIMDB_NOTHROW_PARAM);
		std::pair< Details::PointDescriptor, bool > createPoint(uintptr_t tag, PointType point_type, uint32_t initial_value RTIMDB_NOTHROW_PARAM);
		std::pair< Details::PointDescriptor, bool > createPoint(uintptr_t tag, PointType point_type, float initial_value RTIMDB_NOTHROW_PARAM);
		std::pair< Details::PointDescriptor, bool > createPoint(uintptr_t tag, PointType point_type, double initial_value RTIMDB_NOTHROW_PARAM);

		void associate(Details::PointDescriptor const &point, unsigned int command_queue_id);
		void sendCommand(Details::PointDescriptor const &point, Details::CROB const &crob);

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		unsigned int allocateCommandQueue();
#endif
		std::pair< unsigned int, bool > allocateCommandQueue(RTIMDB_NOTHROW_PARAM_1);
		CommandQueue& getCommandQueue(unsigned int command_queue_id);
		void releaseCommandQueue(unsigned int command_queue_id) noexcept;

	private :
		Database(Database const&) = delete;
		Database& operator=(Database const&) = delete;

		CommandQueue command_queues_[RTIMDB_MAX_COMMAND_QUEUE_COUNT];
		std::atomic< bool > command_queue_allocations_[RTIMDB_MAX_COMMAND_QUEUE_COUNT];
		Core::DataStore data_store_;
	};
}}}

#endif

