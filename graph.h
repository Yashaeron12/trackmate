/**
 * graph.h
 * Graph operations for GPS network management
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "gps_types.h"

// Global graph instance
extern GraphNode graph[MAX_NODES];
extern int node_count;

/**
 * Initialize the graph
 */
void init_graph(void);

/**
 * Add a location to the graph (simple version)
 */
void add_location(int id, const char* name, double lat, double lon);

/**
 * Add an enhanced location with additional metadata
 */
void add_enhanced_location(int id, const char* name, const char* type, 
                          const char* district, double lat, double lon, 
                          double elev, int traffic);

/**
 * Add a bidirectional edge between two locations (simple version)
 */
void add_edge(int from, int to);

/**
 * Add an enhanced bidirectional edge with road information
 */
void add_enhanced_edge(int from, int to, const char* road_type, 
                      int traffic_factor, double speed_limit);

/**
 * Get location by ID
 */
Location* get_location(int id);

/**
 * Find location by name
 */
int find_location_by_name(const char* name);

/**
 * Print graph statistics
 */
void print_graph_stats(void);

/**
 * Clean up graph memory
 */
void cleanup_graph(void);

#endif // GRAPH_H
