/*****************************************************************/
/*                                                               */
/*                 test_Model.cpp                                */
/*                                                               */
/*****************************************************************/

/* This file contains the main program to test the Model class.

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
#include "model.h"

int main(int ac, char *av[])
{
    char modelfile[60];   // file name for model
    int j, layer;

    setpar(ac,av);
    mstpar("modelfile",    "s", modelfile);
    endpar();

    /* reading the model */
    class Model model;
    if(!model.Read(modelfile))
	cerr << "Fail to read model from file %s\n",modelfile;

    layer = model.layers();
    cout << layer << "\n";
    for(j=0; j<layer; j++)
	cout << model.vp(j) << ' ' << model.vs(j) << ' ' << model.d(j) << ' ' << model.th(j) << "\n";

}
 
