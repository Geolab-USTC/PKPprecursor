/****************************************************************************/ 
/*                                                                          */ 
/*                                 GENERAL.H                                */ 
/*                        General Purpose Header File                       */ 
/*                                                                          */ 
/****************************************************************************/ 

/*
   This file contains a number of useful macros, constants, and types.
   Included are:
      boolean (type) - boolean type (TRUE/FALSE)
      streq (macro)  - compares two argument strings for equality


   Revision History:
      16 May 91  Glen George            Initial revision (from 12/3/90 version
                                        of stddef.h).
       6 Aug 91  Glen George            Updated comments.
      28 Oct 91  Glen George            Updated comments, including switching
                                        to C++ style (//) comments.
      23 Mar 92  Glen George            Changed definition of boolean TRUE to
                                        !0 (instead of 1).
       2 Oct 95  Glen George            Added #ifndef and __GENERAL_H__ symbol
                                        to avoid multiple inclusions.
       2 Oct 95  Glen George            Updated comments.
*/



// check if this file is already #included

#ifndef __GENERAL_H__
    #define __GENERAL_H__



/* Constants */  

/* general constants */
    /* none */

/* return codes */
const int  ARG_ERR_EXIT = 2;        // argument error return code
const int  ERROR_EXIT = 1;          // error return code for a program
const int  BAD_EXIT = 1;            // bad return code for a program
const int  GOOD_EXIT = 0;           // good return code for a program

/* string length constants */
const int  MAX_LINE_LEN = 200;      // maximum length of a line
const int  MAX_INPUT_SIZE = 200;    // maximum length of an input line
const int  MAX_PATH_LEN = 200;      // maximum length of a file path
 



 /* Macros */ 

#define  streq(s1,s2)  (strcmp(s1,s2) == 0)      // compare two strings
 



 /* Structures, Unions, and typedefs */ 

 /* bool (TRUE/FALSE) type */ 
// enum  bool  {  FALSE = 0, false = 0, TRUE = !0, true = !0  };

 /* generic types */ 
 typedef    unsigned   char       byte;
 typedef    unsigned   int        word;
 typedef    unsigned   long   int   longword;


 /* Function Prototypes */ 
     /* none */ 


#endif
