#include "map.h"
#include <stddef.h>

void init_map(Map *map) {
  for (size_t c = 0; c < MAX_COLS; c++) {
    for (size_t r = 0; r < MAX_ROWS; r++) {
      map->cells[c][r].type = Undiscovered;
    }
  }
}



void handle_robot_event(Map* map, RobotEvent* event) {
    // TODO: handle collisions
    if(event->type == DiscoveredLine) {
        // TODO: record line
    } else if (event->type == DiscoveredBlock) {
        map->cells[event->row][event->column].type = Block;
    }
    // TODO: "discovered" cells vs "undiscovered"
}
