/*****************************************************************/
/*                                                               */
/*                 ray_tracing.cpp                               */
/*                Ray tracing Program                            */
/*                                                               */
/*****************************************************************/

/* This file contains the main program to ray tracing the generalized
   Ray.

   Revision History:
       6 Jan. 1997, Lianxing Wen              Initial Revision

*/

/*library files */

#include <fstream.h>
#include <stdlib.h>

/* getpar file  */
extern "C" {
    #include "getpar.h"
}

/*local include files */
#include "gray.h"
#include "ray.h"
#include "model.h"


int main(int ac, char *av[])
{
    char modelfile[60];   // file name for model
    char rayfile[60];     // file name for Ray
    ifstream infile;
    double dist, sdepth, rdepth = 0.0;
    double rlen, slen;
    double depth;
    char c;
    int j, nb, layer;
    int nray;

    setpar(ac,av);
    mstpar("modelfile",    "s", modelfile);
    mstpar("rayfile",      "s", rayfile);
    mstpar("sdepth",       "F", &sdepth);
    getpar("rdepth",       "F", &rdepth);
    mstpar("dist",         "F", &dist);
    endpar();

int ray_tracing(class Model model, double sdepth, double rdepth, double dist, int type)
{

    /* construct the generalized ray */
    class gray g1;

    cout << "to=" << g1.to() <<" " << "po=" << g1.po() << "\n";
}





/*
    Ray construcRay(int nb, double slen, int nrec, double rlen, 
		    int nturn, int type)

    Description:  This function implements a method to construct a 
		  Ray instnace based on the inputs.

    Arguments:    

		  nb   (int)      -> source layer 
		  slen (double)   -> first segment 
		  nrec  (int)     -> receiver layer 
		  rlen (double)   -> last segment 
		  nturn  (int)    -> turning layer

    Return Value: ray (Ray) ;

    Last Modified:  9 Jan, 97

*/


Ray construcRay(int nb, double slen, int nrec, double rlen, int nturn, int ncmb, int type)
{

    int nseg, *seg, *stype; 

    int j, n1;

    seg      = new int [nturn *4];
    stype    = new int [nturn *4];

    nseg = 0;

    n1 = ((nturn > ncmb) ? ncmb : nturn);

    /* downgoing */
    for(j = nb; j <= n1; j ++){
	seg[nseg]   = j;
	stype[nseg] = P_SV;
	nseg ++;
    }
    for(j = n1+1; j <= nturn; j ++){
	seg[nseg]   = j;
	stype[nseg] = 5;
	nseg ++;
    }

    /* upgoing */
    for(j = nturn; j >= n1 + 1; j --){
	seg[nseg]   = j;
	stype[nseg] = 5;
	nseg ++;
    }
    for(j = n1; j >= nrec; j --){
	seg[nseg]   = j;
	stype[nseg] = P_SV;
	nseg ++;
    }

    class Ray ray(nseg, seg, stype, slen, rlen);

    delete seg;
    delete stype;

    return ray;

}


/*
    int findnb(class Model  double sdepth, double rdepth, int &nb, 
	       double &slen, int &nrec, double &rlen)

    Description:  This function implements a method to find the 
		  source and receiver layers and relative segements.

    Arguments:    model (Model)   -> model
		  sdepth (double) -> source depth
		  rdepth (double) -> receiver depth

		  nb   (int)      -> source layer (return value)
		  slen (double)   -> first segment (return value)
		  nrec  (int)     -> receiver layer (return value)
		  rlen (double)   -> last segment (return value)

    Return Value:  1;

    Last Modified:  9 Jan, 97

*/

int findnb(class Model model, double sdepth, double rdepth, int &nb, double &slen, int &nrec, double &rlen)
{

    int j, layer;
    double depth;


    layer = model.layers();

    /* finding the source layer */
    depth = 0.0;
    rlen  = 0.0;
    nrec  = 2;
    for(j=0; j<layer; j++){
	depth += model.th(j);
	if(depth < sdepth) {
	    slen  = sdepth - depth;
	    slen /= model.th(j+1);
	    nb    = j+2;
        }
	if(depth < rdepth){
	    rlen  = rdepth - depth;
	    rlen /= model.th(j+1); 
	    nrec  = j+2;
        }
    }

    return 1;

}

/*
    int turninglayer(class Model  double p, int type)

    Description:  This function implements a method to find the 
		  layer where ray is turned back.

    Arguments:    model (Model)   -> model
		  p (double)      -> ray parameter 
		  type            -> 1 for P wave 2: for S wave

    Return Value: (int) layer: ray turns;

    Last Modified:  9 Jan, 97

*/

int turninglayer(class Model model, double p, int type)
{
    int j, layer;

    int nturn;

    layer = model.layers();

    /* finding the source layer */
    for(j=1; j<layer-1; j++){
        v1  = ((type == 1) ? model.vp(j)   : model.vs(j)); 
        v2  = ((type == 1) ? model.vp(j+1) : model.vs(j+1)); 

	v1 *= 0.5;
	v2 *= 0.5;

        if ((p > 1.0/v1 && p <= 1.0/v2) || (p > 1.0/v2 && p <= 1.0/v2))
	    nturn = j;

    }

    return (nturn + 1);  // increase by 1 to match the label

}










