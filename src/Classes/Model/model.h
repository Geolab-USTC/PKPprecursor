/**************************************************************/
/*                                                            */
/*                model.h                                     */
/*              class Model Decleration                       */
/*                                                            */
/**************************************************************/

/*
    This file contains the class declarations for the Model class.
    The implementations are defined in file model.cpp

    Revision History:
	27 Dec. 1996      Lianxing Wen          Initial Revision
	25 Aug. 1997      Lianxing Wen          add nb method
*/

// check if this file is #included
#ifndef  __MODEL_H___
    #define  __MODEL_H___

#include <fstream.h>    

/* class decleration  */

/*  
    Class:             Model

    Description:       This class implements model parameters for 
		       elastic medium. The data members are: 
			   _nlayer    -> number of layers
			   *_vp       -> P wave velocity
			   *_vs       -> S wave velocity
			   *_d        -> density 
			   *_q        -> Q Value 
			   *_th       -> Thickness of the layer

    Author:            Lianxing Wen

    Public Methods:    flatten();

    Public Methods:    
		       layers()
		       vp()
		       vs()
		       d()
		       q()
		       th()
		       setup()
		       setvp()
		       setvs()
		       setd()
		       setq()
		       setth()
    Constructor:       Model();

    Error Handlering:  None;

    Last Modified:     25, Aug., 1997
*/



class Model {
    
    private:

        int    _nlayer;          // number of layers

        double *_vp;             // P wave velocity
        double *_vs;             // S wave velocity
        double *_d;              // Density
        double *_th;             // Thickness of the Layer
        double *_q;              // Q value


    public: 

	Model(int nlayer=0);      // constructor

	int setup(int layer);    // set up model
	int setvp(int layer, double val);  // set P velocity at a layer     
	int setvs(int layer, double val);  // set S velocity at a layer     
	int setd(int layer, double val);   // set density at a layer     
	int setq(int layer, double val);   // set Q Value at a layer     
	int setth(int layer, double val);  // set the thickness of a layer     

	int Read(char *filename);
	int Read(ifstream & infile);

	~Model();                         // destructor
    public:
	inline int  layers() const {return _nlayer;}         
				    // return number of layers
	double vp(int layer) const;    // return P velocity at a layer     
	double vs(int layer) const;    // return S velocity at a layer     
	double d(int layer) const;     // return density at a layer     
	double q(int layer) const;     // return Q Value at a layer     
	double th(int layer) const;    // return the thickness of a layer     

	int    nb(double sdep) const;  // return the layer of sdepth 
 
    public:
	int flatten(double earth_radius = 6371.0);

    public:
        // assign operator
        Model& operator = (const Model &);	
};    



#endif  __MODEL_H___
