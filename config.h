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

#pragma once

#define MASTER_LEFT
//#define MASTER_RIGHT

#define KEYLOGGER_ENABLE

#ifdef LEADER_ENABLE
#define LEADER_TIMEOUT 300
#define LEADER_PER_KEY_TIMING
#endif

#define TAPPING_TERM 200

#define ENCODER_RESOLUTION 2

#define OLED_DISPLAY_128X64

#ifdef OLED_DRIVER_ENABLE
	#define OLED_FONT_H "keyboards/kyria/keymaps/tourist/glcdfont.c"
  	#define OLED_DISPLAY_128X64
#endif

#ifdef RGBLIGHT_ENABLE
  #define RGBLIGHT_HUE_STEP 4
  #define RGBLIGHT_SAT_STEP 8
  #define RGBLIGHT_VAL_STEP 8
#endif

// Memory reduction
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION	
#define NO_ACTION_ONESHOT
#define NO_ACTION_TAPPING
#define TAPPING_FORCE_HOLD	
#define IGNORE_MOD_TAP_INTERRUPT
#define LAYER_STATE_8BIT
//#define NO_USB_STARTUP_CHECK
// Remove this if you want to debug!
#ifndef NO_DEBUG
    #define NO_DEBUG
#endif // !NO_DEBUG
#if !defined(NO_PRINT) && !defined(CONSOLE_ENABLE)
    #define NO_PRINT
#endif // !NO_PRINT

#define SPLIT_USB_DETECT
#define NO_USB_STARTUP_CHECK
//#define WAIT_FOR_USB


