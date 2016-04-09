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
#ifndef vlinder_rtimdb_core_details_observer_hpp
#define vlinder_rtimdb_core_details_observer_hpp

#include <functional>
#include "../pointvalue.hpp"

namespace Vlinder { namespace RTIMDB { namespace Core { namespace Details {
	typedef std::function< void(RTIMDB::Details::Action action, PointValue new_value, PointValue old_value) throw() > Observer;
}}}}

#endif

