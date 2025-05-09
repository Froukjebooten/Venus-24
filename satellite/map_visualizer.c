#include "map_visualizer.h"
#include <stdio.h>
#include <sys/param.h>

// Function to reset view to initial position and zoom
static void reset_view(MapVisualizer* visualizer) {
    // Reset cell size to initial value
    visualizer->cell_size = 20.0f;
    visualizer->zoom_level = 1.0f;
    
    // Center the map
    visualizer->offset_x = (visualizer->window_width - (MAX_COLS * visualizer->cell_size)) / 2;
    visualizer->offset_y = (visualizer->window_height - (MAX_ROWS * visualizer->cell_size)) / 2;
}

// Common function to handle zooming
static void handle_zoom(MapVisualizer* visualizer, float zoom_factor, int center_x, int center_y) {
    // Store old cell size for calculations
    float old_cell_size = visualizer->cell_size;
    
    // Calculate and clamp new cell size
    float new_cell_size = visualizer->cell_size * zoom_factor;
    new_cell_size = MAX(new_cell_size, visualizer->min_cell_size);
    new_cell_size = MIN(new_cell_size, visualizer->max_cell_size);
    
    // Calculate center position relative to the map before zoom
    float map_x = (center_x - visualizer->offset_x) / old_cell_size;
    float map_y = (center_y - visualizer->offset_y) / old_cell_size;
    
    // Update cell size
    visualizer->cell_size = new_cell_size;
    visualizer->zoom_level *= zoom_factor;
    
    // Adjust offset to zoom towards center point
    visualizer->offset_x = center_x - (map_x * new_cell_size);
    visualizer->offset_y = center_y - (map_y * new_cell_size);
}

int init_visualizer(MapVisualizer* visualizer, int window_width, int window_height, float cell_size) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    visualizer->window_width = window_width;
    visualizer->window_height = window_height;
    visualizer->cell_size = cell_size;
    
    // Initialize panning state
    visualizer->offset_x = 0;
    visualizer->offset_y = 0;
    visualizer->is_panning = 0;
    visualizer->last_mouse_x = 0;
    visualizer->last_mouse_y = 0;
    
    // Initialize zoom state
    visualizer->zoom_level = 1.0f;
    visualizer->min_cell_size = 2.0f;    // Allow zooming out more
    visualizer->max_cell_size = 100.0f;  // Maximum zoom level

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

    // Center the map initially
    visualizer->offset_x = (window_width - (MAX_COLS * cell_size)) / 2;
    visualizer->offset_y = (window_height - (MAX_ROWS * cell_size)) / 2;

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

void handle_visualizer_events(MapVisualizer* visualizer, SDL_Event* event) {
    switch (event->type) {
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button == SDL_BUTTON_LEFT) {
                visualizer->is_panning = 1;
                visualizer->last_mouse_x = event->button.x;
                visualizer->last_mouse_y = event->button.y;
            }
            break;
            
        case SDL_MOUSEBUTTONUP:
            if (event->button.button == SDL_BUTTON_LEFT) {
                visualizer->is_panning = 0;
            }
            break;
            
        case SDL_MOUSEMOTION:
            if (visualizer->is_panning) {
                int dx = event->motion.x - visualizer->last_mouse_x;
                int dy = event->motion.y - visualizer->last_mouse_y;
                
                visualizer->offset_x += dx;
                visualizer->offset_y += dy;
                
                visualizer->last_mouse_x = event->motion.x;
                visualizer->last_mouse_y = event->motion.y;
            }
            break;
            
        case SDL_MOUSEWHEEL:
            {
                // Get mouse position for zooming towards cursor
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                
                // Calculate zoom factor (1.1 for zoom in, 0.9 for zoom out)
                float zoom_factor = event->wheel.y > 0 ? 1.1f : 0.9f;
                
                handle_zoom(visualizer, zoom_factor, mouse_x, mouse_y);
            }
            break;
            
        case SDL_MULTIGESTURE:
            {
                // Handle pinch-to-zoom gesture
                if (event->mgesture.numFingers == 2) {
                    // Get the center of the gesture
                    int center_x = event->mgesture.x * visualizer->window_width;
                    int center_y = event->mgesture.y * visualizer->window_height;
                    
                    // Calculate zoom factor based on gesture
                    float zoom_factor = 1.0f + event->mgesture.dDist;
                    
                    handle_zoom(visualizer, zoom_factor, center_x, center_y);
                }
            }
            break;

        case SDL_KEYDOWN:
            if (event->key.keysym.sym == SDLK_c) {
                reset_view(visualizer);
            }
            break;
    }
}

void update_visualization(MapVisualizer* visualizer, const Map* map) {
    // Clear the screen
    SDL_SetRenderDrawColor(visualizer->renderer, 0, 0, 0, 255);
    SDL_RenderClear(visualizer->renderer);

    // Calculate visible map boundaries
    int map_start_x = visualizer->offset_x;
    int map_start_y = visualizer->offset_y;
    int map_end_x = (int)(visualizer->offset_x + MAX_COLS * visualizer->cell_size);
    int map_end_y = (int)(visualizer->offset_y + MAX_ROWS * visualizer->cell_size);

    // Draw each cell
    for (size_t c = 0; c < MAX_COLS; c++) {
        for (size_t r = 0; r < MAX_ROWS; r++) {
            // Skip if we're outside array bounds
            if (r >= MAX_ROWS || c >= MAX_COLS) {
                continue;
            }

            SDL_Rect cell_rect = {
                (int)(c * visualizer->cell_size + visualizer->offset_x),
                (int)(r * visualizer->cell_size + visualizer->offset_y),
                (int)visualizer->cell_size,
                (int)visualizer->cell_size
            };

            // Skip drawing if cell is outside the window
            if (cell_rect.x + cell_rect.w < 0 || cell_rect.x > visualizer->window_width ||
                cell_rect.y + cell_rect.h < 0 || cell_rect.y > visualizer->window_height) {
                continue;
            }

            // Set color based on cell type
            switch (map->cells[r][c].type) {
                case Undiscovered:
                    SDL_SetRenderDrawColor(visualizer->renderer, 50, 50, 50, 255);  // Dark gray
                    break;
                case Empty:
                    SDL_SetRenderDrawColor(visualizer->renderer, 200, 200, 200, 255);  // Light gray
                    break;
                case Block:
                    // Set color based on block color
                    switch (map->cells[r][c].data.block.color) {
                        case Green:
                            SDL_SetRenderDrawColor(visualizer->renderer, 0, 200, 0, 255);  // Green
                            break;
                        case Red:
                            SDL_SetRenderDrawColor(visualizer->renderer, 200, 0, 0, 255);  // Red
                            break;
                        case Blue:
                            SDL_SetRenderDrawColor(visualizer->renderer, 0, 0, 200, 255);  // Blue
                            break;
                        case Black:
                            SDL_SetRenderDrawColor(visualizer->renderer, 50, 50, 50, 255);  // Black
                            break;
                        default:
                            SDL_SetRenderDrawColor(visualizer->renderer, 100, 100, 100, 255);  // Default gray
                    }
                    break;
                default:
                    SDL_SetRenderDrawColor(visualizer->renderer, 255, 0, 0, 255);  // Red for unknown types
            }

            SDL_RenderFillRect(visualizer->renderer, &cell_rect);
        }
    }

    // Draw grid lines
    SDL_SetRenderDrawColor(visualizer->renderer, 100, 100, 100, 255);
    
    // Draw vertical grid lines
    for (size_t c = 0; c <= MAX_COLS; c++) {
        int x = (int)(c * visualizer->cell_size + visualizer->offset_x);
        // Only draw if the line is within the map boundaries
        if (x >= map_start_x && x <= map_end_x) {
            SDL_RenderDrawLine(
                visualizer->renderer,
                x, map_start_y,
                x, map_end_y
            );
        }
    }
    
    // Draw horizontal grid lines
    for (size_t r = 0; r <= MAX_ROWS; r++) {
        int y = (int)(r * visualizer->cell_size + visualizer->offset_y);
        // Only draw if the line is within the map boundaries
        if (y >= map_start_y && y <= map_end_y) {
            SDL_RenderDrawLine(
                visualizer->renderer,
                map_start_x, y,
                map_end_x, y
            );
        }
    }

    // Draw map border in red
    SDL_SetRenderDrawColor(visualizer->renderer, 255, 0, 0, 255);
    int border_thickness = 2;  // Make the border more visible
    
    // Draw top border
    for (int i = 0; i < border_thickness; i++) {
        SDL_RenderDrawLine(
            visualizer->renderer,
            map_start_x, map_start_y + i,
            map_end_x, map_start_y + i
        );
    }
    
    // Draw bottom border
    for (int i = 0; i < border_thickness; i++) {
        SDL_RenderDrawLine(
            visualizer->renderer,
            map_start_x, map_end_y - i,
            map_end_x, map_end_y - i
        );
    }
    
    // Draw left border
    for (int i = 0; i < border_thickness; i++) {
        SDL_RenderDrawLine(
            visualizer->renderer,
            map_start_x + i, map_start_y,
            map_start_x + i, map_end_y
        );
    }
    
    // Draw right border
    for (int i = 0; i < border_thickness; i++) {
        SDL_RenderDrawLine(
            visualizer->renderer,
            map_end_x - i, map_start_y,
            map_end_x - i, map_end_y
        );
    }

    // Update the screen
    SDL_RenderPresent(visualizer->renderer);
} 