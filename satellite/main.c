#include "blocks.h"
#include "map.h"
#include "map_visualizer.h"
#include "robot_events.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

static Map satellite_map;
static MapVisualizer visualizer;

void simulate_exploration(Map *map, int step) {
    // Calculate the size of the spiral ring for this step
    // Add a gap of 2 cells between rings
    int ring_size = (step + 1) * 3;  // Make rings bigger and spaced out
    
    // Start from center coordinates
    int row = CENTER_ROW;
    int col = CENTER_COL;
    
    // Only draw if we have a positive ring size
    if (ring_size > 0) {
        // Check if we've reached the map boundaries
        if (row - ring_size < 0 || row + ring_size >= MAX_ROWS ||
            col - ring_size < 0 || col + ring_size >= MAX_COLS) {
            return;  // Stop if we would exceed map boundaries
        }
        
        // Move right first
        for (int i = 0; i < ring_size; i++) {
            map->cells[row][col++].type = Empty;
        }
        
        // Move down
        for (int i = 0; i < ring_size; i++) {
            map->cells[row++][col].type = Empty;
        }
        
        // Move left
        for (int i = 0; i < ring_size + 1; i++) {
            map->cells[row][col--].type = Empty;
        }
        
        // Move up
        for (int i = 0; i < ring_size + 1; i++) {
            map->cells[row--][col].type = Empty;
        }
        
        // Move right to complete the spiral
        for (int i = 0; i < ring_size + 1; i++) {
            map->cells[row][col++].type = Empty;
        }
    }
}

int main() {
  printf("Satellite system starting...\n");

  // Initialize components
  init_map(&satellite_map);
  if (init_visualizer(&visualizer, 1920, 1080, 20.0f) < 0) {
    printf("Failed to initialize visualizer\n");
    return -1;
  }

  // Main satellite loop
  SDL_Event event;
  int quit = 0;
  int exploration_step = 0;
  Uint32 last_update = SDL_GetTicks();
  
  while (!quit) {
    // Handle SDL events
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
      // Handle visualizer events (panning)
      handle_visualizer_events(&visualizer, &event);
    }

    // Update map every second
    Uint32 current_time = SDL_GetTicks();
    if (current_time - last_update >= 1000) { // 1000ms = 1 second
      simulate_exploration(&satellite_map, exploration_step++);
      last_update = current_time;
      printf("Exploration step: %d\n", exploration_step);
    }

    // Update visualization
    update_visualization(&visualizer, &satellite_map);
    
    // Small delay to prevent high CPU usage
    SDL_Delay(16); // ~60 FPS
  }

  // Cleanup
  cleanup_visualizer(&visualizer);
  return 0;
}