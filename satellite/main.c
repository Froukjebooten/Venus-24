#include <stdio.h>
#include <unistd.h>
#include "map.h"
#include "blocks.h"
#include "robot_events.h"
#include <sys/time.h>

static Map satellite_map;


int main() {
    printf("Satellite system starting...\n");
    
    // Initialize components
    init_map(&satellite_map);
    
    // Main satellite loop
    while (1) {
        // update map
        // sleep
        sleep(1);
    }
    
    return 0;
} 