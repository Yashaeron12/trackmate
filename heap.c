/**
 * heap.c
 * Min-heap implementation for priority queue
 */

#include <string.h>
#include "heap.h"
#include "gps_types.h"

static void swap_nodes(PQNode* a, PQNode* b) {
    PQNode temp = *a;
    *a = *b;
    *b = temp;
}

static void heapify_up(MinHeap* heap, int index) {
    if (index && heap->nodes[index].distance < heap->nodes[(index-1)/2].distance) {
        swap_nodes(&heap->nodes[index], &heap->nodes[(index-1)/2]);
        heapify_up(heap, (index-1)/2);
    }
}

static void heapify_down(MinHeap* heap, int index) {
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

void init_heap(MinHeap* heap) {
    heap->size = 0;
}

int is_empty(MinHeap* heap) {
    return heap->size == 0;
}

void insert_heap(MinHeap* heap, int vertex, double distance) {
    heap->nodes[heap->size].vertex = vertex;
    heap->nodes[heap->size].distance = distance;
    heap->nodes[heap->size].f_cost = distance;
    heapify_up(heap, heap->size);
    heap->size++;
}

void insert_heap_astar(MinHeap* heap, int vertex, double g_cost, double h_cost, int parent) {
    heap->nodes[heap->size].vertex = vertex;
    heap->nodes[heap->size].g_cost = g_cost;
    heap->nodes[heap->size].h_cost = h_cost;
    heap->nodes[heap->size].f_cost = g_cost + h_cost;
    heap->nodes[heap->size].distance = g_cost + h_cost; // For comparison
    heap->nodes[heap->size].parent = parent;
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

void decrease_key(MinHeap* heap, int vertex, double new_distance) {
    // Find the vertex in heap
    for (int i = 0; i < heap->size; i++) {
        if (heap->nodes[i].vertex == vertex) {
            heap->nodes[i].distance = new_distance;
            heap->nodes[i].f_cost = new_distance;
            heapify_up(heap, i);
            break;
        }
    }
}
