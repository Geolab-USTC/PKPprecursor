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
    void ReadRsf(char *file, double g1, double g2, double v1, double v2,double *amp, int *hit, int CalEnergy);
    void PrintOf(char *file, double *amp, int *hit);

    char  rsf[90];  /* scattering output*/
    char  of[90];   /* output*/

    double g1 = 130.0;
    double g2 = 141.0;
    double v2 = 1.0;
    double v1 = 1.0;

    double dlat = 0.2;
    double dlon = 0.2;  

    int CalEnergy = -1;

    setpar(ac,av);
    mstpar("rsf",      "s",     rsf); 
    mstpar("of",       "s",     of); 
    getpar("dlat",     "F",     &dlat); 
    getpar("dlon",     "F",     &dlon); 
    getpar("g1",       "F",     &g1); 
    getpar("g2",       "F",     &g2); 
    getpar("v1",       "F",     &v1); 
    getpar("v2",       "F",     &v2); 
    getpar("CalEnergy",  "d",   &CalEnergy);
    endpar();

    NLAT = (int) ((180.0+0.00001)/dlat); 
    NLON = (int) ((360.0+0.00001)/dlon); 

    amp = (double *) malloc (NLAT*NLON*sizeof(double));
    hit = (int *) malloc (NLAT*NLON*sizeof(int));

    for(i = 0; i< NLAT*NLON; i++){
	amp[i] = BIGNUMBER;   /* set to a big number */
	hit[i] = 0;
    }

    ReadRsf(rsf, g1, g2, v1, v2, amp, hit, CalEnergy);
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

	    lat = 90.0 - ( (int) (j) /NLON) * 180.0/NLAT;
	    lon = (double) (j - (int) (j / NLON) * NLON) * 360.0 / NLON;

	    fprintf(fop,"%lf %lf %lf %d\n", lat, lon, amp[j], hit[j]);
        }
    }
    fclose(fop);
}



void ReadRsf(char *file, double g1, double g2, double v1, double v2, double *amp, int *hit, int CalEnergy)
{
    FILE *fop, *open_file(char *, char *);

    int i, j, ID, num;
    double energy, lat, lon;
    double gcarc;
    double DistAmpCorrection(double g1, double g2, double v1, double v2, double gcarc);

    fop = open_file(file,"r");

    while(fscanf(fop,"%lf %lf %lf %lf\n", &lat, &lon, &energy, &gcarc) != EOF){

        j  = (int) ((90.0 - lat) * NLAT /180.0);
        j  = (int) ((90.0 - lat + 0.001) / (180.0/NLAT));
        j *= NLON;
        j += (int) (lon * NLON / 360.0);


        if(gcarc >= g1 && gcarc <= g2){
        energy /= DistAmpCorrection(g1,g2,v1,v2,gcarc);

       
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
/*
            if(amp[j] > 0.9*BIGNUMBER || (amp[j] < energy) ) amp[j] = energy;
*/
            if(CalEnergy < 0 && energy > 0.00001) energy = 1.0;
	    amp[j] = (hit[j] * amp[j] + energy)/(hit[j] + 1); 
/*
*/
            hit[j] = hit[j] + 1;
        }
        }
    }
    fclose(fop);
}

double DistAmpCorrection(double gmin, double gmax, double v1, double v2, double gcarc){

    double adjust = 1.0;

    if(gcarc >= gmin && gcarc <= gmax){
        adjust = (v2 - v1)/(gmax-gmin) * (gcarc - gmin) + v1;
    }
    else {
        fprintf(stderr, "error in distance range %lf in DisAmpCorrection\n", gcarc);
        exit(-1);
    }

    return adjust;
    
}
