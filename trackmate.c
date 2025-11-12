// TrackMate GPS Tracker - C Backend Implementation
// Implements Dijkstra's Algorithm with Haversine distance calculation
// Author: TrackMate Team
// Date: November 2025

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 20
#define MAX_EDGES 100
#define EARTH_RADIUS 6371.0  // Earth radius in kilometers
#define PI 3.14159265359
#define INF 999999.0

// GPS Location structure
typedef struct {
    int id;
    char name[50];
    double latitude;
    double longitude;
} Location;

// Edge in the graph (represents road between two locations)
typedef struct Edge {
    int destination;
    double weight;  // Distance in kilometers
    struct Edge* next;
} Edge;

// Graph node with adjacency list
typedef struct {
    Location location;
    Edge* edges;  // Adjacency list
} GraphNode;

// Priority queue node for Dijkstra's algorithm
typedef struct {
    int vertex;
    double distance;
} PQNode;

// Min-heap for Dijkstra's algorithm
typedef struct {
    PQNode nodes[MAX_NODES];
    int size;
} MinHeap;

// Global graph
GraphNode graph[MAX_NODES];
int node_count = 0;

// ===== HAVERSINE DISTANCE CALCULATION =====
// Calculate distance between two GPS coordinates using Haversine formula
double haversine_distance(double lat1, double lon1, double lat2, double lon2) {
    // Convert degrees to radians
    lat1 = lat1 * PI / 180.0;
    lon1 = lon1 * PI / 180.0;
    lat2 = lat2 * PI / 180.0;
    lon2 = lon2 * PI / 180.0;
    
    // Haversine formula
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;
    
    double a = sin(dlat/2) * sin(dlat/2) + cos(lat1) * cos(lat2) * sin(dlon/2) * sin(dlon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    
    return EARTH_RADIUS * c;  // Distance in kilometers
}

// ===== MIN-HEAP IMPLEMENTATION FOR DIJKSTRA =====
void init_heap(MinHeap* heap) {
    heap->size = 0;
}

void swap_nodes(PQNode* a, PQNode* b) {
    PQNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int index) {
    if (index && heap->nodes[index].distance < heap->nodes[(index-1)/2].distance) {
        swap_nodes(&heap->nodes[index], &heap->nodes[(index-1)/2]);
        heapify_up(heap, (index-1)/2);
    }
}

void heapify_down(MinHeap* heap, int index) {
    int smallest = index;
    int left = 2*index + 1;
    int right = 2*index + 2;
    
    if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance)
        smallest = left;
    
    if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance)
        smallest = right;
    
    if (smallest != index) {
        swap_nodes(&heap->nodes[index], &heap->nodes[smallest]);
        heapify_down(heap, smallest);
    }
}

void insert_heap(MinHeap* heap, int vertex, double distance) {
    heap->nodes[heap->size].vertex = vertex;
    heap->nodes[heap->size].distance = distance;
    heapify_up(heap, heap->size);
    heap->size++;
}

PQNode extract_min(MinHeap* heap) {
    PQNode min_node = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size-1];
    heap->size--;
    heapify_down(heap, 0);
    return min_node;
}

int is_empty(MinHeap* heap) {
    return heap->size == 0;
}

// ===== GRAPH OPERATIONS =====
void add_location(int id, const char* name, double lat, double lon) {
    graph[node_count].location.id = id;
    strcpy(graph[node_count].location.name, name);
    graph[node_count].location.latitude = lat;
    graph[node_count].location.longitude = lon;
    graph[node_count].edges = NULL;
    node_count++;
}

void add_edge(int from, int to) {
    // Calculate distance using Haversine formula
    double distance = haversine_distance(
        graph[from].location.latitude, graph[from].location.longitude,
        graph[to].location.latitude, graph[to].location.longitude
    );
    
    // Add edge from 'from' to 'to'
    Edge* new_edge = malloc(sizeof(Edge));
    new_edge->destination = to;
    new_edge->weight = distance;
    new_edge->next = graph[from].edges;
    graph[from].edges = new_edge;
    
    // Add reverse edge (bidirectional roads)
    Edge* reverse_edge = malloc(sizeof(Edge));
    reverse_edge->destination = from;
    reverse_edge->weight = distance;
    reverse_edge->next = graph[to].edges;
    graph[to].edges = reverse_edge;
}

// ===== DIJKSTRA'S ALGORITHM =====
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
    
    printf("Starting Dijkstra's algorithm from %s to %s\n", 
           graph[start].location.name, graph[end].location.name);
    
    while (!is_empty(&heap)) {
        PQNode current = extract_min(&heap);
        int u = current.vertex;
        
        if (u == end) {
            printf("Found shortest path! Total distance: %.2f km\n", distances[end]);
            break;
        }
        
        // Explore all adjacent nodes
        Edge* edge = graph[u].edges;
        while (edge != NULL) {
            int v = edge->destination;
            double alt = distances[u] + edge->weight;
            
            if (alt < distances[v]) {
                distances[v] = alt;
                previous[v] = u;
                insert_heap(&heap, v, alt);
            }
            edge = edge->next;
        }
    }
}

// ===== JSON OUTPUT GENERATION =====
void print_shortest_path_json(int start, int end, double distances[], int previous[]) {
    printf("{\n");
    printf("  \"route\": {\n");
    printf("    \"start\": {\n");
    printf("      \"id\": %d,\n", graph[start].location.id);
    printf("      \"name\": \"%s\",\n", graph[start].location.name);
    printf("      \"latitude\": %.6f,\n", graph[start].location.latitude);
    printf("      \"longitude\": %.6f\n", graph[start].location.longitude);
    printf("    },\n");
    printf("    \"end\": {\n");
    printf("      \"id\": %d,\n", graph[end].location.id);
    printf("      \"name\": \"%s\",\n", graph[end].location.name);
    printf("      \"latitude\": %.6f,\n", graph[end].location.latitude);
    printf("      \"longitude\": %.6f\n", graph[end].location.longitude);
    printf("    },\n");
    printf("    \"total_distance\": %.2f,\n", distances[end]);
    printf("    \"path\": [\n");
    
    // Reconstruct path
    int path[MAX_NODES];
    int path_length = 0;
    int current = end;
    
    while (current != -1) {
        path[path_length++] = current;
        current = previous[current];
    }
    
    // Print path in reverse order (start to end)
    for (int i = path_length - 1; i >= 0; i--) {
        int node_id = path[i];
        printf("      {\n");
        printf("        \"id\": %d,\n", graph[node_id].location.id);
        printf("        \"name\": \"%s\",\n", graph[node_id].location.name);
        printf("        \"latitude\": %.6f,\n", graph[node_id].location.latitude);
        printf("        \"longitude\": %.6f\n", graph[node_id].location.longitude);
        if (i > 0) printf("      },\n");
        else printf("      }\n");
    }
    
    printf("    ]\n");
    printf("  },\n");
    printf("  \"status\": \"success\",\n");
    printf("  \"algorithm\": \"dijkstra\",\n");
    printf("  \"timestamp\": \"2025-11-12T10:00:00Z\"\n");
    printf("}\n");
}

// ===== SAMPLE DATA INITIALIZATION =====
void initialize_sample_data() {
    // Sample locations in Mumbai area (real GPS coordinates)
    add_location(0, "Bandra Station", 19.0544, 72.8406);
    add_location(1, "Kurla Junction", 19.0728, 72.8826);
    add_location(2, "Andheri East", 19.1136, 72.8697);
    add_location(3, "Powai Lake", 19.1188, 72.9073);
    add_location(4, "Chhatrapati Shivaji Airport", 19.0896, 72.8656);
    add_location(5, "Bandra-Kurla Complex", 19.0633, 72.8682);
    add_location(6, "Mahim Junction", 19.0410, 72.8397);
    
    // Create road network (edges between locations)
    add_edge(0, 1);  // Bandra to Kurla
    add_edge(0, 5);  // Bandra to BKC
    add_edge(0, 6);  // Bandra to Mahim
    add_edge(1, 2);  // Kurla to Andheri
    add_edge(1, 5);  // Kurla to BKC
    add_edge(2, 3);  // Andheri to Powai
    add_edge(2, 4);  // Andheri to Airport
    add_edge(4, 5);  // Airport to BKC
    add_edge(5, 6);  // BKC to Mahim
    
    printf("Sample GPS network initialized with %d locations\n", node_count);
    printf("Network covers Mumbai area with real coordinates\n\n");
}

// ===== MAIN FUNCTION =====
int main() {
    printf("=== TrackMate GPS Tracker - Backend System ===\n\n");
    
    // Initialize sample GPS data
    initialize_sample_data();
    
    // Find shortest path from Bandra Station (0) to Powai Lake (3)
    int start = 0;  // Bandra Station
    int end = 3;    // Powai Lake
    
    double distances[MAX_NODES];
    int previous[MAX_NODES];
    
    printf("Computing shortest route using Dijkstra's algorithm...\n\n");
    dijkstra(start, end, distances, previous);
    
    printf("\n=== JSON OUTPUT FOR FRONTEND ===\n");
    print_shortest_path_json(start, end, distances, previous);
    
    // Save JSON to file for frontend
    freopen("route_data.json", "w", stdout);
    print_shortest_path_json(start, end, distances, previous);
    fclose(stdout);
    
    printf("\nRoute data saved to 'route_data.json'\n");
    printf("Open 'index.html' to visualize the route on the map!\n");
    
    return 0;
}

// ===== CLEANUP FUNCTION =====
void cleanup_graph() {
    for (int i = 0; i < node_count; i++) {
        Edge* edge = graph[i].edges;
        while (edge != NULL) {
            Edge* temp = edge;
            edge = edge->next;
            free(temp);
        }
    }
}