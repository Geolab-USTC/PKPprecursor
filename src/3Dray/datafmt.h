#ifndef _DATAFMT__H_
      #define _DATAFMT__H_


struct SpeInt
{
int lon;
int lat;
};

struct Spe
{
double lon;
double lat;
};


struct Cart
{
double x;
double y;
double z;
};

struct Ttime 
{
double r;
double t;
};

struct Proj
{
double x;
double y;
};

struct Scatter 
{
double lon;
double lat;
double energy;
};

typedef struct Spe SPE;
typedef struct SpeInt SPEINT;
typedef struct Cart  CART;
typedef struct Ttime TTIME;
typedef struct Scatter SCATTER;

#endif
