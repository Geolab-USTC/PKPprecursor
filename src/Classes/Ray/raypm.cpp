
/************************************************************/
/*                                                          */
/*                   raypf.cpp                              */
/*              Ray Description                             */
/*      Implementation of private methods for class Ray     */
/*                                                          */
/************************************************************/

/*  
    This file contains the implementation for private functions for the  
    Ray class. The class is defined in file ray.h.
    
    Revision History:
        11 Jan. 1995    Lianxing Wen              Initial revision.
*/           


#include <fstream.h>
#include <iostream.h>
#include <math.h>
#include "ray.h"

/* 
    int Ray::raysegs(int nseg, int *seg, int *stype, double slen, double rlen)

    Description:        This function calculates the relative lengths  
		        in each layer for a specific ray	

    Aeguments:          nseg -> number of ray segements
			seg  -> layer for each segement
			stype -> ray type of the segement
			slen  -> relative length of the first segement
			rlen  -> relative length of the last segement

    Return Value:       1: No error  0: Error  

    Last Modified:      Jan 11, 1997

*/


int Ray::raysegs(int nseg, int *seg, int *stype, double slen, double rlen)
{

    int success = 1;

    int j;

    /* first segement  */
    if(stype[0] == 3 || stype[0] == 4 )
	// SV or SH
	_sseg[seg[0]] = ((slen > 0) ? slen : -slen);

    else if ( stype[0] == 5)
	// P Wave
	_pseg[seg[0]] = ((slen > 0) ? slen : -slen);

    else 
	success = 0;

    /* other segements */
    for( j = 1; j < nseg-1; j ++){

	if(stype[j] == 3  || stype[j] == 4)
	    _sseg[seg[j]] += 1.0;
        else if (stype[j] == 5)
	    _pseg[seg[j]] += 1.0;
        else 
	    success = 0;

    }

    rlen = (( _uparriving[_nseg-1] > 0) ? 1.0 - rlen : rlen);
    /* last segement */
    if(nseg > 0){
	if(stype[nseg-1] == 3  || stype[nseg-1] == 4)
	    _sseg[seg[nseg-1]] += rlen;
        else if (stype[nseg-1] == 5)
	    _pseg[seg[nseg-1]] += rlen;
        else 
	    success = 0;
    }

    return success;

}

/* 
    int Ray::checkdownupmost(int nseg, int *seg)

    Description:        This function checks the upmost and downmost 
			layers in the ray specificd by arguments 


    Aeguments:          nseg -> number of ray segements
			seg  -> layer for each segement

    Return Value:       1: No error  0: Error  

    Last Modified:      Jan 3, 1997

*/

int Ray::checkdownupmost(int nseg, int *seg)
{
    int j;

    int success = 1;

    int downmost, upmost, leaving;

    downmost = upmost = seg[0]; 
    leaving = _upleaving;

    for(j = 1; j < nseg; j++){
	if(seg[j] == seg[j-1] + 1){
	    // transmiss to deep layer, update downmost if needed
	    leaving = -1;
	    if(downmost < seg[j]) downmost = seg[j];
        }
	else if (seg[j] == seg[j-1] -1){
	    // transmiss to upper layer, update upmost if needed
	    leaving = 1;
	    if(upmost > seg[j]) upmost = seg[j];
        }
	else if (seg[j] == seg[j-1]){
	    // reflection
	    if(leaving = 1) 
		// ungoing reflected back, update upmost if needed
		if(upmost > seg[j] -1) upmost = seg[j] -1;
            else
		// downgoing refleced back, update downmost if needed
		if(downmost < seg[j] +1) downmost = seg[j] +1;
            
	    leaving *= -1;
        }
	else
	    success = 0;
    }

    _upmostlayer   = upmost;
    _downmostlayer = downmost;

    return success;
}


/* 
    int Ray::checkarriving(int nseg, int *seg, double slen)

    Description:        This function checks the arriving direction 
			of the ray specificd by arguments 


    Aeguments:          nseg -> number of ray segements
			seg  -> layer for each segement
			slen -> relative length of first ray segement

    Return Value:       1: No error  0: Error  

    Last Modified:      Jan 3, 1997

*/

int Ray::checkarriving(int nseg, int *seg, double slen)
{

    int iseg;
    /* checking uparriving and correct specification of ray */ 
    int success = 1;

    int uparriving = 1;

    if(slen  < 0) 
	uparriving = -1;

    _uparriving[0] = uparriving;

    for(iseg = 0; iseg < nseg-1; iseg++){

	if(seg[iseg+1] == seg[iseg] + 1 ){ 
	    if(uparriving != -1) success = 0;
        }

	else if(seg[iseg+1] == seg[iseg] - 1){
	    if(uparriving != 1) success = 0;
        }

        else if(seg[iseg+1] == seg[iseg])
	    uparriving *= -1;

        else
	    cerr << "Error in Specificing the Ray, Abort the Program on checkarriving\n"; 

        _uparriving[iseg+1] = uparriving;

    }

    return success;
}   


/* 
    int Ray::raytype(int nseg, int *stype)

    Description:        This function checks the type 
			of the ray specificd by arguments 


    Aeguments:          nseg  -> number of ray segements
			stype -> type of each segement

    Return Value:       type  -> 2 SH Case
			      -> 21 SV Source P receiver
			      -> 22 SV Source SV Receiver
			      -> 12 P Source SV receiver
			      -> 11 P Source P Receiver

    Last Modified:      Jan 3, 1997

*/


int Ray::raytype(int nseg, int *stype)
{

    /* checking the source type */
    int snum, rnum, type;

    snum  = stype[0];
    rnum  = stype[nseg-1];

    if(snum == 4 ) 
	// SH Wave
	type = 2;
    else {
	// PSV system
	if(snum == 3 && rnum == 3)
	    type = 22;  // SV Source::SV receiver
	if(snum == 3 && rnum == 5)
	    type = 21;  // SV Source::P receiver
	if(snum == 5 && rnum == 3)
	    type = 12;  // P Source::SV receiver
	if(snum == 5 && rnum == 5)
	    type = 11;  // P Source::P receiver
    }

    return type;
}
