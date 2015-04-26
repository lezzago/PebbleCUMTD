#include <pebble.h>
#include "stops.h"
#include "main.h"

 
static Window* window;
static MenuLayer *menu_layer;


void stops_draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
  switch(cell_index->row)
    {
    case 0:
        menu_cell_basic_draw(ctx, cell_layer, "I am Safe", NULL, NULL);
        break;
    case 1:
        menu_cell_basic_draw(ctx, cell_layer, "Call 911", "Send GPS to Police", NULL);
        break;
    }
}


 uint16_t stops_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
 {
   return 2;
 }


void stops_select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
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

void stops_window_load(Window *window)
{
  //Create it - 12 is approx height of the top bar
  menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));

  //Let it receive clicks
  //menu_layer_set_click_config_onto_window(menu_layer, window);

  //Give it its callbacks
  MenuLayerCallbacks callbacks = {
    .draw_row = (MenuLayerDrawRowCallback) stops_draw_row_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) stops_num_rows_callback,
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

void stops_upClickHandler(ClickRecognizerRef recognizer, void *context) {
  menu_layer_set_selected_next(menu_layer, true, MenuRowAlignCenter, false);
}

void stops_downClickHandler(ClickRecognizerRef recognizer, void *context) {
  menu_layer_set_selected_next(menu_layer, false, MenuRowAlignCenter, false);
}

void stops_selectClickHandler(ClickRecognizerRef recognizer, void *context) {
  MenuIndex index = menu_layer_get_selected_index(menu_layer);
  stops_select_click_callback(menu_layer, &index, NULL);
}

void stops_click_config_provider(ClickRecognizerRef recognizer, void *context) {
  window_single_click_subscribe(BUTTON_ID_BACK, NULL);
  window_single_click_subscribe(BUTTON_ID_UP, stops_upClickHandler);
  window_single_click_subscribe(BUTTON_ID_DOWN, stops_downClickHandler);
  window_single_click_subscribe(BUTTON_ID_SELECT, stops_selectClickHandler);
}
  
void stops_init()
{
    window = window_create();
    WindowHandlers handlers = {
        .load = stops_window_load,
        .unload = stops_window_unload
    };
    window_set_window_handlers(window, (WindowHandlers) handlers);
    window_set_click_config_provider(window, (ClickConfigProvider) stops_click_config_provider);
    window_stack_push(window, true);
}
 
void stops_deinit()
{
    window_destroy(window);
}
 

