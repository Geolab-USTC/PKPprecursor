#include <stdio.h>
#include "sachead.h"
#include <fcntl.h>
int rsac(char *name,SACHEAD *h,float *amp)
{
  int in;
  size_t size;
  register int i;

  if( name==NULL || name[0]=='\0')
    in=fileno(stdin);
  else
    in=open(name,O_RDONLY | O_EXCL );

  if( in < 0 ){
    fprintf(stderr,"Open file `%s' failure.\n",name);
    return(-1);
  }

  size=sizeof(*h);
  if(h==NULL){
    if((h = malloc(size))==NULL){
      fprintf(stderr,"\nrsac:error:h, malloc memory error!!\n");
      return(-1);
    } 
  }
  read(in,(char *) h,size);
  if(h->leven!=1){
    fprintf(stderr,"\nrsac:error:Data are not evenly distributed\n");
    return(-1);
  }
  size=sizeof(float)*h->npts;
  if(amp==NULL){ 
    if( (amp = malloc(size))==NULL) {
      fprintf(stderr,"rsac:error:malloc memory error!!\n");
      return(-1);
    } 
  }
  read(in,(char *) amp, size);
  if(in!=fileno(stdin) ) close(in);
  return(h->npts);
} 

#define PERM 0644
int wsac(char *name,SACHEAD *h,float *amp)
{
  register int i;
  size_t size;
  int out;

  if(name==NULL || name[0]=='\0'){
    out=fileno(stdout);
  }
  else  
    out=creat(name,PERM );
  if(out<0)  {
    fprintf(stderr,"\nwsac:error:Open file `%s' failure.\n",name);
    return(-1);
  }
  if(h->leven!=1) {
    fprintf(stderr,"\nwsac:error:Data are not evenly distributed\n");
    return(-1);
  }
  size=sizeof(*h);
  write(out,(char *) h,size);
  size=sizeof(float)*h->npts;
  write(out,(char *) amp, size);
  if(out != fileno(stdout) ) close(out);
  return(h->npts);
} 
   

int rsacd(char *name,SACHEAD *h,double *amp){
  float *sacamp;
  int in;
  size_t size;
  register int i;

  if( name==NULL || name[0]=='\0')
    in=fileno(stdin);
  else
    in=open(name,O_RDONLY | O_EXCL );

  if( in < 0 ){
    fprintf(stderr,"Open file `%s' failure.\n",name);
    return(-1);
  }

  size=sizeof(*h);
  if(h==NULL){
    if((h = malloc(size))==NULL){
      fprintf(stderr,"\nrsacd:error:h, malloc memory error!!\n");
      return(-1);
    } 
  }
  read(in,(char *) h,size);
  if(h->leven!=1){
    fprintf(stderr,"\nrsacd:error:Data are not evenly distributed\n");
    return(-1);
  }
  size=sizeof(float)*h->npts;
  if( (sacamp = malloc(size))==NULL) {
    fprintf(stderr,"rsacd:error:malloc memory error!!\n");
    return(-1);
  } 
  read(in,(char *) sacamp, size);
  for(i=0;i<h->npts;i++) amp[i]=sacamp[i];
  free(sacamp);
  if(in!=fileno(stdin) ) close(in);
  return(h->npts);
}

int wsacd(char *name,SACHEAD *h,double *amp){
  register int i;
  float *sacamp;
  size_t size;
  int out;

  if(name==NULL || name[0]=='\0'){
    out=fileno(stdout);
  }
  else  
    out=creat(name,PERM );
  if(out<0)  {
    fprintf(stderr,"\nwsacd:error:Open file `%s' failure.\n",name);
    return(-1);
  }
  if(h->leven!=1) {
    fprintf(stderr,"\nwsacd:error:Data are not evenly distributed\n");
    return(-1);
  }
  size=sizeof(*h);
  write(out,(char *) h,size);
  size=sizeof(float)*h->npts;
  if((sacamp = malloc(size))==NULL){
    fprintf(stderr,"wsacd:error:malloc memory error!!\n");
    return(-1);
  } 
  for(i=0;i<h->npts;i++) sacamp[i]=amp[i];
  write(out,(char *) sacamp, size);
  free(sacamp);
  if(out != fileno(stdout) ) close(out);
  return(h->npts);
} 
