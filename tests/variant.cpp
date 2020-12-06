#include "exceptions/contract.hpp"
#include "details/variant.hpp"
#include "details/crob.hpp"
#include "catch.hpp"

using namespace Vlinder::RTIMDB::Details;

TEST_CASE("Try to create an instance", "[variant]") {
	CROB crob(CROB::operate_pulse_on__, false, CROB::close__, 1, 0, 0);
	Variant< CROB > var(crob);
	auto da_crob(var.get< CROB >());
	REQUIRE( crob.op_type_ == da_crob.op_type_ );
	REQUIRE( crob.clear_ == da_crob.clear_ );
	REQUIRE( crob.tcc_ == da_crob.tcc_ );
	REQUIRE( crob.count_ == da_crob.count_ );
	REQUIRE( crob.on_time_ == da_crob.on_time_ );
	REQUIRE( crob.off_time_ == da_crob.off_time_ );
};

