
@ARGV == 6 || die "Usage: sacfile type(SH,SV,P,PcP,PKPdf,PKiKP) rayfile cmbexit localdir bindir\n";

my($sacfile, $type,$rayfile,$cmbfile, $LocalDir, $BinDir) = @ARGV;

$M1D = "$LocalDir/Models/prem.grt";
$M3D = "$LocalDir/Models/SteveGrand3D.bin";

$RAD = 111.19;

$TEST_CLASS = "$BinDir/test_class3";

if($type eq "SH"){
    $SKS = "nbounce=1 rlay=148 type=S top=30 bottom=147 of=sks";
} elsif ($type eq "SV"){
    $SKS = "nbounce=1 rlay=148 type=V top=34 bottom=146 of=sks";
} elsif ($type eq "P"){
    $SKS = "nbounce=1 rlay=148 type=P top=34 bottom=146 of=sks";
} elsif ($type eq "PcP"){
    $SKS = "nbounce=1 rlay=148 type=P top=148 bottom=148 of=sks";
} elsif ($type eq "PKPdf"){
    $SKS = "nbounce=1 rlay=148 type=P top=374 bottom=490 of=sks";
} elsif ($type eq "PKiKP"){
    $SKS = "nbounce=1 rlay=148 type=P top=374 bottom=374 of=sks";
} elsif ($type eq "ScS"){
    $SKS = "nbounce=1 rlay=148 type=V top=148 bottom=148 of=sks";
} elsif($type eq "SKS"){
    $SKS = "nbounce=1 rlay=148 type=V top=150 bottom=372 of=sks";
} elsif($type eq "SKKS"){
    $SKS = "nbounce=2 rlay=148 type=VV top=150 bottom=372 of=sks";
} else {
    die "unknown wavetype $type\n";
}

$CMBdepth = "2891";
$tiny     = "1";
$evdp   = `saclst evdp < $sacfile`; chomp($evdp);  
$evla   = `saclst evla < $sacfile`; chomp($evla);  
$evlo   = `saclst evlo < $sacfile`; chomp($evlo);  
$stla   = `saclst stla < $sacfile`; chomp($stla);  
$stlo   = `saclst stlo < $sacfile`; chomp($stlo);  

$SRC = "$evla,$evlo,$evdp";
$ST  = "$stla,$stlo";
$junkfile = "../junkrayfile";
unlink $junkfile;

`$TEST_CLASS model1=$M1D model2=$M3D $SKS source=$SRC station=$ST raypath=$junkfile`;
print stderr "$TEST_CLASS model1=$M1D model2=$M3D $SKS source=$SRC station=$ST raypath=$junkfile";

unlink "sks";

open(RAY0, ">$cmbfile");
open(RAY, ">$rayfile");
open(RAY1, "<$junkfile");
$l = <RAY1>; 
print RAY ">\n";
$line = 0;
while ($l  = <RAY1>){
	chomp($l);
    if($l eq ">"){
		print RAY ">\n";
    } else {
	    $l =~ s{\(}{\,};        # Condense all blank chars into one
	    $l =~ s{\)}{\,};        # Condense all blank chars into one
	    $l =~ s/^\s+//;         # remove the first blank
	    ($dum, $lat, $lon, $dep) = split(/\,/,$l);
	    $dep =~ s/^\s+//;         # remove the first blank
        my($dep) = split(/ /,$dep);
        $dum /= $RAD;
        print RAY "$lat $lon $dum $dep\n";
    	if($dep > $CMBdepth - $tiny && $dep < $CMBdepth + $tiny){
        	print RAY0 "$lat $lon\n";
        }
	}
}
close(RAY1); close(RAY); close(RAY0);

