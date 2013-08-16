/**
 * Periodic elements
 *
 * Basic Pebble watchface that displays the chemical
 * element with the atomic number corresponding to
 * the current minute.
 */

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "periodic-elements.h"

#define MY_UUID { 0xBD, 0x4C, 0x76, 0xFC, 0x8E, 0xB6, 0x4F, 0x7A, 0x82, 0xA0, 0x3C, 0x85, 0xD0, 0xCF, 0xFC, 0x6A }
PBL_APP_INFO(MY_UUID,
             "Periodic Elements", "David Doran",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
Layer border_layer;
TextLayer symbol_layer;
TextLayer name_layer;
TextLayer hour_layer;
TextLayer minute_layer;

static char hour_text[] = "00";
const char* hour_format = "%H";
static char minute_text[] = "00";
const char* minute_format = "%M";

const Element elements[] = {
  {.atomic = 1, .symbol = "H", .name = "Hydrogen"}, 
  {.atomic = 2, .symbol = "He", .name = "Helium"}, 
  {.atomic = 3, .symbol = "Li", .name = "Lithium"}, 
  {.atomic = 4, .symbol = "Be", .name = "Beryllium"}, 
  {.atomic = 5, .symbol = "B", .name = "Boron"}, 
  {.atomic = 6, .symbol = "C", .name = "Carbon"}, 
  {.atomic = 7, .symbol = "N", .name = "Nitrogen"}, 
  {.atomic = 8, .symbol = "O", .name = "Oxygen"}, 
  {.atomic = 9, .symbol = "F", .name = "Fluorine"}, 
  {.atomic = 10, .symbol = "Ne", .name = "Neon"}, 
  {.atomic = 11, .symbol = "Na", .name = "Sodium"}, 
  {.atomic = 12, .symbol = "Mg", .name = "Magnesium"}, 
  {.atomic = 13, .symbol = "Al", .name = "Aluminium"}, 
  {.atomic = 14, .symbol = "Si", .name = "Silicon"}, 
  {.atomic = 15, .symbol = "P", .name = "Phosphorus"}, 
  {.atomic = 16, .symbol = "S", .name = "Sulfur"}, 
  {.atomic = 17, .symbol = "Cl", .name = "Chlorine"}, 
  {.atomic = 18, .symbol = "Ar", .name = "Argon"}, 
  {.atomic = 19, .symbol = "K", .name = "Potassium"}, 
  {.atomic = 20, .symbol = "Ca", .name = "Calcium"}, 
  {.atomic = 21, .symbol = "Sc", .name = "Scandium"}, 
  {.atomic = 22, .symbol = "Ti", .name = "Titanium"}, 
  {.atomic = 23, .symbol = "V", .name = "Vanadium"}, 
  {.atomic = 24, .symbol = "Cr", .name = "Chromium"}, 
  {.atomic = 25, .symbol = "Mn", .name = "Manganese"}, 
  {.atomic = 26, .symbol = "Fe", .name = "Iron"}, 
  {.atomic = 27, .symbol = "Co", .name = "Cobalt"}, 
  {.atomic = 28, .symbol = "Ni", .name = "Nickel"}, 
  {.atomic = 29, .symbol = "Cu", .name = "Copper"}, 
  {.atomic = 30, .symbol = "Zn", .name = "Zinc"}, 
  {.atomic = 31, .symbol = "Ga", .name = "Gallium"}, 
  {.atomic = 32, .symbol = "Ge", .name = "Germanium"}, 
  {.atomic = 33, .symbol = "As", .name = "Arsenic"}, 
  {.atomic = 34, .symbol = "Se", .name = "Selenium"}, 
  {.atomic = 35, .symbol = "Br", .name = "Bromine"}, 
  {.atomic = 36, .symbol = "Kr", .name = "Krypton"}, 
  {.atomic = 37, .symbol = "Rb", .name = "Rubidium"}, 
  {.atomic = 38, .symbol = "Sr", .name = "Strontium"}, 
  {.atomic = 39, .symbol = "Y", .name = "Yttrium"}, 
  {.atomic = 40, .symbol = "Zr", .name = "Zirconium"}, 
  {.atomic = 41, .symbol = "Nb", .name = "Niobium"}, 
  {.atomic = 42, .symbol = "Mo", .name = "Molybdenum"}, 
  {.atomic = 43, .symbol = "Tc", .name = "Technetium"}, 
  {.atomic = 44, .symbol = "Ru", .name = "Ruthenium"}, 
  {.atomic = 45, .symbol = "Rh", .name = "Rhodium"}, 
  {.atomic = 46, .symbol = "Pd", .name = "Palladium"}, 
  {.atomic = 47, .symbol = "Ag", .name = "Silver"}, 
  {.atomic = 48, .symbol = "Cd", .name = "Cadmium"}, 
  {.atomic = 49, .symbol = "In", .name = "Indium"}, 
  {.atomic = 50, .symbol = "Sn", .name = "Tin"}, 
  {.atomic = 51, .symbol = "Sb", .name = "Antimony"}, 
  {.atomic = 52, .symbol = "Te", .name = "Tellurium"}, 
  {.atomic = 53, .symbol = "I", .name = "Iodine"}, 
  {.atomic = 54, .symbol = "Xe", .name = "Xenon"}, 
  {.atomic = 55, .symbol = "Cs", .name = "Caesium"}, 
  {.atomic = 56, .symbol = "Ba", .name = "Barium"}, 
  {.atomic = 57, .symbol = "La", .name = "Lanthanum"}, 
  {.atomic = 58, .symbol = "Ce", .name = "Cerium"}, 
  {.atomic = 59, .symbol = "Pr", .name = "Praseodymium"}, 
  {.atomic = 60, .symbol = "Nd", .name = "Neodymium"}
};

const uint8_t num_elements = 60;

const Element* get_element(uint8_t atomic_number) {
  if (atomic_number < 1) {
    atomic_number = 60;
  }
  if (atomic_number > num_elements) {
    atomic_number = 1;
  }
  return &elements[atomic_number-1];
}

void update_hour_text(PblTm * time) {
  string_format_time(hour_text, sizeof(hour_text), hour_format, time);
  text_layer_set_text(&hour_layer, hour_text);
}

void update_minute_text(PblTm * time) {
  char* display_00 = "60";
  if (0 == time->tm_min) {
    text_layer_set_text(&minute_layer, display_00);
  } else {
    string_format_time(minute_text, sizeof(minute_text), minute_format, time);
    text_layer_set_text(&minute_layer, minute_text);
  }
}

void border_layer_draw(Layer *layer, GContext *ctx) {
  const uint8_t border_width = 5;
  graphics_context_set_fill_color(ctx, GColorBlack);
  //top border
  graphics_fill_rect(ctx, GRect(0, 0, layer->bounds.size.w, border_width), 0, GCornerNone);
  //right border
  graphics_fill_rect(ctx, GRect(layer->bounds.size.w - border_width, 0, border_width, layer->bounds.size.h), 0, GCornerNone);
  //bottom border
  graphics_fill_rect(ctx, GRect(0, layer->bounds.size.h - border_width, layer->bounds.size.w, border_width), 0, GCornerNone);
  //left border (before hour)
  graphics_fill_rect(ctx, GRect(0, 0, border_width, 15), 0, GCornerNone);
  //left border (after hour)
  graphics_fill_rect(ctx, GRect(0, 34 + 18, border_width, 66), 0, GCornerNone);
}

void handle_init(AppContextRef ctx) {
  window_init(&window, "Periodic Elements");
  window_stack_push(&window, true /* Animated */);

  PblTm now;
  get_time(&now);

  const Element* element = get_element(now.tm_min);

  layer_init(&border_layer, GRect(43 - 10, 22, 91, 100));
  border_layer.update_proc = border_layer_draw;
  layer_add_child(&window.layer, &border_layer);

  text_layer_init(&name_layer, GRect(25, 121, 107 /* width */, 40 /* height */));
  text_layer_set_background_color(&name_layer, GColorClear);
  text_layer_set_font(&name_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(&name_layer, GTextAlignmentCenter);
  text_layer_set_text(&name_layer, element->name);
  layer_add_child(&window.layer, &name_layer.layer);

  text_layer_init(&symbol_layer, GRect(38, 67, 81 /* width */, 50 /* height */));
  text_layer_set_background_color(&symbol_layer, GColorClear);
  text_layer_set_font(&symbol_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(&symbol_layer, GTextAlignmentCenter);
  text_layer_set_text(&symbol_layer, element->symbol);
  layer_add_child(&window.layer, &symbol_layer.layer);

  text_layer_init(&hour_layer, GRect(0, 35, 50 /* width */, 35 /* height */));
  text_layer_set_background_color(&hour_layer, GColorClear);
  text_layer_set_font(&hour_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(&hour_layer, GTextAlignmentCenter);
  update_hour_text(&now);
  layer_add_child(&window.layer, &hour_layer.layer);

  text_layer_init(&minute_layer, GRect(38, 35, 81, 35));
  text_layer_set_background_color(&minute_layer, GColorClear);
  text_layer_set_font(&minute_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(&minute_layer, GTextAlignmentCenter);
  update_minute_text(&now);
  layer_add_child(&window.layer, &minute_layer.layer);
}

void handle_tick(AppContextRef app_ctx, PebbleTickEvent *event) {
    const Element* element = get_element(event->tick_time->tm_min);
    text_layer_set_text(&symbol_layer, element->symbol);
    text_layer_set_text(&name_layer, element->name);
    update_hour_text(event->tick_time);    
    update_minute_text(event->tick_time);    
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
