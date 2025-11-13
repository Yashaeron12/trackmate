# TrackMate GPS Tracker - Modular Architecture

## 📁 Project Structure

### Header Files (.h)
- **gps_types.h** - Core data structures and constants
  - Location, Edge, GraphNode, MinHeap, RouteStats
  - Constants: MAX_NODES, EARTH_RADIUS, PI, INF

- **distance.h** - GPS distance calculations
  - Haversine formula implementation
  - Enhanced distance with elevation
  - Bearing calculation

- **heap.h** - Min-heap priority queue
  - Heap operations for Dijkstra and A*
  - Insert, extract_min, decrease_key

- **graph.h** - Graph data structure and operations
  - Add locations and edges
  - Enhanced metadata support
  - Graph statistics and cleanup

- **pathfinding.h** - Route finding algorithms
  - Dijkstra's algorithm
  - A* algorithm with heuristics
  - Path reconstruction

- **json_output.h** - Output generation
  - JSON file generation for web frontend
  - Console output formatting
  - Enhanced statistics

- **data_loader.h** - Sample data initialization
  - Basic Mumbai network
  - Enhanced Mumbai network
  - Custom network support

### Implementation Files (.c)
- **main.c** - Program entry point and UI
- **distance.c** - Distance calculation implementations
- **heap.c** - Min-heap data structure
- **graph.c** - Graph operations
- **pathfinding.c** - Dijkstra and A* algorithms
- **json_output.c** - JSON and console output
- **data_loader.c** - Network data initialization

### Legacy Files (for reference)
- **trackmate.c** - Original monolithic implementation (Dijkstra)
- **trackmate_advanced.c** - Original advanced version (A*)
- **demo.c** - Interactive demonstration
- **test_trackmate.c** - Unit tests

## 🔨 Building the Project

### Compile
```bash
make
```

### Run Interactive Mode
```bash
make run
```

### Quick Run (with defaults)
```bash
make quick
```

### Clean Build
```bash
make clean
make rebuild
```

### Debug Build
```bash
make debug
```

## 📊 Module Dependencies

```
main.c
  ├── gps_types.h
  ├── graph.h
  │   ├── gps_types.h
  │   └── distance.h
  ├── pathfinding.h
  │   ├── gps_types.h
  │   ├── graph.h
  │   ├── heap.h
  │   └── distance.h
  ├── json_output.h
  │   └── gps_types.h
  └── data_loader.h
      └── graph.h
```

## 🎯 Key Features by Module

### Distance Module
- Haversine formula for GPS coordinates
- 3D distance with elevation
- Bearing calculations for navigation

### Heap Module
- Min-heap priority queue
- Optimized for Dijkstra and A*
- O(log n) insert and extract

### Graph Module
- Adjacency list representation
- Bidirectional edges
- Dynamic edge weights
- Traffic and road type metadata

### Pathfinding Module
- **Dijkstra's Algorithm**
  - Guaranteed shortest path
  - No heuristics needed
  
- **A* Algorithm**
  - Faster with heuristics
  - Uses straight-line distance estimate
  - Optimal with admissible heuristic

### Output Module
- JSON API format for web frontend
- Enhanced statistics
- Console-friendly formatting

## 💡 Usage Examples

### Example 1: Basic Route Finding
```c
#include "graph.h"
#include "pathfinding.h"

init_graph();
load_basic_mumbai_network();

double distances[MAX_NODES];
int previous[MAX_NODES];
dijkstra(0, 3, distances, previous);
```

### Example 2: A* with Custom Network
```c
#include "graph.h"
#include "pathfinding.h"

init_graph();
add_enhanced_location(0, "Start", "landmark", "District1", 
                     19.0, 72.8, 10, 2);
add_enhanced_location(1, "End", "landmark", "District2", 
                     19.1, 72.9, 15, 3);
add_enhanced_edge(0, 1, "highway", 1, 80);

int path[MAX_NODES];
double cost;
astar_pathfind(0, 1, path, &cost);
```

## 🧪 Testing

The modular structure makes testing easier:

```bash
# Compile individual modules
gcc -c distance.c -o distance.o
gcc -c heap.c -o heap.o

# Run syntax check
make check
```

## 📈 Performance

- **Compilation**: < 2 seconds
- **Dijkstra**: O((V + E) log V)
- **A***: Typically faster than Dijkstra
- **Memory**: < 5 MB for 50 nodes

## 🔄 Migration from Legacy Code

Old monolithic files remain for reference:
- `trackmate.c` → Now split into modules
- `trackmate_advanced.c` → Integrated into modular structure

## 🚀 Future Enhancements

Modular design allows easy addition of:
- [ ] Bidirectional A*
- [ ] Contraction Hierarchies
- [ ] Real-time traffic updates
- [ ] Multi-criteria optimization
- [ ] Route alternatives

## 📝 Coding Standards

- Header guards in all .h files
- Clear separation of interface (.h) and implementation (.c)
- Consistent naming conventions
- Comprehensive documentation
- Error handling throughout

## 🛠️ Development Workflow

1. Modify module (.c file)
2. Update header if needed (.h file)
3. Run `make rebuild`
4. Test with `make run`
5. Commit changes

## ✅ Advantages of Modular Design

✅ **Maintainability** - Each module has single responsibility  
✅ **Testability** - Test modules independently  
✅ **Reusability** - Import modules in other projects  
✅ **Readability** - Clear structure, easy to navigate  
✅ **Scalability** - Add features without breaking existing code  
✅ **Collaboration** - Multiple developers can work on different modules  

## 📄 License

Educational project for Data Structures coursework.
