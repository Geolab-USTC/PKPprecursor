/************************************************************/
/*                                                          */
/*                   graypf.cpp                             */
/*              Generalized Ray Theory                      */
/*         Class Implementation for Private Members         */
/*                                                          */
/************************************************************/

/*  
    This file contains the implementation for methods for 
    the Generalized Ray Theory. The class is defined in file gray.h.
    
    Revision History:
        12 Dec. 1995    Lianxing Wen              Initial revision.
*/           

#include <iostream.h>
#include <stdlib.h>
#include <math.h>

#include "gray.h"
#include "ray.h"
#include "raypath.h"
#include "model.h"

/* 
    gray::gray(double dist, const class Ray &ray, const class Model &model, double sdepth)

    Description:        This function implements a constructor for 
			gray class.

    Arguments:          dist (double)   -> epicentral distance 
			ray (Ray&)      -> ray
			model(Model&)   -> model
			sdepth          -> depth of source

    Last Modified:      12 Dec, 1996

*/

gray::gray(double dist, const class Ray &ray,  const class Model &model, double sdepth)
{
    setup(dist, ray, model, sdepth);
}   

/* 
    gray::gray(double dist, int bottom, int type, const class Model &model, double sdepth, double rdepth=0)

    Description:        This function implements a constructor for 
			gray class.

    Arguments:          dist (double)   -> epicentral distance 
			bottom          -> bottomest layer of ray 
			type            -> ray type
			model(Model&)   -> model
			sdepth          -> depth of source
			sdepth          -> depth of receiver 

    Last Modified:      12 Dec, 1996

*/

gray::gray(double dist, int bottom, int type, const class Model &model, double sdepth, double rdepth)
{

    int *seg, *stype;
    int i, j;	
    int nrec;
    int direct = 0;

    int nb = model.nb(sdepth) +1;


    nrec = (rdepth < 1.e-6) ? 1 : (model.nb(rdepth) +1);
    
    int nseg = 2*bottom - nb - nrec + 2;

    if(bottom < nrec){
	// direct wave
	bottom = nrec;
	nseg   = bottom - nb + 1;
	direct = 1;
    }

    if(bottom < nb ){
	cerr << "Warning: bottom < nb" << endl;
    }


    seg    = new int[nseg];
    stype  = new int[nseg];

    i = 0;
    for(j=nb; j<=bottom; j++){
	seg[i]   = j-1;
	stype[i] = type;
	i++;
    }
    if(direct == 0){
        for(j=bottom; j>=nrec; j--){
	    seg[i]   = j-1;
	    stype[i] = type;
    	    i++;
        }
    }

    Ray ray(nseg,seg,stype,-1);

    setup(dist, ray, model, sdepth, rdepth);
}   

/* 
    gray::gray(const class Ray &ray, const class Model &model, double po, double sdepth)

    Description:        This function implements a constructor for 
			gray class.

    Arguments:          po (double)     -> ray parameter 
			ray (Ray)       -> ray
			model(Model)    -> model
			sdepth          -> depth of source

    Last Modified:      12 Dec, 1996

*/

gray::gray(const class Ray &ray, const class Model &model, double po, double sdepth)
{
    setup(ray, model, po, sdepth, 1);
}   

/* 
    gray::~gray()

    Description:        This function implements a destructor for 
			gray class.

    Last Modified:      12 Dec, 1996

*/
gray::~gray()
{
}

/* 
    int gray::refresh(double dist, const class Ray &ray, const class Model &model, double sdepth)

    Description:        This function refreshs its data members for 
			gray class.

    Arguments:          dist (double)   -> epicentral distance 
			ray (Ray)       -> ray
			model(Model)    -> model
			sdepth          -> depth of source

    Return Value:       1: success: 0 fails 

    Last Modified:      12 Dec, 1996

*/

int gray::refresh(double dist, const class Ray &ray, const class Model &model, double sdepth)
{
    _ray.~Ray();
    _model.~Model();

    return (setup(dist, ray, model, sdepth));
}   

/* 
    int gray::refresh(const class Ray &ray, const class Model &model, double po, double sdepth)

    Description:        This function refreshs its data members for 
			gray class.

    Arguments:          po (double)     -> ray parameter 
			ray (Ray)       -> ray
			model(Model)    -> model
			sdepth          -> depth of source

    Last Modified:      12 Dec, 1996

*/

int gray::refresh(const class Ray &ray, const class Model &model, double po, double sdepth)
{
    _ray.~Ray();
    _model.~Model();

    return (setup(ray, model, po, sdepth,1));
}   

/* 
    void gray::reset_po(double po)

    Description:        This function resets _po for gray class.

    Arguments:          po (double)   -> ray parameter

    Return Value:       None.

    Last Modified:      12 Dec, 1996

*/

void gray::reset_po(double po)
{
    _po    = po;
    finddist(_ray, _model, po);
}   

/* 
    void gray::reset_ray(const Ray & ray)

    Description:        This function resets _ray for gray class.

    Arguments:          ray  -> new ray 

    Return Value:       None.

    Last Modified:      12 Dec, 1996

*/

void gray::reset_ray(const Ray & ray)
{
    _ray.~Ray();
    setup(_dist, ray, _model,  _sdepth);
}   

/* 
    void gray::reset_model(const Model & model)

    Description:        This function resets _model for gray class.

    Arguments:          model  -> new model 

    Return Value:       None.

    Last Modified:      12 Dec, 1996

*/

void gray::reset_model(const Model & model)
{
    _model.~Model();
    setup(_dist, _ray, model, _sdepth);
}   

/* 
    void gray::reset_sdepth(double sdepth)

    Description:        This function resets _sdepth for gray class.

    Arguments:          sdepth  -> new soure depth 

    Return Value:       None.

    Last Modified:      12 Dec, 1996

*/

void gray::reset_sdepth(double sdepth)
{
    setup(_dist, _ray, _model, sdepth);
}   

/* 
    void gray::reset_dist(double dist)

    Description:        This function resets _dist for gray class.

    Arguments:          dist (double)   -> epicentral distance 

    Return Value:       None.

    Last Modified:      12 Dec, 1996

*/

void gray::reset_dist(double dist)
{
    _dist    = dist;
    findpo(dist, _ray, _model);
}   

/* 
    int gray::setup(const class Ray & ray, const class Model & model, double sdepth)

    Description:        This function sets up an instance  for 
			gray class.

    Arguments:          sdepth (double)   -> source depth 
			ray (Ray)         -> ray
			model(Model)      -> model

    Return Value:       1; 

    Last Modified:      12 Dec, 1996

*/

int gray::setup(const class Ray &ray, const class Model &model, double sdepth, double rdepth)
{
    _ray    = ray;
    _model  = model;
    _sdepth = sdepth;
    _rdepth = rdepth;

    int j, nb, rlayer, layer;
    double rlen, slen, depth = 0.0;

    layer = model.layers();

    for(j=0; j < layer; j++){
	depth += model.th(j);
	if(depth < sdepth){
	    nb = j;
	    slen = sdepth-depth;
        }
    }
    
    nb++;

    slen /= model.th(nb); 

    if(_ray.leaving() < 0)
	slen -= 1.0;

    _ray.setslen(slen);

    depth = 0.0;
    for(j=0; j < layer; j++){
	depth += model.th(j);
	if(depth < rdepth){
            rlayer = j;
	    rlen = (rdepth-depth);
        }
    }
    rlayer++;
    rlen  /= model.th(rlayer);
//    cerr << " rlen = " << rlen << " thick = " << model.th(rlayer) << endl;
    if(_ray.arriving() > 0)
	rlen = 1.0 - rlen;
 //   cerr << " rlayer = " << rlayer << "  = " << _ray.seg(_ray.nseg()-1) << endl;

    _ray.setrlen(rlen);

    if(_ray.stype(0) == 5){
	// P wave
	_ray.setlen(_ray.seg(0),fabs(slen),1);
//	_ray.setlen(rlayer-1,fabs(rlen),1);
//	_ray.setlen(_ray.seg(_ray.nseg()-1),fabs(rlen),1);
    } else{
	_ray.setlen(_ray.seg(0),fabs(slen),0);
//	_ray.setlen(_ray.seg(_ray.nseg()-1),fabs(rlen),0);
//	_ray.setlen(_ray.seg(rlayer-1),fabs(rlen),0);
//	_ray.setlen(rlayer-1,fabs(rlen),0);
    }

    if(nb != _ray.seg(0)){
	cerr << "Error in combining the Ray and Model and source depth\n";
	cerr << "Source layer is not the first segment of the ray\n";
	cerr << "source layer ="<< nb << " 1st seg =" << _ray.seg(0) << endl;
	exit(-1);
    }

    _model.flatten();

    return ( fabs(slen) >= 0.0 && fabs(slen) <= 1.0);


}

/* 
    int gray::setup(double dist, const class Ray &ray, const class Model & model, double po, double sdepth)

    Description:        This function sets up an instance  for 
			gray class.

    Arguments:          dist (double)   -> epicentral distance 
			ray (Ray)       -> ray
			model(Model)    -> model
			sdepth          -> sdepth

    Return Value:       1: success; 0 : fail

    Last Modified:      12 Dec, 1996

*/

int gray::setup(double dist, const class Ray & ray, const class Model & model, double sdepth, double rdepth)
{

    int success;

    success = setup(ray, model, sdepth, rdepth);
    _dist  = dist;
    findpo(dist, _ray, _model);

    return success;
}   

/* 
    int gray::setup(const class Ray &ray, const class Model &model, double po, double sdepth)

    Description:        This function sets up an instance  for 
			gray class.

    Arguments:          po (double)     -> ray parameter 
			ray (Ray)       -> ray
			model(Model)    -> model
			sdepth          -> sdepth

    Return Value:       1: success; 0 : fail

    Last Modified:      12 Dec, 1996

*/

int gray::setup(const class Ray &ray, const class Model &model, double po, double sdepth, int dummy)
{

    int success;

    success = setup(ray, model, sdepth, 0.0);
    _po    = po;
    finddist(_ray, _model, po);

    return success;
}   

/* 
    class raypath gray::Raypath(int headwave=0)

    Description:        This function implements a method for finding 
			the raypath. It returns the an integer:
			   rcase = 1; sucessfully found 
			         = 0; not sucessfully found.

    Arguments:          headwave (int) default = normal path
				       1       = head wave path
    Return Value:       int  -> rcase

    Last Modified:      12 Dec, 1996
*/

class raypath gray::Raypath(int headwave)
{
    double p;         // ray parameter
    int upleaving;    // upleaving
    int nseg, j;
    double rlen, slen, v, dx, dz, length, sdepth;
    double sintheta, costheta;

    nseg  = _ray.nseg();
    rlen  = _ray.rlen();
    slen  = _ray.slen();

    sdepth  = ((slen < 0) ? (1.0+slen) : slen); 
    sdepth *= _model.th(_ray.seg(0));
    for(j = 0; j < _ray.seg(0)-1; j++)
	sdepth += _model.th(j);

    class raypath path(nseg);

    path.setx(0, 0);
    path.setz(0, sdepth);

    p = ((headwave) ? _pc :_po);
    for(j = 0; j < nseg; j++){

	upleaving = _ray.uparriving(j);

	v = ((_ray.stype(j) == 5) ? _model.vp(_ray.seg(j)) : _model.vs(_ray.seg(j)));
	length = 1.0;
	if(j == 0)
	    length = fabs(slen);
        if(j == nseg-1)
	    length = rlen;

        sintheta  = p * v;
	costheta  = sqrt(1.0 - sintheta*sintheta);

	dx = sintheta/costheta * length * _model.th(j);
	dz = ((upleaving) ? -length * _model.th(j) : length * _model.th(j)); 

	path.setx(j+1, path.x(j) + dx);
	path.setz(j+1, path.z(j) + dz); 

    }

    return path;

}


