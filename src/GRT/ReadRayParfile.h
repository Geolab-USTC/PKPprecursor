
/**********************************************************************/
/*                                                                    */
/*                 ReadRayParfile.h                                   */
/*            Read Ray Parfile include file                           */
/*                                                                    */
/**********************************************************************/

/*
   This file contains the prototype of ReadRayParfile Program/

   Revision History:
       16 Jan 97 Lianxing Wen                     Initial version 
*/

#include "general.h"


// check if this file is already #included

#ifndef __READRAYPARFILE__
    #define __READRAYPARFILE__


/* Function Prototypes */ 
bool ReadRayParfile(char *RayParfile, int nstart, int nr, double *par, bool ttime=true);

const bool  TRAVEL_TIME   = true;
const bool  RAY_PARAMETER = false;

#endif
