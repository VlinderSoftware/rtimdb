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
#include "commandqueue.hpp"
#include "details/transitionqueue.hpp"
#include "details/pointdescriptor.hpp"
#include "core/timestamp.hpp"
#include "details/producer.hpp"
#include "details/consumer.hpp"
#include "core/pointtype.hpp"
#include "core/datastore.hpp"
#include "exceptions.hpp"
#include <atomic>
#include <mutex>

namespace Vlinder { namespace RTIMDB {
	namespace Core { namespace Details { class Transaction; } }
	class RTIMDB_API Database
	{
	public :
		Database();
		~Database();

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		Details::Producer const *registerProducer();
		Details::Consumer* registerConsumer();
		unsigned int createPoint(Details::Producer const *producer, Core::PointType const &point_type, Core::PointValue const &initial_value = Core::PointValue(), Core::Flags const &initial_flags = Core::Flags(), Core::Timestamp const &initial_timestamp = Core::Timestamp());

		void sendCommand(unsigned int point_id, Details::CROB const &crob);

		Core::Details::ROTransaction beginTransaction(Details::Consumer *consumer);
#endif
		std::pair< Details::Producer const *, Errors > registerProducer(RTIMDB_NOTHROW_PARAM_1) noexcept;
		std::pair< Details::Consumer*, Errors > registerConsumer(RTIMDB_NOTHROW_PARAM_1) noexcept;
		void unregisterProducer(Details::Producer const *producer) noexcept;
		void unregisterConsumer(Details::Consumer const *consumer) noexcept;

		std::pair< unsigned int, Errors > createPoint(Details::Producer const *producer, Core::PointType const &point_type RTIMDB_NOTHROW_PARAM) noexcept;
		std::pair< unsigned int, Errors > createPoint(Details::Producer const *producer, Core::PointType const &point_type, Core::PointValue const &initial_value RTIMDB_NOTHROW_PARAM) noexcept;
		std::pair< unsigned int, Errors > createPoint(Details::Producer const *producer, Core::PointType const &point_type, Core::PointValue const &initial_value, Core::Flags const &initial_flags RTIMDB_NOTHROW_PARAM) noexcept;
		std::pair< unsigned int, Errors > createPoint(Details::Producer const *producer, Core::PointType const &point_type, Core::PointValue const &initial_value, Core::Flags const &initial_flags, Core::Timestamp const &initial_timestamp RTIMDB_NOTHROW_PARAM) noexcept;

		Errors sendCommand(unsigned int point_id, Details::CROB const &crob RTIMDB_NOTHROW_PARAM);

		std::pair< Core::Details::ROTransaction, Errors > beginTransaction(Details::Consumer *consumer RTIMDB_NOTHROW_PARAM) noexcept;
		Details::TransitionQueueTransaction beginTransaction(Details::Producer const *producer, Core::Timestamp const &timestamp) noexcept;
		void signalOverflow(Details::Producer const *producer) noexcept;
		void commit(Details::Producer const *producer, Details::TransitionQueueTransaction const &transaction);

#ifdef RTIMDB_ALLOW_EXCEPTIONS
		void update();
#endif
		Errors update(RTIMDB_NOTHROW_PARAM_1) noexcept;

		CommandQueue& getCommandQueue(Details::Producer const *producer) noexcept;
		Details::TransitionQueue& getTransitionQueue(Details::Producer const *producer) noexcept;

		std::pair< Core::Details::Optional< Core::Point >, Errors > read(Core::Details::ROTransaction const &transaction, Core::PointType point_type, unsigned int system_id) const noexcept;

	private :
		Database(Database const&) = delete;
		Database& operator=(Database const&) = delete;

		std::pair< unsigned int, Errors > createPoint_(Details::Producer const *producer, Core::PointType point_type, unsigned int data_store_id) noexcept;
		Errors commitTransaction(Core::Timestamp const &timestamp, Core::Details::Transaction &transaction) noexcept;
		Errors dispatch(Core::Timestamp const &timestamp, Core::PointType type, unsigned int id, Core::PointValue const &value, Core::Flags const &flags) noexcept;

		Details::PointDescriptor point_descriptors_[RTIMDB_POINT_COUNT];
		std::atomic< unsigned int > next_point_id_;
		CommandQueue command_queues_[RTIMDB_MAX_PRODUCER_COUNT];
		Details::TransitionQueue transition_queues_[RTIMDB_MAX_PRODUCER_COUNT];
		Core::DataStore data_store_;
		Core::Timestamp latest_timestamp_[RTIMDB_MAX_PRODUCER_COUNT];
		Details::Producer producers_[RTIMDB_MAX_PRODUCER_COUNT];
		Details::Consumer consumers_[RTIMDB_MAX_CONSUMER_COUNT];
		std::atomic< bool > producer_allocations_[RTIMDB_MAX_PRODUCER_COUNT];
		std::atomic< bool > consumer_allocations_[RTIMDB_MAX_CONSUMER_COUNT];
		std::mutex update_lock_;
	};
}}

#endif

