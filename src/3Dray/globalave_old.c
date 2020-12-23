
#include <stdio.h>
#include "datafmt.h"

#define  BIGNUMBER   100 

int    NLAT;
int    NLON;

int main(int ac, char **av)
{

    int i;
    double *amp;
    int    *hit;
    void ReadRsf(char *file, double *amp, int *hit);
    void PrintOf(char *file, double *amp, int *hit);

    char  rsf[90];  /* scattering output*/
    char  of[90];   /* output*/

    double dlat = 0.2;
    double dlon = 0.2;  

    setpar(ac,av);
    mstpar("rsf",      "s",     rsf); 
    mstpar("of",       "s",     of); 
    getpar("dlat",     "F",     &dlat); 
    getpar("dlon",     "F",     &dlon); 
    endpar();

    NLAT = (int) ((180.0+0.00001)/dlat); 
    NLON = (int) ((360.0+0.00001)/dlon); 

    amp = (double *) malloc (NLAT*NLON*sizeof(double));
    hit = (int *) malloc (NLAT*NLON*sizeof(int));

    for(i = 0; i< NLAT*NLON; i++){
	amp[i] = BIGNUMBER;   /* set to a big number */
	hit[i] = 0;
    }

    ReadRsf(rsf, amp, hit);
    PrintOf(of, amp, hit);
    free(amp); free(hit);

    return 1;

}
void PrintOf(char *file, double *amp, int *hit)
{
    int j; double lat, lon;
    FILE *fop, *open_file(char *, char *);

    fop = open_file(file,"w");

    for(j=0; j< NLAT*NLON; j++){
	if(amp[j] < 0.9*BIGNUMBER){

	    lat = 90.0 - (double) (j) /NLON * 180.0/NLAT;
	    lon = (double) (j - (int) (j / NLON) * NLON) * 360.0 / NLON;

	    fprintf(fop,"%lf %lf %lf %d\n", lat, lon, amp[j], hit[j]);
        }
    }
    fclose(fop);
}



void ReadRsf(char *file, double *amp, int *hit)
{
    FILE *fop, *open_file(char *, char *);

    int i, j, ID, num;
    double energy, lat, lon;

    fop = open_file(file,"r");

    while(fscanf(fop,"%d %d %lf\n", &ID, &num, &energy) != EOF){

	for(i=0; i<num; i++){
            fscanf(fop,"%lf %lf %lf %d\n", &lat, &lon, &energy, &ID);

            j  = (int) ((90.0 - lat) * NLAT /180.0);
            j *= (int) (NLON / 360.0); 
            j += (int) (lon * NLON / 360.0);

	    if(energy < -0.1){
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
/*
		if(amp[j] > 0.9*BIGNUMBER || amp[j] < 0)  
		    amp[j] = energy;
                else if(amp[j] > energy ) amp[j] = energy;
*/

            }
        }
    }
    fclose(fop);
}

