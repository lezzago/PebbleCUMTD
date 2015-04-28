#include <pebble.h>
//#include "appmessage.h"
#include "favorites.h"
#include "main.h"
#include "stops.h"

static Departure departs[5];
int departures_num = 0;
char * selected_stop;

static Window* window;
Window* windowother;
int removewindow = 0;
static MenuLayer *menu_layer;

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
    removewindow = shouldremove;
    windowother = window;
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
 
    Tuplet value = TupletCString(STOPID, val);
    dict_write_tuplet(iter, &value);
 
    dict_write_end(iter);
    app_message_outbox_send();
}


void favorites_draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
  switch(cell_index->row)
    {
    case 0:
        menu_cell_basic_draw(ctx, cell_layer, "One Illinois North", NULL, NULL);
        break;
    case 1:
        menu_cell_basic_draw(ctx, cell_layer, "Goodwin and Main", NULL, NULL);
        break;
    case 2:
        menu_cell_basic_draw(ctx, cell_layer, "Illni Union", NULL, NULL);
        break;
    }
}

static void favorites_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, "                CUMTD");
}

uint16_t favorites_num_sections_callback(struct MenuLayer *menu_layer, void *callback_context) {
  return 1;
}

int16_t favorites_header_height_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
  return 16;
}

uint16_t favorites_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
  return 3;
}
 
void favorites_select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
   switch(cell_index->row)
   {
     case 0:
       selected_stop = malloc(sizeof(char) * (strlen("ONENORTH") + 1));
       strcpy(selected_stop, "ONENORTH");
       send_stop("ONENORTH", 0, NULL);
       
       //        caution_init();
       //remove this window frmo the stack
       //window_stack_remove(window, false);
       break;
     case 1:
       selected_stop = malloc(sizeof(char) * (strlen("GWNMN") + 1));
       strcpy(selected_stop, "GWNMN");
       send_stop("GWNMN", 0, NULL);
       
       //window_stack_remove(window, false);
       break;
     case 2:
       selected_stop = malloc(sizeof(char) * (strlen("IU") + 1));
       strcpy(selected_stop, "IU");
       send_stop("IU", 0, NULL);
       break;
  }
}

void favorites_window_load(Window *window)
{
  //Create it - 12 is approx height of the top bar
  menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));

  //Let it receive clicks
  menu_layer_set_click_config_onto_window(menu_layer, window);

  //Give it its callbacks
  MenuLayerCallbacks callbacks = {
    .draw_header = (MenuLayerDrawHeaderCallback) favorites_menu_draw_header_callback,
    .draw_row = (MenuLayerDrawRowCallback) favorites_draw_row_callback,
    .get_num_sections = (MenuLayerGetNumberOfSectionsCallback) favorites_num_sections_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) favorites_num_rows_callback,
    .get_header_height = (MenuLayerGetHeaderHeightCallback) favorites_header_height_callback,
    .select_click = (MenuLayerSelectCallback) favorites_select_click_callback
  };
  menu_layer_set_callbacks(menu_layer, NULL, callbacks);

  //Add to Window
  layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));

}
 
void favorites_window_unload(Window *window)
{
  menu_layer_destroy(menu_layer);

}

void favorites_init()
{
    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_sent(out_sent_handler);
  	app_message_register_outbox_failed(out_failed_handler);
  	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
    window = window_create();
    WindowHandlers handlers = {
        .load = favorites_window_load,
        .unload = favorites_window_unload
    };
    window_set_window_handlers(window, (WindowHandlers) handlers);
    window_stack_push(window, true);
    
    
}
 
void favorites_deinit()
{
    window_destroy(window);
}
