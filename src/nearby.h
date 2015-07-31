#include <pebble.h>
#ifndef NEARBY_H
#define NEARBY_H

typedef struct st {
  char stop_id [16];
  char stop_name [32];
} Stop;
  
void allow_nearby_select();
void nearby_init(Stop stps[], int departures_num);

#endif