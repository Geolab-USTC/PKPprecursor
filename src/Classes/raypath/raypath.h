/************************************************************/
/*                                                          */
/*                    raypath.h                             */
/*                  Ray Path Class                          */
/*                 Class Declaration                        */
/*                                                          */
/************************************************************/

/*  
    This file contains the class declarations for the ray path 
    class.
    
    Revision History:
        12 Dec. 1995    Lianxing Wen              Initial revision.
*/           

//  check if this file is already #included
#ifndef  __RAYPATH_H__
    #define  __RAYPATH_H__


/*  Class Declarations */

/* 
    Class:               raypath

    Description:         This class implements several properties 
                         for the Ray path. 
                         
    Author:              Lianxing Wen
    Last Modified:       12 Dec 1996.

    Data Members:        
			 _nseg (int)        // number of segments
			 _x   (double *)    // x coordinates 
			 _z   (double *)    // z coordinates
			 _y   (double *)    // y coordinates 

			    
    Private Methods:
    
    Public Methods:      nseg()    -> return number of segment 
                         _x(j)     -> return x[j] 
                         _z(j)     -> return z[j] 
                         _y(j)     -> return y[j] 

    General Functions:   
    
    Error Handler: 
    
*/                                                  

class raypath{

    private:

        int          _nseg;      // number of segments 

        double      *_x;         // x coordinates 
        double      *_z;         // z coordinates 
        double      *_y;         // y coordinates 

        // constructor
    public:
        raypath(int nseg = 0);

        // destructor
    public:
	~raypath();

        // public inline methods
    public:
        inline int nseg() { return _nseg;};    
        inline double x(int j)  { return _x[j];};    
        inline double z(int j)  { return _z[j];};    
        inline double y(int j)  { return _y[j];};    

    public:
        int setnseg (int nseg);
	int setx(int j, double x);
	int setz(int j, double y);
	int sety(int j, double z);

};

#endif  __RAYPATH_H__
