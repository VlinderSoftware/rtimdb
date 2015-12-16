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
#include "exceptions.hpp"

using namespace std;

namespace Vlinder { namespace RTIMDB {
#ifdef RTIMDB_ALLOW_EXCEPTIONS
	void throwException(Errors error)
	{
		switch (error)
		{
		case Errors::no_error__ :
			return;
		case Errors::unknown_point__ :
			throw UnknownPoint("Unknown point");
		case Errors::cannot_freeze__:
			throw CannotFreeze("Cannot freeze");
		case Errors::not_allowed__:
			throw NotAllowed("Not allowed");
		case Errors::duplicate_selection__ :
			throw DuplicateSelection("Duplicate selection");
		case Errors::operate_without_select__ :
			throw OperateWithoutSelect("Operate without select");
		case Errors::not_yet_implemented__ : 
			throw NotYetImplemented("Not yet implemented");
		case Errors::expired_while_freezing__:
			throw ExpiredWhileFreezing("Expired while freezing");
		case Errors::database_full__:
			throw DatabaseFull("Database full");
		case Errors::command_queue_full__:
			throw CommandQueueFull("Command queue full");
		case Errors::too_many_transitions__ :
			throw TooManyTransitions("Too many transitions for a single transaction");
		case Errors::transaction_failed__:
			throw TransactionFailed("Transaction failed");
		default:
			throw logic_error("Unknown error type");
		}
	}
#endif
}}

