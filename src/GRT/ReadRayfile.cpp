
#include <iostream.h>
#include <fstream.h>

#include "ray.h"
#include "general.h"

int ReadRayfile(char *rayfile, int &nray, class Ray **ray) 
{

    ifstream infile;

    bool success = true;

    int j;

    infile.open(rayfile);
    if(!infile){
	cerr << "Fail to open file" << rayfile << "ReadRayfile\n";
	success = false;
    }

    infile >> nray;   // number of rays

    *ray  = new Ray[nray];

    for(j = 0; j < nray; j++)
        if(!(*ray)[j].Read(infile)){
	    cerr << "Error Reading the Rayfile\n";
	    success = false;
        }

    infile.close();

    return success;
}

