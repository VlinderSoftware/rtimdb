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
#ifndef vlinder_rtimdb_commandqueue_hpp
#define vlinder_rtimdb_commandqueue_hpp

#include "details/prologue.hpp"
#include "exceptions/exception.hpp"
#include "command.hpp"
#include "rtimdb_config.hpp"
#include <stdexcept>
#include <atomic>

namespace Vlinder { namespace RTIMDB {
	namespace Private {
		class CommandQueueTestAttorney;
	}
	class Database;
	class RTIMDB_API CommandQueue
	{
	public :
		enum CommandQueueErrors {
			  queue_empty__
			, queue_full__
		};
		typedef Vlinder::Exceptions::Exception< std::runtime_error, CommandQueueErrors, queue_empty__ > QueueEmpty;
		typedef Vlinder::Exceptions::Exception< std::runtime_error, CommandQueueErrors, queue_full__ > QueueFull;

		CommandQueue();
		~CommandQueue();

		bool empty() const noexcept;
		unsigned int capacity() const noexcept;
		unsigned int size() const noexcept;
#ifdef RTIMDB_ALLOW_EXCEPTIONS
		Command front() const;
#endif
		std::pair< Command, bool > front(RTIMDB_NOTHROW_PARAM_1) const noexcept;
		void pop() noexcept;

		bool overflow() const;

	private : // API used just by friends
#ifdef RTIMDB_ALLOW_EXCEPTIONS
		void push(Command const &command);
#endif
		bool push(Command const &command RTIMDB_NOTHROW_PARAM) noexcept;

	private :
		CommandQueue(CommandQueue const&) = delete;
		CommandQueue& operator=(CommandQueue const&) = delete;

		Command commands_[RTIMDB_COMMAND_QUEUE_CAPACITY];
		std::atomic< unsigned int > head_;
		std::atomic< unsigned int > tail_;
		unsigned int cached_head_;
		mutable unsigned int cached_tail_;

		mutable std::atomic< bool > overflow_;

		friend class Database;
		friend class Private::CommandQueueTestAttorney;
	};
}}

#endif

