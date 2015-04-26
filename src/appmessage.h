#pragma once
#include "pebble.h"
  
enum{
  STOPID = 0
};

void appmessage_init(void);

void send_stop(char* val);