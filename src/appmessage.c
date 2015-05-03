#include <pebble.h>
#include "appmessage.h"
#include "stops.h"
#include "favorites.h"
  
Window* windowother;
int removewindow = 0;
static Departure departs[5];
int departures_num = 0;
char * selected_stop;

  
static void parse(char* str) {
  char sign [50] = "";
	char time [8] = "";
  int starttime = 0;
  int startsign = 0;
  departures_num = 0;
  for(; *str; str++) {
    if(*str == '[')
		{
			startsign = 1;
		}
		else if(*str == ';')
		{
			startsign = 0;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "SIGN: %s", sign);
      strcpy(departs[departures_num].headsign, sign);
      strcpy(sign, "");
			starttime = 1;			
		}
		else if(*str == ']')
		{
			starttime = 0;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "TIME: %s", time);
      strcpy(departs[departures_num].expected_time, time);
      strcpy(time, "");
      departures_num++;
		}
		else
		{
			if(startsign == 1)
			{
				strncat(sign, &(*str), 1);
			}
      else if(starttime == 1)
      {
        strncat(time, &(*str), 1);
      }
		}
  }
  if(removewindow == 1) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "REMOVE WINDOW");
    window_stack_remove(windowother, false);
  }
  
  stops_init(departs, departures_num, selected_stop);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char headsign_buffer[512];
  APP_LOG(APP_LOG_LEVEL_DEBUG, "receive callback");

  // Read first item
  Tuple *t = dict_read_first(iterator);
  //int count = 0;
  // For all items
  while(t != NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "The value is %s!", t->value->cstring);
    // Which key was received?
    switch(t->key) {
    case KEY_HEADSIGN:
      snprintf(headsign_buffer, sizeof(headsign_buffer), "%s", t->value->cstring);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Headsign is %s!", headsign_buffer);
      //parse(headsign_buffer);
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %s not recognized!", t->value->cstring);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  parse(headsign_buffer);
}



static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void out_sent_handler(DictionaryIterator *sent, void *context) {
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Can't send to phone");
}

void send_stop(char* val, int shouldremove, Window* window)
{
    selected_stop = malloc(sizeof(char) * (strlen(val) + 1));
    strcpy(selected_stop, val);
    removewindow = shouldremove;
    windowother = window;
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
 
    Tuplet value = TupletCString(STOPID, val);
    dict_write_tuplet(iter, &value);
 
    dict_write_end(iter);
    app_message_outbox_send();
}

void send_nearby(char* val, int shouldremove, Window* window)
{  
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
}


void appmessage_init(void) {
    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_sent(out_sent_handler);
  	app_message_register_outbox_failed(out_failed_handler);
  	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
}

