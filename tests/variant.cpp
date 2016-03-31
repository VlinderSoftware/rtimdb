#include "exceptions/contract.hpp"
#include "details/variant.hpp"
#include "details/crob.hpp"

using namespace Vlinder::RTIMDB::Details;

int tryCreateInstance()
{
	CROB crob(CROB::operate_pulse_on__, false, CROB::close__, 1, 0, 0);
	Variant< CROB > var(crob);
	auto da_crob(var.get< CROB >());
	assert(crob.op_type_ == da_crob.op_type_);
	assert(crob.clear_ == da_crob.clear_);
	assert(crob.tcc_ == da_crob.tcc_);
	assert(crob.count_ == da_crob.count_);
	assert(crob.on_time_ == da_crob.on_time_);
	assert(crob.off_time_ == da_crob.off_time_);
	
	return 0;
};

int main()
{
	return 0
		|| tryCreateInstance()
		;
}
