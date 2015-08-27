#! /usr/bin/perl
my @types = qw(binary_input__ binary_output__ counter__ analog_input__ analog_output__ dataset__ octet_string__);

foreach my $outer_type (@types) {
    foreach my $inner_type (@types) {
        if ($outer_type ne $inner_type) {
            print "		|| pointTypeShallNotSilentlyChange< PointType::${outer_type} , PointType::${inner_type}  >()\n";
        }
    }
}
