/**
 * json_output.h
 * JSON output generation for web frontend
 */

#ifndef JSON_OUTPUT_H
#define JSON_OUTPUT_H

/**
 * Generate basic JSON output for route (Dijkstra version)
 */
void generate_json_output(int start, int end, double distances[], int previous[], 
                         const char* filename);

/**
 * Generate enhanced JSON output with full statistics (A* version)
 */
void generate_enhanced_json(int start, int end, int path[], int path_length, 
                           double total_cost, const char* filename);

/**
 * Print route to console in readable format
 */
void print_route_console(int path[], int path_length, double total_distance);

#endif // JSON_OUTPUT_H
