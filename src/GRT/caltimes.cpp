/*****************************************************************/
/*                                                               */
/*                 caltimes.cpp                                  */
/*             find timetable to for Generalized ray             */
/*                                                               */
/*****************************************************************/

/* This file contains the main program to find po and to  the generalized
   Ray.

   Revision History:
       1 March 1999, Lianxing Wen              Initial Revision

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

    double      to;              // Total time for synthetics 
    double      dist;            // epicentral distance
    double      dist1;           // epicentral distance
    double      ddist;           // epicentral distance
    double      dist2;           // epicentral distance
    double      sdepth;          // source depth
    double      rdepth = 0.01;   // receiver depth
    int         raytype = 5;     // raytyp = 4 S, 5 P

    bool        error = false;

    setpar(ac,av);
    mstpar("modelfile",    "s", modelfile);
    getpar("rayfile",      "s", rayfile);

    mstpar("sdepth",       "F", &sdepth);
    getpar("rdepth",       "F", &rdepth);
    mstpar("dist1",        "F", &dist1);
    mstpar("ddist",        "F", &ddist);
    mstpar("dist2",        "F", &dist2);
    mstpar("top",          "d", &top);
    mstpar("bottom",       "d", &bottom);
    getpar("raytype",      "d", &raytype);
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
     
    int i;
    cout.setf(ios::scientific);
    for(dist= dist1; dist <= dist2; dist += ddist){
        
        to = 1.e25;
        for(i = top; i<= bottom; i++){
            class gray g2(dist,i,raytype,model,sdepth);
            to  = (g2.to() < to) ? g2.to() : to;
            cout <<" "<< g2.po(); cout <<" "<< g2.to();
        }
	cout << dist << " " << to <<endl;

    }

    if(error)
        return ERROR_EXIT;
    else
        return GOOD_EXIT;

}
