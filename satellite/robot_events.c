#include "robot_events.h"



RobotEvent parse_robot_event(TransmittedRobotEvent* data, size_t num_bits) {
    // TODO: panic
    RobotEvent e;
    if(num_bits >= 1) {
        // assume 1-to-1
        e.type = data->type;
        e.row = data->row;
        e.column = data->column;
        if (e.type == DiscoveredBlock) {
            e.data.block.size = data->data.block_information.size;
            e.data.block.color = data->data.block_information.color;
        }
    }
    return e;
}
