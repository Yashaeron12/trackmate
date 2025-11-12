# Makefile for TrackMate GPS Tracker

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -lm
TARGET = trackmate
SOURCE = trackmate.c

# Default target
all: $(TARGET)

# Compile the C backend
$(TARGET): $(SOURCE)
	$(CC) $(SOURCE) -o $(TARGET) $(CFLAGS)
	@echo "✅ TrackMate backend compiled successfully!"
	@echo "Run './$(TARGET)' to generate route data"

# Run the backend and generate JSON
run: $(TARGET)
	./$(TARGET)
	@echo "✅ Route data generated!"
	@echo "Open 'index.html' in your browser to visualize the route"

# Clean compiled files
clean:
	rm -f $(TARGET) route_data.json
	@echo "🧹 Cleaned compiled files"

# Install dependencies (for web server)
install:
	@echo "📦 No dependencies to install for the C backend"
	@echo "💡 To serve the frontend locally, you can use:"
	@echo "   python -m http.server 8000"
	@echo "   Then visit: http://localhost:8000"

# Help target
help:
	@echo "TrackMate GPS Tracker - Build Commands:"
	@echo "  make        - Compile the backend"
	@echo "  make run    - Compile and run backend to generate route data"
	@echo "  make clean  - Remove compiled files"
	@echo "  make install- Show web server setup instructions"
	@echo "  make help   - Show this help message"

.PHONY: all run clean install help