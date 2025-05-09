#include "blocks.h"
#include "map.h"
#include "robot_events.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "map_visualizer.h"

static Map satellite_map;
static MapVisualizer visualizer;

int main() {
  printf("Satellite system starting...\n");

  // Initialize components
  init_map(&satellite_map);
  if (init_visualizer(&visualizer, 800, 800, 20) < 0) {
    printf("Failed to initialize visualizer\n");
    return -1;
  }

  // Main satellite loop
  SDL_Event event;
  int quit = 0;
  
  while (!quit) {
    // Handle SDL events
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
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