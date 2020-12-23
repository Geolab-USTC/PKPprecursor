/****************************************************************************/ 
/*                                                                          */ 
/*                             findLeftRight.h                              */ 
/*                        findLeftRight Include Head File                   */ 
/*                                                                          */ 
/****************************************************************************/ 

/*
   This file contains the prototype of findLeftRight Program/

   Revision History:
       16 Jan 97 Lianxing Wen                     Initial version 
*/



// check if this file is already #included

#ifndef __FINDLEFTRIGHT__
    #define __FINDLEFTRIGHT__



/* Constants */  

/* general constants */
    /* none */

/* Function Prototypes */ 
bool findLeftRight(double *t1, double *to, double t, 
     int nr, int &nleft, int &nright, int &middle);

#endif
