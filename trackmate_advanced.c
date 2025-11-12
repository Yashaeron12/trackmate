// trackmate_advanced.c - Enhanced TrackMate with A* Algorithm and Multiple Features
// Advanced GPS routing system with A*, multiple algorithms, and real-time optimization
// Author: TrackMate Advanced Team

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAX_NODES 50
#define MAX_EDGES 200
#define EARTH_RADIUS 6371.0
#define PI 3.14159265359
#define INF 999999.0

// Enhanced Location structure with additional metadata
typedef struct {
    int id;
    char name[50];
    char type[20];  // "station", "airport", "landmark", etc.
    double latitude;
    double longitude;
    double elevation;  // Height above sea level
    int traffic_level; // 1-5 scale
    char district[30];
} Location;

// Enhanced Edge with traffic and road type information
typedef struct Edge {
    int destination;
    double base_distance;    // Physical distance
    double current_weight;   // Dynamic weight with traffic
    char road_type[20];      // "highway", "main", "local"
    int traffic_factor;      // Current traffic multiplier (1-3)
    double speed_limit;      // Speed limit in km/h
    struct Edge* next;
} Edge;

// Enhanced Graph node
typedef struct {
    Location location;
    Edge* edges;
    int is_active;  // For dynamic graph updates
} GraphNode;

// A* Algorithm node
typedef struct {
    int vertex;
    double g_cost;  // Actual cost from start
    double h_cost;  // Heuristic cost to goal
    double f_cost;  // g_cost + h_cost
    int parent;
} AStarNode;

// Priority queue for A*
typedef struct {
    AStarNode nodes[MAX_NODES];
    int size;
} AStarQueue;

// Route statistics
typedef struct {
    double total_distance;
    double estimated_time;
    int waypoint_count;
    char algorithm_used[20];
    double avg_speed;
    char traffic_condition[20];
    time_t calculation_time;
} RouteStats;

// Global variables
GraphNode graph[MAX_NODES];
int node_count = 0;
RouteStats last_route_stats;

// ===== ADVANCED HAVERSINE WITH ELEVATION =====
double enhanced_haversine_distance(double lat1, double lon1, double elev1,
                                 double lat2, double lon2, double elev2) {
    // Convert to radians
    lat1 *= PI / 180.0;
    lon1 *= PI / 180.0;
    lat2 *= PI / 180.0;
    lon2 *= PI / 180.0;
    
    // Haversine calculation
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;
    double a = sin(dlat/2) * sin(dlat/2) + cos(lat1) * cos(lat2) * sin(dlon/2) * sin(dlon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double horizontal_dist = EARTH_RADIUS * c;
    
    // Add elevation difference (3D distance)
    double elevation_diff = (elev2 - elev1) / 1000.0; // Convert to km
    return sqrt(horizontal_dist * horizontal_dist + elevation_diff * elevation_diff);
}

// ===== TRAFFIC-AWARE DYNAMIC WEIGHTS =====
double calculate_dynamic_weight(Edge* edge, int current_hour) {
    double base_weight = edge->base_distance;
    double traffic_multiplier = 1.0;
    
    // Rush hour traffic simulation (7-9 AM, 6-8 PM)
    if ((current_hour >= 7 && current_hour <= 9) || (current_hour >= 18 && current_hour <= 20)) {
        traffic_multiplier = 1.5 + (edge->traffic_factor * 0.3);
    } else if (current_hour >= 22 || current_hour <= 6) {
        traffic_multiplier = 0.8;  // Night time - less traffic
    }
    
    // Road type factor
    if (strcmp(edge->road_type, "highway") == 0) {
        traffic_multiplier *= 0.8;  // Highways are faster
    } else if (strcmp(edge->road_type, "local") == 0) {
        traffic_multiplier *= 1.2;  // Local roads are slower
    }
    
    return base_weight * traffic_multiplier;
}

// ===== A* ALGORITHM IMPLEMENTATION =====
void init_astar_queue(AStarQueue* queue) {
    queue->size = 0;
}

void astar_swap_nodes(AStarNode* a, AStarNode* b) {
    AStarNode temp = *a;
    *a = *b;
    *b = temp;
}

void astar_heapify_up(AStarQueue* queue, int index) {
    if (index && queue->nodes[index].f_cost < queue->nodes[(index-1)/2].f_cost) {
        astar_swap_nodes(&queue->nodes[index], &queue->nodes[(index-1)/2]);
        astar_heapify_up(queue, (index-1)/2);
    }
}

void astar_heapify_down(AStarQueue* queue, int index) {
    int smallest = index;
    int left = 2*index + 1;
    int right = 2*index + 2;
    
    if (left < queue->size && queue->nodes[left].f_cost < queue->nodes[smallest].f_cost)
        smallest = left;
    if (right < queue->size && queue->nodes[right].f_cost < queue->nodes[smallest].f_cost)
        smallest = right;
    
    if (smallest != index) {
        astar_swap_nodes(&queue->nodes[index], &queue->nodes[smallest]);
        astar_heapify_down(queue, smallest);
    }
}

void astar_insert(AStarQueue* queue, int vertex, double g, double h, int parent) {
    queue->nodes[queue->size].vertex = vertex;
    queue->nodes[queue->size].g_cost = g;
    queue->nodes[queue->size].h_cost = h;
    queue->nodes[queue->size].f_cost = g + h;
    queue->nodes[queue->size].parent = parent;
    astar_heapify_up(queue, queue->size);
    queue->size++;
}

AStarNode astar_extract_min(AStarQueue* queue) {
    AStarNode min_node = queue->nodes[0];
    queue->nodes[0] = queue->nodes[queue->size-1];
    queue->size--;
    astar_heapify_down(queue, 0);
    return min_node;
}

double heuristic_distance(int from, int to) {
    return enhanced_haversine_distance(
        graph[from].location.latitude, graph[from].location.longitude, graph[from].location.elevation,
        graph[to].location.latitude, graph[to].location.longitude, graph[to].location.elevation
    );
}

// ===== A* PATHFINDING =====
int astar_pathfind(int start, int goal, int path[], double* total_cost) {
    clock_t start_time = clock();
    
    AStarQueue open_set;
    init_astar_queue(&open_set);
    
    double g_costs[MAX_NODES];
    int parents[MAX_NODES];
    int in_closed_set[MAX_NODES] = {0};
    
    for (int i = 0; i < node_count; i++) {
        g_costs[i] = INF;
        parents[i] = -1;
    }
    
    g_costs[start] = 0;
    double h_start = heuristic_distance(start, goal);
    astar_insert(&open_set, start, 0, h_start, -1);
    
    printf("🔍 A* Algorithm: Searching from %s to %s\n", 
           graph[start].location.name, graph[goal].location.name);
    
    int nodes_explored = 0;
    
    while (open_set.size > 0) {
        AStarNode current = astar_extract_min(&open_set);
        int u = current.vertex;
        nodes_explored++;
        
        if (u == goal) {
            // Reconstruct path
            int path_length = 0;
            int current_node = goal;
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
            
            *total_cost = g_costs[goal];
            
            clock_t end_time = clock();
            double calc_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            
            printf("✅ A* completed! Nodes explored: %d, Time: %.4fs\n", nodes_explored, calc_time);
            
            // Update statistics
            last_route_stats.total_distance = *total_cost;
            last_route_stats.waypoint_count = path_length;
            strcpy(last_route_stats.algorithm_used, "A*");
            last_route_stats.calculation_time = time(NULL);
            
            return path_length;
        }
        
        in_closed_set[u] = 1;
        
        // Explore neighbors
        Edge* edge = graph[u].edges;
        while (edge != NULL) {
            int v = edge->destination;
            
            if (!in_closed_set[v] && graph[v].is_active) {
                time_t now = time(NULL);
                struct tm* local_time = localtime(&now);
                double dynamic_weight = calculate_dynamic_weight(edge, local_time->tm_hour);
                
                double tentative_g = g_costs[u] + dynamic_weight;
                
                if (tentative_g < g_costs[v]) {
                    g_costs[v] = tentative_g;
                    parents[v] = u;
                    double h = heuristic_distance(v, goal);
                    astar_insert(&open_set, v, tentative_g, h, u);
                }
            }
            edge = edge->next;
        }
    }
    
    printf("❌ No path found!\n");
    return 0;
}

// ===== ENHANCED DATA INITIALIZATION =====
void add_enhanced_location(int id, const char* name, const char* type, const char* district,
                          double lat, double lon, double elev, int traffic) {
    graph[node_count].location.id = id;
    strcpy(graph[node_count].location.name, name);
    strcpy(graph[node_count].location.type, type);
    strcpy(graph[node_count].location.district, district);
    graph[node_count].location.latitude = lat;
    graph[node_count].location.longitude = lon;
    graph[node_count].location.elevation = elev;
    graph[node_count].location.traffic_level = traffic;
    graph[node_count].edges = NULL;
    graph[node_count].is_active = 1;
    node_count++;
}

void add_enhanced_edge(int from, int to, const char* road_type, int traffic_factor, double speed_limit) {
    double distance = enhanced_haversine_distance(
        graph[from].location.latitude, graph[from].location.longitude, graph[from].location.elevation,
        graph[to].location.latitude, graph[to].location.longitude, graph[to].location.elevation
    );
    
    // Forward edge
    Edge* new_edge = malloc(sizeof(Edge));
    new_edge->destination = to;
    new_edge->base_distance = distance;
    new_edge->current_weight = distance;
    strcpy(new_edge->road_type, road_type);
    new_edge->traffic_factor = traffic_factor;
    new_edge->speed_limit = speed_limit;
    new_edge->next = graph[from].edges;
    graph[from].edges = new_edge;
    
    // Reverse edge
    Edge* reverse_edge = malloc(sizeof(Edge));
    reverse_edge->destination = from;
    reverse_edge->base_distance = distance;
    reverse_edge->current_weight = distance;
    strcpy(reverse_edge->road_type, road_type);
    reverse_edge->traffic_factor = traffic_factor;
    reverse_edge->speed_limit = speed_limit;
    reverse_edge->next = graph[to].edges;
    graph[to].edges = reverse_edge;
}

void initialize_enhanced_mumbai_network() {
    printf("🗺️  Initializing Enhanced Mumbai GPS Network\n");
    printf("============================================\n");
    
    // Enhanced locations with detailed metadata
    add_enhanced_location(0, "Bandra Station", "station", "Bandra", 19.0544, 72.8406, 12, 4);
    add_enhanced_location(1, "Kurla Junction", "station", "Kurla", 19.0728, 72.8826, 15, 5);
    add_enhanced_location(2, "Andheri East", "business", "Andheri", 19.1136, 72.8697, 18, 4);
    add_enhanced_location(3, "Powai Lake", "landmark", "Powai", 19.1188, 72.9073, 25, 2);
    add_enhanced_location(4, "Mumbai Airport", "airport", "Andheri", 19.0896, 72.8656, 11, 3);
    add_enhanced_location(5, "Bandra-Kurla Complex", "business", "BKC", 19.0633, 72.8682, 14, 5);
    add_enhanced_location(6, "Mahim Junction", "station", "Mahim", 19.0410, 72.8397, 10, 3);
    add_enhanced_location(7, "Worli Sea Face", "landmark", "Worli", 19.0176, 72.8181, 8, 2);
    add_enhanced_location(8, "Colaba Causeway", "commercial", "Colaba", 18.9067, 72.8147, 5, 3);
    add_enhanced_location(9, "Marine Drive", "landmark", "Nariman Point", 18.9427, 72.8245, 7, 2);
    
    // Enhanced road network with realistic road types and traffic
    add_enhanced_edge(0, 1, "main", 2, 60);      // Bandra-Kurla road
    add_enhanced_edge(0, 5, "highway", 1, 80);   // Bandra-BKC express
    add_enhanced_edge(0, 6, "main", 2, 50);      // Linking Road
    add_enhanced_edge(1, 2, "highway", 1, 80);   // Eastern Express Highway
    add_enhanced_edge(1, 5, "main", 3, 40);      // Kurla-BKC connector
    add_enhanced_edge(2, 3, "local", 2, 40);     // Andheri-Powai road
    add_enhanced_edge(2, 4, "highway", 1, 60);   // Airport road
    add_enhanced_edge(4, 5, "highway", 2, 70);   // Airport-BKC highway
    add_enhanced_edge(5, 6, "main", 2, 50);      // BKC-Mahim link
    add_enhanced_edge(6, 7, "main", 2, 60);      // Mahim-Worli connector
    add_enhanced_edge(7, 8, "highway", 2, 80);   // Worli-Colaba sea link
    add_enhanced_edge(7, 9, "main", 2, 50);      // Worli-Marine Drive
    add_enhanced_edge(8, 9, "local", 3, 30);     // South Mumbai circuit
    
    printf("✅ Enhanced network: %d locations, multiple road types\n", node_count);
    printf("🚦 Traffic-aware routing enabled\n");
    printf("🏔️  Elevation data included\n\n");
}

// ===== ADVANCED JSON OUTPUT =====
void generate_enhanced_json(int start, int goal, int path[], int path_length, double total_cost) {
    FILE* file = fopen("route_data.json", "w");
    if (!file) return;
    
    time_t now = time(NULL);
    struct tm* utc_time = gmtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", utc_time);
    
    // Calculate estimated travel time
    double avg_speed = 45.0; // km/h average in Mumbai
    double estimated_minutes = (total_cost / avg_speed) * 60;
    
    fprintf(file, "{\n");
    fprintf(file, "  \"route\": {\n");
    
    // Start location
    fprintf(file, "    \"start\": {\n");
    fprintf(file, "      \"id\": %d,\n", graph[start].location.id);
    fprintf(file, "      \"name\": \"%s\",\n", graph[start].location.name);
    fprintf(file, "      \"type\": \"%s\",\n", graph[start].location.type);
    fprintf(file, "      \"district\": \"%s\",\n", graph[start].location.district);
    fprintf(file, "      \"latitude\": %.6f,\n", graph[start].location.latitude);
    fprintf(file, "      \"longitude\": %.6f,\n", graph[start].location.longitude);
    fprintf(file, "      \"elevation\": %.1f\n", graph[start].location.elevation);
    fprintf(file, "    },\n");
    
    // End location
    fprintf(file, "    \"end\": {\n");
    fprintf(file, "      \"id\": %d,\n", graph[goal].location.id);
    fprintf(file, "      \"name\": \"%s\",\n", graph[goal].location.name);
    fprintf(file, "      \"type\": \"%s\",\n", graph[goal].location.type);
    fprintf(file, "      \"district\": \"%s\",\n", graph[goal].location.district);
    fprintf(file, "      \"latitude\": %.6f,\n", graph[goal].location.latitude);
    fprintf(file, "      \"longitude\": %.6f,\n", graph[goal].location.longitude);
    fprintf(file, "      \"elevation\": %.1f\n", graph[goal].location.elevation);
    fprintf(file, "    },\n");
    
    // Route statistics
    fprintf(file, "    \"statistics\": {\n");
    fprintf(file, "      \"total_distance\": %.2f,\n", total_cost);
    fprintf(file, "      \"estimated_time_minutes\": %.1f,\n", estimated_minutes);
    fprintf(file, "      \"waypoint_count\": %d,\n", path_length);
    fprintf(file, "      \"average_speed_kmh\": %.1f,\n", avg_speed);
    fprintf(file, "      \"algorithm_used\": \"A*\",\n");
    fprintf(file, "      \"traffic_considered\": true,\n");
    fprintf(file, "      \"elevation_considered\": true\n");
    fprintf(file, "    },\n");
    
    // Path details
    fprintf(file, "    \"path\": [\n");
    for (int i = 0; i < path_length; i++) {
        int node_id = path[i];
        fprintf(file, "      {\n");
        fprintf(file, "        \"id\": %d,\n", graph[node_id].location.id);
        fprintf(file, "        \"name\": \"%s\",\n", graph[node_id].location.name);
        fprintf(file, "        \"type\": \"%s\",\n", graph[node_id].location.type);
        fprintf(file, "        \"district\": \"%s\",\n", graph[node_id].location.district);
        fprintf(file, "        \"latitude\": %.6f,\n", graph[node_id].location.latitude);
        fprintf(file, "        \"longitude\": %.6f,\n", graph[node_id].location.longitude);
        fprintf(file, "        \"elevation\": %.1f,\n", graph[node_id].location.elevation);
        fprintf(file, "        \"traffic_level\": %d,\n", graph[node_id].location.traffic_level);
        fprintf(file, "        \"step\": %d\n", i + 1);
        if (i < path_length - 1) fprintf(file, "      },\n");
        else fprintf(file, "      }\n");
    }
    fprintf(file, "    ]\n");
    fprintf(file, "  },\n");
    
    // API metadata
    fprintf(file, "  \"metadata\": {\n");
    fprintf(file, "    \"status\": \"success\",\n");
    fprintf(file, "    \"version\": \"2.0\",\n");
    fprintf(file, "    \"algorithm\": \"A*\",\n");
    fprintf(file, "    \"features\": [\"traffic_aware\", \"elevation\", \"realtime\"],\n");
    fprintf(file, "    \"timestamp\": \"%s\",\n", timestamp);
    fprintf(file, "    \"computation_node\": \"TrackMate Advanced\"\n");
    fprintf(file, "  }\n");
    fprintf(file, "}\n");
    
    fclose(file);
    printf("💾 Enhanced JSON data saved to route_data.json\n");
}

// ===== MAIN ADVANCED SYSTEM =====
int main() {
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║              TrackMate Advanced GPS System v2.0             ║\n");
    printf("║         A* Algorithm + Traffic-Aware Routing                ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");
    
    // Initialize enhanced Mumbai network
    initialize_enhanced_mumbai_network();
    
    // Route calculation: Colaba to Powai (challenging cross-city route)
    int start = 8;  // Colaba Causeway
    int goal = 3;   // Powai Lake
    
    printf("🎯 Computing optimal route: %s → %s\n", 
           graph[start].location.name, graph[goal].location.name);
    printf("⏰ Considering real-time traffic and elevation\n\n");
    
    int path[MAX_NODES];
    double total_cost;
    
    int path_length = astar_pathfind(start, goal, path, &total_cost);
    
    if (path_length > 0) {
        printf("\n🎉 Optimal Route Found!\n");
        printf("═══════════════════════\n");
        printf("📏 Total Distance: %.2f km\n", total_cost);
        printf("⏱️  Estimated Time: %.1f minutes\n", (total_cost / 45.0) * 60);
        printf("📍 Waypoints: %d locations\n", path_length);
        
        printf("\n🗺️  Route Details:\n");
        for (int i = 0; i < path_length; i++) {
            int node_id = path[i];
            printf("  %d. %s (%s, %s)\n", i+1, 
                   graph[node_id].location.name,
                   graph[node_id].location.type,
                   graph[node_id].location.district);
        }
        
        // Generate enhanced JSON
        generate_enhanced_json(start, goal, path, path_length, total_cost);
        
        printf("\n🌐 Ready for visualization!\n");
        printf("   Open index.html to see the enhanced route on the map\n");
    }
    
    return 0;
}