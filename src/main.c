#include <pebble.h>
#include "main.h"
#include "favorites.h"
// #include "appmessage.h"
// #include "emergency_conf.c"

static Window* window;
static MenuLayer *menu_layer;


void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
  switch(cell_index->row)
    {
    case 0:
        menu_cell_basic_draw(ctx, cell_layer, "Favorite Stops", NULL, NULL);
        break;
    case 1:
        menu_cell_basic_draw(ctx, cell_layer, "Find Stops", "Near me", NULL);
        break;
    }
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, "                CUMTD");
}

uint16_t num_sections_callback(struct MenuLayer *menu_layer, void *callback_context) {
  return 1;
}

int16_t header_height_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
  return 16;
}

uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
  return 2;
}
 
void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
   switch(cell_index->row)
   {
     case 0:
       favorites_init();
       break;
     case 1:
       //favorites_init();
       //window_stack_remove(window, false);
       break;
   }
}

void window_load(Window *window)
{
  //Create it - 12 is approx height of the top bar
  menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));

  //Let it receive clicks
  menu_layer_set_click_config_onto_window(menu_layer, window);

  //Give it its callbacks
  MenuLayerCallbacks callbacks = {
    .draw_header = (MenuLayerDrawHeaderCallback) menu_draw_header_callback,
    .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
    .get_num_sections = (MenuLayerGetNumberOfSectionsCallback) num_sections_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
    .get_header_height = (MenuLayerGetHeaderHeightCallback) header_height_callback,
    .select_click = (MenuLayerSelectCallback) select_click_callback
  };
  menu_layer_set_callbacks(menu_layer, NULL, callbacks);

  //Add to Window
  layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));

}
 
void window_unload(Window *window)
{
  menu_layer_destroy(menu_layer);

}

void init()
{
    window = window_create();
    WindowHandlers handlers = {
        .load = window_load,
        .unload = window_unload
    };
    window_set_window_handlers(window, (WindowHandlers) handlers);
    window_stack_push(window, true);
  
    //appmessage_init();
}
 
void deinit()
{
    window_destroy(window);
}
 
int main(void)
{
    init();
    app_event_loop();
    deinit();
}