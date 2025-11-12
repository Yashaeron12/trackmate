# TrackMate - Smart GPS Location Tracker MVP

A minimal GPS-based routing system that finds shortest paths between locations using Dijkstra's algorithm and visualizes routes on an interactive map.

## Features
- **Backend (C)**: Graph-based routing with Dijkstra's algorithm
- **Frontend (Web)**: Interactive map visualization with Leaflet.js
- **GPS Integration**: Real-world coordinates with Haversine distance calculation
- **JSON API**: Clean data exchange between backend and frontend

## Quick Start
1. Compile the backend: `gcc -o trackmate trackmate.c -lm`
2. Run the backend: `./trackmate`
3. Open `index.html` in a web browser
4. See the shortest route visualized on the map!

## Architecture
```
GPS Coordinates → C Backend → Dijkstra's Algorithm → JSON Output → Web Visualization
```

## Sample Route
Mumbai (Bandra) → Mumbai (Andheri) via multiple intermediate points