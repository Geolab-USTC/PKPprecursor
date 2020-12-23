#!/usr/bin/perl -w

@ARGV == 12 or die "Usage: $0 rdepth Gcmin GcMax period dlat dlon dlatshift dlonshift directory appendix maindir bindir\n";

my($rdepth,$GcarcMin, $GcarcMax, $period, $dlat, $dlon, $dlatshift, $dlonshift, $direc,$appendix, $MainDir, $BinDir) = @ARGV;

$FINDPATCH = "$BinDir/findpatch";
$SACLST    = "$BinDir/saclst";

$ts2rf     = "../CMBout$rdepth";   
$tr2sf     = "$MainDir/CMBout$rdepth";
$ds2rf     = "../CMBout${rdepth}1";
$dr2sf     = "$MainDir/CMBout${rdepth}1";



print STDERR "find patchs with files ended by $appendix for gcarc = $GcarcMin, $GcarcMax\n";

chdir $direc;
print STDERR "change to directory $direc\n";

@STATION = `ls \*.$appendix`;
for($st = 0; $st < @STATION; $st++){

    $SACFILE = "$STATION[$st]"; chomp($SACFILE);
    $gcarc = &GCARC($SACFILE);  chomp($gcarc);
    $evdp  = `$SACLST evdp <$SACFILE`; chomp($evdp);
    $stnm  = `$SACLST kstnm <$SACFILE`; chomp($stnm); chomp($stnm);
    print STDERR "evdp = $evdp $stnm gcarc=$gcarc\n";

    &Envelop($SACFILE, "../junksac");

    $patchdir = "patch$rdepth";

    $orf       = "../$patchdir/$SACFILE.r";
    $osf       = "../$patchdir/$SACFILE.s";


    $SACFILE = "../junksac";
    $PKPdf     = &ttimes("PKP",$evdp,$gcarc);
    print STDERR "$orf $osf $PKPdf\n";

    if($gcarc >= $GcarcMin && $gcarc <= $GcarcMax){

        print STDERR "$FINDPATCH Seis=$SACFILE ts2rf=$ts2rf tr2sf=$tr2sf ds2rf=$ds2rf dr2sf=$dr2sf orf=$orf osf=$osf PKPdf=$PKPdf checknoise=0 period=$period dlat=$dlat dlon=$dlon";

        `$FINDPATCH Seis=$SACFILE ts2rf=$ts2rf tr2sf=$tr2sf ds2rf=$ds2rf dr2sf=$dr2sf orf=$orf osf=$osf PKPdf=$PKPdf checknoise=0 period=$period dlat=$dlat dlon=$dlon dlatshift=$dlatshift dlonshift=$dlonshift`;

    }

    unlink "../junksac";

}



############
#subroutine#
############

sub Envelop{
    my($in, $out) = @_;
    open(SAC,"|sac2000");
        print SAC "r $in\n";
        print SAC "rmean\n";
        print SAC "envelope\n";
        print SAC "w $out\n";
        print SAC "quit\n";
    close(SAC);
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
      #  $gc = `$SACLST GCARC <$sfile`; chomp($gc);
        $gc = `$SACLST GCARC <../junksac`; chomp($gc);
    }
	unlink "../junksac";
    return $gc;
}


sub ttimes{
  my ($phase,$depth,$gcarc)=@_;
  my @ll;
  my $l;
  my $t;
  my @c;
  my $tmp_file = "/tmp/LL_times00";
  open(TT,"| $BinDir/ttimes > $tmp_file") || die "Cannot start ttimes!\n";
  print TT "$phase\n\n$depth\n$gcarc\n-1\n-1\n";  close(TT);
  open(TT,"< $tmp_file");  @ll = <TT>;  close(TT); 
  unlink $tmp_file;
  while ($l = shift @ll) {
    if($l =~ /^Source depth \(km\):/){
       shift @ll;       shift @ll;
       @c = split(' ',shift @ll); 
       $t = 50000;
      # if($c[2] =~ /^$phase/) {
       if($c[2] =~ /^PKPdf/) {
	   $t = $c[3];
       }
       while(@c = split(' ', shift @ll)) {
	 if( $c[1] =~ /^$phase/){
	     $t = $t < $c[2] ? $t : $c[2];
         }
       }
     }
  }
  return $t;
}

 
