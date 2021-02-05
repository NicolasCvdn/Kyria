/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#define _COLEMAK 0
#define _CSGO 1
#define _LOWER 2
#define _RAISE 3
#define _ADJUST 4   

#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif
/*
static char logo[] = {
      0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
      0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
      0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
      0};
*/
enum layers {
    COLEMAK = SAFE_RANGE,
  CSGO,
  LOWER,
  RAISE,
  ADJUST,

  M_LBRCT,
  M_RBRCT,
  M_LSGL,
  M_RSGL,
  M_LBTN,
  M_RBTN,
  M_AT,
  M_HASH,
  M_RTBCT,
  M_LSLCT,
  M_RSLCT,
  M_BAR,
  M_NOT,
  M_BSLSH,

  M_PARE,
  M_BRAC,
  M_ACCO,

  M_RAISE,
  M_ENCD,
  M_BONGO,
  M_SWITCH,
  M_UGRV,
  M_LOCK,
  M_RGB,
  M_SNIP,
  M_WBSPC,
  M_CCCV
};

char wpm_str[12];
char key_str[6];

bool scrollFunc = 0;
bool raiseOn = 0;
uint16_t copy_paste_timer;
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*
 * Base Layer: Colemak
 * 
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |   TAB  |   Q  |   W  |   F  |   P  |   G  |                              |   J  |   L  |   U  |   Y  |   '  |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * | LCTRL  |   A  |   R  |  S   |   T  |   D  |                              |   H  |   N  |   E  |   I  |   O  |    à   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |  (   |      |  |   )  |      |   K  |   M  |   ,  |   .  |   -  | RShift |
 * `----------------------+------+------+------+------+ Space|  |Enter +------+------+------+------+----------------------'
 *                        | Caps | LAlt | LSlct| Lwr  |      |  |      | Rse  | RSlct| RAlt | Nav  |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_COLEMAK] = LAYOUT(
      KC_TAB  , KC_Q ,  KC_W   ,  KC_F  ,   KC_P ,   KC_G ,                                        KC_J,   KC_L ,  KC_U ,   KC_Z ,KC_MINS, KC_BSPC,
      KC_LCTRL, KC_A ,  KC_R   ,  KC_S  ,   KC_T ,   KC_D ,                                        KC_H,   KC_N ,  KC_E ,   KC_I ,  KC_O , KC_QUOT,
      KC_LSFT , KC_Y ,  KC_X   ,  KC_C  ,   KC_V ,   KC_B , M_CCCV, XXXXXXX , XXXXXXX, KC_DEL, KC_K,  KC_M ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
                                 KC_CAPS, KC_LALT,   LOWER, KC_SPC, M_PARE, M_WBSPC, KC_ENT, RAISE, KC_RALT, M_ENCD
    ),

/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |   TAB  |   Q  |   W  |   F  |   P  |   G  |                              |   J  |   L  |   U  |   Y  |   '  |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * | LCTRL  |   A  |   R  |  S   |   T  |   D  |                              |   H  |   N  |   E  |   I  |   O  |    à   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  | Left |      |  |      |      |   K  |   M  |   ,  |   .  |   -  | RShift |
 * `----------------------+------+------+------+------+ Space|  |Enter +------+------+------+------+----------------------'
 *                        | Caps | LAlt | Esc  | Down |      |  |      |  Rse | Rslct| RALT | GUI  |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
#ifndef MASTER_RIGHT //CSGO Layer
    [_CSGO] = LAYOUT(
      KC_TAB  , KC_Q ,  KC_W   ,  KC_F  ,   KC_P ,   KC_G ,                                        KC_J,   KC_L ,  KC_U ,   KC_Z ,KC_MINS, KC_BSPC,
      KC_LCTRL, KC_A ,  KC_R   ,  KC_S  ,   KC_T ,   KC_D ,                                        KC_H,   KC_N ,  KC_E ,   KC_I ,  KC_O , KC_QUOT,
      KC_LSFT , KC_Y ,  KC_X   ,  KC_C  ,   KC_V ,   KC_B , KC_ESC, XXXXXXX , XXXXXXX, KC_RGHT, KC_K,  KC_M ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
                                 KC_CAPS, KC_LALT, KC_LEFT, _______, KC_DOWN, KC_ENT, KC_ENT, _______, _______, M_SWITCH
    ),
#else //on left half, the second default layer is QWERTY
    [_CSGO] = LAYOUT(
      _______,   KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                                         KC_Z,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
      _______,   KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                                         KC_H,    KC_J,    KC_K,    KC_L,    KC_MINS, KC_QUOT,
      _______,   KC_Y,   KC_X,   KC_C,   KC_V,   KC_B,   _______,   _______, _______, _______, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                                _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______
    ),
#endif
/*
 * Lower Layer: Nav
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Esc   | Vol+ | Home |  Up  | End  | Pgup |                              |  6   |  7   |  8   |  9   |  ¨   |  Del   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        | Mute | Left | Down | Right| Pgdn |                              |  &   |  ^   |  é   | Bksp |  =   |   ù    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        | Vol- | Prev | Play | Next |      |  [   |      |  |      |  ]   |  ~   |  |   |  (   |  )   |      |        |
 * `----------------------+------+------+------+------+ Enter|  | Bksp +------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_LOWER] = LAYOUT(
       KC_ESC, KC_VOLU, KC_HOME, KC_UP,   KC_END, KC_PGUP,                                      KC_6,    KC_7,    KC_8,    KC_9, KC_RBRC, KC_DEL,
      _______, KC_MUTE, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN,                                     KC_CIRC,  KC_EQL, KC_SCLN, KC_BSPC, KC_RPRN, M_UGRV,
      _______, KC_VOLD, KC_MPRV, KC_MPLY, KC_MNXT, _______, M_LBRCT, _______, _______, M_RBRCT,   M_NOT,   M_BAR, KC_ASTR, KC_LPRN, _______, _______,
                                 _______, _______, _______, KC_ENT,  M_BRAC,  _______, KC_BSPC, _______, KC_RALT, _______
    ),
/*
 * Raise Layer: Numbers, symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |   1  |  2   |  3   |  4   |  5   |                              |  6   |  7   |  8   |  9   |  0   |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |   +  |  "   |  *   |  ç   |  %   |                              |  &   |  /   |  è   |  i   |  =   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |  #   |      |      |  {   |      |  |      |  }   |  ~   |  |   |  [   |  ]   |  {   |   }    |
 * `----------------------+------+------+------+------+      |  |      +------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_RAISE] = LAYOUT(
       KC_ESC, KC_1, 	  KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
      _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                                     KC_CIRC, KC_AMPR, KC_LBRC, M_BSLSH,  KC_RPRN, KC_NUHS,
      _______, _______,  M_AT,    M_HASH, _______, _______,  M_LSGL, _______, _______, M_RSGL,    M_NOT,   M_BAR, M_LBRCT, M_RBRCT,  M_LSGL, M_RSGL,
                                 _______, _______, LOWER, _______, M_ACCO, _______, M_RAISE, _______, _______, _______
    ),
/*
 * Adjust Layer: Function keys, RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * | Switch |  F1  |  F2  |  F3  |  F4  |  F5  |                              |  F6  |  F7  |  F8  |  F9  |  F10 |  Reset |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |  GUI   | SAI  | HUI  | VAI  | Snip |      |                              |      |      |      |  F11 |  F12 | ON/OFF |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |  LOCK  | SAD  | HUD  | VAD  |Purple|ON/OFF|  <   |      |  |      |  >   |      |      |      |      |      |  Snip  |
 * `----------------------+------+------+------+------+      |  |      +------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
    M_SWITCH, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  RESET,
    KC_LGUI, RGB_SAI, RGB_VAI, RGB_HUI,  M_SNIP, _______,                                     _______, _______, _______, KC_F11,  KC_F12,  RGB_TOG,
    M_LOCK, RGB_SAD, RGB_VAD, RGB_HUD, M_RGB, RGB_TOG, KC_NUBS, _______, _______, M_RTBCT, _______, _______, _______, _______, _______, M_SNIP,
                                 _______, _______, _______, _______, KC_NUBS, M_RTBCT, _______, _______, KC_RALT, _______
    ),
// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

#ifdef OLED_DRIVER_ENABLE
  const char *read_keylog(void);
  void set_keylog(uint16_t keycode, keyrecord_t *record);

  oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  	return OLED_ROTATION_180;
  }

  // Credit to https://github.com/qmk/qmk_firmware/blob/master/keyboards/kyria/keymaps/j-inc/keymap.c
  // WPM-responsive animation stuff here
  #define IDLE_FRAMES 5
  #define IDLE_SPEED 20 // below this wpm value your animation will idle

  // #define PREP_FRAMES 1 // uncomment if >1

  #define TAP_FRAMES 2
  #define TAP_SPEED 40 // above this wpm value typing animation to triggere

  #define ANIM_FRAME_DURATION 200 // how long each frame lasts in ms 
  #define ANIM_FRAME_ACCELERATION 1.7 // how much the animation accelerates as a function of the tap speed
  // #define SLEEP_TIMER 60000 // should sleep after this period of 0 wpm, needs fixing
  #define ANIM_SIZE 416 // number of bytes in array, minimize for adequate firmware size, max is 1024
  #define ANIM_SIZE2 548

  uint32_t anim_timer = 0;
  uint32_t anim_sleep = 0;
  uint8_t current_idle_frame = 0;
  // uint8_t current_prep_frame = 0; // uncomment if PREP_FRAMES >1
  uint8_t current_tap_frame = 0;
  uint32_t anim_frame_duration = ANIM_FRAME_DURATION;

  static void render_anim(void) {
    static const char PROGMEM idle[IDLE_FRAMES][ANIM_SIZE] = {
        {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  4,  4,  4,  8, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24,100,130,  2,  2,  2,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 56,  4,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    
        8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,
        },
        {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  4,  4,  4,  8, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24,100,130,  2,  2,  2,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 56,  4,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,
        },
        {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16,  8,  4,  2,  2,  4, 24, 96,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 60,194,  1,  1,  2,  2,  4,  4,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 96, 96,  0,129,130,130,132,  8, 16, 32, 64,128,  0,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 25,  6,  0,  0,  0,  0,  0,  0,  0, 24, 24, 24, 27,  3,  0, 64,160, 34, 36, 20, 18, 18, 18, 11,  8,  8,  8,  8,  5,  5,  9,  9, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,
        },
        {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,225,  0,  0,  1,  1,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,
        },
        {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  4,  2,  2,  2,  4, 56, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 28,226,  1,  1,  2,  2,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,
        }

      };
    static const char PROGMEM prep[][ANIM_SIZE2] = {
        {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,225,  0,  0,  1,  1,  2,  2,129,128,128,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,  0,  1,225, 26,  6,  9, 49, 53,  1,138,124,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0, 24,  6,  5,152,153,132,195,124, 65, 65, 64, 64, 32, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  4,  4,  4,  4,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        }
      };
    static const char PROGMEM tap[TAP_FRAMES][ANIM_SIZE2] = {
        {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,248,248,248,248,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,225,  0,  0,  1,  1,  2,  2,129,128,128,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,  0,  1,  1,  2,  4,  8, 16, 32, 67,135,  7,  1,  0,184,188,190,159, 95, 95, 79, 76, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0, 24,  6,  5,152,153,132, 67,124, 65, 65, 64, 64, 32, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1, 61,124,252,252,252,252,252, 60, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        },
        {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,225,  0,  0,  1,  1,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,  0,  1,225, 26,  6,  9, 49, 53,  1,138,124,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  4,  4,  4,  4,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,122,122,121,121,121,121, 57, 49,  2,  2,  4,  4,  8,  8,  8,136,136,135,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48,120,124,254,255, 63,  7,  0,  0,  0,  0,255,255,127,127, 63, 62, 28, 24,
        },
      };
      
    //assumes 1 frame prep stage
    void animation_phase(void) {
      if(get_current_wpm() <=IDLE_SPEED){
        current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
        oled_write_raw_P(idle[abs((IDLE_FRAMES-1)-current_idle_frame)], ANIM_SIZE);
        anim_frame_duration = ANIM_FRAME_DURATION;
       }
       if(get_current_wpm() >IDLE_SPEED && get_current_wpm() <TAP_SPEED){
         // oled_write_raw_P(prep[abs((PREP_FRAMES-1)-current_prep_frame)], ANIM_SIZE); // uncomment if IDLE_FRAMES >1
         oled_write_raw_P(prep[0], ANIM_SIZE2);  // remove if IDLE_FRAMES >1
         anim_frame_duration = ANIM_FRAME_DURATION;
       }
       if(get_current_wpm() >=TAP_SPEED){
         current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;
         oled_write_raw_P(tap[abs((TAP_FRAMES-1)-current_tap_frame)], ANIM_SIZE2);
         if(get_current_wpm() < 130) {
          anim_frame_duration = ANIM_FRAME_DURATION - (uint32_t)((get_current_wpm() - TAP_SPEED) * ANIM_FRAME_ACCELERATION); //Tap speed calculations
         } else {
          anim_frame_duration = ANIM_FRAME_DURATION - (uint32_t)((130 - TAP_SPEED) * ANIM_FRAME_ACCELERATION);
         }
       }
    }
    if(get_current_wpm() != 000) {
      if(timer_elapsed32(anim_timer) > anim_frame_duration) {
          anim_timer = timer_read32();
          animation_phase();
      }
      anim_sleep = timer_read32();
    } else {
      if(timer_elapsed32(anim_sleep) > OLED_TIMEOUT) {
          oled_off();
      } else {
        if(timer_elapsed32(anim_timer) > anim_frame_duration) {
          anim_timer = timer_read32();
          animation_phase();
        }
      }
    }
  }

  #define ANIM2_FRAME_DURATION 500
  #define KEYCODE_FRAME_DURATION 5000
  uint32_t anim2_sleep = 0;
  uint32_t anim2_timer = 0;
  uint32_t anim2_frame_duration = ANIM2_FRAME_DURATION;
  uint32_t keycode_timer = 0;
  uint32_t keycode_timer_duration = KEYCODE_FRAME_DURATION;
  bool anim2_frame = 0;
  bool keycode_write = 0; 


  // Keylogger, made by Tourist

  void keylog(void) {
    
    if(timer_elapsed32(anim2_timer) > anim2_frame_duration) {
       anim2_timer = timer_read32();
       anim2_frame = !anim2_frame;     
    } 
    oled_write_P(PSTR(">:"), false);

    #ifdef KEYLOGGER_ENABLE
      if(keycode_write) {
        sprintf(key_str, "%s", read_keylog());
        oled_write(key_str, false);
        if(timer_elapsed32(keycode_timer) > keycode_timer_duration) {
           keycode_write = 0;    
        } 
    }
    #endif
    if(anim2_frame) {
        oled_write_ln_P(PSTR("_"), false);
      } else {  
        oled_write_ln_P(PSTR(" "), false);
      }
      //if(timer_elapsed32(anim2_sleep) > OLED_TIMEOUT) {
        //oled_off();
      //}
  }

void render_layer(void) {
  uint8_t default_layer = eeconfig_read_default_layer();
    switch (get_highest_layer(layer_state)) {
      case _COLEMAK: //default layer
          if(default_layer & (1UL << _COLEMAK))
            oled_write_P(PSTR("Colemak"), false);
          else if (default_layer & (1UL << _CSGO))
            #ifndef MASTER_RIGHT
              oled_write_P(PSTR("CS:GO  "), false);
            #else
              oled_write_P(PSTR("Qwerty "), false);
          #endif
          else
            oled_write_P(PSTR("ErState"), false);
          break;
      case _LOWER:
          oled_write_P(PSTR("Lower    "), false);
          break;
      case _RAISE:
          oled_write_P(PSTR("Raise    "), false);
          break;
      case _ADJUST:
          oled_write_P(PSTR("Adjust   "), false);
          break;
      default:
          oled_write_P(PSTR("Undefined"), false);
    }
}

void render_status(void) {
  uint8_t led_usb_state = host_keyboard_leds();
  oled_write_P(IS_LED_ON(led_usb_state, USB_LED_CAPS_LOCK) ? PSTR("CAPS  ") : PSTR("      "), false);
  oled_write_P(scrollFunc ? PSTR("* ") : PSTR("  "), false);
}


  static void render_host(void) {

    // Cat
    render_anim();
    oled_set_cursor(0, 5);
    
    // Keylogger
    keylog();
    oled_set_cursor(6, 7);

    // WPM calculation
    sprintf(wpm_str, "WPM: %03d\n", get_current_wpm());
    oled_write(wpm_str, false);
    oled_set_cursor(13, 0);

    // Host Keyboard Layer Status
    render_layer();
    oled_set_cursor(13, 2);

    // Host Keyboard LED Status
    render_status();
  }    

  void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
      0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
      0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
      0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};

    oled_write_P(qmk_logo, false);
  }
     

  void oled_task_user(void) {

      if (is_keyboard_master()) {
          render_host(); 
      } else {
          oled_write_ln_P(PSTR("\n"), false);
          render_logo();
      }
  }
  #endif

  #ifdef ENCODER_ENABLE
  bool has_scrolled = false;
  bool button_pressed = false;
  void encoder_update_user(uint8_t index, bool clockwise) {
    #ifdef MASTER_RIGHT
      clockwise = !clockwise;
    #endif
      if (index == 0) {
        if(scrollFunc) {
          // zoom
          if (clockwise) {
              tap_code(KC_WH_L);
          } else {
              tap_code(KC_WH_R);
          }
        } else {
          // Volume control
          if (clockwise) {
              tap_code(KC_VOLD);
          } else {
              tap_code(KC_VOLU);
          }
        }
      }
      else if (index == 1) {
        if(scrollFunc) {
          // left and right
          if (clockwise) {
              tap_code(KC_LEFT);
          } else {
              tap_code(KC_RIGHT);
          }
        } else {
          if(button_pressed) {
            has_scrolled = true;
            if (clockwise) {
              SEND_STRING(SS_DOWN(X_LALT) SS_DOWN(X_LSFT) SS_TAP(X_TAB) SS_UP(X_LSFT));
            } else {
              SEND_STRING(SS_DOWN(X_LALT) SS_TAP(X_TAB));
            } 
          } else { 
            if (clockwise) {
              tap_code16(C(KC_Z));
            } else {
              tap_code16(C(KC_Y));
            }
          }
        }
      }
    }
#endif


bool process_record_user(uint16_t keycode, keyrecord_t *record) {  

  #ifdef OLED_DRIVER_ENABLE
    anim2_sleep = anim_sleep = timer_read32();

    oled_on();  // turn on oled if a keystroke is detected

    set_keylog(keycode, record);

    #ifdef KEYLOGGER_ENABLE
      keycode_write = 1;
      keycode_timer = timer_read32(); // set the timers for the animations
      anim2_timer = timer_read32();
      anim2_frame = 0;
    #endif
  #endif

  uint8_t default_layer = eeconfig_read_default_layer(); // read the current default layer
  if (keycode == M_SWITCH) { // Switch default layers
    if(default_layer & (1UL << _COLEMAK)) {
      keycode = CSGO;
    } else {
      keycode = COLEMAK;
    }
  }

switch(keycode) {
  case COLEMAK: // Switch to colemak
    if (record->event.pressed) {
      set_single_persistent_default_layer(_COLEMAK);
    }
    return false;
    break;

    case CSGO: // Switch to CSGO or QWERTY
    if (record->event.pressed) {
      set_single_persistent_default_layer(_CSGO);
    } else {
    }
    return false;
    break;

  case LOWER: // Lower
    if (record->event.pressed) {
      layer_on(_LOWER);
    } else {
      layer_off(_LOWER);
    }
    return false;
    break;

  case RAISE: // Raise
    if(!raiseOn) {
      if (record->event.pressed) {
        layer_on(_RAISE);
      } else {
        layer_off(_RAISE);
      } 
    } else {
        if (record->event.pressed) {
          layer_off(_RAISE);
          raiseOn = 0;
        } else {
      }
    }
    return false;
    break;

  case ADJUST: // Adjust
    if (record->event.pressed) {
      layer_on(_ADJUST);
    } else {
      layer_off(_ADJUST);
    }
    return false;
    break;

  case M_LBRCT: // [
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_RALT) SS_DOWN(X_LBRC));
    } else {
      SEND_STRING(SS_UP(X_RALT) SS_UP(X_LBRC));
    }
    return false;
    break;

  case M_RBRCT: // ]
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_RALT) SS_DOWN(X_RBRC));
    } else {
      SEND_STRING(SS_UP(X_RALT) SS_UP(X_RBRC));
    }
    return false;
    break;

  case M_LSGL: // {
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_RALT) SS_DOWN(X_QUOT));
    } else {
      SEND_STRING(SS_UP(X_RALT) SS_UP(X_QUOT));
    }
    return false;
    break;

  case M_RSGL: // }
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_RALT) SS_DOWN(X_NUHS));
    } else {
      SEND_STRING(SS_UP(X_RALT) SS_UP(X_NUHS));
    }
    return false;
    break;

  case M_LBTN: // ( 
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_LSFT) SS_DOWN(X_8));
    } else {
      SEND_STRING(SS_UP(X_LSFT) SS_UP(X_8));
    }
    return false;
    break;

  case M_RBTN: // )
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_LSFT) SS_DOWN(X_9));
    } else {
      SEND_STRING(SS_UP(X_LSFT) SS_UP(X_9));
    }
    return false;
    break;

    case M_AT: // @
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_RALT) SS_DOWN(X_2));
    } else {
       SEND_STRING(SS_UP(X_2) SS_UP(X_RALT));
    }
    return false;
    break;

    case M_HASH: // #
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_RALT) SS_DOWN(X_3));
    } else {
      SEND_STRING(SS_UP(X_3) SS_UP(X_RALT));
    }
    return false;
    break;

    case M_UGRV: // ù
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_LSFT) SS_TAP(X_EQL) SS_UP(X_LSFT) SS_TAP(X_U));
    } else {
    }
    return false;
    break; 

    case M_RTBCT:  // >
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_LSFT) SS_DOWN(X_NUBS));
    } else {
      SEND_STRING(SS_UP(X_LSFT) SS_UP(X_NUBS));
    }
    return false;
    break;

    case M_ENCD: // Switches the encoder functions, handles alt-tabing when encoder is held
    if (record->event.pressed) {  
      button_pressed = true;   
      if(has_scrolled) {
        button_pressed = false;
        SEND_STRING(SS_UP(X_LALT));
      }
    } else {
      if(!has_scrolled) {
        scrollFunc = !scrollFunc;
        button_pressed = false;  
      } else if(!button_pressed) {
        has_scrolled = false;
      }
    }
    return false;
    break;

    case M_RAISE: // When raise + space are pressed, the raise layer is kept on
    if (record->event.pressed) {
      if(raiseOn) {
        layer_off(_RAISE);
      } else {
        layer_on(_RAISE);
      }
      raiseOn = !raiseOn;
    } else {
    }
    return false;
    break;

    case M_BAR: // |
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_RALT) SS_DOWN(X_7));
    } else {
      SEND_STRING(SS_UP(X_RALT) SS_UP(X_7));
    }
    return false;
    break;

    case M_NOT: // ~
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_RALT) SS_DOWN(X_EQL));
    } else {
      SEND_STRING(SS_UP(X_RALT) SS_UP(X_EQL));
    }
    return false;
    break;

    case M_BSLSH: // Backslash
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_RALT) SS_DOWN(X_NUBS));
    } else {
      SEND_STRING(SS_UP(X_RALT) SS_UP(X_NUBS));
    }
    return false;
    break;

    case M_LOCK: // Locks screen. Win + L
    if (record->event.pressed) {
      SEND_STRING(SS_LGUI("l"));
    }
    return false;
    break;

    case M_RGB: // Sets the RGB to a custom colour
    if (record->event.pressed) {
      rgblight_sethsv(103, 255, 255);
    }
    return false;
    break;

    case M_SNIP: // Takes a screenshot. Win + Shift + S
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_LGUI) SS_DOWN(X_LSFT) SS_TAP(X_S) SS_UP(X_LGUI) SS_UP(X_LSFT));
    }
    return false;
    break;

    case M_WBSPC: // Deletes word. Ctrl + Backspace
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_LCTRL) SS_TAP(X_BSPC) SS_UP(X_LCTRL));
    }
    return true;
    break;
    
    case M_PARE: // ( + ) + LEFT
    if (record->event.pressed) {
      SEND_STRING(SS_LSFT("8") SS_LSFT("9") SS_TAP(X_LEFT));
    }
    return true;
    break;

    case M_BRAC: // [ + ] + LEFT
    if (record->event.pressed) {
      SEND_STRING(SS_RALT("[]") SS_TAP(X_LEFT));
    }
    return true;
    break;

    case M_ACCO: // { + } + LEFT
    if (record->event.pressed) {
      SEND_STRING(SS_DOWN(X_RALT) SS_TAP(X_QUOT) SS_TAP(X_NUHS) SS_TAP(X_RALT) SS_TAP(X_LEFT));
    }
    return true;
    break;

    case M_CCCV:  // One key copy/paste
    if (record->event.pressed) {
      copy_paste_timer = timer_read();  
    } else {
      if (timer_elapsed(copy_paste_timer) > TAPPING_TERM) {  // Hold, copy
        tap_code16(LCTL(KC_C));
      } else { // Tap, paste
        tap_code16(LCTL(KC_V));
      }
    }
    break;
    /*
    
    case :
    if (record->event.pressed) {
      
    }
    return true;
    break;

    */
    }
  return true;
}

#ifdef LEADER_ENABLE
//Leader key
LEADER_EXTERNS();
void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();
    
    SEQ_ONE_KEY(KC_S) { // Snipping tool
        SEND_STRING(SS_LGUI("S"));
    }
    SEQ_ONE_KEY(KC_L) { // Lock
        SEND_STRING(SS_LGUI("l"));
    }
    SEQ_ONE_KEY(KC_B) { // ()
        SEND_STRING(SS_LSFT("8") SS_LSFT("9") SS_TAP(X_LEFT));
    } 
    SEQ_ONE_KEY(KC_D) { // []
        SEND_STRING(SS_RALT("[]") SS_TAP(X_LEFT));
    }
    SEQ_ONE_KEY(KC_G) { // {}
        SEND_STRING(SS_DOWN(X_RALT) SS_TAP(X_QUOT) SS_TAP(X_NUHS) SS_TAP(X_RALT) SS_TAP(X_LEFT));
    }
  }
}
#endif