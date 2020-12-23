
#include <stdio.h>
#include "datafmt.h"
#include "Spe_Cart.h"
#include "sachead.h"

#define TINY         1.e-9
#define RATIO        2.0
#define NOISELEVEL   0.001 

int main(int ac, char **av)
{
    SPE pole, old;
    char  ts2rf[90], ds2rf[90], tr2sf[90], dr2sf[90];
    double PKPt2;

    char    seis[90];    /* SAC file */
    SACHEAD sach = sac_null;
    int rsacd(char *file, SACHEAD *h, double *amp);
    int np;
    double *amp, *pamp;

    double PKPduration = 6.0; 

    char  orf[90];  /* receiver-side scattering output*/
    char  osf[90];  /* source-side scattering output*/
    int patch(SPE pole, SPE old, char *tfile, char *dfile, double PKPt2, char *ofile, SACHEAD sach, double *amp, double period, int checknoise);
    void NormAmp(SACHEAD sach, double *amp, double PKPduration);
    double tshift = -1000.0;
    double period = 1.0;      /* seconds */
    int    checknoise = 1;

    setpar(ac,av);
    mstpar("Seis",       "s",   seis);    /* SAC file */
    mstpar("ts2rf",      "s",   ts2rf);   /* source to receiver turning*/
    mstpar("tr2sf",      "s",   tr2sf);   /* receiver to source turning*/
    mstpar("ds2rf",      "s",   ds2rf);   /* source to receiver direct */
    mstpar("dr2sf",      "s",   dr2sf);   /* receiver to source direct */
    mstpar("PKPdf",      "F",   &PKPt2);  /* theoretical PKPdf time */
    mstpar("orf",        "s",   orf);     /* output: receiver-side scattering*/ 
    mstpar("osf",        "s",   osf);     /* output: source-side scattering */
    getpar("PKPwavelet", "F",   &PKPduration);
    getpar("period",     "F",   &period); /* mapping period */
    getpar("checknoise", "d",   &checknoise);
    getpar("tshift",     "F",   &tshift);  /* time shift with respect to theoretical PKP Arrival */
    endpar();

    amp = (double *) malloc (100000 * sizeof(double));
    np  = rsacd(seis, &sach, amp);
    if(sach.o > -10000.){
        sach.b -= sach.o;
        fprintf(stderr,"sach.o %f\n",sach.o);
        sach.o  = 0.0;
        fprintf(stderr,"reset sach.b to %f\n",sach.b);
    }
    if(tshift > -100.0){
        sach.t0 = PKPt2 + tshift;
        fprintf(stderr,"reset sach.t0 to %f\n",sach.t0);
    }

    NormAmp(sach,amp,PKPduration);

    pole.lat = sach.stla*RAD; pole.lon = sach.stlo*RAD;
    old.lat  = sach.evla*RAD; old.lon  = sach.evlo*RAD;

    patch(pole, old, tr2sf, ds2rf, PKPt2, osf, sach, amp, period, checknoise);
    patch(old, pole, ts2rf, dr2sf, PKPt2, orf, sach, amp, period, checknoise);

    free(amp);
    return 1;

}

void NormAmp(SACHEAD sach, double *amp, double PKPduration)
{
    int n, nlast;

    int n0 = (int) ((sach.t0 - sach.b)/sach.delta);
    double sum;

/*
    for(n=0; n<sach.npts; n++)
	amp[n] *= amp[n];
*/

    nlast = (int) (PKPduration / sach.delta);

    sum = 0.0;
    for(n = n0; n<n0+nlast; n++)
	sum = (amp[n] > sum) ? amp[n]: sum;

    for(n=0; n<n0; n++)
	amp[n] /= sum;
}

int patch(SPE pole, SPE old, char *tfile, char *dfile, double PKPt2, char *ofile, SACHEAD sach, double *amp, double period, int checknoise)
{
    SPE pnew, *out;
    TTIME *tt, *td;
    int ntt, ntd;
    TTIME *ReadFile(char *file, int *n);
    double delta(double lat1, double lon1, double lat2, double lon2);
    double findtime(int n, TTIME *tt, double dist);
    int isotime(int ntt, TTIME *tt, int ntd, TTIME *td, double PKPt, SPE pnew, SPE *out);

    int i, j, k;
    int ID = 0;
    double dist;
    double lat1, lon1, lat2, lon2;
    double t1, t2, t, dt;
    int nn, nsample;  
    double Energy(double *amp, int n1, int n2);
    double energy = 0.0;
    double energy1;
    SPE trans(SPE pole, SPE old, int reverse);

    double PKPt1 = 0.0;
    FILE  *open_file(char *file, char *access_mode);
    FILE  *fop;

    double *pamp;

    nsample = (int) (period / sach.delta);
    if(nsample < 0){
        fprintf(stderr, "error in specifing period, it must be > delta \n";
        exit(-1);
    }

    pamp = amp + (int) ((sach.t0 - sach.b)/sach.delta);  /* picktime of PKPdf */

    out = (SPE *) malloc (5000*sizeof(SPE));

    pnew = trans(pole,old,1);
    tt   = ReadFile(tfile, &ntt);
    td   = ReadFile(dfile, &ntd);

    /* following for determining the earliest arrival of precursors*/
    lat1 = 0.5*PI-tt[0].r; /* convert distance to lat */ 
    lon1 = pnew.lon; 
    t1   = tt[0].t;
    dist = delta(lat1,lon1,pnew.lat,pnew.lon);
    t2   = findtime(ntd, td, dist);   /* earliest arrival at lat1*/	
    if(t2 > 0.1){
        PKPt1 = t1 + t2-1.e-10; /* set the earliest arrival */
    } else {
	fprintf(stderr, "Cannot determine the earliest arrival\n");
	exit(-1);
    }
    /* end of determining the earliest arrival of precursors*/

    /* check for the noisy level */
    nn = (int)((PKPt1 - PKPt2)/sach.delta);
    energy  = Energy(pamp,nn,0);
    energy1 = Energy(pamp,2*nn,nn);

    if(energy /energy1 < RATIO && energy > NOISELEVEL && checknoise == 1)
	/* noisy level */
	energy = -1;

    fop = open_file(ofile,"w");

    dt = nsample * sach.delta;
    for(t = PKPt2; t >= PKPt1; t-=dt){

	nn = (int) ((t - PKPt2)/sach.delta);
	if(nn == 0) nn = -1;

        if(energy > 0)
	    energy =  Energy(pamp, nn, nn+1);
        else
	    energy = -Energy(pamp, nn, nn+1);


        ntt = isotime(ntt, tt, ntd, td, t, pnew, out);

        fprintf(fop,"%d %d %lf\n",ID, 2*ntt-1, energy); 
        for(k=0; k<ntt; k++){
            old.lat = out[k].lat;
            old.lon = out[k].lon;
            old = trans(pole,old,-1);
            fprintf(fop,"%lf %lf %lf %d\n",old.lat*DAR,old.lon*DAR,energy,ID); 
        }
        for(k=ntt-2; k>=0; k--){
            old.lat = out[k].lat;
            old.lon = 2.0*pnew.lon - out[k].lon;
            old = trans(pole,old,-1);
            fprintf(fop,"%lf %lf %lf %d\n",old.lat*DAR,old.lon*DAR,energy,ID); 
        }
	ID++;
    }

    fclose(fop);
    free(tt); free(td); free(out);
    return 1;
}

double Energy(double *amp, int n1, int n2)
{
    int i;
    double energy = 0.0;
    for(i = n1; i < n2; i++)
	energy += amp[i];
    energy /= (n2-n1);
    return energy;
}


int isotime(int ntt, TTIME *tt, int ntd, TTIME *td, double PKPt, SPE pnew, SPE* out)
{
     int i, j, ntt0;
     double lon1, lon2, lat1, lat2, t1, t2, t3, dist;
     double delta(double lat1, double lon1, double lat2, double lon2);
     double findtime(int n, TTIME *tt, double dist);

     j = 0;
     for(i = 0; i< ntt; i++){

	lat1 = 0.5*PI-tt[i].r; /* convert distance to lat */ 
	lon1 = pnew.lon; 
        t1   = tt[i].t;
        dist = delta(lat1,lon1,pnew.lat,pnew.lon);
        t2   = findtime(ntd, td, dist);   /* earliest arrival at lat1*/	

	if(t2 > 0.1){  /* in the list */
	    t2 += t1;
	    if( t2 <= PKPt){

                lon2 = lon1 + 10.*RAD;
                while ( (t1 + findtime(ntd,td,delta(lat1,lon2,pnew.lat,pnew.lon)))<=PKPt) lon2 += 10.*RAD;

                while( (lon2 -lon1) > TINY){ /* search for the cross-point */
                    dist = delta(lat1,0.5*(lon2+lon1),pnew.lat,pnew.lon);	
                    t3 = findtime(ntd, td, dist);	
		    if(t3 > 0) 
			t3 += t1;
                    else 
			fprintf(stderr, "not enough points\n");
                   
		    if( t3 <= PKPt)
			lon1 = 0.5*(lon2+lon1);
	            else 
			lon2 = 0.5*(lon2+lon1);
                }
		out[j].lat = lat1; out[j].lon = lon1;
		j++;
            } else {
		fprintf(stderr, "Great circle not in the time window i=%d t2=%lf\n",i,t2);
            }

        } else {
	    fprintf(stderr, "Direct ray not in the time window\n");
        } 
    }
    if(j > 0){
        /* search for the last point at the great circle */
        lat1 = 0.5*PI-tt[j-1].r;  
        lat2 = 0.5*PI-tt[j].r;  
        lon1 = pnew.lon; 

	while( (lat2 - lat1) > TINY){
	    dist = delta(0.5*(lat2+lat1), lon1, pnew.lat, pnew.lon);
	    t3  = findtime(ntd,td,dist);
	    t1  = findtime(j,tt,0.5*(lat2+lat1));
            if(t3 > 0 && t1 > 0)
	        t3 += t1;
            else
		fprintf(stderr, "not enough points\n");
            if( t3 <= PKPt)
		lat1 = 0.5*(lat2+lat1);
            else
		lat2 = 0.5*(lat2+lat1);
        }
	out[j].lat = lat1; out[j].lon = lon1;
	j++;
    }

    return j;
}


double findtime(int n, TTIME *tt, double dist)
{

    int i, found;
    double ttt;

    if( ! (( dist >= tt[0].r && dist <= tt[n-1].r) || (dist <= tt[0].r && dist >= tt[n-1].r)) ){
	fprintf(stderr, "Warning:: Did not find time \n");
	return 0.0;
    } else {
	found = 0;
	for(i=0; i<n-1; i++){
	    if(found == 0 && ( (tt[i].r <= dist && tt[i+1].r >dist) || (tt[i].r >= dist && tt[i+1].r <dist) )){
		/* linear interpolation */
		ttt = tt[i].t + (tt[i+1].t - tt[i].t)/(tt[i+1].r - tt[i].r)*(dist - tt[i].r);
		found = 1;
            }
        }
	if(found == 0){
	    fprintf(stderr, "Something is wrong in findtime\n");
	    exit(-1);
        }
	return ttt;
    }
}



TTIME *ReadFile(char *file, int *total)
{
    FILE  *open_file(char *file, char *access_mode);
    FILE *fop = open_file(file,"r");

    TTIME *t1, *t2;
    int i=0;

    if( (t1 = (TTIME *) malloc (500*sizeof(TTIME))) == NULL){
	fprintf(stderr, "can not allocate memory in Readfile\n");
	exit(-1);
    }
    while(fscanf(fop,"%le %le\n", &t1[i].r, &t1[i].t) != EOF) i++;

    fclose(fop);

    *total = i;

    if( (t2 = (TTIME *) malloc (*total*sizeof(TTIME))) == NULL){
	fprintf(stderr, "can not allocate memory in Readfile\n");
	exit(-1);
    }
    for(i=0; i<*total; i++){
	t2[i].r = t1[i].r/(EARTH_RADIUS/1000);
	t2[i].t = t1[i].t;
    }


    free(t1);

    return t2;
}     
