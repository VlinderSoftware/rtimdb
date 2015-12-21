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
#ifndef vlinder_rtimdb_details_crob_hpp
#define vlinder_rtimdb_details_crob_hpp

#include "prologue.hpp"
#include <cstdint>

namespace Vlinder { namespace RTIMDB { namespace Details {
	struct CROB
	{
		enum OpType {
			  operate_nul__
			, operate_pulse_on__
			, operate_pulse_off__
			, operate_latch_on__
			, operate_latch_off__
		};
		enum TripCloseCode {
			  nul__
			, close__
			, trip__
		};

		CROB()
			: op_type_(operate_nul__)
		{ /* no-op */ }
		CROB(
			  OpType op_type
			, bool clear
			, TripCloseCode tcc
			, unsigned int count
			, uint32_t on_time
			, uint32_t off_time
			)
			: op_type_(op_type)
			, clear_(clear)
			, tcc_(tcc)
			, count_(count)
			, on_time_(on_time)
			, off_time_(off_time)
		{ /* no-op */ }

		OpType op_type_;
		bool clear_; // indicates to clear any pending commands for this point and execute only this one (execute nothing if op_type_ is operate_nul__)
		TripCloseCode tcc_;
		unsigned int count_;
		uint32_t on_time_;
		uint32_t off_time_;
	};
}}}

#endif

