#pragma once
#include "pebble.h"
  
// enum{
//   STOPID = 0,
// };

// typedef struct dep {
//   char headsign [50];
//   char expected_time [20];
// } Departure;
  
void appmessage_init(void);

void send_stop(char* val, int shouldremove, Window* window);

void send_nearby(char* val, int shouldremove, Window* window);