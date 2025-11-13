/**
 * json_output.c
 * JSON output generation implementation
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "json_output.h"
#include "graph.h"
#include "pathfinding.h"

void generate_json_output(int start, int end, double distances[], int previous[], 
                         const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not create %s\n", filename);
        return;
    }
    
    // Reconstruct path
    int path[MAX_NODES];
    int path_length = reconstruct_path(end, previous, path);
    
    // Get current timestamp
    time_t now = time(NULL);
    struct tm* utc_time = gmtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", utc_time);
    
    fprintf(file, "{\n");
    fprintf(file, "  \"route\": {\n");
    
    // Start location
    fprintf(file, "    \"start\": {\n");
    fprintf(file, "      \"id\": %d,\n", graph[start].location.id);
    fprintf(file, "      \"name\": \"%s\",\n", graph[start].location.name);
    fprintf(file, "      \"latitude\": %.6f,\n", graph[start].location.latitude);
    fprintf(file, "      \"longitude\": %.6f\n", graph[start].location.longitude);
    fprintf(file, "    },\n");
    
    // End location
    fprintf(file, "    \"end\": {\n");
    fprintf(file, "      \"id\": %d,\n", graph[end].location.id);
    fprintf(file, "      \"name\": \"%s\",\n", graph[end].location.name);
    fprintf(file, "      \"latitude\": %.6f,\n", graph[end].location.latitude);
    fprintf(file, "      \"longitude\": %.6f\n", graph[end].location.longitude);
    fprintf(file, "    },\n");
    
    fprintf(file, "    \"total_distance\": %.2f,\n", distances[end]);
    fprintf(file, "    \"path\": [\n");
    
    // Path waypoints
    for (int i = 0; i < path_length; i++) {
        int node_id = path[i];
        fprintf(file, "      {\n");
        fprintf(file, "        \"id\": %d,\n", graph[node_id].location.id);
        fprintf(file, "        \"name\": \"%s\",\n", graph[node_id].location.name);
        fprintf(file, "        \"latitude\": %.6f,\n", graph[node_id].location.latitude);
        fprintf(file, "        \"longitude\": %.6f\n", graph[node_id].location.longitude);
        if (i < path_length - 1) fprintf(file, "      },\n");
        else fprintf(file, "      }\n");
    }
    
    fprintf(file, "    ]\n");
    fprintf(file, "  },\n");
    fprintf(file, "  \"status\": \"success\",\n");
    fprintf(file, "  \"algorithm\": \"dijkstra\",\n");
    fprintf(file, "  \"timestamp\": \"%s\"\n", timestamp);
    fprintf(file, "}\n");
    
    fclose(file);
    printf("💾 JSON output saved to %s\n", filename);
}

void generate_enhanced_json(int start, int end, int path[], int path_length, 
                           double total_cost, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not create %s\n", filename);
        return;
    }
    
    time_t now = time(NULL);
    struct tm* utc_time = gmtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", utc_time);
    
    // Calculate estimated travel time
    double avg_speed = 45.0; // km/h average
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
    fprintf(file, "      \"id\": %d,\n", graph[end].location.id);
    fprintf(file, "      \"name\": \"%s\",\n", graph[end].location.name);
    fprintf(file, "      \"type\": \"%s\",\n", graph[end].location.type);
    fprintf(file, "      \"district\": \"%s\",\n", graph[end].location.district);
    fprintf(file, "      \"latitude\": %.6f,\n", graph[end].location.latitude);
    fprintf(file, "      \"longitude\": %.6f,\n", graph[end].location.longitude);
    fprintf(file, "      \"elevation\": %.1f\n", graph[end].location.elevation);
    fprintf(file, "    },\n");
    
    // Route statistics
    fprintf(file, "    \"statistics\": {\n");
    fprintf(file, "      \"total_distance\": %.2f,\n", total_cost);
    fprintf(file, "      \"estimated_time_minutes\": %.1f,\n", estimated_minutes);
    fprintf(file, "      \"waypoint_count\": %d,\n", path_length);
    fprintf(file, "      \"average_speed_kmh\": %.1f,\n", avg_speed);
    fprintf(file, "      \"algorithm_used\": \"A*\"\n");
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
    fprintf(file, "    \"timestamp\": \"%s\"\n", timestamp);
    fprintf(file, "  }\n");
    fprintf(file, "}\n");
    
    fclose(file);
    printf("💾 Enhanced JSON saved to %s\n", filename);
}

void print_route_console(int path[], int path_length, double total_distance) {
    printf("\n🗺️  Route Details:\n");
    printf("════════════════\n");
    
    for (int i = 0; i < path_length; i++) {
        int node_id = path[i];
        printf("  %d. %s", i + 1, graph[node_id].location.name);
        
        if (strlen(graph[node_id].location.type) > 0 && 
            strcmp(graph[node_id].location.type, "general") != 0) {
            printf(" (%s)", graph[node_id].location.type);
        }
        
        if (i < path_length - 1) {
            printf(" →\n");
        } else {
            printf("\n");
        }
    }
    
    printf("\n📏 Total Distance: %.2f km\n", total_distance);
    printf("⏱️  Estimated Time: %.1f minutes (at 45 km/h avg)\n", 
           (total_distance / 45.0) * 60);
}
