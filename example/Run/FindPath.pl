#!/usr/bin/perl -w
@ARGV == 10 or die "Usage: $0 cdistmin cdistmax cdistinc  rdepth direct file direc appendix maindir bindir\n";

my($cdistmin,$cdistmax,$cdistinc, $rdepth, $direct, $file,$direc,$appendix, $MainDir, $BinDir)  = @ARGV;

$FINDPATH = "$BinDir/findpath1";
$SACLST    = "$BinDir/saclst";

$modelfile   = "$MainDir/prem1.par";
$top         = 154;
$bottom      = 373;

$RAD         = 111.194924748;

$cdistmax *= $RAD;
$cdistmin *= $RAD;
$cdistinc *= $RAD;

chdir $direc;
print  STDERR "change to directory $direc\n";

@STATION = `ls \*.$appendix`;
for($st = 0; $st < 1; $st++){

    $SACFILE = "$STATION[$st]"; chomp($SACFILE);    
    $sdepth  = `$SACLST evdp < $SACFILE`; chomp($sdepth);
     print STDERR  "evdp = $sdepth\n";
}

&MinTs();


sub MinTs{

    @l = `$FINDPATH modelfile=$modelfile sdepth=$sdepth top=$top bottom=$bottom dist=$cdistmin,$cdistmax,$cdistinc rdepth=$rdepth direct=$direct`;

    open(OUT,">$file") or die "cannot open file $file\n";
        print OUT @l;
    close(OUT);
}




