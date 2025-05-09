#ifndef MAP_VISUALIZER_H
#define MAP_VISUALIZER_H

#include "map.h"
#include "SDL2/SDL.h"

// Structure to hold SDL-related data
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int window_width;
    int window_height;
    float cell_size;
    // Viewport offset for panning
    int offset_x;
    int offset_y;
    // Panning state
    int is_panning;
    int last_mouse_x;
    int last_mouse_y;
    // Zoom state
    float zoom_level;
    float min_cell_size;
    float max_cell_size;
} MapVisualizer;

// Initialize the visualizer and create the window
// Returns 0 on success, -1 on failure
int init_visualizer(MapVisualizer* visualizer, int window_width, int window_height, float cell_size);

// Clean up SDL resources
void cleanup_visualizer(MapVisualizer* visualizer);

// Update the window with the current map state
void update_visualization(MapVisualizer* visualizer, const Map* map);

// Handle SDL events for the visualizer
void handle_visualizer_events(MapVisualizer* visualizer, SDL_Event* event);

#endif // MAP_VISUALIZER_H 