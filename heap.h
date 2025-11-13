/**
 * heap.h
 * Min-heap implementation for priority queue in pathfinding algorithms
 */

#ifndef HEAP_H
#define HEAP_H

#include "gps_types.h"

/**
 * Initialize an empty min-heap
 */
void init_heap(MinHeap* heap);

/**
 * Check if heap is empty
 */
int is_empty(MinHeap* heap);

/**
 * Insert a node into the heap (for Dijkstra)
 */
void insert_heap(MinHeap* heap, int vertex, double distance);

/**
 * Insert a node into the heap (for A*)
 */
void insert_heap_astar(MinHeap* heap, int vertex, double g_cost, double h_cost, int parent);

/**
 * Extract the minimum element from the heap
 */
PQNode extract_min(MinHeap* heap);

/**
 * Update the distance of an existing node (decrease key operation)
 */
void decrease_key(MinHeap* heap, int vertex, double new_distance);

#endif // HEAP_H
