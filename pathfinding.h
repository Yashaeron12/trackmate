/**
 * pathfinding.h
 * Pathfinding algorithms (Dijkstra and A*)
 */

#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "gps_types.h"

/**
 * Find shortest path using Dijkstra's algorithm
 * @param start Starting vertex index
 * @param end Ending vertex index
 * @param distances Array to store distances to each vertex
 * @param previous Array to store previous vertex in shortest path
 */
void dijkstra(int start, int end, double distances[], int previous[]);

/**
 * Find shortest path using A* algorithm
 * @param start Starting vertex index
 * @param end Ending vertex index
 * @param path Array to store the path vertices
 * @param total_cost Pointer to store total path cost
 * @return Length of the path (number of vertices)
 */
int astar_pathfind(int start, int end, int path[], double* total_cost);

/**
 * Heuristic function for A* (straight-line distance to goal)
 */
double heuristic_distance(int from, int to);

/**
 * Reconstruct path from previous array
 * @param end End vertex
 * @param previous Array of previous vertices
 * @param path Array to store reconstructed path
 * @return Length of the path
 */
int reconstruct_path(int end, int previous[], int path[]);

#endif // PATHFINDING_H
