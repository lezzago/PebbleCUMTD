#include <pebble.h>
#include "appmessage.h"
#include "favorites.h"
#include "main.h"
#include "stops.h"
#include "nearby.h"

static Stop stops[5];
static int num_stops = 5;
static Window* window;
static MenuLayer *menu_layer;
static int select_allowed = 1;

void allow_favorites_select() {
  select_allowed = 1;
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
  if(select_allowed == 1)
  {
    select_allowed = 0;
    switch(cell_index->row)
    {
      case 0:
        send_stop("ONENORTH", 0, NULL);
        break;
      case 1:
        send_stop("GWNMN", 0, NULL);
        break;
      case 2:
        send_stop("IU", 0, NULL);
        break;
    }
    //select_allowed = 1;
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

void favorites_init(Stop stps[], int departures_num)
{
    for (int i = 0; i < departures_num; i++) {
      strcpy(stops[i].stop_id, stps[i].stop_id);
      strcpy(stops[i].stop_name, stps[i].stop_name);
      
      APP_LOG(APP_LOG_LEVEL_DEBUG, "STOP ID: %s", stops[i].stop_id);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "STOP Name: %s", stops[i].stop_name);
    }
    //cur_stop = malloc(sizeof(char) * (strlen(stop) + 1));
    //strcpy(cur_stop, stop);
    num_stops = departures_num;
  
    select_allowed = 1;
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
