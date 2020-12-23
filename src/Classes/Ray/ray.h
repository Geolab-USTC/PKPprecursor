/**************************************************************/
/*                                                            */
/*                ray.h                                       */
/*              Class Ray Decleration                         */
/*                                                            */
/**************************************************************/

/*
    This file contains the class declarations for the Ray class.
    The implementation of private functions are defined in file 
    raypf.cpp and that of public functions are defined in file 
    ray.cpp.

    Revision History:
        11 Jan 1996            Lianxing Wen       Initial Revision
*/        

#ifndef  __RAY_H___
    #define  __RAY_H___

#include <fstream.h>

/* class declaration */

/*
    Class:                Ray
    
    Description:          This class implements ray parameters for 
                          a specific ray. The data members are:
			     int _type     -> ray types: 
						2 for SH 
					        11 P  source::P  receiver
					        12 P  source::SV receiver
					        21 SV source::P  receiver
					        22 SV source::SV receiver

			     int *_upleaving   -> 1: upleaving 0 downleaving 
			     int *_slayer      -> source layer 

			     int *_uparriving  -> 1: uparriving 0 downarriving 
			     int *_rlayer      -> receiver layer 

			     int _upmostlayer    -> upmost layer
			     int _downmostlayer  -> downmost layer

			     double *_pseg       -> P wave sgement
			     double *_sseg       -> S wave sgement
			     double _rlen;       -> last segment
			     double _slen;       -> first segment

    Author:                Lianxing Wen

    Error Handling:        None.

    Last Modified:         31, Dec 1996.

*/

class Ray {

    private:

	int _type;      // 2 for love; 0 for PSV-H

	int _upleaving;    // upleaving or downleaving
	int _slayer;       // source layer 

	int *_uparriving;   // uparriving ot downarriving
	int _rlayer;       // receiver layer 

	int _upmostlayer;    // upmost layer
	int _downmostlayer;  // downmost layer

	double *_pseg;        // P wave segment
	double *_sseg;        // S wave segment

	double _rlen;         // last segment
	double _slen;         // first segment

	int     _nseg;        // Number of Segments    
	int    *_seg;         // layer of Segment of the ray     
	int    *_stype;       // type Segment of the ray     

    private:
	int raytype(int nseg, int *stype);
	int checkarriving(int nseg, int *seg, double slen);
	int raysegs(int nseg, int *seg, int *stype, double slen, double rlen);
	int checkdownupmost(int nseg, int *seg);

    public:

	inline Ray(int nseg = 0){ };                  // constructor

	Ray(int nseg, int *seg, int *stype, double slen = 1.0, double rlen = 0.0);    
				 // constructor

	~Ray();     // destructor

    public:

	int setup(int nseg, int *seg, int *stype, double slen = 1.0, double rlen = 0.0);
	   // setup the ray;

        int setlen(int layer, double len, int p = 1, int rep =1);  
	     // Modify the ray path a particular segment; rep -> repeat times
	     // p = 1: P wave 0: SH or SV waves

	inline int setrlen(double val) {_rlen= val; return (val >= -1.0 && val <= 1.0);};
	inline int setslen(double val) {_slen= val; return (val >= -1.0 && val <= 1.0);};

    public:
	
	inline int type() const {  return _type;};   // source type

	inline int leaving() const {  return _upleaving;}; // 1: upleaving source
						      // -1: doleaving source
	inline int slayer() const  { return _slayer;};     // source layer

	inline int arriving() const { return _uparriving[_nseg-1];}; 
	    // 1: uparriving receiver; -1 downarriving receiver

	inline int uparriving(int nseg) const { return _uparriving[nseg];};
	    // 1: uparriving::-1 downarriving of each ray segment 

	inline int rlayer() const  { return _rlayer;}; // receiver layer

	inline int upmostlayer() const { return _upmostlayer;};
	inline int downmostlayer() const { return _downmostlayer;};

	inline double pseg(int layer) const { return _pseg[layer];};
	inline double sseg(int layer) const { return _sseg[layer];};

	inline int nseg() const  {return _nseg;};
	inline int seg(int nseg) const  {return _seg[nseg];};
	inline int stype(int nseg) const {return _stype[nseg];};

	inline double rlen() const {return _rlen;};
	inline double slen() const {return _slen;};

    public: 
	int Read(ifstream &infile, int nature =1, double slen = 1.0, double rlen = 0.0);

    public:
	Ray& operator = (const Ray &);

};    


#endif  __RAY_H___
