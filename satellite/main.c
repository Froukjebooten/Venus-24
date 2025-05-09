#include "blocks.h"
#include "map.h"
#include "map_visualizer.h"
#include "robot_events.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

static Map satellite_map;
static MapVisualizer visualizer;

// Test exploration pattern
void simulate_exploration(Map *map, int step) {
  // discover the entire row at index "step" from left to right
  RobotEvent line_event = {
      .type = DiscoveredLine,
      .row = step,
      .column = step,
      .data = {.line = {.initial_row = step, .initial_column = 0}}};

  // Handle the event
  handle_robot_event(map, &line_event);
}

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
  int exploration_step = 0;
  Uint32 last_update = SDL_GetTicks();

  while (!quit) {
    // Handle SDL events
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
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