#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ezxml.h"

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
double distance(double lat1, double lon1, double lat2, double lon2, char unit) {
    double theta, dist;
    if ((lat1 == lat2) && (lon1 == lon2)) {
        return 0;
    }
    else {
        theta = lon1 - lon2;
        dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
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


int main()
{
    ezxml_t xml = ezxml_parse_file("history-2020-03-20.xml"), placemark, point, line_string;
    xml = ezxml_child(xml, "Document");
    for (placemark = ezxml_child(xml, "Placemark"); placemark; placemark = placemark->next){
        for (point = ezxml_child(placemark, "Point"); point; point = point->next){
            printf("%s\n", ezxml_child(point, "coordinates")->txt);
        }
        for (line_string = ezxml_child(placemark, "LineString"); line_string; line_string = line_string->next){
            printf("%s\n", ezxml_child(line_string, "coordinates")->txt);
        }
    }
    ezxml_free(xml);
    return 0;
}