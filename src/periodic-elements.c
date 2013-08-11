/**
 * Periodic elements
 *
 * Basic Pebble watchface that currently just
 * displays the current second and the chemical
 * element with the corresponding atomic number.
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
TextLayer symbol_layer;
TextLayer second_layer;

static char second_text[] = "00";
const char* second_format = "%S";

const Element elements[] = {
  {.atomic = 1, .symbol = "H", .name = "Hydrogen", .origin = "the Greek \'hydro\' and \'genes\' meaning water-forming", .group = 1, .period = 1, .weight = 1}, 
  {.atomic = 2, .symbol = "He", .name = "Helium", .origin = "the Greek, \'helios\' meaning sun", .group = 18, .period = 1, .weight = 4}, 
  {.atomic = 3, .symbol = "Li", .name = "Lithium", .origin = "the Greek \'lithos\' meaning stone", .group = 1, .period = 2, .weight = 6}, 
  {.atomic = 4, .symbol = "Be", .name = "Beryllium", .origin = "the Greek name for beryl, \'beryllo\'", .group = 2, .period = 2, .weight = 9}, 
  {.atomic = 5, .symbol = "B", .name = "Boron", .origin = "the Arabic \'buraq\', which was the name for borax", .group = 13, .period = 2, .weight = 10}, 
  {.atomic = 6, .symbol = "C", .name = "Carbon", .origin = "the Latin \'carbo\', meaning charcoal", .group = 14, .period = 2, .weight = 12}, 
  {.atomic = 7, .symbol = "N", .name = "Nitrogen", .origin = "the Greek \'nitron\' and \'genes\' meaning nitre-forming", .group = 15, .period = 2, .weight = 14}, 
  {.atomic = 8, .symbol = "O", .name = "Oxygen", .origin = "the Greek \'oxy\' and \'genes\' meaning acid-forming", .group = 16, .period = 2, .weight = 15}, 
  {.atomic = 9, .symbol = "F", .name = "Fluorine", .origin = "the Latin \'fluere\', meaning to flow", .group = 17, .period = 2, .weight = 18}, 
  {.atomic = 10, .symbol = "Ne", .name = "Neon", .origin = "the Greek \'neos\', meaning new", .group = 18, .period = 2, .weight = 20}, 
  {.atomic = 11, .symbol = "Na", .name = "Sodium", .origin = "the English word soda (natrium in Latin)[2]", .group = 1, .period = 3, .weight = 22}, 
  {.atomic = 12, .symbol = "Mg", .name = "Magnesium", .origin = "Magnesia, a district of Eastern Thessaly in Greece", .group = 2, .period = 3, .weight = 24}, 
  {.atomic = 13, .symbol = "Al", .name = "Aluminium", .origin = "the Latin name for alum, \'alumen\' meaning bitter salt", .group = 13, .period = 3, .weight = 26}, 
  {.atomic = 14, .symbol = "Si", .name = "Silicon", .origin = "the Latin \'silex\' or \'silicis\', meaning flint", .group = 14, .period = 3, .weight = 28}, 
  {.atomic = 15, .symbol = "P", .name = "Phosphorus", .origin = "the Greek \'phosphoros\', meaning bringer of light", .group = 15, .period = 3, .weight = 30}, 
  {.atomic = 16, .symbol = "S", .name = "Sulfur", .origin = "Either from the Sanskrit \'sulvere\', or the Latin \'sulfurium\', both names for sulfur[2]", .group = 16, .period = 3, .weight = 32}, 
  {.atomic = 17, .symbol = "Cl", .name = "Chlorine", .origin = "the Greek \'chloros\', meaning greenish yellow", .group = 17, .period = 3, .weight = 35}, 
  {.atomic = 18, .symbol = "Ar", .name = "Argon", .origin = "the Greek, \'argos\', meaning idle", .group = 18, .period = 3, .weight = 39}, 
  {.atomic = 19, .symbol = "K", .name = "Potassium", .origin = "the English word potash (kalium in Latin)[2]", .group = 1, .period = 4, .weight = 39}, 
  {.atomic = 20, .symbol = "Ca", .name = "Calcium", .origin = "the Latin \'calx\' meaning lime", .group = 2, .period = 4, .weight = 40}, 
  {.atomic = 21, .symbol = "Sc", .name = "Scandium", .origin = "Scandinavia (with the Latin name Scandia)", .group = 3, .period = 4, .weight = 44}, 
  {.atomic = 22, .symbol = "Ti", .name = "Titanium", .origin = "Titans, the sons of the Earth goddess of Greek mythology", .group = 4, .period = 4, .weight = 47}, 
  {.atomic = 23, .symbol = "V", .name = "Vanadium", .origin = "Vanadis, an old Norse name for the Scandinavian goddess Freyja", .group = 5, .period = 4, .weight = 50}, 
  {.atomic = 24, .symbol = "Cr", .name = "Chromium", .origin = "the Greek \'chroma\', meaning colour", .group = 6, .period = 4, .weight = 51}, 
  {.atomic = 25, .symbol = "Mn", .name = "Manganese", .origin = "Either the Latin \'magnes\', meaning magnet or from the black magnesium oxide, \'magnesia nigra\'", .group = 7, .period = 4, .weight = 54}, 
  {.atomic = 26, .symbol = "Fe", .name = "Iron", .origin = "the Anglo-Saxon name iren (ferrum in Latin)", .group = 8, .period = 4, .weight = 55}, 
  {.atomic = 27, .symbol = "Co", .name = "Cobalt", .origin = "the German word \'kobald\', meaning goblin", .group = 9, .period = 4, .weight = 58}, 
  {.atomic = 28, .symbol = "Ni", .name = "Nickel", .origin = "the shortened of the German \'kupfernickel\' meaning either devil\'s copper or St. Nicholas\'s copper", .group = 10, .period = 4, .weight = 58}, 
  {.atomic = 29, .symbol = "Cu", .name = "Copper", .origin = "the Old English name coper in turn derived from the Latin \'Cyprium aes\', meaning a metal from Cyprus", .group = 11, .period = 4, .weight = 63}, 
  {.atomic = 30, .symbol = "Zn", .name = "Zinc", .origin = "the German, \'zinc\', which may in turn be derived from the Persian word \'sing\', meaning stone", .group = 12, .period = 4, .weight = 65}, 
  {.atomic = 31, .symbol = "Ga", .name = "Gallium", .origin = "France (with the Latin name Gallia)", .group = 13, .period = 4, .weight = 69}, 
  {.atomic = 32, .symbol = "Ge", .name = "Germanium", .origin = "Germany (with the Latin name Germania)", .group = 14, .period = 4, .weight = 72}, 
  {.atomic = 33, .symbol = "As", .name = "Arsenic", .origin = "the Greek name \'arsenikon\' for the yellow pigment orpiment", .group = 15, .period = 4, .weight = 74}, 
  {.atomic = 34, .symbol = "Se", .name = "Selenium", .origin = "Moon (with the Greek name selene)", .group = 16, .period = 4, .weight = 78}, 
  {.atomic = 35, .symbol = "Br", .name = "Bromine", .origin = "the Greek \'bromos\' meaning stench", .group = 17, .period = 4, .weight = 79}, 
  {.atomic = 36, .symbol = "Kr", .name = "Krypton", .origin = "the Greek \'kryptos\', meaning hidden", .group = 18, .period = 4, .weight = 83}, 
  {.atomic = 37, .symbol = "Rb", .name = "Rubidium", .origin = "the Latin \'rubidius\', meaning deepest red", .group = 1, .period = 5, .weight = 85}, 
  {.atomic = 38, .symbol = "Sr", .name = "Strontium", .origin = "Strontian, a small town in Scotland", .group = 2, .period = 5, .weight = 87}, 
  {.atomic = 39, .symbol = "Y", .name = "Yttrium", .origin = "Ytterby, Sweden", .group = 3, .period = 5, .weight = 88}, 
  {.atomic = 40, .symbol = "Zr", .name = "Zirconium", .origin = "the Persian \'zargun\', meaning gold coloured", .group = 4, .period = 5, .weight = 91}, 
  {.atomic = 41, .symbol = "Nb", .name = "Niobium", .origin = "Niobe, daughter of king Tantalus from Greek mythology", .group = 5, .period = 5, .weight = 92}, 
  {.atomic = 42, .symbol = "Mo", .name = "Molybdenum", .origin = "the Greek \'molybdos\' meaning lead", .group = 6, .period = 5, .weight = 95}, 
  {.atomic = 43, .symbol = "Tc", .name = "Technetium", .origin = "the Greek \'tekhnetos\' meaning artificial", .group = 7, .period = 5, .weight = 98}, 
  {.atomic = 44, .symbol = "Ru", .name = "Ruthenium", .origin = "Russia (with the Latin name Ruthenia)", .group = 8, .period = 5, .weight = 101}, 
  {.atomic = 45, .symbol = "Rh", .name = "Rhodium", .origin = "the Greek \'rhodon\', meaning rose coloured", .group = 9, .period = 5, .weight = 102}, 
  {.atomic = 46, .symbol = "Pd", .name = "Palladium", .origin = "From the asteroid Pallas which had been recently discovered and named at the time. The asteroid was considered a planet when it was discovered", .group = 10, .period = 5, .weight = 106}, 
  {.atomic = 47, .symbol = "Ag", .name = "Silver", .origin = "the Anglo-Saxon name siolfur (argentum in Latin)[2]", .group = 11, .period = 5, .weight = 107}, 
  {.atomic = 48, .symbol = "Cd", .name = "Cadmium", .origin = "the Latin name for the mineral calmine, \'cadmia\'", .group = 12, .period = 5, .weight = 112}, 
  {.atomic = 49, .symbol = "In", .name = "Indium", .origin = "the Latin \'indicium\', meaning violet or indigo", .group = 13, .period = 5, .weight = 114}, 
  {.atomic = 50, .symbol = "Sn", .name = "Tin", .origin = "the Anglo-Saxon word tin (stannum in Latin, meaning hard)", .group = 14, .period = 5, .weight = 118}, 
  {.atomic = 51, .symbol = "Sb", .name = "Antimony", .origin = "the Greek \'anti-monos\', meaning not alone (stibium in Latin)", .group = 15, .period = 5, .weight = 121}, 
  {.atomic = 52, .symbol = "Te", .name = "Tellurium", .origin = "Earth, the third planet on solar system (with the Latin word tellus)", .group = 16, .period = 5, .weight = 127}, 
  {.atomic = 53, .symbol = "I", .name = "Iodine", .origin = "the Greek \'iodes\' meaning violet", .group = 17, .period = 5, .weight = 126}, 
  {.atomic = 54, .symbol = "Xe", .name = "Xenon", .origin = "the Greek \'xenos\' meaning stranger", .group = 18, .period = 5, .weight = 131}, 
  {.atomic = 55, .symbol = "Cs", .name = "Caesium", .origin = "the Latin \'caesius\', meaning sky blue", .group = 1, .period = 6, .weight = 132}, 
  {.atomic = 56, .symbol = "Ba", .name = "Barium", .origin = "the Greek \'barys\', meaning heavy", .group = 2, .period = 6, .weight = 137}, 
  {.atomic = 57, .symbol = "La", .name = "Lanthanum", .origin = "the Greek \'lanthanein\', meaning to lie hidden", .group = 0, .period = 6, .weight = 138}, 
  {.atomic = 58, .symbol = "Ce", .name = "Cerium", .origin = "Ceres, the Roman God of agriculture", .group = 0, .period = 6, .weight = 140}, 
  {.atomic = 59, .symbol = "Pr", .name = "Praseodymium", .origin = "the Greek \'prasios didymos\' meaning green twin", .group = 0, .period = 6, .weight = 140}, 
  {.atomic = 60, .symbol = "Nd", .name = "Neodymium", .origin = "the Greek \'neos didymos\' meaning new twin", .group = 0, .period = 6, .weight = 144}
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

void update_second_text(PblTm * time) {
  char* display_00 = "60";
  if (0 == time->tm_sec) {
    text_layer_set_text(&second_layer, display_00);
  } else {
    string_format_time(second_text, sizeof(second_text), second_format, time);
    text_layer_set_text(&second_layer, second_text);
  }
}

void handle_init(AppContextRef ctx) {
  window_init(&window, "Periodic Elements");
  window_stack_push(&window, true /* Animated */);

  PblTm now;
  get_time(&now);

  const Element* element = get_element(now.tm_sec);

  text_layer_init(&symbol_layer, GRect(0, 30, 144 /* width */, 50 /* height */));
  text_layer_set_font(&symbol_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(&symbol_layer, GTextAlignmentCenter);
  text_layer_set_text(&symbol_layer, element->symbol);
  layer_add_child(&window.layer, &symbol_layer.layer);

  text_layer_init(&second_layer, GRect(0, 80, 144 /* width */, 50 /* height */));
  text_layer_set_font(&second_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(&second_layer, GTextAlignmentCenter);
  update_second_text(&now);
  layer_add_child(&window.layer, &second_layer.layer);
}

void handle_tick(AppContextRef app_ctx, PebbleTickEvent *event) {
    const Element* element = get_element(event->tick_time->tm_sec);
    text_layer_set_text(&symbol_layer, element->symbol);
    update_second_text(event->tick_time);    
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_tick,
      .tick_units = SECOND_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
