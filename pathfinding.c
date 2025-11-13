/**
 * pathfinding.c
 * Implementation of Dijkstra and A* algorithms
 */

#include <stdio.h>
#include <time.h>
#include "pathfinding.h"
#include "graph.h"
#include "heap.h"
#include "distance.h"

void dijkstra(int start, int end, double distances[], int previous[]) {
    MinHeap heap;
    init_heap(&heap);
    
    // Initialize distances and previous nodes
    for (int i = 0; i < node_count; i++) {
        distances[i] = INF;
        previous[i] = -1;
    }
    
    distances[start] = 0.0;
    insert_heap(&heap, start, 0.0);
    
    printf("Starting Dijkstra's algorithm: %s → %s\n", 
           graph[start].location.name, graph[end].location.name);
    
    int nodes_explored = 0;
    
    while (!is_empty(&heap)) {
        PQNode current = extract_min(&heap);
        int u = current.vertex;
        nodes_explored++;
        
        if (u == end) {
            printf("✅ Found shortest path! Distance: %.2f km\n", distances[end]);
            printf("   Nodes explored: %d\n", nodes_explored);
            break;
        }
        
        // Skip if we've already found a better path
        if (current.distance > distances[u]) {
            continue;
        }
        
        // Explore all adjacent nodes
        Edge* edge = graph[u].edges;
        while (edge != NULL) {
            int v = edge->destination;
            double alt = distances[u] + edge->current_weight;
            
            if (alt < distances[v]) {
                distances[v] = alt;
                previous[v] = u;
                insert_heap(&heap, v, alt);
            }
            edge = edge->next;
        }
    }
}

double heuristic_distance(int from, int to) {
    return enhanced_haversine_distance(
        graph[from].location.latitude, 
        graph[from].location.longitude, 
        graph[from].location.elevation,
        graph[to].location.latitude, 
        graph[to].location.longitude, 
        graph[to].location.elevation
    );
}

int astar_pathfind(int start, int end, int path[], double* total_cost) {
    clock_t start_time = clock();
    
    MinHeap open_set;
    init_heap(&open_set);
    
    double g_costs[MAX_NODES];
    int parents[MAX_NODES];
    int in_closed_set[MAX_NODES] = {0};
    
    for (int i = 0; i < node_count; i++) {
        g_costs[i] = INF;
        parents[i] = -1;
    }
    
    g_costs[start] = 0;
    double h_start = heuristic_distance(start, end);
    insert_heap_astar(&open_set, start, 0, h_start, -1);
    
    printf("Starting A* algorithm: %s → %s\n", 
           graph[start].location.name, graph[end].location.name);
    
    int nodes_explored = 0;
    
    while (open_set.size > 0) {
        PQNode current = extract_min(&open_set);
        int u = current.vertex;
        nodes_explored++;
        
        if (u == end) {
            // Reconstruct path
            int path_length = 0;
            int current_node = end;
            while (current_node != -1) {
                path[path_length++] = current_node;
                current_node = parents[current_node];
            }
            
            // Reverse path
            for (int i = 0; i < path_length / 2; i++) {
                int temp = path[i];
                path[i] = path[path_length - 1 - i];
                path[path_length - 1 - i] = temp;
            }
            
            *total_cost = g_costs[end];
            
            clock_t end_time = clock();
            double calc_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            
            printf("✅ A* completed! Distance: %.2f km\n", *total_cost);
            printf("   Nodes explored: %d, Time: %.4fs\n", nodes_explored, calc_time);
            
            return path_length;
        }
        
        in_closed_set[u] = 1;
        
        // Explore neighbors
        Edge* edge = graph[u].edges;
        while (edge != NULL) {
            int v = edge->destination;
            
            if (!in_closed_set[v] && graph[v].is_active) {
                double tentative_g = g_costs[u] + edge->current_weight;
                
                if (tentative_g < g_costs[v]) {
                    g_costs[v] = tentative_g;
                    parents[v] = u;
                    double h = heuristic_distance(v, end);
                    insert_heap_astar(&open_set, v, tentative_g, h, u);
                }
            }
            edge = edge->next;
        }
    }
    
    printf("❌ No path found!\n");
    return 0;
}

int reconstruct_path(int end, int previous[], int path[]) {
    int path_length = 0;
    int current = end;
    
    while (current != -1) {
        path[path_length++] = current;
        current = previous[current];
    }
    
    // Reverse path
    for (int i = 0; i < path_length / 2; i++) {
        int temp = path[i];
        path[i] = path[path_length - 1 - i];
        path[path_length - 1 - i] = temp;
    }
    
    return path_length;
}
