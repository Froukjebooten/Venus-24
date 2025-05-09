#include "../map.h"
#include "../robot_events.h"
#include "../map_visualizer.h"
#include <stdio.h>
#include <SDL2/SDL.h>

static Map satellite_map;
static MapVisualizer visualizer;

// Function to simulate cliff detection events
void simulate_cliff_detection(Map *map, int step) {
    // Create a ring of cliffs that expands outward
    int ring_size = (step + 1) * 2;
    
    // Start from center coordinates
    int row = CENTER_ROW;
    int col = CENTER_COL;
    
    // Create a cliff detection event
    RobotEvent cliff_event = {
        .type = DiscoveredCliff,
        .row = row + ring_size,
        .column = col + ring_size
    };
    
    // Handle the cliff event
    handle_robot_event(map, &cliff_event);
    
    // Create another cliff event on the opposite side
    cliff_event.row = row - ring_size;
    cliff_event.column = col - ring_size;
    handle_robot_event(map, &cliff_event);
}

int main() {
    printf("Cliff Detection Test Starting...\n");
    printf("Watch as cliffs are detected in an expanding pattern\n");

    // Initialize components
    init_map(&satellite_map);
    if (init_visualizer(&visualizer, 1920, 1080, 20.0f) < 0) {
        printf("Failed to initialize visualizer\n");
        return -1;
    }

    // Main satellite loop
    SDL_Event event;
    int quit = 0;
    int step = 0;
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
            simulate_cliff_detection(&satellite_map, step++);
            last_update = current_time;
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