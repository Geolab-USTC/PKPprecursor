/************************************************************/
/*                                                          */
/*                   raypath.cpp                            */
/*         Class Implementation for Methods                 */
/*                                                          */
/************************************************************/

/*  
    This file contains the implementation for methods for 
    the raypath class. The class is defined in file raypath.h.
    
    Revision History:
        12 Dec. 1995    Lianxing Wen              Initial revision.
*/           

#include <iostream.h>

#include "raypath.h"

/* 
    raypath::raypath(int nseg)

    Description:        This function implements a constructor for 
			raypath class.

    Arguments:          nseg (int)    -> number of segments

    Return Value:       None.

    Inputs:             None.
    Outputs:            None.

    Error Handling:     None.

    Algorithm:          None.

    Last Modified:      12 Dec, 1996

*/

raypath::raypath(int nseg)
{
    _nseg = nseg;
    _x = new double [_nseg];
    _z = new double [_nseg];
    _y = new double [_nseg];
    if(_x == 0 || _z == 0 || _y == 0)
	cerr << "Can not allocate memory in raypath \n";
}   

/* 
    raypath::~raypath()

    Description:        This function implements a destructor for 
			raypath class.

    Arguments:          None.

    Return Value:       None.

    Inputs:             None.
    Outputs:            None.

    Error Handling:     None.

    Algorithm:          None.

    Last Modified:      12 Dec, 1996

*/
raypath::~raypath()
{
    if( _x != 0) delete _x;
    if( _z != 0) delete _z;
    if( _y != 0) delete _y;
}


/* 
    int raypath::setnseg(int nseg)

    Description:        This function sets up a raypath class.
			return 1, if successfully.

    Arguments:          nseg (int) -> number of segments 

    Return Value:       1: successfully done 

    Inputs:             None.
    Outputs:            None.

    Error Handling:     None.

    Algorithm:          None.

    Last Modified:      12 Dec, 1996

*/

int raypath::setnseg(int nseg)
{
    int success = 1;
    _nseg = nseg;
    _x = new double [_nseg];
    _z = new double [_nseg];
    _y = new double [_nseg];
    if(_x == 0 || _z == 0 || _y == 0)
	success = -1;

    return success;
}   

    
/* 
    int raypath::setx(int j, double x)

    Description:        This function sets the value of _x 
			coordinateat jth segment with value x.
			return 1, if successfully.

    Arguments:          j (int)    -> jth  fsegments 
			x (double) -> value to be set

    Return Value:       1: successfully done 

    Inputs:             None.
    Outputs:            None.

    Error Handling:     None.

    Algorithm:          None.

    Last Modified:      12 Dec, 1996

*/

int raypath::setx(int j, double x)
{
    int success = 1;

    if( j > 0 && j < _nseg)
	_x[j] = x;
    else
	success = -1;

    return success;

}


/* 
    int raypath::sety(int j, double y)

    Description:        This function sets the value of _y 
			coordinateat jth segment with value y.
			return 1, if successfully.

    Arguments:          j (int)    -> jth  fsegments 
			y (double) -> value to be set

    Return Value:       1: successfully done 

    Inputs:             None.
    Outputs:            None.

    Error Handling:     None.

    Algorithm:          None.

    Last Modified:      12 Dec, 1996

*/

int raypath::sety(int j, double y)
{
    int success = 1;

    if( j > 0 && j < _nseg)
	_y[j] = y;
    else
	success = -1;

    return success;

}


/* 
    int raypath::setz(int j, double z)

    Description:        This function sets the value of _z 
			coordinateat jth segment with value z.
			return 1, if successfully.

    Arguments:          j (int)    -> jth  fsegments 
			z (double) -> value to be set

    Return Value:       1: successfully done 

    Inputs:             None.
    Outputs:            None.

    Error Handling:     None.

    Algorithm:          None.

    Last Modified:      12 Dec, 1996

*/

int raypath::setz(int j, double z)
{
    int success = 1;

    if( j > 0 && j < _nseg)
	_z[j] = z;
    else
	success = -1;

    return success;

}

