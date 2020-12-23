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
    using namespace std;
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
    double      dist[3];         // epicentral distance
    double      sdepth;          // source depth
    double      rdepth = 2901;   // receiver depth

    double      xmin;

    bool        error = false;
    bool        direct = true;
    int         direc  = 1;
    double findto(double xmin,int i, int, class Model &model,double sdepth,double rdepth);
    double tt;

    setpar(ac,av);
    mstpar("modelfile",    "s",     modelfile);
    getpar("rayfile",      "s",     rayfile);

    getpar("sdepth",       "F",     &sdepth);
    getpar("rdepth",       "F",     &rdepth);
    mstpar("dist",         "vF[3]", dist);
    mstpar("top",          "d",     &top);
    mstpar("bottom",       "d",     &bottom);
    getpar("direct",       "d",     &direc);
    endpar();

    if(direc != 1) direct = false;

 
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

    cout.setf(ios::scientific);
    int Ntotal = int ( (dist[1] - dist[0]) /dist[2]) + 1;
    int i, j;
    cerr << "dist = "<< dist[0] << " " << dist[1] << " " << dist[2] << endl;

    if(direct){
   
	// calculating the direct rays
        class gray g1(dist[0], 50, 5, model, sdepth,rdepth);
	for(j=0; j < Ntotal; j++){
            xmin = dist[0] + j * dist[2];	
            g1.reset_dist(xmin);

	    cout << xmin << " " << g1.to() << endl; 
        }

    } else {
	// calculating the turnning rays

	for(j=0; j < Ntotal; j++){
            xmin = dist[0] + j * dist[2];	
	    bool found = false;
	    double t0  = 1.0;
	    double t1  = 0.1;
            for(i = top; i<= bottom; i++){
		if(!found){
		    tt = findto(xmin,i,5,model,sdepth,rdepth);
		    if(tt > t0 && t0 < t1){
			cout << xmin << " " << tt << endl;
			found = true;
                    } else {
			t1 = t0;
			t0 = tt;
                    }
                }
            }
	    if(!found) 
		cerr << "Warning, raypath was not found at " << xmin/111.119 
		     << " degree " << endl;
        }
    }

    if(error)
        return ERROR_EXIT;
    else
        return GOOD_EXIT;

}
double findto(double xmin,int i, int type, class Model &model,double sdepth,double rdepth){
    class gray g2(xmin,i,type,model,sdepth,rdepth);
    return g2.to();
}
