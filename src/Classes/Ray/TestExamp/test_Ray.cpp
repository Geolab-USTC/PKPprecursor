/*****************************************************************/
/*                                                               */
/*                 test_Ray.cpp                                  */
/*                                                               */
/*****************************************************************/

/* This file contains the main program to test the Ray class.

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
#include "ray.h"

int main(int ac, char *av[])
{
    char rayfile[60];   // file name for model
    ifstream infile;
    int j, nray;
    double sdepth;

    setpar(ac,av);
    mstpar("rayfile",    "s", rayfile);
    mstpar("sdepth",     "F", &sdepth);
    endpar();

    /* reading the model */
    class Ray *ray;

    infile.open(rayfile);

    if(!infile){
	cerr << "Fail to open file" << rayfile << "in Main\n";
	exit(-1);
    }

    infile >> nray;  // number of rays

    ray = new Ray[nray];

    for(j=0; j < nray; j++)
       if(!ray[j].Read(infile,1,sdepth,0.0))
	   cerr << "Error \n";
    infile.close();

    int i, nseg;

    for(j=0; j < nray; j++){
        nseg = ray[j].nseg();

        cout << nseg << ' ';
	for(i=0; i <nseg; i++)
	    cout << ray[j].seg(i) << ' ';
        cout << "\n";

        cout << 1 << ' ';
	for(i=0; i <nseg; i++)
	    cout << ray[j].stype(i) << ' ';
        cout << "\n";

	cout << "type=" << ray[j].type() << " " << "leaving=" << ray[j].leaving() <<" " << "slayer="<<ray[j].slayer() << "\n";

	cout << "arriving=" << ray[j].arriving() << " " << "rlayer=" << ray[j].rlayer() <<" " << "upmost="<<ray[j].upmostlayer() << " " << "downmost=" << ray[j].downmostlayer() <<  "\n";

	for(i=0; i< nseg; i++)
	    cout << ray[j].uparriving(i) << " ";
        cout << "\n";
        cout << "\n";
    }



    return 1;

}
 
