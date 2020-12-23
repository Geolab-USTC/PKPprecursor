/************************************************************/
/*                                                          */
/*                   ray.cpp                                */
/*              Ray Description                             */
/*         Class Implementations                            */
/*                                                          */
/************************************************************/

/*  
    This file contains the implementation for functions for the  
    Ray class. The class is defined in file ray.h.
    
    Revision History:
        11 Jan. 1995    Lianxing Wen              Initial revision.
*/           

#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include "ray.h"


/* 
    Ray::Ray(int nseg, int *seg, int *stype, double slen = 1.0, 
	     double rlen = 0.0)

    Description:        This function implements a constructor for Ray 
			class. 

    Arguments:          nseg   -> number of segment
			seg    -> ray segments
			stype  -> ray type corresponding to each segment
			slen   -> relative length of first segment
				  positive: upgoing; negative: downgoing.
                        rlen   -> relative distance of receiver to 
				  the very toppest layer above the receiver

    Return Value:       None.

    Last Modified:      11 Jan 1996

*/

Ray::Ray(int nseg, int *seg, int *stype, double slen, double rlen)
{
    setup(nseg, seg, stype, slen, rlen);
}   

/* 
    Ray::~Ray()

    Description:        This function implements a destructor for Ray 
			class

    Last Modified:      11 Jan, 1996

*/

Ray::~Ray()
{
    if(_uparriving != 0)
	delete _uparriving;
    if(_seg != 0)
	delete _seg;
    if(_stype != 0)
	delete _stype;

    if(_pseg != 0)
	delete _pseg;
    if(_sseg != 0)
	delete _sseg;
}


/* 
    int Read(ifstream &infile, int nature = 1, 
		double slen = 1.0, double rlen = 0.0)

    Description:        This function implements a Read function
			for reading the parameters descripting 
			the Ray

    Arguments:          infile -> file stream to be read
			nature -> 1: input layer is from 1 ... nlayer
				  0: input layer is from 0 ... nlayer-1
			slen   -> relative length of first segment
			rlen   -> relative distance from receiver to the 
				  very above layer


    Return Value:       success (int) 1: successfully read
				      0: otherwise

    Inputs:             Read models from infile. 

    Last Modified:      11 Jan, 1996
*/

int Ray::Read(ifstream &infile, int nature, double slen, double rlen)
{
    int success = 1;

    int j;

    char c;	

    int rep, nseg, *seg, *stype;

    infile >> nseg;  // total segements

    seg   = new int[nseg];
    stype = new int[nseg];
    if(seg == 0 || stype == 0){
	cerr << "Can not allocate the memory for seg and stype \n";
	exit(-1);
    }

    for(j = 0; j<nseg; j++){
	infile >> seg[j];
	seg[j] -= nature;
    }

    if(!infile.good())
         success = 0;

    infile >> rep;

    for(j = 0; j<nseg; j++)
	infile >> stype[j];

    if(!infile.good())
         success = 0;

    if(rep < 0) slen = -slen;


    if(success)
	success = setup(nseg, seg, stype, slen, rlen);

    if(seg   != 0) delete seg;
    if(stype != 0) delete stype;

    return success;

}

/* 
    int Ray::setlen(int layer, double len, int p =1, int rep =1)

    Description:        This function modifies the relative length
			of a particular ray segment

    Arguments:          layer  -> layer number of the segment
			len    -> the relative segment ( 1.0 -> len) 
			rep    -> repeat times
			p      -> wave type. 1 for P wave: 0 for SV wave

    Return Value:       1: success: 0 error.

    Last Modified:      11 Jan 1996

*/

int Ray::setlen(int layer, double len, int p, int rep)
{
    int success = 1;

    if(len > 1.0){ 
	cerr << " Error in specifying the len in setlen \n";
	success = 0;
    }

    len = -(1.0 - len) * (double) (rep);

    if( p ){
	_pseg[layer] += len;
	if(_pseg[layer] < 0.0){
	    cerr << "Illegal specification in setlen for P segment len=" <<len<< "\n";
	    success = 0;
        }

    }
    else{
	_sseg[layer] += len;
	if(_sseg[layer] < 0.0){
	    cerr << " Illegal specification in setlen for S segment\n";
	    success = 0;
        }
    }

    return success;

}


/* 
    int Ray::setup(int nseg, int *seg, int *stype, 
		   double slen = 1.0, double rlen = 0.0)

    Description:        This function sets up an instance of Ray 
			class

    Arguments:          nseg   -> number of segment
			seg    -> ray segments
			stype  -> ray type corresponding to each segment
			slen   -> relative length of first segment
				  positive: upgoing; negative: downgoing.
                        rlen   -> relative distance of receiver to 
				  the very toppest layer above the receiver

    Return Value:       1.

    Last Modified:      11 Jan 1996

*/

int Ray::setup(int nseg, int *seg, int *stype, double slen, double rlen)
{

    
    _type = raytype(nseg, stype);
    _rlen  = rlen;
    _slen  = slen;

    /* checking upleaving*/
    _upleaving   = ( (slen < 0) ? -1 : 1);
    _slayer      = seg[0];

    /* checking uparriving*/
    _uparriving  = new int[nseg];
    if(_uparriving == 0)
       cerr << "Can not allocate the memory in Ray for uparriving\n";

    if(checkarriving(nseg, seg, slen) == 0){
	cerr << "Error in Ray specification, Aborting the program \n";
	exit(-1);
    }
    _rlayer      = seg[nseg-1];

    int iseg, downmost;

    if( ! checkdownupmost(nseg, seg))
	cerr << "Error in checking the downupmost \n";

    downmost = _downmostlayer + 1;

    /* store nseg and seg and stype */
    _nseg  =  nseg;
    _seg   =  new int[_nseg];
    _stype =  new int[_nseg];

    if(_seg == 0 || _stype == 0 ){

       cerr << "Unable to allocate the memory for Ray \n";
       exit(-1);
    }

    for(iseg = 0; iseg < _nseg; iseg ++){
	_seg[iseg]   = seg[iseg];
	_stype[iseg] = stype[iseg];
    }

    /* calculate the ray segements */
    _pseg =  new double[downmost];
    _sseg =  new double[downmost];

    if(_pseg == 0 || _sseg == 0 ){

       cerr << "Unable to allocate the memory for p ans s seg in Ray \n";
       exit(-1);
    }

    for(iseg = 0; iseg < downmost; iseg ++){
	_pseg[iseg] = _sseg[iseg] = 0.0;
    }

    if( ! raysegs(nseg, seg, stype, slen, rlen) )
	cerr << "Error in ray description \n";

    return 1;

}

/* 
    Ray& Ray::operator = (const Ray &ray)

    Description:        This function implements an operator 
			for assignments 

    Return Value:       success (int) 1: successfully read
				      0: otherwise

    Last Modified:      11 Jan, 1996

*/

Ray& Ray::operator = (const Ray &ray)
{

    int j;

    _type          = ray._type;

    _upleaving     = ray._upleaving;
    _slayer        = ray._slayer;
    _rlen          = ray._rlen;
    _slen          = ray._slen;

    _uparriving    = new int [ray._nseg];
    if(_uparriving == 0)
	cerr << "Can not allocate the memory in operator = in Ray class\n";
    for(j=0; j<ray._nseg; j++)
        _uparriving[j]    = ray._uparriving[j];

    _rlayer        = ray._rlayer;

    _upmostlayer   = ray._upmostlayer;
    _downmostlayer = ray._downmostlayer;

    _pseg = new double[_downmostlayer+1];
    _sseg = new double[_downmostlayer+1];

    if(_pseg == 0 || _sseg == 0)
	cerr << "Can not allocate the memory in operator = in Ray class\n";

    for(j=0; j <= _downmostlayer; j++){
        _pseg[j]    = ray._pseg[j];
        _sseg[j]    = ray._sseg[j];
    }

    _nseg    = ray._nseg;
    _seg     = new int[_nseg];
    _stype   = new int[_nseg];

    if(_seg == 0 || _stype == 0)
	cerr << "Can not allocate the memory in operator = in Ray class\n";
    for(j=0; j<_nseg; j++){
	_seg[j]    = ray._seg[j];
	_stype[j]  = ray._stype[j];
    }

    return *this;

}



