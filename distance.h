/**
 * distance.h
 * GPS distance calculation using Haversine formula
 */

#ifndef DISTANCE_H
#define DISTANCE_H

/**
 * Calculate distance between two GPS coordinates using Haversine formula
 * @param lat1 Latitude of first point (degrees)
 * @param lon1 Longitude of first point (degrees)
 * @param lat2 Latitude of second point (degrees)
 * @param lon2 Longitude of second point (degrees)
 * @return Distance in kilometers
 */
double haversine_distance(double lat1, double lon1, double lat2, double lon2);

/**
 * Enhanced distance calculation including elevation difference
 * @param lat1 Latitude of first point (degrees)
 * @param lon1 Longitude of first point (degrees)
 * @param elev1 Elevation of first point (meters)
 * @param lat2 Latitude of second point (degrees)
 * @param lon2 Longitude of second point (degrees)
 * @param elev2 Elevation of second point (meters)
 * @return 3D distance in kilometers
 */
double enhanced_haversine_distance(double lat1, double lon1, double elev1,
                                  double lat2, double lon2, double elev2);

/**
 * Calculate bearing between two GPS coordinates
 * @param lat1 Latitude of first point (degrees)
 * @param lon1 Longitude of first point (degrees)
 * @param lat2 Latitude of second point (degrees)
 * @param lon2 Longitude of second point (degrees)
 * @return Bearing in degrees (0-360)
 */
double calculate_bearing(double lat1, double lon1, double lat2, double lon2);

#endif // DISTANCE_H
