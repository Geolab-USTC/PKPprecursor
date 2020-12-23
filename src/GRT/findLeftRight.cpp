
/**********************************************************************/
/*                                                                    */
/*                 findLeftRight.cpp                                  */
/*        finding the left and right boundaries of Kirchhoff          */
/*                                                                    */
/**********************************************************************/

#include <fstream.h>
#include <iostream.h>

#include "general.h"

/* 
    findLeftRight(double *t1, double *to, double t, int nstart, 
		  int nr, int &nleft, int &nright, int &middle)
    
    Description:       This subroutine implemented to find the left 
		       and right boundary of Kirchhoff, based on the 
		       travel times in the interface from the source 
		       and receiver. The summation of two arrival times
		       from source and receiver is the arrival time from 
		       source to receiver. It finds the mininal total 
		       time, and truncates the boundaries where total 
		       time = mininal total time and duration time t.

    Arguments:         t1           -> travel time from receiver 
                       to           -> travel time from source
		       nstart       -> starting point at receiver
		       nr           -> points to check
		       nelft        -> left boundary index (returned)
		       nright       -> right boundary index (returned)
		       middle       -> middle index (returned)

    Last Modified:     

	Jan 23, 1997          Lianxing Wen      Initial revision

*/

bool findLeftRight(double *t1, double *to, double t, 
		  int nr, int &nleft, int &nright, int &middle)
{

    int i;

    //finding the minimal summation time 
    double tsmin = 1.e20;
    for(i=0; i < nr; i++){
	to[i] += t1[i];
	if(tsmin > to[i]){
	    tsmin = to[i];
	    middle = i;
        }
    }

    tsmin += t;

    bool left  = true;
    bool right = true;

    nleft = 0;
    for(i=middle; i >= 0; i--){
        if(to[i] > tsmin && left){
	    nleft = i;
	    left  = false;
        }
    }

    for(i=middle; i < nr; i++){
        if(to[i] > tsmin && right){
	    nright = i;
	    right = false;
        }
    }
    cout << "nright=" << nright <<" "<<"middle=" << middle <<" "<<"nleft=" << nleft <<"\n";

    return  (!left && !right && (nright > nleft));
}
