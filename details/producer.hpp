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
#ifndef vlinder_rtimdb_details_producer_hpp
#define vlinder_rtimdb_details_producer_hpp

namespace Vlinder { namespace RTIMDB { namespace Details {
	struct Producer
	{
		Producer()
			: transition_queue_id_(-1)
			, command_queue_id_(-1)
		{ /* no-op */ }
		Producer(
			  unsigned int transition_queue_id
			, unsigned int command_queue_id
			)
			: transition_queue_id_(transition_queue_id)
			, command_queue_id_(command_queue_id)
		{ /* no-op */ }
		unsigned int transition_queue_id_;
		unsigned int command_queue_id_;
	};
}}}

#endif


