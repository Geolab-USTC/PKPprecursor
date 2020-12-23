#include "Spe_Cart.h"

SPE trans(SPE pole, SPE old, int reverse)
/* transform the sperical coordinates old in the old sperical coordinates 
   poled at (0,90), to the new coordinates in the new sperical cooridnates 
   poled at pole (in the case reverse = 1), or vice verse (reverse = -1)
*/
{

    SPE  new;
    CART tmp1, tmp2;

    double l1, m1, n1;    
    double l2, m2, n2;    
    double l3, m3, n3;    

    double colat = -(0.5*PI - pole.lat) * reverse;

    l1 = cos(colat);
    m1 = 0.0;
    n1 = -sin(colat);

    l2 = 0.0;
    m2 = 1.0;
    n2 = 0.0;

    l3 = sin(colat);
    m3 = 0.0;
    n3 = cos(colat);

    if(reverse > 0){

        /* rotate X axis to the meridian of the pole, fix the pole */
        new.lon = old.lon - pole.lon;
        new.lat = old.lat;

        /* rotate pole to the pole*/
        tmp1 = ned_xyz(new);

        tmp2.x = l1 * tmp1.x + l2 * tmp1.y + l3 *tmp1.z; 
        tmp2.y = m1 * tmp1.x + m2 * tmp1.y + m3 *tmp1.z; 
        tmp2.z = n1 * tmp1.x + n2 * tmp1.y + n3 *tmp1.z; 

        new  = xyz_ned(tmp2);
    } else {
        tmp1 = ned_xyz(old);

        tmp2.x = l1 * tmp1.x + l2 * tmp1.y + l3 *tmp1.z; 
        tmp2.y = m1 * tmp1.x + m2 * tmp1.y + m3 *tmp1.z; 
        tmp2.z = n1 * tmp1.x + n2 * tmp1.y + n3 *tmp1.z; 

        new  = xyz_ned(tmp2);
         
        /* rotate X axis to the meridian of the pole, fix the pole */
        new.lon +=  pole.lon;

    }

    return new;

}

/* Convert ned coordinates to Cartisan coordinaes */
struct Cart ned_xyz(spe_point)
struct Spe spe_point;
    {
	double cos(), sin();
	struct Cart point;
	
	point.x=cos(spe_point.lat)*cos(spe_point.lon);
	point.y=cos(spe_point.lat)*sin(spe_point.lon);
	point.z=sin(spe_point.lat);
        return point;
    }

/* Convert Cartisan coordinates to ned coordinaes */
struct Spe xyz_ned(point)
struct Cart point;
    {
	double x1, x2, x3;
	double fabs(), atan2(), asin();
	struct Spe spe_point;

	x1=fabs(point.x);
	x2=fabs(point.y);
	x3=fabs(point.z);

	spe_point.lat = PI;
	if(x1<EPISON && x2<EPISON){
	    spe_point.lon= 0.;
	    if(x3<EPISON)spe_point.lat=0.;
	    return spe_point;
        }
	
	spe_point.lat= asin(x3);
	if(point.z<0.)spe_point.lat *=-1.;
	spe_point.lon= atan2(x2,x1);

	if(point.x<0. && point.y>0.) spe_point.lon=PI-spe_point.lon;
	if(point.x<0. && point.y<=0.)spe_point.lon=PI+spe_point.lon;
	if(point.x>0. && point.y<0.) spe_point.lon=2.0*PI-spe_point.lon;
	return spe_point;
    }

double dotproduct(p1,p2)
struct Cart p1, p2;
    
    {
	return (double)(p1.x*p2.x+p1.y*p2.y+p1.z*p2.z);
    }

double amp(p)
struct Cart p;
    {
	double sqrt();
	return sqrt((double)(p.x*p.x+p.y*p.y+p.z*p.z));
    }

struct Cart crossproduct(p1,p2)
struct Cart p1, p2;

    {
	struct Cart p;

	p.x=p1.y*p2.z-p1.z*p2.y;
	p.y=p1.z*p2.x-p1.x*p2.z;
	p.z=p1.x*p2.y-p1.y*p2.x;
	return p;
    }

double azimuth(sp1,sp2)
struct Spe sp1,sp2;
   
    {
	double az;
	double dotproduct(),amp();
	struct Cart pz, p1, p2, p0, p3, ned_xyz(),crossproduct();
	struct Spe xyz_ned();
	double atan(),sqrt(), sin(),fabs();

	pz.x=0.;
	pz.y=0.;
	pz.z=1.;

	p1=ned_xyz(sp1);
	p2=ned_xyz(sp2);

	p0=crossproduct(p1,p2);
	p0=crossproduct(p0,p1);

	p3=crossproduct(p1,pz);
	p3=crossproduct(p3,p1);

	az=dotproduct(p0,p3)/(amp(p3)*amp(p0));
	if(fabs(az)>1.)printf("az=%f\n",az);
	az=atan(sqrt(1.-az*az)/az);
	if(sin((sp2.lon-sp1.lon))<0.)az=2.*PI-az;

	return az;

    }

/* Project a bunch of data points into x-y plan */
/* SouthPole==1, SouthPole projection; default=NorthPole */
/*  
    x=colat*cos(lon)
    y=colat*sin(lon)
*/


void ned_pole_project(ndata,sp,p,SouthPole)
int ndata,SouthPole; struct Spe *sp; struct Proj * p;

    {
	double colat;
	double cos(), sin();
	struct Proj * p0;
	
	p0=p;
	while(ndata--){
	    colat=((SouthPole==1) ? 0.5*PI+sp->lat : 0.5*PI-sp->lat);
	    p0->x=colat*cos(sp->lon);
	    p0->y=colat*sin(sp->lon);
	    sp++;
	    p0++;
        }
    }











