#include "map_visualizer.h"
#include <stdio.h>

int init_visualizer(MapVisualizer* visualizer, int window_width, int window_height, int cell_size) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    visualizer->window_width = window_width;
    visualizer->window_height = window_height;
    visualizer->cell_size = cell_size;

    visualizer->window = SDL_CreateWindow(
        "Venus Map Visualization",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        window_width,
        window_height,
        SDL_WINDOW_SHOWN
    );

    if (!visualizer->window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    visualizer->renderer = SDL_CreateRenderer(
        visualizer->window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!visualizer->renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(visualizer->window);
        return -1;
    }

    return 0;
}

void cleanup_visualizer(MapVisualizer* visualizer) {
    if (visualizer->renderer) {
        SDL_DestroyRenderer(visualizer->renderer);
    }
    if (visualizer->window) {
        SDL_DestroyWindow(visualizer->window);
    }
    SDL_Quit();
}

void update_visualization(MapVisualizer* visualizer, const Map* map) {
    // Clear the screen
    SDL_SetRenderDrawColor(visualizer->renderer, 0, 0, 0, 255);
    SDL_RenderClear(visualizer->renderer);

    // Draw each cell
    for (size_t c = 0; c < MAX_COLS; c++) {
        for (size_t r = 0; r < MAX_ROWS; r++) {
            SDL_Rect cell_rect = {
                c * visualizer->cell_size,
                r * visualizer->cell_size,
                visualizer->cell_size,
                visualizer->cell_size
            };

            // Set color based on cell type
            switch (map->cells[c][r].type) {
                case Undiscovered:
                    SDL_SetRenderDrawColor(visualizer->renderer, 50, 50, 50, 255);  // Dark gray
                    break;
                case Empty:
                    SDL_SetRenderDrawColor(visualizer->renderer, 200, 200, 200, 255);  // Light gray
                    break;
                case Block:
                    SDL_SetRenderDrawColor(visualizer->renderer, 100, 100, 100, 255);  // Medium gray
                    break;
                default:
                    SDL_SetRenderDrawColor(visualizer->renderer, 255, 0, 0, 255);  // Red for unknown types
            }

            SDL_RenderFillRect(visualizer->renderer, &cell_rect);
        }
    }

    // Draw grid lines
    SDL_SetRenderDrawColor(visualizer->renderer, 100, 100, 100, 255);
    for (size_t c = 0; c <= MAX_COLS; c++) {
        SDL_RenderDrawLine(
            visualizer->renderer,
            c * visualizer->cell_size, 0,
            c * visualizer->cell_size, MAX_ROWS * visualizer->cell_size
        );
    }
    for (size_t r = 0; r <= MAX_ROWS; r++) {
        SDL_RenderDrawLine(
            visualizer->renderer,
            0, r * visualizer->cell_size,
            MAX_COLS * visualizer->cell_size, r * visualizer->cell_size
        );
    }

    // Update the screen
    SDL_RenderPresent(visualizer->renderer);
} 