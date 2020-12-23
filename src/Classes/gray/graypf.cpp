/************************************************************/
/*                                                          */
/*                   graypf.cpp                             */
/*              Generalized Ray Theory                      */
/*         Class Implementation for Private Members         */
/*                                                          */
/************************************************************/

/*  
    This file contains the implementation for private function for 
    the Generalized Ray Theory. The class is defined in file gray.h.
    
    Revision History:
        12 Dec. 1995    Lianxing Wen              Initial revision.
*/           

#include <math.h>
#include <iostream.h>


#include "gray.h"
#include "ray.h"
#include "model.h"

/* 
    int gray::findpo(double dist, const class Ray &ray, const class Model &model)

    Description:        This function implements a method for finding 
			po and to for the ray. It returns the an integer:
			   rcase = 1; sucessfully found 
			         = 2; not sucessfully found, beyond the 
				      criterion for misfit.
    Arguments:          None.
    Return Value:       int  -> rcase
			   rcase = 1; sucessfully found 
			         = 2; not sucessfully found, beyond the 
				      criterion for misfit.

    Inputs:             None.
    Outputs:            None.

    Error Handling:     None.

    Algorithm:          None.

    Last Modified:      12 Dec, 1996

*/

int gray::findpo(double dist, const class Ray &ray, const class Model &model)
{
    double p, p1, p2;


    double bl, eta, etb;
    double temp, maxvel;
    double *dvsq, *dvpq;
    int  max_layer;
    int j;

    int    rcase;     // return value

    const double START_P  = -1.e-6;     // Starting P for searching
    const double PTINY    =  1.e-16;    // Criterion for delta P
    const double BLTINY   =  1.e-20;    // Criterion for misfit
    const double TINY     =  1.e-30;    // Criterion for checking is there
					// a particular segment

    _pc  =  0.0;
    _tc  = -1.0;

    p1  = START_P;

    max_layer = ray.downmostlayer();

    maxvel = 0.0;
    for(j=0; j <= max_layer; j++){
	if(ray.sseg(j) > TINY && maxvel < model.vs(j))
	    maxvel  = model.vs(j);
	if(ray.pseg(j) > TINY && maxvel < model.vp(j))
	    maxvel  = model.vp(j);
    }

    p2  = 1./maxvel;

    dvsq = new double[max_layer+1];
    dvpq = new double[max_layer+1];

    if(dvsq == 0 || dvpq == 0)
	cerr << "Error in allocating the memory in Ray:findpo\n";

    for(j = 1; j<=max_layer; j++){
	dvsq[j] = 1.0/(model.vs(j)*model.vs(j));
	dvpq[j] = 1.0/(model.vp(j)*model.vp(j));
    }

    // search for the _po
    do {

        p = 0.5* (p1 + p2);

        temp = 0.0;
        for(j = 1; j<=max_layer; j++){
            eta  = sqrt(fabs(dvpq[j] - p * p));
            etb  = sqrt(fabs(dvsq[j] - p * p));

            temp -= (ray.pseg(j)/eta + ray.sseg(j)/etb)*model.th(j);
        }
        
        bl  = dist + temp *p;    

        if( bl > 0)
            p1 = p;
        else
            p2 = p;    



   } while ( (fabs(p2 - p1) > PTINY) && (fabs(bl) > BLTINY) );

   _po  = p;

   // calculate to;
   _to  = _po * dist;
   for(j = 1; j<=max_layer; j++){
       eta  = sqrt(fabs(dvpq[j] - p * p));
       etb  = sqrt(fabs(dvsq[j] - p * p));
       _to += (ray.pseg(j)*eta + ray.sseg(j)*etb)*model.th(j);
   }    

   if(fabs(_po) > 1./maxvel){
       // past the critical angle
       _pc  = 1.0/maxvel;
       p = _pc;
       _tc  = p * dist;
       for(j = 1; j<=max_layer; j++){
           eta  = sqrt(fabs(dvpq[j] - p * p));
           etb  = sqrt(fabs(dvsq[j] - p * p));
           _tc += (ray.pseg(j)*eta + ray.sseg(j)*etb)*model.th(j);
       }
   }    


   delete dvsq;
   delete dvpq;


   rcase  = ( (fabs(bl) <= BLTINY) ?  1 : 2 );

   return rcase;
}   


/* 
    int gray::finddist(const class Ray &ray, const class Model & model, double po)

    Description:        This function implements a method for finding 
			po and to for the ray. It returns 1; 

    Arguments:          None.
    Return Value:       int  ->  1

    Inputs:             None.
    Outputs:            None.

    Error Handling:     None.

    Algorithm:          None.

    Last Modified:      12 Dec, 1996

*/

int gray::finddist(const class Ray &ray, const class Model &model, double po)
{

    int j, max_layer;

    double temp, eta, etb;

    max_layer = ray.downmostlayer();

    temp = 0.0;
    for(j = 1; j<=max_layer; j++){
        eta  = sqrt(fabs(1.0/(model.vp(j)*model.vp(j)) - po * po));
        etb  = sqrt(fabs(1.0/(model.vs(j)*model.vs(j)) - po * po));

        temp += (ray.pseg(j)/eta + ray.sseg(j)/etb)*model.th(j);
    }

    _dist = temp * po;

    return 1;

}
        
