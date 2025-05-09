#ifndef __ROBOT_EVENTS_H__
#define __ROBOT_EVENTS_H__

#include <stdint.h>
#include <stddef.h>
#include "blocks.h"

typedef enum {
    DiscoveredLine  = 0,
    DiscoveredBlock  = 1,
    DiscoveredCliff = 2
} EventType;

typedef struct {
    size_t initial_row;
    size_t initial_column;
} LineInformation;

typedef struct {
  EventType type;
  size_t row;
  size_t column;
  union {
    BlockInformation block;
    LineInformation line;
  } data;
} RobotEvent;


typedef struct __attribute__((packed)) {
  uint8_t size: 1;
  uint8_t color: 2;
} TransmittedBlockInformation;

typedef struct __attribute__((packed)) {
    uint8_t type: 1;
    uint32_t row;
    uint32_t column;
    union {
        TransmittedBlockInformation block_information;
    } data;
} TransmittedRobotEvent;

RobotEvent parse_robot_event(TransmittedRobotEvent* data, size_t num_bits);



#endif
