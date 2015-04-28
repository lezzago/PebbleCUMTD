#include <pebble.h>
//#include "appmessage.h"
//#include "stops.h"
#include "main.h"
#include "stops.h"
#include "favorites.h"


static Departure deps[5];
static char * cur_stop;
static Window* window;
static MenuLayer *menu_layer;
int num_departures = 5;

static void parse(char* str) {
  char sign [50] = "";
	char time [8] = "";
  int starttime = 0;
  int startsign = 0;
  num_departures = 0;
  for(; *str; str++) {
    if(*str == '[')
		{
			startsign = 1;
		}
		else if(*str == ';')
		{
			startsign = 0;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "SIGN: %s", sign);
      strcpy(deps[num_departures].headsign, sign);
      strcpy(sign, "");
			starttime = 1;			
		}
		else if(*str == ']')
		{
			starttime = 0;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "TIME: %s", time);
      strcpy(deps[num_departures].expected_time, time);
      strcpy(time, "");
      num_departures++;
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
  //remove this window frmo the stack
  //window_stack_remove(window, false);
  stops_init(deps, num_departures, cur_stop);
  
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

// void send_stop(char* val)
// {
//     DictionaryIterator *iter;
//     app_message_outbox_begin(&iter);
 
//     Tuplet value = TupletCString(STOPID, val);
//     dict_write_tuplet(iter, &value);
 
//     dict_write_end(iter);
//     app_message_outbox_send();
// }


void stops_draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
  menu_cell_basic_draw(ctx, cell_layer, deps[cell_index->row].headsign, deps[cell_index->row].expected_time, NULL);
}

static void stops_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, "                CUMTD");
}

uint16_t stops_num_sections_callback(struct MenuLayer *menu_layer, void *callback_context) {
  return 1;
}

int16_t stops_header_height_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
  return 16;
}

uint16_t stops_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
  return num_departures;
}
 
void stops_select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
  send_stop(cur_stop);
  
  switch(cell_index->row)
   {
     case 0:
      //send_stop(cur_stop);
      //send_int(0);
      //init();
      //remove this window frmo the stack
      //window_stack_remove(window, false);
      break;
    case 1:
      //send_stop(cur_stop);
      //send_int(3);
      //emergency_conf_init();
      //remove this window frmo the stack
      //window_stack_remove(window, false);
      break;
   }
}

void stops_window_load(Window *window)
{
  //Create it - 12 is approx height of the top bar
  menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));

  //Let it receive clicks
  menu_layer_set_click_config_onto_window(menu_layer, window);

  //Give it its callbacks
  MenuLayerCallbacks callbacks = {
    .draw_header = (MenuLayerDrawHeaderCallback) stops_menu_draw_header_callback,
    .draw_row = (MenuLayerDrawRowCallback) stops_draw_row_callback,
    .get_num_sections = (MenuLayerGetNumberOfSectionsCallback) stops_num_sections_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) stops_num_rows_callback,
    .get_header_height = (MenuLayerGetHeaderHeightCallback) stops_header_height_callback,
    .select_click = (MenuLayerSelectCallback) stops_select_click_callback
  };
  menu_layer_set_callbacks(menu_layer, NULL, callbacks);

  //Add to Window
  layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));

}
 
void stops_window_unload(Window *window)
{
  menu_layer_destroy(menu_layer);

}

void stops_init(Departure departs[], int departure_num, char * stop)
{

  APP_LOG(APP_LOG_LEVEL_DEBUG, "START COPYING OVER DEPARTURES");
    for (int i = 0; i < departure_num; i++) {
      strcpy(deps[i].headsign, departs[i].headsign);
      strcpy(deps[i].expected_time, departs[i].expected_time);
      
      APP_LOG(APP_LOG_LEVEL_DEBUG, "SIGN: %s", deps[i].headsign);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "TIME: %s", deps[i].expected_time);
    }
    cur_stop = malloc(sizeof(char) * (strlen(stop) + 1));
    strcpy(cur_stop, stop);
    num_departures = departure_num;    
  
    APP_LOG(APP_LOG_LEVEL_DEBUG, "start init");
    window = window_create();
    WindowHandlers handlers = {
        .load = stops_window_load,
        .unload = stops_window_unload
    };

    window_set_window_handlers(window, (WindowHandlers) handlers);
    window_stack_push(window, true);
}
 
void stops_deinit()
{
    window_destroy(window);
}

