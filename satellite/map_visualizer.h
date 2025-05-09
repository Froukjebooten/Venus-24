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
    int cell_size;
} MapVisualizer;

// Initialize the visualizer and create the window
// Returns 0 on success, -1 on failure
int init_visualizer(MapVisualizer* visualizer, int window_width, int window_height, int cell_size);

// Clean up SDL resources
void cleanup_visualizer(MapVisualizer* visualizer);

// Update the window with the current map state
void update_visualization(MapVisualizer* visualizer, const Map* map);

#endif // MAP_VISUALIZER_H 