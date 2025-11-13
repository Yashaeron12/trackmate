/**
 * graph.c
 * Graph operations implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "distance.h"

// Global graph
GraphNode graph[MAX_NODES];
int node_count = 0;

void init_graph(void) {
    node_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        graph[i].edges = NULL;
        graph[i].is_active = 0;
    }
}

void add_location(int id, const char* name, double lat, double lon) {
    graph[node_count].location.id = id;
    strncpy(graph[node_count].location.name, name, sizeof(graph[node_count].location.name) - 1);
    graph[node_count].location.name[sizeof(graph[node_count].location.name) - 1] = '\0';
    strcpy(graph[node_count].location.type, "general");
    strcpy(graph[node_count].location.district, "unknown");
    graph[node_count].location.latitude = lat;
    graph[node_count].location.longitude = lon;
    graph[node_count].location.elevation = 0.0;
    graph[node_count].location.traffic_level = 1;
    graph[node_count].edges = NULL;
    graph[node_count].is_active = 1;
    node_count++;
}

void add_enhanced_location(int id, const char* name, const char* type, 
                          const char* district, double lat, double lon, 
                          double elev, int traffic) {
    graph[node_count].location.id = id;
    strncpy(graph[node_count].location.name, name, sizeof(graph[node_count].location.name) - 1);
    graph[node_count].location.name[sizeof(graph[node_count].location.name) - 1] = '\0';
    strncpy(graph[node_count].location.type, type, sizeof(graph[node_count].location.type) - 1);
    graph[node_count].location.type[sizeof(graph[node_count].location.type) - 1] = '\0';
    strncpy(graph[node_count].location.district, district, sizeof(graph[node_count].location.district) - 1);
    graph[node_count].location.district[sizeof(graph[node_count].location.district) - 1] = '\0';
    graph[node_count].location.latitude = lat;
    graph[node_count].location.longitude = lon;
    graph[node_count].location.elevation = elev;
    graph[node_count].location.traffic_level = traffic;
    graph[node_count].edges = NULL;
    graph[node_count].is_active = 1;
    node_count++;
}

void add_edge(int from, int to) {
    // Calculate distance using Haversine formula
    double distance = haversine_distance(
        graph[from].location.latitude, graph[from].location.longitude,
        graph[to].location.latitude, graph[to].location.longitude
    );
    
    // Add forward edge
    Edge* new_edge = malloc(sizeof(Edge));
    new_edge->destination = to;
    new_edge->base_distance = distance;
    new_edge->current_weight = distance;
    strcpy(new_edge->road_type, "main");
    new_edge->traffic_factor = 1;
    new_edge->speed_limit = 50.0;
    new_edge->next = graph[from].edges;
    graph[from].edges = new_edge;
    
    // Add reverse edge (bidirectional)
    Edge* reverse_edge = malloc(sizeof(Edge));
    reverse_edge->destination = from;
    reverse_edge->base_distance = distance;
    reverse_edge->current_weight = distance;
    strcpy(reverse_edge->road_type, "main");
    reverse_edge->traffic_factor = 1;
    reverse_edge->speed_limit = 50.0;
    reverse_edge->next = graph[to].edges;
    graph[to].edges = reverse_edge;
}

void add_enhanced_edge(int from, int to, const char* road_type, 
                      int traffic_factor, double speed_limit) {
    double distance = enhanced_haversine_distance(
        graph[from].location.latitude, graph[from].location.longitude, 
        graph[from].location.elevation,
        graph[to].location.latitude, graph[to].location.longitude, 
        graph[to].location.elevation
    );
    
    // Forward edge
    Edge* new_edge = malloc(sizeof(Edge));
    new_edge->destination = to;
    new_edge->base_distance = distance;
    new_edge->current_weight = distance;
    strncpy(new_edge->road_type, road_type, sizeof(new_edge->road_type) - 1);
    new_edge->road_type[sizeof(new_edge->road_type) - 1] = '\0';
    new_edge->traffic_factor = traffic_factor;
    new_edge->speed_limit = speed_limit;
    new_edge->next = graph[from].edges;
    graph[from].edges = new_edge;
    
    // Reverse edge
    Edge* reverse_edge = malloc(sizeof(Edge));
    reverse_edge->destination = from;
    reverse_edge->base_distance = distance;
    reverse_edge->current_weight = distance;
    strncpy(reverse_edge->road_type, road_type, sizeof(reverse_edge->road_type) - 1);
    reverse_edge->road_type[sizeof(reverse_edge->road_type) - 1] = '\0';
    reverse_edge->traffic_factor = traffic_factor;
    reverse_edge->speed_limit = speed_limit;
    reverse_edge->next = graph[to].edges;
    graph[to].edges = reverse_edge;
}

Location* get_location(int id) {
    if (id >= 0 && id < node_count) {
        return &graph[id].location;
    }
    return NULL;
}

int find_location_by_name(const char* name) {
    for (int i = 0; i < node_count; i++) {
        if (strcmp(graph[i].location.name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void print_graph_stats(void) {
    printf("Graph Statistics:\n");
    printf("  Total Nodes: %d\n", node_count);
    
    int total_edges = 0;
    for (int i = 0; i < node_count; i++) {
        Edge* edge = graph[i].edges;
        while (edge != NULL) {
            total_edges++;
            edge = edge->next;
        }
    }
    printf("  Total Edges: %d (bidirectional counted once: %d)\n", 
           total_edges, total_edges / 2);
}

void cleanup_graph(void) {
    for (int i = 0; i < node_count; i++) {
        Edge* edge = graph[i].edges;
        while (edge != NULL) {
            Edge* temp = edge;
            edge = edge->next;
            free(temp);
        }
        graph[i].edges = NULL;
    }
    node_count = 0;
}
