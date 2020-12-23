#!/usr/bin/perl -w
@ARGV == 19 or die "Usage: $0 xshift yshift lo0 lo1 lo2 lo3 la0 la1 la2 la3 patchfile cmbexits eventfile stationfile pathfile txtfile dlat dlon PSfile\n";

my($xshift,$yshift,$lo0,$lo1,$lo2,$lo3,$la0,$la1,$la2,$la3,$patch, $cmb, $EVENT, $STATION, $PATH, $TXT,  $dlat, $dlon, $PS) = @ARGV;


$R         = "-R$lo0/$lo1/$la0/$la1";
$B         = "-B20f10/a20f10WSne";
$PORT      = "-P";
$J="-Jx0.09d";

if($la1 ==90 ){$J="-Jx0.05d";}

@patchfile     = split(',',$patch);
@cmbexits      = split(',',$cmb);
@EVENT_FILE    = split(',',$EVENT);
@STATION_FILE  = split(',',$STATION);
@PATH_FILE     = split(',',$PATH);
@TXT_FILE      = split(',',$TXT);

&open_plt($xshift,$yshift);

$dshift = 1.5; $yy =16;
$xx=1;

@lati0=($la0,$la2);
@lati1=($la1,$la3);
@long0=($lo0,$lo2);
@long1=($lo1,$lo3);

for($j=0;$j<@lati1;$j++)
{
	$lat0       = $lati0[$j];
	$lat1       = $lati1[$j];
	$lon0       = $long0[$j];
	$lon1       = $long1[$j];
	$R          = "-R$lon0/$lon1/$lat0/$lat1";

	for($ii = 0; $ii < @patchfile; $ii++) {

	    &Plt_patch($j,$ii,$xx,$yy,"$patchfile[$ii]","$cmbexits[$ii]","$EVENT_FILE[$ii]","$STATION_FILE[$ii]","$PATH_FILE[$ii]","$TXT_FILE[$ii]");
	 	$yy=16;$xx=9;
	}

	$B   = "-B20f10/a20f10wSnE";

}


$xshift *= -1; $yshift *= -1;
&close_plt($xshift,$yshift);





sub Plt_patch{
    my($watch,$num,$x,$y,$patchfile,$cmbexits,$EVENT_FILE,$STATION_FILE,$PATH_FILE,$TXT_FILE) = @_;

    $plotpatch       = "yes";
    $plothotspot     = "no";
    $plotcmbexits    = "no";
    $plotevents      = "no";
    $plotstations    = "no";
    $plotgcr         = "no";
    $plottext        = "no";
    $plottraveltime  = "no";
    $plotscale       = ($num == 0) ? "yes" : "no";
#	$plotscale       = "no";

    &shift_plt($x,$y);

    if($plotpatch eq "yes"){
# 	Plot the patchs 
# 		$dx = 3.5; $dy = 0;
#    	$dx = 9.8; $dy = 0;
		$dx = 0; $dy = -11;
    
	    $intensity = 1;
	    &plt_patch($watch,$intensity,$patchfile,$plotscale);
	    &shift_plt($dx,$dy);
	    $dx *= -1.0; $dy *= -1;

	    $intensity = 0;
	    &plt_patch($watch,$intensity,$patchfile,$plotscale);
	    `pscoast $R $J $B -K -O -W1p/50 -A500 >> $PS`;
	    &shift_plt($dx,$dy);
    }

    `pscoast $R $J $B -K -O -W1p/50 -A500 >> $PS`;
    
     if($plotgcr eq "yes"){
        # Plot the great circle path 
       # `psxy $R $J -W3/150 -: -M -K -O  $PATH_FILE >> $PS`;
        `psxy $R $J -W5/150 -: -M -K -O  $PATH_FILE >> $PS`;
       # `psxy $R $J -W5/0 -: -M -K -O  $PATH_FILE >> $PS`;
    }

    if($plotcmbexits eq "yes"){
        #Plot cmbexits:
    }

    if($plotcmbexits eq "yes"){
        #Plot cmbexits:
      #  `psxy $R $J -Sx0.2 -: -Gblack -K -O $cmbexits >> $PS`;
        `psxy $R $J -Sc0.3 -: -G120/170/120 -K -O $cmbexits >> $PS`;
      #  `psxy $R $J -Sc0.3 -: -Gwhite -Wthick -K -O $cmbexits >> $PS`;
      #  `psxy $R $J -Sc0.1 -: -Gwhite -Wthick -K -O $cmbexits >> $PS`;
    }

    if($plotevents eq "yes"){
        #Plot events:
        #`psxy $R $J -Sa0.2 -: -K -O $EVENT_FILE -G0/0/0>> $PS`;
        `psxy $R $J -Sa0.5 -:  -Gred -K -O $EVENT_FILE -G0/0/0>> $PS`;
    }

    if($plotstations eq "yes"){
        # Plot stations:
      #  `psxy $R $J -St0.2 -: -K -O -Ggreen $STATION_FILE >> $PS`;
       # `psxy $R $J -St0.2 -: -Ggreen -K -O $STATION_FILE >> $PS`;
        `psxy $R $J -St0.4 -: -Gblack -K -O $STATION_FILE >> $PS`;
	
#	open(PS,"|pstext $R  $J -O  -K -P -N >> $PS");
#	print PS
  	#  "-7.01  117     12 0 1 2  2011/11/28\n",      
#    	"115   -9.5     12 0 1 2  2011/11/28\n",     
#    	"122.51 -11.5    12 0 1 2  2011/06/27\n",
#    	"116.76  -6   12 0 1 2  2011/03/10\n";
#	close(PS);

    }

    if($plothotspot eq "yes"){
        # Plot hotspot 
        `psxy $R $J -Sc0.08 -: -K -O $hotspot >> $PS`;
    }
 #   if($plotgcr eq "yes"){
        # Plot the great circle path 
       # `psxy $R $J -W3/150 -: -M -K -O  $PATH_FILE >> $PS`;
 #       `psxy $R $J -W8/0 -: -M -K -O  $PATH_FILE >> $PS`;
 #   }

    if($plottext eq "yes"){
        # Plot the texts:
        `pstext $R $J  -O -: -K -N $TXT_FILE>> $PS`;
    }
#    if($plottraveltime eq "yes"){
#        # Plot stations:
#        $norm = 0.01;
#        `awk \'{print(\$1,\$2,\$3*$norm)} \' $timefile > junkfile`;
#        `psxy $R $J -Ss -: -K -O junkfile >> $PS`;
#         unlink "junkfile";
#    }

#    `pscoast $R $J $B -K -O -W1p/50 -A500 >> $PS`;

     $x *= -1; $y *= -1;
     &shift_plt($x,$y);

}

sub open_plt{
    my($x, $y) = @_;
    open(PS,"|psxy $R $J -K $PORT -X$x -Y$y > $PS");
    close(PS);
}

sub shift_plt{
    my($x, $y) = @_;
    open(PS,"|psxy $R $J -K $PORT -O -X$x -Y$y >> $PS");
    close(PS);
}
sub close_plt{
    my($x, $y) = @_;
    open(PS,"|psxy $R $J -O $PORT -X$x -Y$y >> $PS");
    close(PS);
}

sub plt_patch{
  my($watch,$intensity,$file,$plotscale) = @_;
  my $COLOR = "-Ccolors.bar";
  $GRDFILE = "junk.grd";

  @colors = ("255 40 200", "255 80 160", "255 120 120", "255 160 80", "255 210 40", "255 255 0", "190 210 60", "120 170 120", "60 130 190", "0 100 255");

  $val1 = 0; 
  $interval = ($intensity == 1) ? 10 : 5;
  
   open(CS,">colors.bar");
   for($i = 0; $i < @colors; $i++){
   		$val2 = $val1 + $interval;
    	print CS "$val1  $colors[$i]   $val2 $colors[$i]\n";
    	$val1 = $val2;
   }
   print CS "F $colors[@colors-1]\n";
   print CS "B 255 255 255\n"; 
   close(CS);
  

   if($intensity == 1){
      `awk \'{print(\$2,\$1,\$3*100)} \' $file| xyz2grd -Ddegree/degree/Intensity/1/0//kjk -G$GRDFILE -N-1 -I$dlon/$dlat $R`;
      `awk \'{print(\$2,\$1,\$3*100)} \' $file > junkout`;
      `grd2xyz $GRDFILE $R -S > junk1.out`;
   } else {
       #plot hit counts
      `awk \'{print(\$2,\$1,\$4)} \' $file| xyz2grd -G$GRDFILE -N-1 -I$dlon/$dlat $R`;
   }
  `grdimage  $J  $R  -K $COLOR $B $GRDFILE -P -O >> $PS`;
  
  if($plotscale eq "yes" && $watch == 0){
  
	if($intensity==1){
    	`psscale -D7/-1.2/8/0.2h -B20:"Probability"::,"%": -O -K $COLOR >> $PS`;
  	}
  	else{
     	`psscale -D7/-1.2/8/0.2h -B10:"Hit counts": -O -K $COLOR >> $PS`;
  	}
  }

  unlink $GRDFILE; unlink "junk1.out"; unlink "junkout"; unlink "colors.bar";

}

