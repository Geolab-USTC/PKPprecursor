#include <math.h>

double delta(double lat1, double lon1, double lat2, double lon2)
{
    return (acos(sin(lat1)*sin(lat2) +cos(lat1)*cos(lat2)*cos(lon2-lon1)));
}
