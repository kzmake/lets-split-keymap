#include "lets_split.h"
#include "rekeymap.h"
#include "action_layer.h"
#include "eeconfig.h"

extern keymap_config_t keymap_config;

#define QWERTY_LAYER 0
#define LOWER_LAYER 1
#define UPPER_LAYER 2
#define SPACEFN_LAYER 3
#define TENKEY_LAYER 4
#define NORMAN_LAYER 5
#define UNDERGLOW_LAYER 6

// Tap Dance
enum {
  SFT_CAPS = 0,
  MPLY_MUTE = 2,
};

// Macros
enum macro_id {
  M_USERNAME,
  M_RANDDIGIT,
  M_RANDLETTER,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[QWERTY_LAYER] = KEYMAP( \
  KC_ESC,               KC_Q,     KC_W,     KC_E,     KC_R,  KC_T,              KC_Y,     KC_U,  KC_I,     KC_O,     KC_P,     KC_BSPC, \
  KC_TAB,               KC_A,     KC_S,     KC_D,     KC_F,  KC_G,              KC_H,     KC_J,  KC_K,     KC_L,     KC_SCLN,  KC_QUOT, \
  MOD_LSFT,             KC_Z,     KC_X,     KC_C,     KC_V,  KC_B,              KC_N,     KC_M,  KC_COMM,  KC_DOT,   KC_SLSH,  MT(MOD_RSFT, KC_ENT), \
  MO(UNDERGLOW_LAYER),  KC_LGUI,   KC_LALT, XXXXXXX,  F(1),  CTL_T(KC_SPC),     KC_SPC,   F(2),  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT \
),

[LOWER_LAYER] = KEYMAP( 
  KC_TILD,  KC_F1,     KC_F2,    KC_HASH,  KC_DLR,   KC_PERC,  KC_CIRC,  KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN,  KC_PIPE, \
  KC_GRV,   KC_1,      KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_BSLASH, \
  _______,  KC_MINS,   KC_UNDS,  KC_PLUS,  KC_EQL,   _______,  _______,  KC_LBRC,  KC_RBRC,  KC_LCBR,  KC_RCBR,  _______, \
  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______ \
),

[UPPER_LAYER] = KEYMAP( \
  KC_F1,    KC_F2,     KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12, \
  KC_GRV,   KC_1,      KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_BSLASH, \
  _______,  KC_F1,     KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11, \
  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_F12 \
),

[UNDERGLOW_LAYER] = KEYMAP( \
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______, \
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______, \
  _______,  _______,  RGB_MOD,  RGB_HUD,  RGB_HUI,  RGB_SAD,  RGB_SAI,  RGB_VAD,  RGB_VAI,  _______,  _______,  _______, \
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______ \
),
};

const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_DEFAULT_LAYER_SET(QWERTY_LAYER),

    // Tap for backspace, hold for LOWER
    [1] = ACTION_LAYER_TAP_KEY(LOWER_LAYER, KC_BSPC),

    // Tap for enter, hold for RAISE
    [2] = ACTION_LAYER_TAP_KEY(UPPER_LAYER, KC_ENT),
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  uint8_t clockbyte=0;
  clockbyte = TCNT1 % 256;
  uint8_t rval;

  // MACRODOWN only works in this function
  switch(id) {
    case M_USERNAME:
      if (record->event.pressed) {
        SEND_STRING("nicinabox");
      }
      break;

    case M_RANDDIGIT:
      // Generate, based on random number generator, a keystroke for
      // a numeric digit chosen at random
      random_value = ((random_value + randadd) * randmul) % randmod;
      if (record->event.pressed) {
        // Here, we mix the LCRNG with low bits from one of the system
        // clocks via XOR in the theory that this may be more random
        // than either separately
        rval = (random_value ^ clockbyte) % 10;
        // Note that KC_1 thru KC_0 are a contiguous range
        register_code (KC_1 + rval);
        unregister_code (KC_1 + rval);
      }
      break;

    case M_RANDLETTER:
      // Generate, based on random number generator, a keystroke for
      // a letter chosen at random
      // Here, we mix the LCRNG with low bits from one of the system
      // clocks via XOR in the theory that this may be more random
      // than either separately
      random_value = ((random_value + randadd) * randmul) % randmod;
      if (record->event.pressed) {
        rval = (random_value ^ clockbyte) % 26;
        register_code (KC_A + rval);
        unregister_code (KC_A + rval);
      }
      break;
  }

  return MACRO_NONE;
};
