/**
 * main.c
 * TrackMate GPS Tracker - Main Entry Point
 * 
 * A GPS pathfinding system using Dijkstra's and A* algorithms
 * Author: TrackMate Team
 * Date: November 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gps_types.h"
#include "graph.h"
#include "pathfinding.h"
#include "json_output.h"
#include "data_loader.h"

void print_banner(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║              TrackMate GPS Tracker System v2.0              ║\n");
    printf("║          Shortest Path Navigation with Dijkstra & A*        ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void print_menu(void) {
    printf("\n📋 Select Algorithm:\n");
    printf("══════════════════\n");
    printf("1. Dijkstra's Algorithm (Classic shortest path)\n");
    printf("2. A* Algorithm (Heuristic-based, faster)\n");
    printf("3. Compare both algorithms\n");
    printf("4. Exit\n");
    printf("\nChoice: ");
}

void run_dijkstra(int start, int end) {
    printf("\n🔍 Running Dijkstra's Algorithm\n");
    printf("═══════════════════════════════\n");
    
    double distances[MAX_NODES];
    int previous[MAX_NODES];
    
    dijkstra(start, end, distances, previous);
    
    // Reconstruct and display path
    int path[MAX_NODES];
    int path_length = reconstruct_path(end, previous, path);
    
    print_route_console(path, path_length, distances[end]);
    
    // Generate JSON output
    generate_json_output(start, end, distances, previous, "route_data.json");
}

void run_astar(int start, int end) {
    printf("\n🎯 Running A* Algorithm\n");
    printf("═══════════════════════\n");
    
    int path[MAX_NODES];
    double total_cost;
    
    int path_length = astar_pathfind(start, end, path, &total_cost);
    
    if (path_length > 0) {
        print_route_console(path, path_length, total_cost);
        generate_enhanced_json(start, end, path, path_length, total_cost, 
                             "enhanced_route_data.json");
    } else {
        printf("No path found!\n");
    }
}

void compare_algorithms(int start, int end) {
    printf("\n⚖️  Comparing Dijkstra vs A*\n");
    printf("════════════════════════════\n\n");
    
    // Run Dijkstra
    printf("--- Dijkstra's Algorithm ---\n");
    double distances[MAX_NODES];
    int previous[MAX_NODES];
    dijkstra(start, end, distances, previous);
    
    int dijkstra_path[MAX_NODES];
    int dijkstra_length = reconstruct_path(end, previous, dijkstra_path);
    
    printf("\n--- A* Algorithm ---\n");
    int astar_path[MAX_NODES];
    double astar_cost;
    int astar_length = astar_pathfind(start, end, astar_path, &astar_cost);
    
    printf("\n📊 Comparison Results:\n");
    printf("══════════════════════\n");
    printf("Dijkstra: %.2f km, %d waypoints\n", distances[end], dijkstra_length);
    printf("A*:       %.2f km, %d waypoints\n", astar_cost, astar_length);
    
    if (astar_cost <= distances[end] + 0.01) {
        printf("✅ Both algorithms found optimal path!\n");
    }
}

int main(int argc, char* argv[]) {
    print_banner();
    
    // Initialize graph
    init_graph();
    
    // Load network data
    printf("📦 Select Network Dataset:\n");
    printf("═════════════════════════\n");
    printf("1. Basic Mumbai Network (7 locations)\n");
    printf("2. Enhanced Mumbai Network (10 locations)\n");
    printf("\nChoice [default=2]: ");
    
    int network_choice = 2;
    if (argc > 1) {
        network_choice = atoi(argv[1]);
    } else {
        char input[10];
        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (input[0] >= '1' && input[0] <= '2') {
                network_choice = input[0] - '0';
            }
        }
    }
    
    if (network_choice == 1) {
        load_basic_mumbai_network();
    } else {
        load_enhanced_mumbai_network();
    }
    
    print_graph_stats();
    
    // Select start and end points
    printf("\n📍 Route Configuration:\n");
    printf("══════════════════════\n");
    printf("Available locations:\n");
    for (int i = 0; i < node_count; i++) {
        printf("  %d. %s\n", i, graph[i].location.name);
    }
    
    int start = 0;
    int end = 3;
    
    printf("\nStart location [default=0]: ");
    char input[10];
    if (fgets(input, sizeof(input), stdin) != NULL && input[0] >= '0' && input[0] <= '9') {
        start = atoi(input);
    }
    
    printf("End location [default=3]: ");
    if (fgets(input, sizeof(input), stdin) != NULL && input[0] >= '0' && input[0] <= '9') {
        end = atoi(input);
    }
    
    if (start < 0 || start >= node_count || end < 0 || end >= node_count) {
        printf("❌ Invalid location indices!\n");
        cleanup_graph();
        return 1;
    }
    
    printf("\n🎯 Route: %s → %s\n", 
           graph[start].location.name, 
           graph[end].location.name);
    
    // Algorithm selection
    int choice = 0;
    if (argc > 2) {
        choice = atoi(argv[2]);
    } else {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            choice = 2; // Default to A*
        }
    }
    
    switch (choice) {
        case 1:
            run_dijkstra(start, end);
            break;
        case 2:
            run_astar(start, end);
            break;
        case 3:
            compare_algorithms(start, end);
            break;
        case 4:
            printf("\n👋 Goodbye!\n");
            break;
        default:
            printf("\n⚠️  Invalid choice, running A* by default\n");
            run_astar(start, end);
    }
    
    printf("\n🌐 Visualization Ready!\n");
    printf("   Open index.html in your browser to see the route on the map\n");
    printf("   JSON data: route_data.json or enhanced_route_data.json\n\n");
    
    // Cleanup
    cleanup_graph();
    
    return 0;
}
