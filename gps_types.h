/**
 * gps_types.h
 * Core data structures and constants for TrackMate GPS Tracker
 */

#ifndef GPS_TYPES_H
#define GPS_TYPES_H

// Constants
#define MAX_NODES 50
#define MAX_EDGES 200
#define EARTH_RADIUS 6371.0  // Earth radius in kilometers
#define PI 3.14159265359
#define INF 999999.0

// GPS Location structure
typedef struct {
    int id;
    char name[50];
    char type[20];      // "station", "airport", "landmark", etc.
    double latitude;
    double longitude;
    double elevation;   // Height above sea level in meters
    int traffic_level;  // 1-5 scale (1=low, 5=high)
    char district[30];
} Location;

// Edge in the graph (represents road between two locations)
typedef struct Edge {
    int destination;
    double base_distance;    // Physical distance in km
    double current_weight;   // Dynamic weight considering traffic
    char road_type[20];      // "highway", "main", "local"
    int traffic_factor;      // Current traffic multiplier (1-3)
    double speed_limit;      // Speed limit in km/h
    struct Edge* next;
} Edge;

// Graph node with adjacency list
typedef struct {
    Location location;
    Edge* edges;
    int is_active;  // For dynamic graph updates
} GraphNode;

// Priority queue node for pathfinding algorithms
typedef struct {
    int vertex;
    double distance;
    double g_cost;  // Actual cost from start (A* specific)
    double h_cost;  // Heuristic cost to goal (A* specific)
    double f_cost;  // Total cost = g + h (A* specific)
    int parent;     // Previous vertex in path
} PQNode;

// Min-heap for Dijkstra's and A* algorithms
typedef struct {
    PQNode nodes[MAX_NODES];
    int size;
} MinHeap;

// Route statistics
typedef struct {
    double total_distance;
    double estimated_time;
    int waypoint_count;
    char algorithm_used[20];
    double avg_speed;
    char traffic_condition[20];
    long timestamp;
} RouteStats;

#endif // GPS_TYPES_H
