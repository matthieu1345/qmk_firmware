#include QMK_KEYBOARD_H
#include "version.h"

enum layers {
    BASE, // default layer
	NOTD, //Default Without TapDance
    SYMB, // symbols
    MDIA,  // media keys
	FIFO, // Firefox keys
};

enum TD {
	TD_SPACE_ENT = 0,
	TD_HOME_CTRL_PTT,
	TD_X_Cut,
	TD_C_Copy,
	TD_V_Paste,
	TD_Left,
	TD_Right,
	TD_Up,
	TD_Down,
};

typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP, // Send two single taps
    TRIPLE_TAP,
    TRIPLE_HOLD
};

uint8_t cur_dance(qk_tap_dance_state_t *state);
void home_finished(qk_tap_dance_state_t *state, void *user_data);
void home_reset(qk_tap_dance_state_t *state, void *user_data);
void process_light_button(void);
void process_light_timeout(void);
void setLights(uint8_t layer);

enum custom_keycodes {
#ifdef ORYX_CONFIGURATOR
  MULTI_BTN1 = EZ_SAFE_RANGE,
#else
  MULTI_BTN1 = SAFE_RANGE,
#endif
};

//{ custom defines
#define TDSpace TD(TD_SPACE_ENT)
#define TDHome TD(TD_HOME_CTRL_PTT)
#define ScrSht LGUI(LSFT(KC_S))
#define TD_X TD(TD_X_Cut)
#define TD_C TD(TD_C_Copy)
#define TD_V TD(TD_V_Paste)
//}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   =    |   1  |   2  |   3  |   4  |   5  |ScrSht|           |PrtScr|   6  |   7  |   8  |   9  |   0  |   -    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Del    |   Q  |   W  |   E  |   R  |   T  |  L1  |           |  L2  |   Y  |   U  |   I  |   O  |   P  |   \    |
 * |--------+------+------+------+------+------| SYMB |           | MDIA |------+------+------+------+------+--------|
 * | BkSp   |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |; / L2|' / Cmd |
 * |--------+------+------+------+------+------| Hyper|           | Meh  |------+------+------+------+------+--------|
 * | LShift |Z/Ctrl|   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |//Ctrl| RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |Grv/L1|  '"  |AltShf| Left | Right|                                       |  Up  | Down |   [  |   ]  | ~L1  |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,---------------.
 *                                        | App  | ESC  |       | Alt  |Ctrl/Esc|
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      | End  |       | PgUp |        |      |
 *                                 | Space|Backsp|------|       |------|  Tab   |Enter |
 *                                 |      |ace   |TDHome|       | PgDn |        |      |
 *                                 `--------------------'       `----------------------'
 */
[BASE] = LAYOUT_ergodox_pretty(
  // left hand																														// Right hand
    KC_EQUAL,       KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           ScrSht,							KC_PSCR,     KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           	KC_MINUS,
    KC_DELETE,      KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,           TG(SYMB),						TG(MDIA),       KC_Y,           KC_U,           KC_I,           KC_O,           KC_P,           	KC_BSLS,
    KC_BSPC,      KC_A,           KC_S,           KC_D,           KC_F,           KC_G,															KC_H,           KC_J,           KC_K,           KC_L,           LT(MDIA,KC_SCLN),	LGUI_T(KC_QUOTE),
    KC_LSFT,      KC_Z,           TD_X,           TD_C,           TD_V,           KC_B,           KC_HYPR,						KC_MEH,         KC_N,           KC_M,           KC_COMMA,       KC_DOT,         RCTL_T(KC_SLASH),	KC_RSFT,
    LT(SYMB,KC_GRAVE), KC_LCTL,    KC_LALT,        TD(TD_Left),    TD(TD_Right),																					TD(TD_Up),      TD(TD_Down),    KC_LPRN,        KC_RPRN,        	TG(NOTD),
																					KC_LGUI,        KC_ESCAPE,						LGUI(KC_TAB),   LALT(LCTL(KC_TAB)),
																									LALT_T(KC_END),					KC_PGUP,
																	TDSpace, 		KC_BSPC,    	TDHome,							KC_PGDN,      KC_TAB,         KC_ENTER
  ),
/* Keymap 1: Tap Dance Disabled
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   =    |   1  |   2  |   3  |   4  |   5  |ScrSht|           |PrtScr|   6  |   7  |   8  |   9  |   0  |   -    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Del    |   Q  |   W  |   E  |   R  |   T  |  L1  |           |  L2  |   Y  |   U  |   I  |   O  |   P  |   \    |
 * |--------+------+------+------+------+------| SYMB |           | MDIA |------+------+------+------+------+--------|
 * | BkSp   |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |; / L2|' / Cmd |
 * |--------+------+------+------+------+------| Hyper|           | Meh  |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |//Ctrl| RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |Grv/L1| ctrl |  Alt | Left | Right|                                       |  Up  | Down |   [  |   ]  | L0  |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,---------------.
 *                                        | Win  | ESC  |       |AltTab|Ctrl/Tab|
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      |AltEnd|       | PgUp |        |      |
 *                                 | Space|Backsp|------|       |------|  Tab   |Enter |
 *                                 |      |ace   | F13  |       | PgDn |        |      |
 *                                 `--------------------'       `----------------------'
 */
[NOTD] = LAYOUT_ergodox_pretty(
  // left hand																														// Right hand
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_R,           KC_Q,           KC_W,           KC_E,			KC_TRANSPARENT, KC_TRANSPARENT,                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_F,           KC_A,           KC_S,           KC_D,			KC_TRANSPARENT,                                 				KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_V, 			KC_Z,           KC_X,           KC_C,           KC_TRANSPARENT, KC_TRANSPARENT,					KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,	KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_LEFT, 		KC_RIGHT,																						KC_UP, 			KC_DOWN, 		KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
																					KC_TRANSPARENT, KC_TRANSPARENT,					KC_TRANSPARENT, KC_TRANSPARENT,
																									KC_TRANSPARENT,					KC_TRANSPARENT,
																	KC_SPACE,		KC_TRANSPARENT, KC_F13,							KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
),
/* Keymap 2: Symbol Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |Version  |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |   !  |   @  |   {  |   }  |   |  |      |           |      |   Up |   7  |   8  |   9  |   -  |    /   |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |   #  |   $  |   (  |   )  |   `  |------|           |------| Down |   4  |   5  |   6  |   +  |    *   |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |   %  |   ^  |   [  |   ]  |   ~  |      |           |      |   &  |   1  |   2  |   3  |      |   F12  |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      |   0  |   .  |  ent |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       | To 0 |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |Hue-  |Hue+  |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      | Caps |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[SYMB] = LAYOUT_ergodox_pretty(
  // left hand
    KC_ESCAPE,      KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,          KC_CAPS,						KC_NUM, 				KC_F6,			KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_F11,
    KC_TRANSPARENT, KC_EXLM,        KC_AT,          KC_LCBR,        KC_RCBR,        KC_PIPE,        KC_TRANSPARENT,					KC_TRANSPARENT, 		KC_UP,          KC_KP_7,        KC_KP_8,        KC_KP_9,        KC_KP_MINUS,    KC_KP_SLASH,
    KC_TRANSPARENT, KC_HASH,        KC_DLR,         KC_LPRN,        KC_RPRN,        KC_GRAVE,																KC_DOWN,        KC_KP_4,        KC_KP_5,        KC_KP_6,        KC_KP_PLUS,     KC_KP_ASTERISK,
    KC_TRANSPARENT, KC_PERC,        KC_CIRC,        KC_LBRC,    	KC_RBRC,    KC_TILD,        KC_TRANSPARENT,					KC_TRANSPARENT, 		KC_AMPR,        KC_KP_1,        KC_KP_2,        KC_KP_3,        KC_TRANSPARENT, KC_F12,
    KC_TRANSPARENT, KC_EQUAL,       KC_TRANSPARENT, KC_TRANSPARENT, KC_INSERT,																								KC_TRANSPARENT, KC_KP_0,        KC_KP_DOT,      KC_KP_ENTER,    KC_TRANSPARENT,
                                                                    KC_TRANSPARENT, KC_TRANSPARENT, 								TO(BASE),				KC_TRANSPARENT,
                                                                                                    KC_TRANSPARENT, 				KC_TRANSPARENT,
                                                                    KC_TRANSPARENT, KC_TRANSPARENT, KC_CAPS, 						KC_TRANSPARENT, 		KC_TRANSPARENT, KC_KP_ENTER
  ),
/* Keymap 3: Media and mouse keys
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |Rclk++| Rclk | MsUp | Lclk |      |      |           |      |      |      |Arw Up|      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |MsLeft|MsDown|MsRght|      |------|           |------|      |ArwLef|ArwDwn|ArwRig|      |  Play  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |MsWhDw|      |MsWhUp|      |      |           |      |      |      | Prev | Next |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |VolUp |VolDn | Mute |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |Brwser|
 *                                 |      |      |------|       |------|      |Back  |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[MDIA] = LAYOUT_ergodox_pretty(
  // left hand
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, MULTI_BTN1,     KC_MS_BTN1,     KC_MS_UP,       KC_MS_BTN2,     KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_UP,          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_MS_LEFT,     KC_MS_DOWN,     KC_MS_RIGHT,    KC_TRANSPARENT,                                                                 KC_TRANSPARENT, KC_LEFT,        KC_DOWN,        KC_RIGHT,       KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_MS_WH_DOWN,  KC_TRANSPARENT, KC_MS_WH_UP,    KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_MEDIA_PLAY_PAUSE,KC_MEDIA_PREV_TRACK,KC_MEDIA_NEXT_TRACK,KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                                                                                 KC_AUDIO_VOL_UP,KC_AUDIO_VOL_DOWN,KC_AUDIO_MUTE,  KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                                    KC_TRANSPARENT, KC_TRANSPARENT, TO(BASE),          KC_TRANSPARENT,
                                                                                                                    KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                    KC_TRANSPARENT, KC_TRANSPARENT, OSL(FIFO),		KC_TRANSPARENT, KC_TRANSPARENT, KC_WWW_BACK
  ),
/* Keymap 4: Firefox Shortcuts
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      | MsUp |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |MsLeft|MsDown|MsRght|      |------|           |------|      |      |      |      |      |  Play  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      | Prev | Next |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      | Lclk | Rclk |                                       |VolUp |VolDn | Mute |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |Brwser|
 *                                 |      |      |------|       |------|      |Back  |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[FIFO] = LAYOUT_ergodox_pretty(
  // left hand
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, C(KC_GRAVE), 	C(S(KC_UP)), 	C(S(KC_GRAVE)), KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, C(KC_PGUP), 	C(S(KC_DOWN)), 	C(KC_PGDN), 	KC_TRANSPARENT,                                                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,	KC_TRANSPARENT,	KC_TRANSPARENT,	KC_TRANSPARENT, KC_TRANSPARENT,
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                                                                                 KC_TRANSPARENT,	KC_TRANSPARENT,	KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                                    KC_TRANSPARENT, KC_TRANSPARENT, TO(BASE),       KC_TRANSPARENT,
                                                                                                                    KC_TRANSPARENT, KC_TRANSPARENT,
                                                                                    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
  ),
};

static uint16_t rapid_timer = 0;
static bool rapid_pressed = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		process_light_button();
	}
	
	switch (keycode) {
	case MULTI_BTN1:
		if (record->event.pressed) {
			// turn on rapid fire of btn1
			if (rapid_timer == 0 && !rapid_pressed) {
				rapid_timer = timer_read();
				rapid_pressed = true;
			}
		} else {
			// turn off rapid fire of btn1
			rapid_timer = 0;
			rapid_pressed = false;
		}
		
	}
  return true;
};

void matrix_scan_user(void) {

	process_light_timeout();
	
	// process rapid fire btn 1
	if (timer_elapsed(rapid_timer) > 100 && rapid_pressed) {
		tap_code(KC_MS_BTN1);
		rapid_timer = timer_read();
	}

}

// Runs just one time when the keyboard initializes.
void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_COLOR_LAYER_0
  rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
};

//{ Calculate Tapdance State
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        // DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TRIPLE_TAP;
        else return TRIPLE_HOLD;
    } else return 8; // Magic number. At some point this method will expand to work for more presses
};

uint8_t cur_dance_permissive(qk_tap_dance_state_t *state)
{
	if (state->count == 1)
	{
		if(!state->pressed) return SINGLE_TAP;
		else return SINGLE_HOLD;
	}
	else return (cur_dance(state));
};
//}

//{ Home Tapdance
// Create an instance of 'tap' for the 'home' tap dance.
static tap hometap_state = {
    .is_press_action = true,
    .state = 0
};

void home_finished(qk_tap_dance_state_t *state, void *user_data) {
	hometap_state.state = cur_dance_permissive(state);
    switch (hometap_state.state) {
        case SINGLE_TAP: register_code(KC_HOME); break;
        case SINGLE_HOLD: register_code(KC_LCTL); break;
        case DOUBLE_TAP: register_code(KC_ESC); break;
        case DOUBLE_HOLD: register_code(KC_F13); break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        case DOUBLE_SINGLE_TAP: tap_code(KC_HOME); register_code(KC_HOME);
    }
};

void home_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (hometap_state.state) {
        case SINGLE_TAP: unregister_code(KC_HOME); break;
        case SINGLE_HOLD: unregister_code(KC_LCTL); break;
        case DOUBLE_TAP: unregister_code(KC_ESC); break;
        case DOUBLE_HOLD: unregister_code(KC_F13); break;
        case DOUBLE_SINGLE_TAP: unregister_code(KC_HOME);
    }
    hometap_state.state = 0;
};
//}

//{ Cut-, Copy-, Paste- Tapdance
//{ Cut
// Create an instance of 'tap' for the 'cut' tap dance.
static tap cuttap_state = {
    .is_press_action = true,
    .state = 0
};

void cut_each(qk_tap_dance_state_t *state, void *user_data)
{
	if(state->count == 2)
	{
		tap_code(KC_X);
		tap_code(KC_X);
	}
	else if(state-> count > 2)
		tap_code(KC_X);
}

void cut_finished(qk_tap_dance_state_t *state, void *user_data) {
	cuttap_state.state = cur_dance(state);
    switch (cuttap_state.state) {
        case SINGLE_TAP: register_code(KC_X); break;
        case SINGLE_HOLD: register_code16(C(KC_X)); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        case DOUBLE_SINGLE_TAP: break;
    }
};

void cut_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (cuttap_state.state) {
        case SINGLE_TAP: unregister_code(KC_X); break;
        case SINGLE_HOLD: unregister_code16(C(KC_X)); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        case DOUBLE_SINGLE_TAP: break;
    }
    cuttap_state.state = 0;
};
//}
//{ copy
// Create an instance of 'tap' for the 'copy' tap dance.
static tap copytap_state = {
    .is_press_action = true,
    .state = 0
};

void copy_each(qk_tap_dance_state_t *state, void *user_data)
{
	if(state->count == 2)
	{
		tap_code(KC_C);
		tap_code(KC_C);
	}
	else if(state-> count > 2)
		tap_code(KC_C);
}

void copy_finished(qk_tap_dance_state_t *state, void *user_data) {
	copytap_state.state = cur_dance(state);
    switch (copytap_state.state) {
        case SINGLE_TAP: register_code(KC_C); break;
        case SINGLE_HOLD: register_code16(C(KC_C)); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        case DOUBLE_SINGLE_TAP: break;
    }
};

void copy_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (copytap_state.state) {
        case SINGLE_TAP: unregister_code(KC_C); break;
        case SINGLE_HOLD: unregister_code16(C(KC_C)); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        case DOUBLE_SINGLE_TAP: break;
    }
    copytap_state.state = 0;
};
//}
//{ paste
// Create an instance of 'tap' for the 'paste' tap dance.
static tap pastetap_state = {
    .is_press_action = true,
    .state = 0
};

void paste_each(qk_tap_dance_state_t *state, void *user_data)
{
	if(state->count == 2)
	{
		tap_code(KC_V);
		tap_code(KC_V);
	}
	else if(state-> count > 2)
		tap_code(KC_V);
}

void paste_finished(qk_tap_dance_state_t *state, void *user_data) {
	pastetap_state.state = cur_dance(state);
    switch (pastetap_state.state) {
        case SINGLE_TAP: register_code(KC_V); break;
        case SINGLE_HOLD: register_code16(C(KC_V)); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        case DOUBLE_SINGLE_TAP: break;
    }
};

void paste_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (pastetap_state.state) {
        case SINGLE_TAP: unregister_code(KC_V); break;
        case SINGLE_HOLD: unregister_code16(C(KC_V)); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        case DOUBLE_SINGLE_TAP: break;
    }
    pastetap_state.state = 0;
};
//}
//}

//{ Left Right Up and Down arrow Tapdance
//{ Left arrow
static tap Lefttap_state = {
    .is_press_action = true,
    .state = 0
};

void Left_each(qk_tap_dance_state_t *state, void *user_data)
{
	if(state->count == 2)
	{
		tap_code(KC_LEFT);
		tap_code(KC_LEFT);
	}
	else if(state-> count > 2)
		tap_code(KC_LEFT);
}

void Left_finished(qk_tap_dance_state_t *state, void *user_data) {
	Lefttap_state.state = cur_dance(state);
    switch (Lefttap_state.state) {
        case SINGLE_TAP: register_code(KC_LEFT); break;
        case SINGLE_HOLD: register_code16(C(KC_LEFT)); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        case DOUBLE_SINGLE_TAP: break;
    }
};

void Left_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (Lefttap_state.state) {
        case SINGLE_TAP: unregister_code(KC_LEFT); break;
        case SINGLE_HOLD: unregister_code16(C(KC_LEFT)); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        case DOUBLE_SINGLE_TAP: break;
    }
    Lefttap_state.state = 0;
};
//}
//{ right arrow
static tap Righttap_state = {
    .is_press_action = true,
    .state = 0
};

void Right_each(qk_tap_dance_state_t *state, void *user_data)
{
	if(state->count == 2)
	{
		tap_code(KC_RIGHT);
		tap_code(KC_RIGHT);
	}
	else if(state-> count > 2)
		tap_code(KC_RIGHT);
}

void Right_finished(qk_tap_dance_state_t *state, void *user_data) {
	Righttap_state.state = cur_dance(state);
    switch (Righttap_state.state) {
        case SINGLE_TAP: register_code(KC_RIGHT); break;
        case SINGLE_HOLD: register_code16(C(KC_RIGHT)); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        case DOUBLE_SINGLE_TAP: break;
    }
};

void Right_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (Righttap_state.state) {
        case SINGLE_TAP: unregister_code(KC_RIGHT); break;
        case SINGLE_HOLD: unregister_code16(C(KC_RIGHT)); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        case DOUBLE_SINGLE_TAP: break;
    }
    Righttap_state.state = 0;
};
//}
//{ Up arrow
static tap Uptap_state = {
    .is_press_action = true,
    .state = 0
};

void Up_each(qk_tap_dance_state_t *state, void *user_data)
{
	if(state->count == 2)
	{
		tap_code(KC_UP);
		tap_code(KC_UP);
	}
	else if(state-> count > 2)
		tap_code(KC_UP);
}

void Up_finished(qk_tap_dance_state_t *state, void *user_data) {
	Uptap_state.state = cur_dance(state);
    switch (Uptap_state.state) {
        case SINGLE_TAP: register_code(KC_UP); break;
        case SINGLE_HOLD: register_code16(KC_PGUP); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        case DOUBLE_SINGLE_TAP: break;
    }
};

void Up_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (Uptap_state.state) {
        case SINGLE_TAP: unregister_code(KC_UP); break;
        case SINGLE_HOLD: unregister_code16(KC_PGUP); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        case DOUBLE_SINGLE_TAP: break;
    }
    Uptap_state.state = 0;
};
//}
//{ Down arrow
static tap Downtap_state = {
    .is_press_action = true,
    .state = 0
};

void Down_each(qk_tap_dance_state_t *state, void *user_data)
{
	if(state->count == 2)
	{
		tap_code(KC_DOWN);
		tap_code(KC_DOWN);
	}
	else if(state-> count > 2)
		tap_code(KC_DOWN);
}

void Down_finished(qk_tap_dance_state_t *state, void *user_data) {
	Downtap_state.state = cur_dance(state);
    switch (Downtap_state.state) {
        case SINGLE_TAP: register_code(KC_DOWN); break;
        case SINGLE_HOLD: register_code16(KC_PGDN); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        case DOUBLE_SINGLE_TAP: break;
    }
};

void Down_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (Downtap_state.state) {
        case SINGLE_TAP: unregister_code(KC_DOWN); break;
        case SINGLE_HOLD: unregister_code16(KC_PGDN); break;
        case DOUBLE_TAP: ; break;
        case DOUBLE_HOLD: ; break;
        case DOUBLE_SINGLE_TAP: break;
    }
    Downtap_state.state = 0;
};
//}
//}

qk_tap_dance_action_t tap_dance_actions[] = {

[TD_SPACE_ENT]  = ACTION_TAP_DANCE_DOUBLE(KC_SPACE, KC_ENT),
[TD_HOME_CTRL_PTT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, home_finished, home_reset),
[TD_X_Cut] = ACTION_TAP_DANCE_FN_ADVANCED(cut_each, cut_finished, cut_reset),
[TD_C_Copy] = ACTION_TAP_DANCE_FN_ADVANCED(copy_each, copy_finished, copy_reset),
[TD_V_Paste] = ACTION_TAP_DANCE_FN_ADVANCED(paste_each, paste_finished, paste_reset),
[TD_Left] = ACTION_TAP_DANCE_FN_ADVANCED(Left_each, Left_finished, Left_reset),
[TD_Right] = ACTION_TAP_DANCE_FN_ADVANCED(Right_each, Right_finished, Right_reset),
[TD_Up] = ACTION_TAP_DANCE_FN_ADVANCED(Up_each, Up_finished, Up_reset),
[TD_Down] = ACTION_TAP_DANCE_FN_ADVANCED(Down_each, Down_finished, Down_reset),
// Other declarations would go here, separated by commas, if you have them
};

//{ turn of lights after 10 min of no use
// Backlight timeout feature
#define BACKLIGHT_TIMEOUT 5    // in minutes
static uint16_t idle_timer = 0;
static uint8_t halfmin_counter = 0;
static uint8_t lastLayer = -1;
static bool led_on = true;

void process_light_button(void)
{
	if (led_on == false) {
        setLights(lastLayer);
        led_on = true;
    }
	
	idle_timer = timer_read();
	halfmin_counter = 0;
}

void process_light_timeout(void)
{
	// idle_timer needs to be set one time
    if (idle_timer == 0) idle_timer = timer_read();
	
	if ( led_on && timer_elapsed(idle_timer) > 30000) {
		halfmin_counter++;
		idle_timer = timer_read();
	}
	
	if ( led_on && halfmin_counter >= BACKLIGHT_TIMEOUT * 2) {
		setLights(-1);
		led_on = false;
		halfmin_counter = 0;
	}
}

//}

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
	uint8_t layer = get_highest_layer(state);
	lastLayer = layer;
	setLights(layer);
	return state;
}
	
void setLights(uint8_t layer)
{
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
  
    switch (layer) {
      case 0:
        #ifdef RGBLIGHT_COLOR_LAYER_0
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
        #endif
        break;
      case 1:
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_1
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_1);
        #endif
        break;
      case 2:
        ergodox_right_led_1_on();
        #ifdef RGBLIGHT_COLOR_LAYER_2
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_2);
        #endif
        break;
      case 3:
        ergodox_right_led_2_on();
        #ifdef RGBLIGHT_COLOR_LAYER_3
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_3);
        #endif
        break;
      case 4:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        #ifdef RGBLIGHT_COLOR_LAYER_4
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_4);
        #endif
        break;
      case 5:
        ergodox_right_led_1_on();
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_5
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_5);
        #endif
        break;
      case 6:
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_6
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_6);
        #endif
        break;
      case 7:
	    ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_7
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_7);
        #endif
        break;
      default:
        break;
    }
};
