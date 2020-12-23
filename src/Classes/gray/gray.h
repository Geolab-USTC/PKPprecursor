/************************************************************/
/*                                                          */
/*                    gray.h                                */
/*              Generalized Ray Theory                      */
/*                 Class Declaration                        */
/*                                                          */
/************************************************************/

/*  
    This file contains the class declarations for the Generalized 
    Ray Theory. The implementations are defined in file gray.cpp.
    
    Revision History:
        12 Dec. 1995    Lianxing Wen              Initial revision.
        25 Aug. 1997    Lianxing Wen              add one constructor 
*/           

//  check if this file is already #included
#ifndef  __GRAY_H__
    #define  __GRAY_H__

#include <complex.h>


#include "ray.h"
#include "model.h"
#include "raypath.h"
#include "enumerate.h"


/*  Class Declarations */

/* 
    Class:               gray

    Description:         This class implements several properties 
                         for the Generalized Ray, based on Cagnard-
                         de-Hoop technique. The gray class contains 
                         the data members of class Ray and class Model 
			 and epicentral distance. 
                         
    Author:              Lianxing Wen
    Last Modified:       25 Dec 1997.

    Data Members:        _ray   (class Ray)    // Ray specification
                         _model (class Model)  // Model specification
			 _dist  (double)       // epicentral distance

                         _po     (double)      // po of the ray
                         _pc     (double)      // 0 if does not pass critical angle 
                         _to     (double)      // to of the ray
                         _tc     (double)      // negative if does not pass critical angle
                          
    Private Methods:
    
    Public Methods:      po()    -> return _po ray parameters
                         to()    -> return _to
                         pc()    -> return _pc
                         tc()    -> return _tc
			 dist()  -> return _dist

    General Functions:   
    
    Error Handler: 
    
*/                                                  

class gray {

    private:

        class Ray    _ray;        // ray description  
        class Model  _model;      // model description  

        double       _dist;       // epicentral distance
	double       _sdepth;     // source depth
	double       _rdepth;     // receiver depth

        double       _to;         // to for the ray
        double       _tc;         // tc for the ray
        double       _po;         // ray parameter
        double       _pc;         // ray parameter

	int          _headwave;   // past critical angle or not

        int          _max_layer;  // deepest layer
        double       _max_vel;    // maximal velocity 

        // private methods
    private:
        int setup(const class Ray & ray,  const class Model & model, double sdepth, double rdepth = 0.0);

        int setup(double dist, const class Ray& ray, const class Model & model, double sdepth, double rdepth=0.0);
	    // set up a Generalized Ray

        int setup(const class Ray & ray, const class Model & model, double po, double sdepth, int dummy);
	    // set up a Generalized Ray
        // private methods
    private:
        int findpo(double dist, const class Ray & ray, const class Model & model);  
			     // find po, to;
        int finddist(const class Ray & ray, const class Model & model, double po);  
			     // find dist;
        int findpi(const double pr, double &pi); // given pr, find pi;
//        int dpdt(complex p);                     // dp/dt;
        
        // private methods
    private:
           // reflection-transmission Coefficients and Radiation Pattern
 //       complex reftran(complex p, enum SourceType source = DoubleCouple); 
//        complex recev(enum SeisType seistype);    // receiver functions

        // constructor
    public:
	gray() { };
        gray(double dist, const class Ray &ray, const class Model &model, double sdepth);
        gray(const class Ray &ray, const class Model &model,double po, double sdepth);
        gray(double dist, int bottom, int type, const class Model &model, double sdepth, double rdepth=0.0);

        // constructor
	~gray();

    public:
        int refresh(double dist, const class Ray &ray, const class Model &model, double sdepth);
        int refresh(const class Ray &ray, const class Model &model, double po, double sdepth);

        void reset_dist(double dist);
        void reset_po(double po);
        void reset_ray(const Ray& ray);
        void reset_model(const Model& model);
        void reset_sdepth(double sdepth);

        // public inline methods
    public:
        inline double po()   const { return _po;}    
        inline double to()   const { return _to;}    
        inline double pc()   const { return _pc;}    
        inline double tc()   const { return _tc;}    
        inline double dist() const { return _dist;}    

    public:
	class raypath Raypath(int headwave=0);

};

#endif  __GRAY_H__
