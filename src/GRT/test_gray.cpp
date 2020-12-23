/*****************************************************************/
/*                                                               */
/*                 ray_tracing.cpp                               */
/*             Ray tracing of a Generalized ray                  */
/*                                                               */
/*****************************************************************/

/* This file contains the main program to ray tracing the generalized
   Ray.

   Revision History:
       6 Jan. 1997, Lianxing Wen              Initial Revision

*/

/*library files */

#include <fstream.h>
#include <iostream.h>
#include <assert.h>
#include <stdlib.h>

/* getpar file  */
extern "C" {
    #include "getpar.h"
}

/*local include files */
#include "gray.h"
#include "ray.h"
#include "model.h"
#include "general.h"

#include "ReadRayfile.h"
#include "ReadRayParfile.h"
#include "findLeftRight.h"

int main(int ac, char *av[])
{
    char modelfile[60];   // file name for model
    char rayfile[60];     // file name for Ray
    char RayParfile[60];  // filename for Kir librarg

    double       t;              // Total time for synthetics 
    double       dist1;          // min epicentral distance
    double       dist2;          // max epicentral distance
    double      sdepth;          // source depth
    double      rdepth = 0.0;    // receiver depth
    double      dx;              // separation of dist
    double      x0;              // starting distance in Kir library

    int         nr;              // number of points to read 
    double      x;

    bool        error = false;

    setpar(ac,av);
    mstpar("modelfile",    "s", modelfile);
    mstpar("rayfile",      "s", rayfile);
    mstpar("RayParFile",   "s", RayParfile);

    mstpar("sdepth",       "F", &sdepth);
    getpar("rdepth",       "F", &rdepth);
    mstpar("dist1",        "F", &dist1);
    mstpar("dist2",        "F", &dist2);
    mstpar("dx",           "F", &dx);
    mstpar("x0",           "F", &x0);
    mstpar("t",            "F", &t);
    mstpar("nr",           "d", &nr);
    endpar();

 
    if(dist1 > dist2){
	cerr << "Error in specifying dist1 and dist2 (dist1 <= dist2)\n";
	return ERROR_EXIT;
    }
	
    /* reading the model */
    class Model model;

    if(!model.Read(modelfile)){
	cerr << "Fail to read model from file %s\n",modelfile;
	return ERROR_EXIT;
    }

    /* reading the Rays */
    class Ray   *ray;
    int         nray;
    if(!ReadRayfile(rayfile, nray, &ray))
	return ERROR_EXIT;

    int    i;
    double *to;   

    double xsource = 100.0;

    class gray g1(xsource, ray[0], model, sdepth);
     
    to  = new double[nr];

    for(i = 0; i< nr; i++){
	x = xsource + i * dx;
	g1.reset_dist(x);
	to[i] = g1.to();
	cout << "x=" << x;
	cout << "to(" << i << ")=" << to[i];
	cout << "po(" << i << ")=" << g1.po() << "\n";
    }

    int nleft, nright, middle;

    double *t1 = new double[nr];

    if(t1 == 0){
	cerr << "Error in allocating the memmory for t1 in main\n";
	error  = true;
    }

    double xkir    = dist1 -xsource;
    int    nstart  = (int) ((x0 - xkir)/dx);
    if(!error)
        if(!ReadRayParfile(RayParfile, nstart, nr, t1, TRAVEL_TIME)){
	    cerr << "Error in Reading RayParfile " << RayParfile << "\n";
	    error  = true;
        }

    for(i=0; i<nr; i++)
	cout << "t1(" << i << ")=" << t1[i] << "\n";

    if(!error)
        if(!findLeftRight(t1, to, t, nr, nleft, nright, middle)){
	    cerr << " Error in finding Left and Right boundaries\n";
            error  = true;
        }

    if(to != 0) 
	delete to;
    if(t1 != 0) 
	delete t1;

    cout << (nleft *dx + xsource) << " " << (nright*dx + xsource) << " " <<(middle*dx + xsource) << "\n"; 

    if(error)
        return ERROR_EXIT;
    else
        return GOOD_EXIT;

}
