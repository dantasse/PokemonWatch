#include <pebble.h>
#include "png.h"

static Window *s_main_window;
static TextLayer *s_time_layer;
static BitmapLayer *s_bitmap_layer;
static GBitmap *s_current_pokemon;
static int NUM_POKEMON = 60;
static char* s_names[] = {
  "MissingNo", "Bulbasaur", "Ivysaur", "Venusaur", "Charmander",
  "Charmeleon", "Charizard", "Squirtle", "Wartortle", "Blastoise",
  "Caterpie", "Metapod", "Butterfree", "Weedle", "Kakuna",
  "Beedrill", "Pidgey", "Pidgeotto", "Pidgeot", "Rattata",
  "Raticate", "Spearow", "Fearow", "Ekans", "Arbok",
  "Pikachu", "Raichu", "Sandshrew", "Sandslash", "Nidoran Female",
  "Nidorina", "Nidoqueen", "Nidoran Male", "Nidorino", "Nidoking",
  "Clefairy", "Clefable", "Vulpix", "Ninetales", "Jigglypuff",
  "Wigglytuff", "Zubat", "Golbat", "Oddish", "Gloom",
  "Vileplume", "Paras", "Parasect", "Venonat", "Venomoth",
  "Diglett", "Dugtrio", "Meowth", "Persian", "Psyduck",
  "Golduck", "Mankey", "Primeape", "Growlithe", "Arcanine"};

static uint32_t s_resource_ids[] = {
  RESOURCE_ID_POKEMON_0, RESOURCE_ID_POKEMON_1, RESOURCE_ID_POKEMON_2, RESOURCE_ID_POKEMON_3, RESOURCE_ID_POKEMON_4,
  RESOURCE_ID_POKEMON_5, RESOURCE_ID_POKEMON_6, RESOURCE_ID_POKEMON_7, RESOURCE_ID_POKEMON_8, RESOURCE_ID_POKEMON_9,
  RESOURCE_ID_POKEMON_10, RESOURCE_ID_POKEMON_11, RESOURCE_ID_POKEMON_12, RESOURCE_ID_POKEMON_13, RESOURCE_ID_POKEMON_14,
  RESOURCE_ID_POKEMON_15, RESOURCE_ID_POKEMON_16, RESOURCE_ID_POKEMON_17, RESOURCE_ID_POKEMON_18, RESOURCE_ID_POKEMON_19,
  RESOURCE_ID_POKEMON_20, RESOURCE_ID_POKEMON_21, RESOURCE_ID_POKEMON_22, RESOURCE_ID_POKEMON_23, RESOURCE_ID_POKEMON_24,
  RESOURCE_ID_POKEMON_25, RESOURCE_ID_POKEMON_26, RESOURCE_ID_POKEMON_27, RESOURCE_ID_POKEMON_28, RESOURCE_ID_POKEMON_29,
  RESOURCE_ID_POKEMON_30, RESOURCE_ID_POKEMON_31, RESOURCE_ID_POKEMON_32, RESOURCE_ID_POKEMON_33, RESOURCE_ID_POKEMON_34,
  RESOURCE_ID_POKEMON_35, RESOURCE_ID_POKEMON_36, RESOURCE_ID_POKEMON_37, RESOURCE_ID_POKEMON_38, RESOURCE_ID_POKEMON_39,
  RESOURCE_ID_POKEMON_40, RESOURCE_ID_POKEMON_41, RESOURCE_ID_POKEMON_42, RESOURCE_ID_POKEMON_43, RESOURCE_ID_POKEMON_44,
  RESOURCE_ID_POKEMON_45, RESOURCE_ID_POKEMON_46, RESOURCE_ID_POKEMON_47, RESOURCE_ID_POKEMON_48, RESOURCE_ID_POKEMON_49,
  RESOURCE_ID_POKEMON_50, RESOURCE_ID_POKEMON_51, RESOURCE_ID_POKEMON_52, RESOURCE_ID_POKEMON_53, RESOURCE_ID_POKEMON_54,
  RESOURCE_ID_POKEMON_55, RESOURCE_ID_POKEMON_56, RESOURCE_ID_POKEMON_57, RESOURCE_ID_POKEMON_58, RESOURCE_ID_POKEMON_59,
};

static int which_pokemon = 0;

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  which_pokemon = tick_time -> tm_min;
                   
  GBitmap *previous_pokemon = s_current_pokemon;
  s_current_pokemon = gbitmap_create_with_png_resource(s_resource_ids[which_pokemon]);
  bitmap_layer_set_bitmap(s_bitmap_layer, s_current_pokemon);
  text_layer_set_text(s_time_layer, s_names[which_pokemon]);
  // delete prev pokemon
  gbitmap_destroy(previous_pokemon);
}

static void main_window_load(Window *window) {
  s_current_pokemon = gbitmap_create_with_png_resource(RESOURCE_ID_POKEMON_0); // will be overwritten almost instantly
  s_bitmap_layer = bitmap_layer_create(GRect(0, 0, 144, 144));
  bitmap_layer_set_bitmap(s_bitmap_layer, s_current_pokemon);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));

  s_time_layer = text_layer_create(GRect(0, 144, 144, 24));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorClear);
  text_layer_set_text(s_time_layer, "");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}
static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  bitmap_layer_destroy(s_bitmap_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  update_time();
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
