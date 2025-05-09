#include "../blocks.h"
#include "../map.h"
#include "../map_visualizer.h"
#include "../robot_events.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

static Map satellite_map;
static MapVisualizer visualizer;

// Function to place blocks in a pattern
void place_blocks(Map *map, int step) {
    // Place blocks in a checkerboard pattern for the current ring only
    int ring_size = (step + 1) * 3;
    
    // Start from center coordinates
    int row = CENTER_ROW;
    int col = CENTER_COL;
    
    // Get the current color based on step
    BlockColor current_color = (BlockColor)(step % 4); // Cycle through colors
    
    // Place blocks only in the current ring
    for (int r = row - ring_size; r <= row + ring_size; r++) {
        for (int c = col - ring_size; c <= col + ring_size; c++) {
            // Skip if outside map boundaries
            if (r < 0 || r >= MAX_ROWS || c < 0 || c >= MAX_COLS) {
                continue;
            }
            
            // Only place blocks on the outer ring
            if (r == row - ring_size || r == row + ring_size ||
                c == col - ring_size || c == col + ring_size) {
                // Place blocks in a checkerboard pattern
                if ((r + c) % 2 == 0) {
                    map->cells[r][c].type = Block;
                    map->cells[r][c].data.block.size = Small;
                    map->cells[r][c].data.block.color = current_color;
                } else {
                    map->cells[r][c].type = Empty;
                }
            }
        }
    }
}

int main() {
    printf("Colored Blocks Pattern Test Starting...\n");
    printf("Colors will cycle through: Green -> Red -> Blue -> Black\n");

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
            place_blocks(&satellite_map, step++);
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