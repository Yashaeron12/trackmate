/**
 * distance.c
 * Implementation of GPS distance calculations
 */

#include <math.h>
#include "distance.h"
#include "gps_types.h"

double haversine_distance(double lat1, double lon1, double lat2, double lon2) {
    // Convert degrees to radians
    lat1 = lat1 * PI / 180.0;
    lon1 = lon1 * PI / 180.0;
    lat2 = lat2 * PI / 180.0;
    lon2 = lon2 * PI / 180.0;
    
    // Haversine formula
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;
    
    double a = sin(dlat/2) * sin(dlat/2) + 
               cos(lat1) * cos(lat2) * sin(dlon/2) * sin(dlon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    
    return EARTH_RADIUS * c;  // Distance in kilometers
}

double enhanced_haversine_distance(double lat1, double lon1, double elev1,
                                  double lat2, double lon2, double elev2) {
    // Convert to radians
    lat1 *= PI / 180.0;
    lon1 *= PI / 180.0;
    lat2 *= PI / 180.0;
    lon2 *= PI / 180.0;
    
    // Haversine calculation for horizontal distance
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;
    double a = sin(dlat/2) * sin(dlat/2) + 
               cos(lat1) * cos(lat2) * sin(dlon/2) * sin(dlon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double horizontal_dist = EARTH_RADIUS * c;
    
    // Add elevation difference (convert meters to km)
    double elevation_diff = (elev2 - elev1) / 1000.0;
    
    // 3D distance using Pythagorean theorem
    return sqrt(horizontal_dist * horizontal_dist + elevation_diff * elevation_diff);
}

double calculate_bearing(double lat1, double lon1, double lat2, double lon2) {
    // Convert to radians
    lat1 *= PI / 180.0;
    lon1 *= PI / 180.0;
    lat2 *= PI / 180.0;
    lon2 *= PI / 180.0;
    
    double dlon = lon2 - lon1;
    
    double y = sin(dlon) * cos(lat2);
    double x = cos(lat1) * sin(lat2) - 
               sin(lat1) * cos(lat2) * cos(dlon);
    
    double bearing = atan2(y, x);
    
    // Convert to degrees and normalize to 0-360
    bearing = bearing * 180.0 / PI;
    bearing = fmod((bearing + 360.0), 360.0);
    
    return bearing;
}
