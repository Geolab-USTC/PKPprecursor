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

    double       dist;           // max epicentral distance
    double      sdepth;          // source depth
    double      rdepth = 0.0;    // receiver depth


    bool        error = false;

    setpar(ac,av);
    mstpar("modelfile",    "s", modelfile);
    mstpar("rayfile",      "s", rayfile);

    mstpar("sdepth",       "F", &sdepth);
    getpar("rdepth",       "F", &rdepth);
    mstpar("dist",         "F", &dist);
    endpar();

 
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

    class gray g1(dist, ray[0], model, sdepth);
     
    cout << "to=" << g1.to() << "\n";

     return GOOD_EXIT;

}
