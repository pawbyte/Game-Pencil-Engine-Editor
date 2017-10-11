/*
input_manager.h
This file is part of:
GAME PENCI ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the �Software�), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-Game Pencil Engine <https://create.pawbyte.com>


*/

#pragma once
#ifndef input_manager_H
#define input_manager_H
#include "GPE_CIncludes.h"
#include "timer.h"
#include <string>

// Input commands enum

const int kb_nokey = -1;
const int kb_anykey = -2;

const int kb_backspace = 8;
const int kb_tab = 9;
const int kb_enter = 13;
const int kb_pause = 19;
const int kb_caps = 20;


const int kb_shift = 16;
const int kb_ctrl = 17;
const int kb_alt = 18;

/*
var gp_fslash = 191;
var gp_gaccent = 192;
var gp_obracket = 220;
var gp_bslash = 220;
var gp_cbracket = 221;
var gp_squote = 222;
*/

const int kb_esc = 27;
const int kb_space = 32;
const int kb_page_up = 33;
const int kb_page_down = 34;
const int kb_end = 35;
const int kb_home = 36;

const int kb_left = 37;
const int kb_up = 38;
const int kb_right = 39;
const int kb_down = 40;

const int kb_insert = 45;
const int kb_delete = 46;

const int kb_0 = 48;
const int kb_1 = 49;
const int kb_2 = 50;
const int kb_3 = 51;
const int kb_4 = 52;
const int kb_5 = 53;
const int kb_6 = 54;
const int kb_7 = 55;
const int kb_8 = 56;
const int kb_9 = 57;

const int kb_a = 65;
const int kb_b = 66;
const int kb_c = 6;
const int kb_d = 68;
const int kb_e = 69;
const int kb_f = 70;
const int kb_g = 71;
const int kb_h = 72;
const int kb_i = 73;
const int kb_j = 74;
const int kb_k = 75;
const int kb_l = 76;
const int kb_m = 77;
const int kb_n = 78;
const int kb_o = 79;
const int kb_p = 80;
const int kb_q = 81;
const int kb_r = 82;
const int kb_s = 83;
const int kb_t = 84;
const int kb_u = 85;
const int kb_v = 86;
const int kb_w = 87;
const int kb_x = 88;
const int kb_y = 89;
const int kb_z = 90;

const int kb_f1 = 112;
const int kb_f2 = 113;
const int kb_f3 = 114;
const int kb_f4 = 115;
const int kb_f5 = 116;
const int kb_f6 = 117;
const int kb_f7 = 118;
const int kb_f8 = 119;
const int kb_f9 = 120;
const int kb_f10 = 121;
const int kb_f11 = 122;
const int kb_f12 = 123;

const int kb_plus = 187;
const int kb_comma = 188;
const int kb_minus = 189;
const int kb_period = 190;

const int kb_application = 200;

//Mouse constants [ BEGIN ]

const int mb_left = 0;
const int mb_right = 1;
const int mb_middle = 2;
const int mb_center = 2;

//Mouse constants [ END ]

/**
 * class GPE_InputManager
 *
 * Handles keyboard and mouse states
 */

class GPE_InputManager
{
    public:
        bool programJustStarted;
        int programStartTicks;
        time_t lastTimeScreenResized;
        bool keyInputReceivedInFrame;
        bool mouseInputReceivedInFrame;
        bool mouseMovementInputReceivedInFrame;
        bool mouseWheelInputReceivedInFrame;
        bool inputHappenedInFrame;
        bool windowEventHappendInFrame;
        char* dropped_filedir;
        static const int key_count = 256;
        static const int joy_key_count = 2;
        static const int mouse_button_count = 7;
        int binding[key_count];
        int binding_alt[key_count];
        int joy_binding[joy_key_count];
        int mouse_x, mouse_y;
        bool mouseScrollingUp;
        bool mouseScrollingDown;
        bool mouseScrollingLeft;
        bool mouseScrollingRight;
        bool backSpaceIsPressed;
        bool shiftKeyIsPressed;
        std::string binding_name[key_count];
        //mouse buttons
        int prevMouseButton[mouse_button_count];
        int downMouseButton[mouse_button_count];
        int pressedMouseButton[mouse_button_count];
        int releasedMouseButton[mouse_button_count];
        int doubleClickedMouseButton[mouse_button_count];
        std::string mouse_button[mouse_button_count];
        std::vector < std::string >fileDroppedList;
        bool forceRedrawNeeded;
        GPE_InputManager();
        ~GPE_InputManager();
        void defaultQwertyKeyBindings();
        void loadKeyBindings();
        void saveKeyBindings();
        void handle(bool dump_event = false, bool isEmbedded=false);
        //void handle_modifers( SDLMod mod );
        void resetScroll();
        bool anyKeyIsPress();
        //keyboard arrays
        bool prev[key_count];
        bool down[key_count];
        bool pressed[key_count];
        bool released[key_count];
        bool lock[key_count];
        bool joy_pressed[joy_key_count];
        bool check_keyboard_down(int buttonId=-1);
        bool check_keyboard_pressed(int buttonId=-1);
        bool check_keyboard_released(int buttonId=-1);
        bool input_received();
        int move_cursor(int cursor, int maxValue, int rowMax=1);
        bool check_mouse_doubleclicked(int buttonId=-1);
        bool check_mouse_down(int buttonId=-1);
        bool check_mouse_pressed(int buttonId=-1);
        bool check_mouse_released(int buttonId=-1);
        void reset_all_input();
        bool done;

        std::string inkeys;
        int last_key;
        int last_button;
        bool scroll_up;
        bool scroll_down;
};

#endif
