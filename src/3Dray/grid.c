
#include <stdio.h>
#include "datafmt.h"

#define  BIGNUMBER   100 

int setgrids(double dlat, double dlon, double dlatshift, double dlonshift,int ndata, double gcarc, SCATTER *sout, char *file){

    SCATTER *psout = sout;

    int i, j;
    double energy, lat, lon;
    int NLAT = (int) ((180.0+0.00001)/dlat); 
    int NLON = (int) ((360.0+0.00001)/dlon); 

    double *amp = (double *) malloc (NLAT*NLON*sizeof(double));
    int *hit    = (int *) malloc (NLAT*NLON*sizeof(int));
    void PrintOf(int, int, double, double, char *file, double gcarc, double *amp, int *hit);

    for(i = 0; i< NLAT*NLON; i++){
	amp[i] = BIGNUMBER;   /* set to a big number */
	hit[i] = 0;
    }
    for(i=0; i<ndata; i++){
        lat    = psout->lat;
        lon    = psout->lon;
        energy = psout->energy;

        j  = (int) ((90.0 - lat - dlatshift) * NLAT /180.0);
        j *= NLON ; 
        j += (int) ( (lon + dlonshift) * NLON / 360.0);

	if(energy < -0.1){
            fprintf(stderr,"below noise level at lat=%lf lon=%lf\n",lat,lon);
	    /* it cannot confidently identified, but has a cap*/
	    if(amp[j] > 0.9*BIGNUMBER)  /*first time set the number*/
	        amp[j] = energy;
            else if( amp[j] > -energy){ /*trigger the cap */
	        amp[j] = -energy;
	        hit[j] += 1;
            }
            else if( amp[j] < energy) /*set a new cap */
	        amp[j] = energy;

        } else{
	    /* taking the  average energy */
	    amp[j] = (hit[j] * amp[j] + energy)/(hit[j] + 1); 
	    hit[j] = hit[j] + 1;
           /* take the maximum */
            amp[j] = (amp[j] < energy || amp[j] > 0.9*BIGNUMBER) ? energy : amp[j];
        }
        psout++;
    }
    PrintOf(NLAT,NLON,dlatshift,dlonshift,file,gcarc,amp,hit);
    free(amp); free(hit);
    return 1;
}

void PrintOf(int NLAT, int NLON, double dlatshift, double dlonshift, char *file, double gcarc, double *amp, int *hit)
{
    int j; double lat, lon;
    FILE *fop, *open_file(char *, char *);

    fop = open_file(file,"w");

    for(j=0; j< NLAT*NLON; j++){
	if(amp[j] < 0.9*BIGNUMBER){

	    lat = 90.0 - ((int) (j) /NLON) * 180.0/NLAT - dlatshift;
	    lon = (double) (j - (int) (j / NLON) * NLON) * 360.0 / NLON - dlonshift;

	    fprintf(fop,"%lf %lf %lf %lf\n", lat, lon, amp[j], gcarc);
        }
    }
    fclose(fop);
}
