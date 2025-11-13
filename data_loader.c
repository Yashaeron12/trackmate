/**
 * data_loader.c
 * Sample data initialization implementation
 */

#include <stdio.h>
#include "data_loader.h"
#include "graph.h"

void load_basic_mumbai_network(void) {
    printf("📍 Loading basic Mumbai GPS network...\n");
    
    // Sample locations in Mumbai area (real GPS coordinates)
    add_location(0, "Bandra Station", 19.0544, 72.8406);
    add_location(1, "Kurla Junction", 19.0728, 72.8826);
    add_location(2, "Andheri East", 19.1136, 72.8697);
    add_location(3, "Powai Lake", 19.1188, 72.9073);
    add_location(4, "Mumbai Airport", 19.0896, 72.8656);
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
    
    printf("✅ Loaded %d locations with basic road network\n\n", node_count);
}

void load_enhanced_mumbai_network(void) {
    printf("🗺️  Loading enhanced Mumbai GPS network...\n");
    
    // Enhanced locations with detailed metadata
    add_enhanced_location(0, "Bandra Station", "station", "Bandra", 
                         19.0544, 72.8406, 12, 4);
    add_enhanced_location(1, "Kurla Junction", "station", "Kurla", 
                         19.0728, 72.8826, 15, 5);
    add_enhanced_location(2, "Andheri East", "business", "Andheri", 
                         19.1136, 72.8697, 18, 4);
    add_enhanced_location(3, "Powai Lake", "landmark", "Powai", 
                         19.1188, 72.9073, 25, 2);
    add_enhanced_location(4, "Mumbai Airport", "airport", "Andheri", 
                         19.0896, 72.8656, 11, 3);
    add_enhanced_location(5, "Bandra-Kurla Complex", "business", "BKC", 
                         19.0633, 72.8682, 14, 5);
    add_enhanced_location(6, "Mahim Junction", "station", "Mahim", 
                         19.0410, 72.8397, 10, 3);
    add_enhanced_location(7, "Worli Sea Face", "landmark", "Worli", 
                         19.0176, 72.8181, 8, 2);
    add_enhanced_location(8, "Colaba Causeway", "commercial", "Colaba", 
                         18.9067, 72.8147, 5, 3);
    add_enhanced_location(9, "Marine Drive", "landmark", "Nariman Point", 
                         18.9427, 72.8245, 7, 2);
    
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
    
    printf("✅ Loaded %d locations with enhanced metadata\n", node_count);
    printf("🚦 Traffic-aware routing enabled\n");
    printf("🏔️  Elevation data included\n\n");
}

void load_custom_network(void) {
    printf("🔧 Custom network loader - Not implemented yet\n");
    printf("   Loading basic network instead...\n\n");
    load_basic_mumbai_network();
}
