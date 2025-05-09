#ifndef __MAP_H__
#define __MAP_H__

#include <stdint.h>
#include "blocks.h"
#include "robot_events.h"

#define MAX_ROWS (100)
#define MAX_COLS (100)
#define CENTER_ROW (MAX_ROWS / 2)
#define CENTER_COL (MAX_COLS / 2)

typedef enum {
  Undiscovered = 0,
  Empty = 1,
  Mountain = 2,
  Cliff = 3,
  Block = 4
} CellType;

typedef struct {
  CellType type;
  union {
    BlockInformation block;
  } data;
} Cell;

typedef struct {
  Cell cells[MAX_ROWS][MAX_COLS];
} Map;


void init_map(Map* map);


void handle_robot_event(Map* map, RobotEvent* event);


#endif

