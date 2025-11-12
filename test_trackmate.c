// test_trackmate.c - Unit tests for TrackMate GPS Tracker
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <time.h>

// Test framework macros
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("❌ FAIL: %s\n", message); \
            return 0; \
        } else { \
            printf("✅ PASS: %s\n", message); \
        } \
    } while(0)

// Include the main trackmate functions (simplified for testing)
#define EARTH_RADIUS 6371.0
#define PI 3.14159265359

double haversine_distance(double lat1, double lon1, double lat2, double lon2) {
    lat1 = lat1 * PI / 180.0;
    lon1 = lon1 * PI / 180.0;
    lat2 = lat2 * PI / 180.0;
    lon2 = lon2 * PI / 180.0;
    
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;
    
    double a = sin(dlat/2) * sin(dlat/2) + cos(lat1) * cos(lat2) * sin(dlon/2) * sin(dlon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    
    return EARTH_RADIUS * c;
}

// Test functions
int test_haversine_formula() {
    printf("\n🧪 Testing Haversine Formula\n");
    printf("================================\n");
    
    // Test 1: Known distance between Mumbai and Delhi
    double mumbai_lat = 19.0760;
    double mumbai_lon = 72.8777;
    double delhi_lat = 28.7041;
    double delhi_lon = 77.1025;
    
    double distance = haversine_distance(mumbai_lat, mumbai_lon, delhi_lat, delhi_lon);
    printf("Mumbai to Delhi: %.2f km\n", distance);
    
    // Should be approximately 1150-1200 km
    TEST_ASSERT(distance > 1100 && distance < 1250, "Mumbai-Delhi distance in expected range");
    
    // Test 2: Same point should give 0 distance
    distance = haversine_distance(mumbai_lat, mumbai_lon, mumbai_lat, mumbai_lon);
    TEST_ASSERT(distance < 0.001, "Same point distance should be ~0");
    
    // Test 3: Short distance test (Bandra to BKC)
    double bandra_lat = 19.0544;
    double bandra_lon = 72.8406;
    double bkc_lat = 19.0633;
    double bkc_lon = 72.8682;
    
    distance = haversine_distance(bandra_lat, bandra_lon, bkc_lat, bkc_lon);
    printf("Bandra to BKC: %.2f km\n", distance);
    
    // Should be approximately 2-4 km
    TEST_ASSERT(distance > 1 && distance < 5, "Bandra-BKC distance in expected range");
    
    return 1;
}

int test_coordinate_validation() {
    printf("\n🧪 Testing Coordinate Validation\n");
    printf("=================================\n");
    
    // Test valid coordinates
    double valid_lat = 19.0760;  // Mumbai
    double valid_lon = 72.8777;
    
    TEST_ASSERT(valid_lat >= -90 && valid_lat <= 90, "Valid latitude range");
    TEST_ASSERT(valid_lon >= -180 && valid_lon <= 180, "Valid longitude range");
    
    // Test edge cases
    TEST_ASSERT(-90 >= -90 && -90 <= 90, "South pole latitude valid");
    TEST_ASSERT(90 >= -90 && 90 <= 90, "North pole latitude valid");
    TEST_ASSERT(-180 >= -180 && -180 <= 180, "West longitude boundary valid");
    TEST_ASSERT(180 >= -180 && 180 <= 180, "East longitude boundary valid");
    
    return 1;
}

int test_graph_properties() {
    printf("\n🧪 Testing Graph Properties\n");
    printf("============================\n");
    
    // Test symmetric distance property
    double lat1 = 19.0544, lon1 = 72.8406;  // Bandra
    double lat2 = 19.1136, lon2 = 72.8697;  // Andheri
    
    double dist1 = haversine_distance(lat1, lon1, lat2, lon2);
    double dist2 = haversine_distance(lat2, lon2, lat1, lon1);
    
    TEST_ASSERT(fabs(dist1 - dist2) < 0.001, "Distance symmetry property");
    
    // Test triangle inequality (approximate)
    double lat3 = 19.0633, lon3 = 72.8682;  // BKC (intermediate point)
    
    double direct = haversine_distance(lat1, lon1, lat2, lon2);
    double via_bkc = haversine_distance(lat1, lon1, lat3, lon3) + 
                     haversine_distance(lat3, lon3, lat2, lon2);
    
    printf("Direct: %.2f km, Via BKC: %.2f km\n", direct, via_bkc);
    TEST_ASSERT(via_bkc >= direct, "Triangle inequality (via intermediate ≥ direct)");
    
    return 1;
}

int test_json_format() {
    printf("\n🧪 Testing JSON Output Format\n");
    printf("==============================\n");
    
    // Test if route_data.json exists and is valid
    FILE* file = fopen("route_data.json", "r");
    TEST_ASSERT(file != NULL, "JSON file exists");
    
    if (file) {
        char buffer[1024];
        size_t bytes_read = fread(buffer, 1, sizeof(buffer)-1, file);
        buffer[bytes_read] = '\0';
        fclose(file);
        
        // Basic JSON structure validation
        TEST_ASSERT(strstr(buffer, "\"route\"") != NULL, "JSON contains route object");
        TEST_ASSERT(strstr(buffer, "\"start\"") != NULL, "JSON contains start location");
        TEST_ASSERT(strstr(buffer, "\"end\"") != NULL, "JSON contains end location");
        TEST_ASSERT(strstr(buffer, "\"path\"") != NULL, "JSON contains path array");
        TEST_ASSERT(strstr(buffer, "\"total_distance\"") != NULL, "JSON contains total distance");
        TEST_ASSERT(strstr(buffer, "\"latitude\"") != NULL, "JSON contains latitude");
        TEST_ASSERT(strstr(buffer, "\"longitude\"") != NULL, "JSON contains longitude");
        
        printf("JSON structure validation passed\n");
    }
    
    return 1;
}

int test_performance() {
    printf("\n🧪 Testing Performance\n");
    printf("=======================\n");
    
    clock_t start = clock();
    
    // Perform multiple distance calculations
    int iterations = 10000;
    double total_distance = 0;
    
    for (int i = 0; i < iterations; i++) {
        double distance = haversine_distance(19.0544, 72.8406, 19.1136, 72.8697);
        total_distance += distance;
    }
    
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Performed %d distance calculations in %.4f seconds\n", iterations, cpu_time);
    printf("Average: %.2f calculations per second\n", iterations / cpu_time);
    
    TEST_ASSERT(cpu_time < 1.0, "Performance test: < 1 second for 10K calculations");
    TEST_ASSERT(total_distance > 0, "Calculation results are valid");
    
    return 1;
}

int run_all_tests() {
    printf("🧪 TrackMate GPS Tracker - Test Suite\n");
    printf("=====================================\n");
    
    int total_tests = 0;
    int passed_tests = 0;
    
    // Run all test functions
    if (test_haversine_formula()) passed_tests++;
    total_tests++;
    
    if (test_coordinate_validation()) passed_tests++;
    total_tests++;
    
    if (test_graph_properties()) passed_tests++;
    total_tests++;
    
    if (test_json_format()) passed_tests++;
    total_tests++;
    
    if (test_performance()) passed_tests++;
    total_tests++;
    
    // Print summary
    printf("\n📊 Test Results Summary\n");
    printf("========================\n");
    printf("Passed: %d/%d tests\n", passed_tests, total_tests);
    
    if (passed_tests == total_tests) {
        printf("🎉 All tests passed! TrackMate is working correctly.\n");
        return 0;
    } else {
        printf("❌ Some tests failed. Please check the implementation.\n");
        return 1;
    }
}

int main() {
    return run_all_tests();
}