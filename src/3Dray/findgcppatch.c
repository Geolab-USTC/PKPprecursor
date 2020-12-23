
#include <stdio.h>
#include "datafmt.h"
#include "Spe_Cart.h"
#include "sachead.h"

double Tmark = -1;
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
    int patch(SPE pole, SPE old, char *tfile, char *dfile, double PKPt2, SACHEAD sach, double *amp, double period, int checknoise, SCATTER *out, double PKPduration);
    void NormAmp(SACHEAD sach, double *amp, double PKPduration);
    int setgrids(double dlat, double dlon, int ndata, double gcarc, SCATTER *sout, char *file);
    double tshift = -1000.0;
    double period = 1.0;      /* seconds */
    int    checknoise = 1;

    double dlon = 0.2;
    double dlat = 0.2;

    int  ndata;

    SCATTER   *sout;

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
    getpar("dlat",       "F",   &dlat); /* mapping grid */
    getpar("dlon",       "F",   &dlon); /* mapping grid */
    getpar("checknoise", "d",   &checknoise);
    getpar("tshift",     "F",   &tshift);  /* time shift with respect to theoretical PKP Arrival */
    endpar();

    amp = (double *) malloc (100000 * sizeof(double));
    np  = rsacd(seis, &sach, amp);

/* for this case only */
/*
*/

    sach.t0 = sach.t1;
    if(sach.t9 > -1000.0){
        sach.t0 = sach.t9;
    }

    if(sach.o > -10000.){
        sach.b -= sach.o;
        sach.t0 -= sach.o;
        if(sach.t2 > -10000){
            sach.t2 -= sach.o;
        }
        fprintf(stderr,"sach.o %f\n",sach.o);
        sach.o  = 0.0;
        fprintf(stderr,"reset sach.b to %f\n",sach.b);
    }
    if(sach.t2 > -10000){
        Tmark    = sach.t2;
    }
    if(tshift > -100.0){
        sach.t0 = PKPt2 + tshift;
        fprintf(stderr,"reset sach.t0 to %f\n",sach.t0);
    }

    NormAmp(sach,amp,PKPduration);

    pole.lat = sach.stla*RAD; pole.lon = sach.stlo*RAD;
    old.lat  = sach.evla*RAD; old.lon  = sach.evlo*RAD;

    sout = (SCATTER *) malloc (1000000*sizeof(SCATTER));

/*
    fprintf(stderr,"pass 1\n");
*/
    ndata = patch(pole, old, tr2sf, ds2rf, PKPt2, sach, amp, period, checknoise, sout, PKPduration);
/*
    fprintf(stderr,"pass 2\n");
*/
    setgrids(dlat, dlon, ndata, sach.gcarc, sout, osf);

    ndata = patch(old, pole, ts2rf, dr2sf, PKPt2, sach, amp, period, checknoise, sout, PKPduration);
    setgrids(dlat, dlon, ndata, sach.gcarc, sout, orf);

    free(amp); free(sout);
    return 1;

}

int patch(SPE pole, SPE old, char *tfile, char *dfile, double PKPt2, SACHEAD sach, double *amp, double period, int checknoise, SCATTER *sout, double PKPduration)
{
    SPE pnew, *out;
    TTIME *tt, *td;
    int ntt, ntd;
    TTIME *ReadFile(char *file, int *n);
    double delta(double lat1, double lon1, double lat2, double lon2);
    double findtime(int n, TTIME *tt, double dist);
    int isotime(int *ntt, TTIME *tt, int ntd, TTIME *td, double PKPt, SPE pnew, SPE *out);

    int i, j, k;
    int ID = 0;
    double dist;
    double lat1, lon1, lat2, lon2;
    double t1, t2, t, dt;
    int nn, nn1, nn2, nt1, nsample;  
    double Energy(double *amp, int n1, int n2, int average);
    double energy = 0.0;
    double energy1;
    SPE trans(SPE pole, SPE old, int reverse);

    double PKPt1 = 0.0;
    FILE  *open_file(char *file, char *access_mode);
    double *pamp;
    SCATTER *psout = sout;

    out  = (SPE *) malloc (50000 * sizeof(SPE));

    nsample = (int) (period / sach.delta);
    if(nsample < 0){
        fprintf(stderr, "error in specifing period, it must be > delta \n");
        exit(-1);
    }

    pamp = amp + (int) ((sach.t0 - sach.b)/sach.delta);  /* picktime of PKPdf */

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
    energy  = Energy(pamp,nn,0,1);
    energy1 = Energy(pamp,2*nn,nn,1);

    if(energy /energy1 < RATIO && energy > NOISELEVEL && checknoise == 1)
	/* noisy level */
	energy = -1;

    dt = nsample * sach.delta;

    if(Tmark > 0) nsample = 2 * (int) (PKPduration/sach.delta);
    for(t = PKPt2; t >= PKPt1; t-=dt){

	nn1 = (int) ((t - PKPt2)/sach.delta) - 1;

/*
	if(nn1 == -1) nn1 = -2;
        nn2 = nn1+1;
*/
        nn2 = nn1 + nsample;
        if(nn2 >= 0) nn2 = -1;

        if(energy > 0)
	    energy =  Energy(pamp, nn1, nn2,0);
        else
	    energy = -Energy(pamp, nn1, nn2,0);

    
        energy = ( Tmark < 0 || (Tmark > 0 && t >= Tmark && t < Tmark+dt)) ? energy : 0.000001; 

        nt1 = isotime(&ntt, tt, ntd, td, t, pnew, out);

/*
        fprintf(stderr,"PKpt1=%lf %lf %lf %d energy=%lf\n",PKPt2, PKPt1, t, nt1, energy);
*/

        for(k=0; k<nt1; k++){
            old.lat = out[k].lat;
            old.lon = out[k].lon;
            old = trans(pole,old,-1);
            psout->lat    = old.lat * DAR;
            psout->lon    = old.lon * DAR;
            psout->energy = energy;
            psout++; 
        }
        for(k=nt1-1; k>=0; k--){
            old.lat = out[k].lat;
            old.lon = 2.0*pnew.lon - out[k].lon;
            old = trans(pole,old,-1);
            psout->lat    = old.lat * DAR;
            psout->lon    = old.lon * DAR;
            psout->energy = energy;
            psout++; 
        }
	ID++;
    }

    free(tt); free(td); free(out);
    return psout - sout;
}

double Energy(double *amp, int n1, int n2, int average)
{
    int i;
    double energy = 0.0;
    if(average == 1){
        for(i = n1; i < n2; i++)
	    energy += amp[i];
        energy /= (n2-n1);
    } else {
        for(i = n1; i < n2; i++)
	    energy = (amp[i] > energy) ? amp[i] : energy;
    }
    return energy;
}


int isotime(int *ntt, TTIME *tt, int ntd, TTIME *td, double PKPt, SPE pnew, SPE* out)
{
     int i, j, k, ntt0;
     double lon1, lon2, lat1, lat2, lat3, lon3, t1, t2, t3, dist;
     double delta(double lat1, double lon1, double lat2, double lon2);
     double findtime(int n, TTIME *tt, double dist);
     int num = 10;

     j = 0;
     for(i = 0; i< *ntt; i++){

	lat1 = 0.5*PI-tt[i].r; /* convert distance to lat */ 
	lon1 = pnew.lon; 
        t1   = tt[i].t;
        dist = delta(lat1,lon1,pnew.lat,pnew.lon);
        t2   = findtime(ntd, td, dist);   /* earliest arrival at lat1*/	

	if(t2 > 0.1){  /* in the list */
	    t2 += t1;
	    if( t2 < PKPt){

                lon2 = lon1 + 10.0*RAD;
                while ( (t1 + findtime(ntd,td,delta(lat1,lon2,pnew.lat,pnew.lon)))<=PKPt) lon2 += 10.0*RAD;

                while( (lon2 -lon1) > TINY){ /* search for the cross-point */
                    dist = delta(lat1,0.5*(lon2+lon1),pnew.lat,pnew.lon);	
                    t3 = findtime(ntd, td, dist);	
		    if(t3 > 0) 
			t3 += t1;
                    else 
			fprintf(stderr, "not enough points\n");
                   
		    if( t3 < PKPt)
			lon1 = 0.5*(lon2+lon1);
	            else 
			lon2 = 0.5*(lon2+lon1);
                }
                if ( j == 0){
		    out[j].lat = lat1; out[j].lon = lon1;
		    j++;
                } else {
                    lat3 = out[j-1].lat; lon3 = out[j-1].lon;
                    for(k = 0; k < num; k++){
	                out[j].lat = lat3 + k*(lat1 - lat3)/(num -1); 
                        out[j].lon = lon3 + k*(lon1 - lon3)/(num -1);
	                j++;
                    }
                }
/*
		    out[j].lat = lat1; out[j].lon = lon1;
*/
                ntt0 = i;
            } else if (fabs(t2-PKPt) < 1.e-3){
                if ( j == 0){
		    out[j].lat = lat1; out[j].lon = lon1;
		    j++;
                } else {
                    lat3 = out[j-1].lat; lon3 = out[j-1].lon;
                    for(k = 0; k < num; k++){
	                out[j].lat = lat3 + k*(lat1 - lat3)/(num -1); 
                        out[j].lon = lon3 + k*(lon1 - lon3)/(num -1);
	                j++;
                    }
                }
/*
		    out[j].lat = lat1; out[j].lon = lon1;
*/
                ntt0 = i;
            } else {
/*
		fprintf(stderr, "Great circle not in the time window i=%d t2=%lf\n",i,t2);
*/
            }

        } else {
	    fprintf(stderr, "Direct ray not in the time window\n");
        } 
    }
    if(j > 0){
        /* comment out */
        /* search for the point at the great circle */
	lat1 = 0.5*PI-tt[ntt0+1].r; /* convert distance to lat */ 
	lat2 = 0.5*PI-tt[ntt0].r; /* convert distance to lat */ 
        lon1 = pnew.lon; 

	while( (lat2 - lat1) > TINY){
	    dist = delta(0.5*(lat2+lat1), lon1, pnew.lat, pnew.lon);
	    t3  = findtime(ntd,td,dist);
	    t1  = findtime(*ntt,tt,0.5*PI-0.5*(lat2+lat1));
            if(t3 > 0 && t1 > 0)
	        t3 += t1;
            else
		fprintf(stderr, "not enough points t1=%lf t3=%lf %lf %lf\n",t1,t3,lat1/RAD, lat2/RAD);
            if( t3 <= PKPt)
		lat2 = 0.5*(lat2+lat1);
            else
		lat1 = 0.5*(lat2+lat1);
        }

        /* do interpolation between last two points */
        lat2 = out[j-1].lat; lon2 = out[j-1].lon;
        
        for(i = 0; i < num; i++){
	    out[j].lat = lat2 + i*(lat1 - lat2)/(num -1); 
            out[j].lon = lon2 + i*(lon1 - lon2)/(num -1);
	    j++;
            ntt0 ++;
        }
    }


/*
    *ntt = ntt0+2;
*/

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

    if( (t1 = (TTIME *) malloc (5000*sizeof(TTIME))) == NULL){
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
