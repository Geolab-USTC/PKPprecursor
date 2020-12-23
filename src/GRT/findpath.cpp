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

int main(int ac, char *av[])
{
    char modelfile[60];   // file name for model
    char rayfile[60];     // file name for Ray

    int         top;
    int         bottom;

    double      t;               // Total time for synthetics 
    double      dist;            // epicentral distance
    double      sdepth;          // source depth
    double      rdepth = 2901;   // receiver depth

    double      xmin;

    bool        error = false;

    setpar(ac,av);
    mstpar("modelfile",    "s", modelfile);
    getpar("rayfile",      "s", rayfile);

    mstpar("sdepth",       "F", &sdepth);
    getpar("rdepth",       "F", &rdepth);
    mstpar("dist",         "F", &dist);
    mstpar("xmin",         "F", &xmin);
    mstpar("top",          "d", &top);
    mstpar("bottom",       "d", &bottom);
    endpar();

 
    /* reading the model */
    class Model model;

    if(!model.Read(modelfile)){
	cerr << "Fail to read model from file %s\n",modelfile;
	return ERROR_EXIT;
    }

    /* reading the Rays */
    /*
    class Ray   *ray;
    int         nray;
    if(!ReadRayfile(rayfile, nray, &ray))
	return ERROR_EXIT;
*/
    class gray g1(xmin, 50, 5, model, sdepth,rdepth);
     
    g1.reset_dist(xmin);

    int i;
    cout << " to   ";
    cout << " to1  ";
    cout << " to2  ";
    cout << " po1  ";
    cout << " po2  ";
    cout << endl;

    cout.setf(ios::scientific);
    for(i = top; i<= bottom; i++){
        class gray g2(dist-xmin,i,5,model,sdepth,rdepth);
	cout << i;
        cout <<" "<< g1.to()+g2.to();
        cout <<" "<< g1.to();
        cout <<" "<< g2.to();
        cout <<" "<<  g1.po();
        cout <<" "<<  g2.po();
	cout << endl;
    }

    if(error)
        return ERROR_EXIT;
    else
        return GOOD_EXIT;

}
