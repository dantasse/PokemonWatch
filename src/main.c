#include <pebble.h>
#include "png.h"

static Window *s_main_window;
static TextLayer *s_time_layer;
static BitmapLayer *s_bitmap_layer;
// static GBitmap *s_previous_pokemon;
static GBitmap *s_current_pokemon;
static int NUM_POKEMON = 12;
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

static int POKEMON_PER_SPRITE = 6;
static uint32_t s_sprite_resource_ids[] = {
  RESOURCE_ID_POKEMON_0_5,
  RESOURCE_ID_POKEMON_6_11,
  RESOURCE_ID_POKEMON_12_17,
  RESOURCE_ID_POKEMON_18_23,
  RESOURCE_ID_POKEMON_24_29,
};
static GBitmap *s_current_sprite;

static int which_pokemon = 1;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  which_pokemon = (which_pokemon + 1) % NUM_POKEMON;
  //GBitmap *previous_sprite = s_current_sprite;
  //s_current_sprite = gbitmap_create_with_png_resource(RESOURCE_ID_POKEMON_0_11);
//   which_sprite = which_pokemon / POKEMON_PER_SPRITE;
  // TODO ^^^
  int offset = (which_pokemon % POKEMON_PER_SPRITE) * 144;
  
  APP_LOG(APP_LOG_LEVEL_DEBUG,"hello");
  GRect sub_rect = GRect(0, 0, 144, 144);
  //GRect sub_rect = GRect(0,0,20,20);
  GBitmap *previous_pokemon = s_current_pokemon;
  s_current_pokemon = gbitmap_create_as_sub_bitmap(s_current_sprite, GRect(offset,0,144,144));	
  //s_current_pokemon = gbitmap_create_with_png_resource(s_resource_ids[which_pokemon]);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "here is sub_rect %d", sub_rect);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "here is s_current_pokemon %p", s_current_pokemon);
  bitmap_layer_set_bitmap(s_bitmap_layer, s_current_pokemon);
  text_layer_set_text(s_time_layer, s_names[which_pokemon]);
  APP_LOG(APP_LOG_LEVEL_DEBUG,"made it to here...");
  // delete prev pokemon
  gbitmap_destroy(previous_pokemon);
}

static void main_window_load(Window *window) {
  s_current_sprite = gbitmap_create_with_png_resource(RESOURCE_ID_POKEMON_0_5);
  s_current_pokemon = gbitmap_create_as_sub_bitmap(s_current_sprite, GRect(0, 0, 144, 144));	
  // TODO remove this^^^
  // TODO make it start from the real time, not from 0.
  s_bitmap_layer = bitmap_layer_create(GRect(0, 0, 144, 144));
  bitmap_layer_set_bitmap(s_bitmap_layer, s_current_pokemon);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));

  s_time_layer = text_layer_create(GRect(0, 144, 144, 24));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorClear);
  text_layer_set_text(s_time_layer, "");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
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
//   tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  
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
