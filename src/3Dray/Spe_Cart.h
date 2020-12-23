
#ifndef _SPE_CART_H__
     #define _SPE_CART_H__

#include <math.h>
#include "sgeom.h"
#include "datafmt.h"

SPE trans(SPE pole, SPE old, int reverse);
/* transform the sperical coordinates old in the old sperical coordinates 
   poled at (0,90), to the new coordinates in the new sperical cooridnates 
   poled at pole (in the case reverse = 1), or vice verse (reverse = -1)
*/

/* Convert ned coordinates to Cartisan coordinaes */
struct Cart ned_xyz(SPE spe_point);

/* Convert Cartisan coordinates to ned coordinaes */
struct Spe xyz_ned(CART point);

double dotproduct(CART p1, CART p2);

double amp(CART p);

CART crossproduct(CART p1, CART p2);

double azimuth(SPE sp1, SPE sp2);


void ned_pole_project(int ndata, SPE *sp, struct Proj *p,int SouthPole);

#endif
