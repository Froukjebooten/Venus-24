#include "map.h"
#include <assert.h>
#include <stddef.h>

void init_map(Map *map) {
  for (size_t c = 0; c < MAX_COLS; c++) {
    for (size_t r = 0; r < MAX_ROWS; r++) {
      map->cells[c][r].type = Undiscovered;
    }
  }
}

void update_undiscovered_cell(Map *map, size_t r, size_t c, CellType type) {
  // assert that the cell is undiscovered
  // TODO: support an already discovered cell with smarter heuristics
  assert(map->cells[r][c].type == Undiscovered);
  // update the cell
  map->cells[r][c].type = type;
}

void handle_robot_event(Map *map, RobotEvent *event) {
  // TODO: handle collisions
  if (event->type == DiscoveredLine) {
    // the line event contains the initial position of the line and the current
    // position at the point the event was recorded
    // assert that this is a straight line in x or y direction
    assert(event->data.line.initial_column == event->column ||
           event->data.line.initial_row == event->row);

    // record the line in the map
    for (size_t c = event->data.line.initial_column; c <= event->column; c++) {
      for (size_t r = event->data.line.initial_row; r <= event->row; r++) {
        update_undiscovered_cell(map, r, c, Empty);
      }
    }
  } else if (event->type == DiscoveredBlock) {
    update_undiscovered_cell(map, event->row, event->column, Block);
  }
}
