/************************************************************/
/*                                                          */
/*                   model.cpp                              */
/*              Class Model for Synthetics                  */
/*         Class Implementation for Members                  */
/*                                                          */
/************************************************************/

/*  
    This file contains the implementation for functions for the 
    Model class.  The class is defined in file model.h.
    
    Revision History:
        12 Dec. 1995    Lianxing Wen              Initial revision.
	25 Aug. 1997    Lianxing Wen              add nb method
*/           

#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>

#include "model.h"

/* 
    Model::Model(int nlayer = 0)

    Description:        This function implements a constructor for class
			Model. 

    Arguments:          nlayer  -> number of layer 

    Error Handling:     If fails to allocate, exit; 

    Last Modified:      12 Dec, 1996

*/

Model::Model(int nlayer)
{
    if(!setup(nlayer)){
	cerr << " Can not allocate the memory\n";
	exit(-1);
    }
}


/* 
    Model::~Model()

    Description:        This function implements a destructor for class
			Model. 

    Last Modified:      12 Dec, 1996

*/

Model::~Model()
{
    if( _vp != 0) delete _vp;
    if( _vs != 0) delete _vs;
    if( _d  != 0) delete _d;
    if( _th != 0) delete _th;
    if( _q  != 0) delete _q;
}

/* 

    int Model::nb(double sdepth)

    Description:        This function returns the layer number of 
			sdepth.

    Arguments:          sdepth   -> depth

    Return Value:       number of layer of sdepth.

    Last Modified:      25 Aug, 1997

*/

int Model::nb(double sdepth) const
{
    int     j;         // layer index
    double  depth=0;   // depth of the layer
    int     nb;

    depth = 0.0;
    for(j=0; j<_nlayer; j++){
	depth  += _th[j];
	if(depth < sdepth){
	    nb = j;
        }
    }

    nb++;
    return nb;
}
	     


/* 

    int Model::flatten(double earth_radius)

    Description:        This function adjusts the values of parameters for 
			class Model by flattening the Earth. If fails, 
			return -1; otherwise return 1;

    Arguments:          earth_raidus  -> radius of the Earth 

    Return Value:       1.

    Last Modified:      12 Dec, 1996

*/

int Model::flatten(double radius)
{
    int     j;         // layer index
    double  depth;     // average depth of the layer
    double  q;         // correction factor
    double  th;

    depth = th = 0.0;
    for(j=0; j<_nlayer; j++){
	depth  += 0.5*(_th[j] + th);
        th = _th[j];

	q       = radius/(radius - depth);

	_vp[j]  *= q;
	_vs[j]  *= q;
	_d[j]   *= q;
	_th[j]  *= q;
    }

    return 1;
}   

/* 
    int Model::Read(char *filename) 

    Description:        This function reads the model parameters from 
			a file. It sets the values for the class.
			It returns :
			    case = 1;  sucessfully read 
			         = -1; not sucessfully read. 
    Arguments:          filename (char *).
    Return Value:       int  -> case

    Inputs:             None.
    Outputs:            If fails to open a file, abort the program and 
			output messages. 

    Last Modified:      12 Dec, 1996

*/

int Model::Read(char *filename)
{
    int success;
    ifstream infile;

    char c;         

    infile.open(filename);

    if(!infile){
	// error opening the file, abort the program
	cerr << "Fail to open file " << filename <<" in Model::Read\n";
	exit(-1);
    }

    success = Read(infile);
    infile.close();

    return success;
}    


/* 
    int Model::Read(ifstream &infile) 

    Description:        This function reads the model parameters from 
			a file stream. It sets the values for the class.
			It returns :
			    case = 1;  sucessfully read 
			         = -1; not sucessfully read. 
    Arguments:          infile (ifstream &) .
    Return Value:       int  -> case

    Last Modified:      12 Dec, 1996

*/

int Model::Read(ifstream &infile)
{
    int success = 1;

    int j;
    double vp, vs, d, th;

    infile >> _nlayer;   // total number of layer

    setup(_nlayer);

    if(infile.good()){

        for(j = 0; j < _nlayer; j++){
        
            infile >> _vp[j] >> _vs[j] >> _d[j] >> _th[j];

            if(!infile.good())
                success = -1; 
        }        
     }
     else
         success = -1;           

    return success;

}




/* 

    int Model::setup(int nlayer)

    Description:        This function implements a setup for class
			Model. If fails, return 0; otherwise return 1;

    Arguments:          nlayer (int).

    Return Value:       1: success, 0 fails.

    Last Modified:      12 Dec, 1996

*/

int Model::setup(int nlayer)
{
    int success = 0;

    _nlayer  = nlayer;
    _vp   = new double[nlayer];
    _vs   = new double[nlayer];
    _d    = new double[nlayer];
    _q    = new double[nlayer];
    _th   = new double[nlayer];

    if(_vp != 0 && _vs != 0 && _d != 0 
       && _q != 0 && _th != 0)
        success = 1;

    return success;
}



/* 
    double Model::vp(int layer)

    Description:        This function implements a method for returning 
			P-wave velocity in a layer. It returns -1.0, if 
			layer is out of the range.

    Arguments:          layer (int).

    Return Value:       _vp[layer]   -> if 0<=layer<_nlayer 
			-1           -> otherwise


    Last Modified:      12 Dec, 1996

*/

double Model::vp(int layer) const
{

   double value = -1.0;

   if(layer >= 0 && layer < _nlayer)
       value = _vp[layer];

   return value;
}   

/* 
    double Model::vs(int layer)

    Description:        This function implements a method for returning 
			S-wave velocity in a layer. It returns -1.0, if 
			layer is out of the range.

    Arguments:          layer (int).

    Return Value:       _vs[layer]   -> if 0<=layer<_nlayer 
			-1           -> otherwise

    Last Modified:      12 Dec, 1996

*/

double Model::vs(int layer) const
{

   double value = -1.0;

   if(layer >= 0 && layer < _nlayer)
       value = _vs[layer];

   return value;
}   


/* 
    double Model::d(int layer)

    Description:        This function implements a method for returning 
			density in a layer. It returns -1.0, if layer 
			is out of the range.

    Arguments:          layer (int).

    Return Value:       _d[layer]    -> if 0<=layer<_nlayer 
			-1           -> otherwise

    Last Modified:      12 Dec, 1996

*/

double Model::d(int layer) const
{

   double value = -1.0;

   if(layer >= 0 && layer < _nlayer)
       value = _d[layer];

   return value;
}   

/* 
    double Model::q(int layer)

    Description:        This function implements a method for returning 
			Q-value in a layer. It returns -1.0, if layer 
			is out of the range.

    Arguments:          layer (int).

    Return Value:       _q[layer]    -> if 0<=layer<_nlayer 
			-1           -> otherwise

    Last Modified:      12 Dec, 1996

*/

double Model::q(int layer) const
{

   double value = -1.0;

   if(layer >= 0 && layer < _nlayer)
       value = _q[layer];

   return value;
}   


/* 
    double Model::th(int layer)

    Description:        This function implements a method for returning 
			thickness of a layer. It returns -1.0, if layer 
			is out of the range.

    Arguments:          layer (int).

    Return Value:       _th[layer]   -> if 0<=layer<_nlayer 
			-1           -> otherwise


    Last Modified:      12 Dec, 1996

*/

double Model::th(int layer) const
{

   double value = -1.0;

   if(layer >= 0 && layer < _nlayer)
       value = _th[layer];

   return value;
}   





/* 
    int Model::setvp(int layer, double value)

    Description:        This function implements a method for setting 
			P-wave velocity in a layer. It returns -1.0, if 
			layer is out of the range.

    Arguments:          layer (int), value (double).

    Return Value:       1       -> if 0<=layer<_nlayer 
		       -1      -> otherwise


    Last Modified:      12 Dec, 1996

*/

int Model::setvp(int layer, double value)
{

   int success = -1;

   if(layer >= 0 && layer < _nlayer){
       _vp[layer] = value;
       success  = 1;
   }

   return success;
}   

/* 
    int Model::setvs(int layer, double value)

    Description:        This function implements a method for setting 
			S-wave velocity in a layer. It returns -1.0, if 
			layer is out of the range.

    Arguments:          layer (int), value (double).

    Return Value:       1       -> if 0<=layer<_nlayer 
		       -1      -> otherwise

    Last Modified:      12 Dec, 1996

*/

int Model::setvs(int layer, double value)
{

   int success = -1;

   if(layer >= 0 && layer < _nlayer){
       _vs[layer] = value;
       success  = 1;
   }

   return success;
}   

/* 
    int Model::setd(int layer, double value)

    Description:        This function implements a method for setting 
			density in a layer. It returns -1.0, if 
			layer is out of the range.

    Arguments:          layer (int), value (double).

    Return Value:       1       -> if 0<=layer<_nlayer 
		       -1      -> otherwise


    Last Modified:      12 Dec, 1996

*/

int Model::setd(int layer, double value)
{

   int success = -1;

   if(layer >= 0 && layer < _nlayer){
       _d[layer] = value;
       success  = 1;
   }

   return success;
}   

/* 
    int Model::setq(int layer, double value)

    Description:        This function implements a method for setting 
			Q-value in a layer. It returns -1.0, if 
			layer is out of the range.

    Arguments:          layer (int), value (double).

    Return Value:       1       -> if 0<=layer<_nlayer 
		       -1      -> otherwise


    Last Modified:      12 Dec, 1996

*/

int Model::setq(int layer, double value)
{

   int success = -1;

   if(layer >= 0 && layer < _nlayer){
       _q[layer] = value;
       success  = 1;
   }

   return success;
}   

/* 
    int Model::setth(int layer, double value)

    Description:        This function implements a method for setting 
			thickness of a layer. It returns -1.0, if 
			layer is out of the range.

    Arguments:          layer (int), value (double).

    Return Value:       1       -> if 0<=layer<_nlayer 
		       -1      -> otherwise


    Last Modified:      12 Dec, 1996

*/

int Model::setth(int layer, double value)
{

   int success = -1;

   if(layer >= 0 && layer < _nlayer){
       _th[layer] = value;
       success  = 1;
   }

   return success;
}   

/* 
    Model& operator = (const Model &) 

    Description:        This function implements the operator = 
                        for Model class. 

    Arguments:          model (const Model &) 

    Return Value:       Model &


    Last Modified:      12 Dec, 1996

*/

Model& Model::operator = (const Model &model)
{

    int j, nlayer;
    
    nlayer = model.layers();

    setup(nlayer);

    for(j=0; j < nlayer; j++){
        _vp[j]  = model.vp(j);
        _vs[j]  = model.vs(j);
        _d[j]   = model.d(j);
        _q[j]   = model.q(j);
        _th[j]  = model.th(j);
    }    
   
    return *this;
}   


