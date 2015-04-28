#include <pebble.h>
#ifndef STOPS_H
#define STOPS_H

// enum{
//   STOPID = 1,
//   KEY_HEADSIGN = 0,
//   KEY_EXPECTED = 2
// };

typedef struct dep {
  char headsign [50];
  char expected_time [20];
} Departure;
  
void stops_init(Departure departs[], int departure_num);

#endif