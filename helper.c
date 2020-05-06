#include <math.h>
#include "helper.h"

#define PI 3.14159265358979323846

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts decimal degrees to radians             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg2rad(double deg) {
    return (deg * PI / 180);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts radians to decimal degrees             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double rad2deg(double rad) {
    return (rad * 180 / PI);
}

// Unit 'K' is kilometers (default)
// Unit 'M' is statute miles
// 'N' is nautical miles
// https://www.geodatasource.com/developers/c
double distance(struct coord coord1, struct coord coord2, char unit) {
    double theta, dist;
    if ((coord1.lat == coord2.lat) && (coord1.lon == coord2.lon)) {
        return 0;
    }
    else {
        theta = coord1.lon - coord2.lon;
        dist = sin(deg2rad(coord1.lat)) * sin(deg2rad(coord2.lat)) + cos(deg2rad(coord1.lat)) * cos(deg2rad(coord2.lat)) * cos(deg2rad(theta));
        dist = acos(dist);
        dist = rad2deg(dist);
        dist = dist * 60 * 1.853159616;
        switch(unit) {
            case 'M':
                dist = dist / 1.609344;
                break;
            case 'N':
                dist = dist * 0.53959874;
                break;
            case 'K':
            default:
                break;
        }
        return (dist);
    }
}