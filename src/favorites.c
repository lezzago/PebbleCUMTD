#include <pebble.h>
//#include "appmessage.h"
#include "favorites.h"
#include "main.h"
#include "stops.h"

static Departure departs[5];
int departures_num = 0;

static Window* window;
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
  stops_init(departs, departures_num);
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

void send_stop(char* val)
{
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
 
    Tuplet value = TupletCString(STOPID, val);
    dict_write_tuplet(iter, &value);
 
    dict_write_end(iter);
    app_message_outbox_send();
}

// static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
//   // Store incoming information
//   static char headsign_buffer[512];
//   static char expected_time_buffer[32];
//   APP_LOG(APP_LOG_LEVEL_DEBUG, "receive callback");
  
// //   Tuple *sign = dict_find(iterator, 0);
// //   Tuple *esttime = dict_find(iterator, 1);
  
// //   while (1) {
// //     if(!sign) {
// //     APP_LOG(APP_LOG_LEVEL_DEBUG, "NO STOP");
// //     }
// //     else {
// //       char *stopString = sign->value->cstring;
// //       APP_LOG(APP_LOG_LEVEL_DEBUG, "HAVE STOP: %s", stopString);
// //     }
// //     if(!esttime) {
// //       APP_LOG(APP_LOG_LEVEL_DEBUG, "NO time");
// //     }
// //     else {
// //       APP_LOG(APP_LOG_LEVEL_DEBUG, "HAVE time");
// //     }
// //     if(sign && esttime) {
// //       char *stopString = sign->value->cstring;
// //       char *codeString = esttime->value->cstring;
    
// //       APP_LOG(APP_LOG_LEVEL_DEBUG, "Stop: %s", stopString);
// //       APP_LOG(APP_LOG_LEVEL_DEBUG, "Time %s", codeString);
// //     }
// //     else {
// //       break;
// //     }
// //   }
  
  
//   // Read first item
//   Tuple *t = dict_read_first(iterator);
//   //int count = 0;
//   // For all items
//   while(t != NULL) {
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "The value is %s!", t->value->cstring);
//     // Which key was received?
//     switch(t->key) {
//     case KEY_HEADSIGN:
//       snprintf(headsign_buffer, sizeof(headsign_buffer), "%s", t->value->cstring);
// //       deps[count].headsign = malloc(sizeof(char) * (strlen(headsign_buffer) + 1));
// //       strcpy(deps[count].headsign, headsign_buffer);
//       APP_LOG(APP_LOG_LEVEL_DEBUG, "Headsign is %s!", headsign_buffer);
//       break;
//     case KEY_EXPECTED:
//       snprintf(expected_time_buffer, sizeof(expected_time_buffer), "%s", t->value->cstring);
// //       deps[count].expected_time = malloc(sizeof(char) * (strlen(expected_time_buffer) + 1));
// //       strcpy(deps[count].expected_time, expected_time_buffer);
//       APP_LOG(APP_LOG_LEVEL_DEBUG, "Expected time is %s!", expected_time_buffer);
//       break;
//     default:
//       APP_LOG(APP_LOG_LEVEL_ERROR, "Key %s not recognized!", t->value->cstring);
//       break;
//     }

//     // Look for next item
//     t = dict_read_next(iterator);
//   }
// //  APP_LOG(APP_LOG_LEVEL_DEBUG, "Count is %d", (int)count);
  
//   // Assemble full string and display
//   //snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
//   //text_layer_set_text(s_weather_layer, weather_layer_buffer);
// }

// static void inbox_dropped_callback(AppMessageResult reason, void *context) {
//   APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
// }

// static void out_sent_handler(DictionaryIterator *sent, void *context) {
// }

// static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
// 	APP_LOG(APP_LOG_LEVEL_DEBUG, "Can't send to phone");
// }

// void send_stop(char* val)
// {
//     DictionaryIterator *iter;
//     app_message_outbox_begin(&iter);
 
//     Tuplet value = TupletCString(STOPID, val);
//     dict_write_tuplet(iter, &value);
 
//     dict_write_end(iter);
//     app_message_outbox_send();
// }

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
       send_stop("ONENORTH");
       
       //        caution_init();
       //remove this window frmo the stack
       //window_stack_remove(window, false);
       break;
     case 1:
       send_stop("GWNMN");
       
       //window_stack_remove(window, false);
       break;
     case 2:
       send_stop("IU");
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


/*
 uint16_t favorites_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
 {
   return 2;
 }


void favorites_select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
  switch(cell_index->row)
   {
     case 0:
      //send_int(0);
      init();
      //remove this window frmo the stack
      //window_stack_remove(window, false);
      break;
    case 1:
      //send_int(3);
      //emergency_conf_init();
      //remove this window frmo the stack
      //window_stack_remove(window, false);
      break;
   }
}

void favorites_window_load(Window *window)
{
  //Create it - 12 is approx height of the top bar
  menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));

  //Let it receive clicks
  //menu_layer_set_click_config_onto_window(menu_layer, window);

  //Give it its callbacks
  MenuLayerCallbacks callbacks = {
    .draw_row = (MenuLayerDrawRowCallback) favorites_draw_row_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) favorites_num_rows_callback,
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

void favorites_upClickHandler(ClickRecognizerRef recognizer, void *context) {
  menu_layer_set_selected_next(menu_layer, true, MenuRowAlignCenter, false);
}

void favorites_downClickHandler(ClickRecognizerRef recognizer, void *context) {
  menu_layer_set_selected_next(menu_layer, false, MenuRowAlignCenter, false);
}

void favorites_selectClickHandler(ClickRecognizerRef recognizer, void *context) {
  MenuIndex index = menu_layer_get_selected_index(menu_layer);
  favorites_select_click_callback(menu_layer, &index, NULL);
}

void favorites_click_config_provider(ClickRecognizerRef recognizer, void *context) {
  window_single_click_subscribe(BUTTON_ID_BACK, NULL);
  window_single_click_subscribe(BUTTON_ID_UP, favorites_upClickHandler);
  window_single_click_subscribe(BUTTON_ID_DOWN, favorites_downClickHandler);
  window_single_click_subscribe(BUTTON_ID_SELECT, favorites_selectClickHandler);
}
  
void favorites_init()
{
    window = window_create();
    WindowHandlers handlers = {
        .load = favorites_window_load,
        .unload = favorites_window_unload
    };
    window_set_window_handlers(window, (WindowHandlers) handlers);
    window_set_click_config_provider(window, (ClickConfigProvider) favorites_click_config_provider);
    window_stack_push(window, true);
}
 
void favorites_deinit()
{
    window_destroy(window);
}
 
*/
