#include <pebble.h>
#include "appmessage.h"
//#include "libs/pebble-assist.h"
//#include "skipstone.h"

// static void in_received_handler(DictionaryIterator *iter, void *context);
// static void in_dropped_handler(AppMessageResult reason, void *context);
static void out_sent_handler(DictionaryIterator *sent, void *context);
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context);

void appmessage_init(void) {
// 	app_message_register_inbox_received(in_received_handler);
// 	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_register_outbox_sent(out_sent_handler);
	app_message_register_outbox_failed(out_failed_handler);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

// static void in_received_handler(DictionaryIterator *iter, void *context) {
// 	skipstone_in_received_handler(iter);
// }

// static void in_dropped_handler(AppMessageResult reason, void *context) {
// }


static void out_sent_handler(DictionaryIterator *sent, void *context) {
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Can't send to phone");
}

/*void send_stop(char* val)
{
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
 
    Tuplet value = TupletCString(STOPID, val);
    dict_write_tuplet(iter, &value);
 
    dict_write_end(iter);
    app_message_outbox_send();
}*/
