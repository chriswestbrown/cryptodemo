#!/usr/bin/perl -w

my @F;

my $total = 0;
while(<>) {
    if ($_ =~ /\["([a-z][a-z])",(\d*)\]/) {
	my $bg = $1;
	my $freq = $2;
	$total += $freq;
	my @A = unpack("C*", $bg);
	my $ascii_a = 97;
	#print "$bg $A[0] $A[1]\n";
	my $i = $A[0] - 97;
	my $j = $A[1] - 97;
	$F[$i][$j] = $freq;
	print "$bg $i $j $freq     $F[$i][$j]\n";
    }
}

print "double bigramfreq[26][26] = [\n";
for(my $i = 0; $i < 26; $i++) {
    print "[";
    for(my $j = 0; $j < 26; $j++) {
	my $f = $F[$i][$j]/$total;
	if ($f == 0) { $f = 1.0/$total; }
	if ($j == 25) { print "$f]"; } else { print "$f, "; }
    }
    if ($i == 25) { print("];\n"); } else { print "],\n"; }
}
