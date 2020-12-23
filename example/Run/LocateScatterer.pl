#!/usr/bin/perl -w

#
# perl script for relocating the seismic scatteres using PKP precursor onset times.
#

$num = -1; 	# -1 : all events
			#  0 : for the first event
			#  1 : for the second event
			#  ...

$NUM =  0;  # Combined with num to control which event to be plot.

$rdepth =2891; # assumed depth of the seismic scatter.
#$rdepth =2691;

###	Find local path
$MainDir = `pwd`; chomp($MainDir);

chdir "\.\./";
$LocalDir = `pwd`; chomp($LocalDir);

chdir "\.\./bin";
$BinDir = `pwd`; chomp($BinDir);

print STDERR "$LocalDir\n$BinDir\n";

chdir $MainDir;
print STDERR "Work in $MainDir.\n";
###


$DIRPATH      = "$LocalDir/Seism";			# data directory
$FINDPATH     = "$MainDir/FindPath.pl";		# Find travel times at different distances (km) from source depth  
											# to seismic scatterer for the turning or direct rays.
$FINDPATCH    = "$MainDir/FindPatch.pl"; 	# Find the "isotime scatterer arcs".
$FINDRAYPATH  = "$MainDir/FindRaypath.pl";	# Find ray paths.
$GLOBALAVE  = "$BinDir/globalave"; 			# average the scatterer probability.

$cmbexitfile = "$DIRPATH/cmbexits"; 		# PKPdf entry or exit points at the CMB
$eventfile   = "$DIRPATH/events";			# event locations
$stationfile = "$DIRPATH/stations"; 		# station locations
$txtfile     = "$DIRPATH/txts";				# texts for plotting
$patchfile   = "$DIRPATH/patchs$rdepth"; 	# averaged scatterer probability and hit counts.
$pathfile = "$DIRPATH/path"; 				# ray paths

$phase       = "PKPdf";

$dlon    = 2.; 								# mapping grid, default 0.2
$dlat    = 2.; 								# mapping grid, default 0.2

$dlonshift = 0.0; 							# mapping grid shift, default 0.0
$dlatshift = 0.0; 							# mapping grid shift, default 0.0
#$dlonshift = 0.5*$dlon;					# mapping grid shift, default 0.0
#$dlatshift = 0.5*$dlat;					# mapping grid shift, default 0.0

$period  = 0.2; 							# mapping period, default 1.0
$Gcmin   = 130.0;							# minimum gcarc
$Gcmax   = 141.0;							# maximum gcarc
$v1      = 0.5;								# coefficent to correct amplitude with distance 	
$v2      = 3.0;								# coefficent to correct amplitude with distance

@DIR = ("201103101708360");



&findpath($rdepth,$num);

&findraypath($Gcmin, $Gcmax);

&findpatch($rdepth,$Gcmin,$Gcmax, $dlat, $dlon, $dlatshift,$dlonshift,$period,$num);


for(; $num < $NUM; $num++){

#	get *.phase in cmbexitfile.
    &getcmbexits($phase,$cmbexitfile,$num);

#	get event latitude and longitude in eventfile.
    &getevents($eventfile,$num); 

#	get station latitude and longitude in stationfile.
    &getstations($Gcmin,$Gcmax,$stationfile,$num);

#	get evla evlo size angle font justify event in txtfile.
    &gettxts($txtfile,$num); 

#	get the scatterer probability and hit counts in patchfile.
    &getpatchs($rdepth,$Gcmin, $Gcmax, $v1, $v2, $patchfile,$num,$dlat,$dlon);

#	get ray paths.
    &getpaths($pathfile);

    if($num == 0){
        $patch  = "$patchfile$num";   
		$path  = "$pathfile$num";
		$cmb    = "$cmbexitfile$num";
        $station = "$stationfile$num";
        $txt     = "$txtfile$num";
        $events  = "$eventfile$num";
    } elsif( $num > 0){
        $patch  = "$patch,$patchfile$num";
		$path  = "$path,$pathfile$num";
		$cmb    = "$cmb,$cmbexitfile$num";
        $station = "$station,$stationfile$num";
        $txt     = "$txt,$txtfile$num";
        $events = "$events,$eventfile$num";
    } else  {
        $patch  = "$patchfile";
		$path  = "$pathfile";
		$cmb    = "$cmbexitfile";
        $station = "$stationfile";
        $txt     = "$txtfile";
        $events  = "$eventfile";
    }

}



##plot scatter probability and hit counts.
$sla0	 = -20; $sla1	= 30;
$slo0	 = 90; $slo1   = 150;

$rla0    = 20;  $rla1   = 70;
$rlo0    = 230; $rlo1   = 290;

$xshift=1.5; $yshift=5;

$PS="Scaterer${rdepth}.ps";

`perl plot1.pl $xshift $yshift $slo0 $slo1 $rlo0 $rlo1 $sla0 $sla1 $rla0 $rla1 $patch $cmb $events $station $pathfile $txt $dlat $dlon $PS`;



############
#subroutine#
############


sub getpaths{
    my($tmpfile) = @_;
    $paths="paths";
  #  $tmpfile = "junk1111";
    if($num < 0){
        #all events
        `cat $DIRPATH/\*/$paths/*  > $tmpfile`;
     } else {
        $tmpfile = "$tmpfile$num";
        $Dir = $DIR[$num]; chomp($Dir);
        $Dir = "$DIRPATH/$Dir";
        `cat $Dir/$paths/\*  > $tmpfile`;
     }
   #  unlink $tmpfile;
}

sub getpatchs{
    my($rdepth,$Gcmin, $Gcmax, $v1, $v2, $file,$num,$dlat,$dlon) = @_;

    $tmpfile = "junk000000";

    $patchdir = "patch$rdepth";

    if($num < 0){
        #all events
        `cat $DIRPATH/\*/$patchdir/\*.r $DIRPATH/\*/$patchdir/\*.s > $tmpfile`;
     } else {
        $Dir = $DIR[$num]; chomp($Dir);
        $Dir = "$DIRPATH/$Dir";
        `cat $Dir/$patchdir/\*.r $Dir/$patchdir/\*.s > $tmpfile`;
        $file = "$file$num";
     }
   
    `$GLOBALAVE rsf=$tmpfile of=$file g1=$Gcmin g2=$Gcmax v1=$v1 v2=$v2 dlon=$dlon dlat=$dlat`; 

     unlink $tmpfile;

}

sub getcmbexits{
    my($phase, $file, $num) = @_;
    if($num < 0){
        for($dir = 0; $dir < @DIR; $dir++){
            $Dir = $DIR[$dir]; chomp($Dir); 
            $Dir = "$DIRPATH/$Dir/cmbexits";
            if(-d $Dir){
                chdir "$Dir";  
                `cat *.$phase > cmbexits`;
            }
        }
         chdir "$DIRPATH";
        `cat $DIRPATH/\*/cmbexits/cmbexits > $file`;
    } else {
        $file1 = "$file$num";
        $Dir = $DIR[$num]; chomp($Dir); 
        $Dir = "$DIRPATH/$Dir/cmbexits";
        print "$Dir\n";
        `cat $Dir/\*.$phase > $file1`;
    }
	chdir $MainDir;

}

sub gettxts{
    my($file,$num) = @_;

    @appendix = ("z2");
    @DIRINSTR = ("data");


    $dir1 = 0; $dir2 = @DIR;
    if($num >= 0){
        $dir1 = $num; $dir2 = $dir1 + 1;
        $file = "$file$num";
    }

    $angle     = 0.0;
    $font      = 1;
    $size      = 16;
    $justify   = "LM";

    open(OUT,">$file");
    for($dir = $dir1; $dir < $dir2; $dir++){
        $Dir = $DIR[$dir]; chomp($Dir);
        $Dir = "$DIRPATH/$Dir";
        for($instr = 0; $instr < @DIRINSTR; $instr ++){
            $DIR = "$Dir/$DIRINSTR[$instr]/";
            if(-d $DIR){
                chdir $DIR;
                @FILES = `ls \*.$appendix[0] `;
                $SACFILE = $FILES[0]; chomp($SACFILE);
                $evla = `saclst evla < $SACFILE`; chomp($evla);
                $evlo = `saclst evlo < $SACFILE`; chomp($evlo);
                $nzyear = `saclst nzyear <$SACFILE`; chomp($nzyear);
                $nzjday = `saclst nzjday <$SACFILE`; chomp($nzjday);
                $event  = "$nzyear$nzjday";

                $evlo += 3;
                print OUT "$evla $evlo $size $angle $font $justify $event\n";
            }
        }
    }
    close(OUT);
	chdir $MainDir;

}

sub getstations{
    my($g1,$g2,$file,$num) = @_;

    @appendix = ("z2");
    @DIRINSTR = ("data");


    $dir1 = 0; $dir2 = @DIR;
    if($num >=0){
        $dir1 = $num; $dir2 = $dir1 + 1;
        $file = "$file$num";
    }

    open(OUT,">$file");
    for($dir = $dir1; $dir < $dir2; $dir++){
        $Dir = $DIR[$dir]; chomp($Dir);
        $Dir = "$DIRPATH/$Dir";
        for($instr = 0; $instr < @DIRINSTR; $instr ++){
            $DIR = "$Dir/$DIRINSTR[$instr]/";
            if(-d $DIR){
                chdir $DIR;
                @FILES = `ls \*.$appendix[0] `;
                for ($ifile = 0; $ifile < @FILES; $ifile++){
                    $SACFILE = $FILES[$ifile]; chomp($SACFILE);
                    $gc   = `saclst gcarc < $SACFILE`; chomp($gc);
                    if($gc >= $g1 && $gc <= $g2){
                    $evla = `saclst stla < $SACFILE`; chomp($evla);
                    $evlo = `saclst stlo < $SACFILE`; chomp($evlo);
                    if($evlo < 0) { $evlo += 360;}
                    print OUT "$evla $evlo\n";
                    }
                }
            }
        }
    }
    close(OUT);
	chdir $MainDir

}

sub getevents{
    my($file,$num) = @_;

    @appendix = ("z2");
    @DIRINSTR = ("data");


    $dir1 = 0; $dir2 = @DIR;
    if($num >=0){
        $dir1 = $num; $dir2 = $dir1 + 1;
        $file = "$file$num";
    }

    open(OUT,">$file");
    for($dir = $dir1; $dir < $dir2; $dir++){
        $Dir = $DIR[$dir]; chomp($Dir);
        $Dir = "$DIRPATH/$Dir";
        for($instr = 0; $instr < @DIRINSTR; $instr ++){
            $DIR = "$Dir/$DIRINSTR[$instr]/";
            if(-d $DIR){
                chdir $DIR;
                @FILES = `ls \*.$appendix[0] `;
                $SACFILE = $FILES[0]; chomp($SACFILE);
                $evla = `saclst evla < $SACFILE`; chomp($evla);
                $evlo = `saclst evlo < $SACFILE`; chomp($evlo);
                if($evlo < 0) { $evlo += 360;}
                print OUT "$evla $evlo\n";
            }
        }
    }
    close(OUT);
	chdir $MainDir

}


sub findpath{
    my($rdepth,$num) = @_;

    @appendix = ("z2");
    @DIRINSTR = ("data");
    $dir1 = 0; $dir2 = @DIR;

    if($num >= 0){
        $dir1 = $num; $dir2 = $dir1 + 1;
    }

    for($dir = $dir1; $dir < $dir2; $dir++){

        $Dir = $DIR[$dir]; chomp($Dir);
        $Dir = "$DIRPATH/$Dir";
        chdir $Dir;  

        for($instr = 0; $instr < @DIRINSTR; $instr ++){

            $DIR = "$Dir/$DIRINSTR[$instr]/";
            if(-d $DIR){
                for($direct = 0; $direct < 2; $direct ++){
                    if($direct == 0){
                          #following for the turning rays
                          $cdistmin = 114;
                          $cdistmax = 145;
                          $cdistinc = 0.1;
                          $file     = "../CMBout${rdepth}";
                     } else {
                          # following for the direct rays
                          $cdistmin = 0;
                          $cdistmax = 30;
                          $cdistinc = 0.1;
                          $file     = "../CMBout${rdepth}1";
                     } 

                     @l = `perl $FINDPATH $cdistmin $cdistmax $cdistinc $rdepth $direct \"$file\" $DIR $appendix[$instr] $MainDir $BinDir`;

                }
            }
        }
    }

}

sub findpatch{

    my($rdepth,$Gcmin, $Gcmax, $dlat, $dlon, $dlatshift, $dlonshift, $period, $num) = @_;

    $Mode = '0775';
    @appendix = ("z2");
    @DIRINSTR = ("data");
    $dir1 = 0; $dir2 = @DIR;

    if($num >= 0){
        $dir1 = $num; $dir2 = $dir1 + 1;
    }
  
    for($dir = $dir1; $dir < $dir2; $dir++){
        $Dir = $DIR[$dir]; chomp($Dir);
        $Dir = "$DIRPATH/$Dir";

        if(-d $Dir){
            chdir $Dir;  
            $patchdir = "patch$rdepth";
            `rm -r $patchdir`; 
            &CreatDir($patchdir,$Mode);
            for($instr = 0; $instr < @DIRINSTR; $instr ++){
                $DIR = "$Dir/$DIRINSTR[$instr]/";
                if(-d $DIR){
                    @l = `perl $FINDPATCH $rdepth $Gcmin $Gcmax $period $dlat $dlon $dlatshift $dlonshift $DIR $appendix[$instr] $MainDir $BinDir`;
                }
            }
        }
    }

}

sub findraypath{

    my($Gcmin, $Gcmax) = @_;

    $Mode = '0775';
    @appendix = ("z2");
    @DIRINSTR = ("data");
  
    for($dir = 0; $dir < @DIR; $dir++){
        $Dir = $DIR[$dir]; chomp($Dir);
        $Dir = "$DIRPATH/$Dir";
        if(-d $Dir){
            chdir $Dir;
            print "change to directory $Dir\n";
            `rm -r paths`; `rm -r cmbexits`;
            &CreatDir("paths",$Mode);
            &CreatDir("cmbexits",$Mode);
            for($instr = 0; $instr < @DIRINSTR; $instr ++){
                $DIR = "$Dir/$DIRINSTR[$instr]/";
                if(-d $DIR){
                    @l = `perl $FINDRAYPATH $Gcmin $Gcmax $DIR $appendix[$instr] $MainDir $LocalDir $BinDir`;
                }
            }
        }
    }

}

sub CreatDir{
    ($ndir,$mode) = @_;
    if(!(-d $ndir)){
        print "make directory $ndir with mode $mode\n";
        mkdir("$ndir",0644);
        `chmod 777 $ndir`;
    }
}
        
