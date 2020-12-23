/****************************************************************************/ 
/*                                                                          */ 
/*                             ReadRayfile.h                                */ 
/*                        ReadRayfile Include Head File                     */ 
/*                                                                          */ 
/****************************************************************************/ 

/*
   This file contains the prototype of ReadRayfile Program/

   Revision History:
       16 Jan 97 Lianxing Wen                     Initial version 
*/



// check if this file is already #included

#ifndef __READRAYFILE__
    #define __READRAYFILE__



/* Constants */  

/* general constants */
    /* none */

/* Function Prototypes */ 
    int ReadRayfile(char *rayfile, int &nray, class Ray **ray); 


#endif
