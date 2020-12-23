
/**********************************************************************/
/*                                                                    */
/*                 ReadRayParfile.cpp                                 */
/*         Read Ray Parfile from output of WKM run                    */
/*                                                                    */
/**********************************************************************/

/*    
     
*/

#include <fstream.h>
#include <iostream.h>

#include "general.h"

/* 
    bool ReadRayParfile(char *RayParfile, int nstart, int nr, double *par, bool ttime=true)

    Description:       This subroutine reads the parfile outputed from 
		       the WKM calculations. It returns true if successfully 
		       read. The output series is in argument par. 

    Arguments:         RayParfile  -> file contains the par parameters & time s
				   -> format "n x p tstart \n"
                       nstart      -> starting point to read
		       nr          -> points to read 
		       par         -> series returned back
				      ttime = true  -> par = travel time
				      ttime = false -> par = ray parameter
                       ttime

    Last Modified:     

	Jan 23, 1997          Lianxing Wen      Initial revision

*/

bool ReadRayParfile(char *RayParfile, int nstart, int nr, double *par, bool ttime)
{
    ifstream infile;

    int i;

    bool success = true;

    infile.open(RayParfile);
    if(!infile){
	cerr << " Can not open Raypar file " << RayParfile << "\n";
	success = false;
    }

    int n;
    double x, tstart, p;

    // skip start points
    for(i=0; i < nstart; i++)
	infile >> n >> x >> p >> tstart;
//	infile >> n >> x >> p >> tstart >> "\n";

    //Reading other components
    for(i=0; i < nr; i++){
	infile >> n >> x >> p >> tstart;
//	infile >> n >> x >> p >> tstart >> "\n";
	par[i] = (ttime) ? tstart : p;
	if(!infile.good())
	    success = false;
    }

    infile.close();

    return success;
}
