/**************************************************************/
/*                                                            */
/*                enumerate.h                                 */
/*              enumerate types Decleration                   */
/*                                                            */
/**************************************************************/

/*
    This file contains the enum declarations for the generate use of
    Generalized Ray Theory. 

    Revision History:
	27 Dec. 1996      Lianxing Wen          Initial Revision
*/

// check if this file is #included
#ifndef  __ENUMERATE_H___
    #define  __ENUMERATE_H___

enum SourceType { DoubleCouple = 0, Explosion = 1};

enum SeisType { U= 0, W=1, Txx = 0, Txz = 1, Tzz = 2};


#endif  __ENUMERATE_H___
