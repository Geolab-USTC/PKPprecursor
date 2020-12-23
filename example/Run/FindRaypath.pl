#!/usr/bin/perl -w

@ARGV == 7 or die "Usage: $0 Gcmin Gcmax directory appendix maindir localdir bindir\n";

my($Gcmin, $Gcmax, $direc,$appendix, $MainDir, $LocalDir, $BinDir) = @ARGV;

$SACLST    = "$BinDir/saclst";
$RAYPATH = "$MainDir/raypath.pl";

$phase    = "PKPdf";

chdir $direc;
print STDERR "change to directory $direc\n";

@STATION = `ls \*.$appendix`;
for($st = 0; $st < @STATION; $st++){

    $SACFILE = "$STATION[$st]"; chomp($SACFILE);

    print STDERR "SAC file = $SACFILE\n";
    $rayfile = "../paths/$SACFILE.$phase";
    $cmbfile = "../cmbexits/$SACFILE.$phase";

    $gcarc = &GCARC($SACFILE);

    if($gcarc >= $Gcmin && $gcarc <= $Gcmax){
     	print STDERR "SAC = $SACFILE\n";
        `perl $RAYPATH $SACFILE $phase $rayfile $cmbfile $LocalDir $BinDir`;
    }

}

sub GCARC{

    my($sfile) = @_;
    $gc = `$SACLST GCARC <$sfile`; chomp($gc);
    if($gc < 0) {
        open(SAC,"|sac2000");
            print SAC "r $sfile\n";
            print SAC "w ../junksac\n";
            print SAC "quit\n";
        close(SAC);
        $gc = `$SACLST GCARC <../junksac`; chomp($gc);
    }
	unlink "../junksac";
    return $gc;

}

