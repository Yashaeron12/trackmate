# TrackMate GPS Tracker - Complete Guide

## 🎯 Overview

TrackMate is a GPS pathfinding system implementing Dijkstra's and A* algorithms with a clean, modular architecture. This project demonstrates professional C programming practices with proper separation of concerns.

## 📂 File Structure

### ✅ Modular C Files (NEW - Recommended)

#### Header Files (.h)
| File | Purpose | Key Components |
|------|---------|----------------|
| `gps_types.h` | Core data structures | Location, Edge, GraphNode, MinHeap, RouteStats |
| `distance.h` | Distance calculations | Haversine, enhanced distance, bearing |
| `heap.h` | Priority queue | Min-heap for Dijkstra/A* |
| `graph.h` | Graph operations | Add locations/edges, graph stats |
| `pathfinding.h` | Route algorithms | Dijkstra, A*, path reconstruction |
| `json_output.h` | Output generation | JSON files, console display |
| `data_loader.h` | Sample data | Mumbai network initialization |

#### Implementation Files (.c)
| File | Lines | Description |
|------|-------|-------------|
| `distance.c` | ~80 | GPS distance calculations |
| `heap.c` | ~100 | Min-heap implementation |
| `graph.c` | ~150 | Graph data structure |
| `pathfinding.c` | ~150 | Dijkstra & A* algorithms |
| `json_output.c` | ~200 | Output formatting |
| `data_loader.c` | ~100 | Network initialization |
| `main.c` | ~200 | Program entry point |

**Total: 7 header files + 7 implementation files = 14 modular files**

### 📜 Legacy Files (For Reference)
- `trackmate.c` - Original monolithic version (~500 lines)
- `trackmate_advanced.c` - Advanced version with A* (~700 lines)
- `demo.c` - Interactive demonstration
- `test_trackmate.c` - Unit tests

### 🔧 Build Scripts
- `build.bat` - Windows compile script
- `clean.bat` - Remove build artifacts
- `run.bat` - Quick run with defaults
- `Makefile` - Linux/Unix build system
- `setup.bat` / `setup.sh` - Project setup

### 📖 Documentation
- `MODULAR_STRUCTURE.md` - Architecture documentation
- `README.md` - Project overview
- `TECHNICAL_DOCS.md` - Technical details
- `GUIDE.md` - This file

## 🚀 Quick Start

### Windows

```batch
# Build the project
build.bat

# Run with defaults (Enhanced network, A* algorithm)
run.bat

# Clean build artifacts
clean.bat
```

### Linux/Unix

```bash
# Build
make

# Run interactively
make run

# Quick run with defaults
make quick

# Clean
make clean
```

## 🏗️ Architecture Overview

```
┌─────────────────────────────────────────────────────────┐
│                      main.c                             │
│         (User Interface & Program Flow)                 │
└─────────────────┬───────────────────────────────────────┘
                  │
        ┌─────────┴─────────────────────────┐
        │                                   │
┌───────▼────────┐              ┌──────────▼─────────┐
│  data_loader   │              │   pathfinding      │
│  (Network      │              │   (Algorithms)     │
│   Setup)       │              │                    │
└───────┬────────┘              └──────────┬─────────┘
        │                                   │
        │                       ┌───────────▼─────────┐
        │                       │      heap           │
        │                       │  (Priority Queue)   │
        │                       └─────────────────────┘
        │
┌───────▼────────┐              ┌─────────────────────┐
│     graph      │◄─────────────┤    distance         │
│  (Data         │              │  (GPS Calc)         │
│   Structure)   │              └─────────────────────┘
└───────┬────────┘
        │
┌───────▼────────┐
│  json_output   │
│  (Results)     │
└────────────────┘
```

## 📊 Module Responsibilities

### 1. Core Data (gps_types.h)
- **What**: Defines all data structures
- **Why**: Central type definitions prevent circular dependencies
- **Example**:
  ```c
  typedef struct {
      int id;
      char name[50];
      double latitude;
      double longitude;
  } Location;
  ```

### 2. Distance Module
- **What**: GPS distance calculations
- **Why**: Reusable mathematical functions
- **Algorithms**:
  - Haversine formula for spherical distance
  - 3D distance with elevation
  - Bearing calculation
- **Usage**:
  ```c
  double dist = haversine_distance(lat1, lon1, lat2, lon2);
  ```

### 3. Heap Module
- **What**: Min-heap priority queue
- **Why**: Efficient O(log n) operations for pathfinding
- **Operations**:
  - Insert: O(log n)
  - Extract min: O(log n)
  - Decrease key: O(n)
- **Usage**:
  ```c
  MinHeap heap;
  init_heap(&heap);
  insert_heap(&heap, vertex, distance);
  PQNode min = extract_min(&heap);
  ```

### 4. Graph Module
- **What**: Graph data structure and operations
- **Why**: Manages GPS network topology
- **Features**:
  - Adjacency list representation
  - Bidirectional edges
  - Dynamic weights
  - Metadata (traffic, road type, elevation)
- **Usage**:
  ```c
  init_graph();
  add_location(0, "Station", 19.0, 72.8);
  add_edge(0, 1);
  ```

### 5. Pathfinding Module
- **What**: Route finding algorithms
- **Why**: Core navigation logic
- **Algorithms**:

  **Dijkstra's Algorithm**
  - Guaranteed shortest path
  - Time: O((V+E) log V)
  - Space: O(V)
  
  **A* Algorithm**
  - Heuristic-based optimization
  - Faster in practice
  - Still optimal with admissible heuristic
  
- **Usage**:
  ```c
  // Dijkstra
  double distances[MAX_NODES];
  int previous[MAX_NODES];
  dijkstra(start, end, distances, previous);
  
  // A*
  int path[MAX_NODES];
  double cost;
  int len = astar_pathfind(start, end, path, &cost);
  ```

### 6. Output Module
- **What**: Result formatting and display
- **Why**: Separates presentation from logic
- **Formats**:
  - JSON for web frontend
  - Console text for debugging
  - Enhanced statistics
- **Usage**:
  ```c
  generate_json_output(start, end, distances, previous, "route.json");
  print_route_console(path, length, distance);
  ```

### 7. Data Loader Module
- **What**: Sample network initialization
- **Why**: Separates data from code
- **Networks**:
  - Basic Mumbai (7 locations)
  - Enhanced Mumbai (10 locations)
  - Custom network support
- **Usage**:
  ```c
  load_enhanced_mumbai_network();
  ```

## 🎓 Educational Value

### Data Structures Demonstrated
✅ Adjacency List (Graph representation)  
✅ Min-Heap (Priority queue)  
✅ Linked List (Edge lists)  
✅ Arrays (Distance/previous arrays)  

### Algorithms Implemented
✅ Dijkstra's Algorithm (Single-source shortest path)  
✅ A* Algorithm (Heuristic search)  
✅ Haversine Formula (Spherical distance)  

### Software Engineering Practices
✅ Modular design  
✅ Separation of concerns  
✅ Header/implementation split  
✅ Defensive programming  
✅ Comprehensive documentation  

## 🔬 Testing

### Functionality Tests
```bash
# Test compilation
build.bat

# Test basic route (Bandra → Powai)
# Input: 1, 1, 0, 3
trackmate.exe

# Test enhanced network with A*
# Input: 2, 2, 0, 3
trackmate.exe
```

### Expected Output
- Graph loads successfully
- Routes calculate in < 1 second
- JSON files generated
- Optimal paths found
- A* explores fewer nodes than Dijkstra

## 📈 Performance Analysis

### Time Complexity
| Operation | Complexity | Notes |
|-----------|------------|-------|
| Add Location | O(1) | Append to array |
| Add Edge | O(1) | Prepend to list |
| Dijkstra | O((V+E) log V) | With min-heap |
| A* | O((V+E) log V) | Best case better |
| Haversine | O(1) | Constant math |

### Space Complexity
| Structure | Space | Notes |
|-----------|-------|-------|
| Graph | O(V + E) | Adjacency list |
| Heap | O(V) | Max nodes |
| Path Arrays | O(V) | Distance/previous |
| **Total** | **O(V + E)** | Linear in graph size |

### Benchmark Results (10 locations, 13 edges)
- Compilation: < 2 seconds
- Route calculation: < 0.01 seconds
- Memory usage: < 1 MB
- JSON generation: < 0.001 seconds

## 🔄 Comparison: Monolithic vs Modular

### Before (trackmate.c - 500 lines)
❌ All code in one file  
❌ Hard to maintain  
❌ Difficult to test  
❌ Can't reuse components  
❌ Merge conflicts likely  

### After (Modular - 14 files)
✅ Clear separation of concerns  
✅ Easy to maintain and extend  
✅ Unit testable modules  
✅ Reusable components  
✅ Team-friendly development  

## 💡 Usage Examples

### Example 1: Find Route with Dijkstra
```c
#include "graph.h"
#include "pathfinding.h"
#include "data_loader.h"
#include "json_output.h"

int main() {
    // Setup
    init_graph();
    load_basic_mumbai_network();
    
    // Find route
    double distances[MAX_NODES];
    int previous[MAX_NODES];
    dijkstra(0, 3, distances, previous);
    
    // Output
    int path[MAX_NODES];
    int len = reconstruct_path(3, previous, path);
    print_route_console(path, len, distances[3]);
    generate_json_output(0, 3, distances, previous, "route.json");
    
    cleanup_graph();
    return 0;
}
```

### Example 2: Custom Network with A*
```c
#include "graph.h"
#include "pathfinding.h"

int main() {
    init_graph();
    
    // Add custom locations
    add_enhanced_location(0, "Home", "residential", "Area1", 
                         19.0, 72.8, 10, 1);
    add_enhanced_location(1, "Office", "commercial", "Area2", 
                         19.1, 72.9, 15, 3);
    add_enhanced_location(2, "Mall", "commercial", "Area3", 
                         19.05, 72.85, 12, 4);
    
    // Add roads
    add_enhanced_edge(0, 2, "local", 2, 40);
    add_enhanced_edge(2, 1, "highway", 1, 80);
    
    // Find fastest route
    int path[MAX_NODES];
    double cost;
    int len = astar_pathfind(0, 1, path, &cost);
    
    print_route_console(path, len, cost);
    
    cleanup_graph();
    return 0;
}
```

## 🐛 Troubleshooting

### Build Errors

**Problem**: `gcc: command not found`  
**Solution**: Install MinGW (Windows) or GCC (Linux)

**Problem**: `undefined reference to 'sin'`  
**Solution**: Add `-lm` flag for math library

**Problem**: Warning about `strlen`/`strcmp`  
**Solution**: Already fixed - `#include <string.h>` added

### Runtime Errors

**Problem**: No path found  
**Solution**: Check that edges exist between start and end nodes

**Problem**: JSON not generated  
**Solution**: Check file permissions in output directory

## 🚀 Future Enhancements

### Planned Features
- [ ] Bidirectional A* (search from both ends)
- [ ] Alternative routes (k-shortest paths)
- [ ] Real-time traffic integration
- [ ] Multi-modal transport (car, bus, walk)
- [ ] Turn-by-turn directions

### Potential Modules to Add
- `traffic.h/c` - Real-time traffic updates
- `routing_table.h/c` - Precomputed routes
- `multi_criteria.h/c` - Multiple optimization goals
- `geocoding.h/c` - Address to coordinates

## 📚 Learning Resources

### Data Structures
- Graph Theory basics
- Priority Queues and Heaps
- Adjacency List representation

### Algorithms
- Dijkstra's Algorithm
- A* Search Algorithm
- Greedy algorithms
- Dynamic programming

### C Programming
- Modular programming in C
- Header guards
- Memory management
- File I/O

## 📝 Code Standards

### Naming Conventions
- Functions: `snake_case` (e.g., `dijkstra`, `add_location`)
- Types: `PascalCase` (e.g., `GraphNode`, `Location`)
- Constants: `UPPER_CASE` (e.g., `MAX_NODES`, `PI`)
- Variables: `snake_case` (e.g., `node_count`, `total_cost`)

### Documentation
- Header comments for all files
- Function documentation in headers
- Inline comments for complex logic
- README files for each major component

## ✅ Project Checklist

### Implementation
- [x] Core data structures
- [x] Graph operations
- [x] Dijkstra's algorithm
- [x] A* algorithm
- [x] JSON output
- [x] Sample data
- [x] Interactive UI

### Code Quality
- [x] Modular design
- [x] Header files
- [x] Memory management
- [x] Error handling
- [x] Code documentation

### Testing
- [x] Compilation tests
- [x] Route calculation
- [x] JSON generation
- [x] Algorithm comparison

### Documentation
- [x] README
- [x] Architecture guide
- [x] API documentation
- [x] Build instructions

## 📄 License

Educational project for Data Structures and Algorithms coursework.

## 👥 Contributors

TrackMate Team - Data Structures Project Based Learning

---

**Last Updated**: November 2025  
**Version**: 2.0 (Modular Architecture)
