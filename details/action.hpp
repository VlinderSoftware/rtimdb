#ifndef vlinder_rtimdb_details_action_hpp
#define vlinder_rtimdb_details_action_hpp

namespace Vlinder { namespace RTIMDB { namespace Details {
	enum struct Action {
		  update__
		, write__
		, select__
		, operate__
		, direct_operate__
		, freeze__
		, freeze_and_clear__
		};
}}}

#endif

