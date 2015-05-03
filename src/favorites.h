#include <pebble.h>
#ifndef FAVORITES_H
#define FAVORITES_H
  
enum{
  STOPID = 1,
  KEY_HEADSIGN = 0,
  KEY_EXPECTED = 2
};
  
void favorites_init();

void allow_select();

void send_stop(char * val, int shouldremove, Window* window);

#endif