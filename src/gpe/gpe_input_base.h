/*
gpe_input_base.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#ifndef gpe_input_base_h
#define gpe_input_base_h

#include <ctime>
#include <string>
#include <vector>
#include "gpe_error_logger.h"
#include "gpe_input_event.h"
#include "gpe_window_controller_base.h"

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

//Mouse constants [ BEGIN ]
const int mb_anybutton = -1;
const int mb_left = 0;
const int mb_right = 1;
const int mb_middle = 2;
const int mb_center = 2;

//Mouse constants [ END ]

//gamepad constants [ BEGIN ]
const int gp_anycontroller = -1;
const int gp_nocontroller = -1;
const int gp_nobutton = -1;
const int gp_anybutton = -2;

const int gp_joystick_left_x = 0;
const int gp_joystick_left_y = 1;
const int gp_joystick_right_x = 2;
const int gp_joystick_right_y = 3;
const int gp_joystick_trigger_left = 4;
const int gp_joystick_trigger_right = 5;
const int gp_joystick_dpad_x = 6;
const int gp_joystick_dpad_y = 7;

const int gp_left = 0;
const int gp_right = 1;
const int gp_up = 2;
const int gp_down= 3;

const int gp_but0 = 4;
const int gp_but1 = 5;
const int gp_but2 = 6;
const int gp_but3 = 7;

const int gp_l1= 8;
const int gp_l2= 9;
const int gp_l3 = 10;

const int gp_r1= 11;
const int gp_r2= 12;
const int gp_r3= 13;

const int gp_start = 14;
const int gp_select = 15;
const int gp_home = 16;
const int gp_share = 17;

const float gp_deadzone = 32767.f;
const int gp_button_count = 25; //way more than enough for a video game controller
const int gp_axes_count = 16; //also way more than enough for any normal controller

//gamepad constants [ END ]

const int gp_max_devices = 16; //max of 16 players. Change this if you want manually.

namespace gpe
{
    class gamepad_base
    {
        protected:
            std::string system_handler;
            bool device_connected;
            std::string detected_device_type;
            std::string device_extension;
            std::string device_name;
            std::string device_type;
            int mapping;
            float timestamp;

            int touch_mouse_position_x; //for touch screen enabled controllers
            int touch_mouse_position_y;

        public:
            //Controller buttons
            int button_previous[gp_button_count];
            int button_button[gp_button_count];
            int button_pressed[gp_button_count];
            int button_released[gp_button_count];
            int button_locked[gp_button_count];
            int button_map[gp_button_count];

            //Axes / Joysticks
            int axes_map[gp_axes_count];
            int gp_system_axes_count;
            float axes_value[gp_axes_count];

            //Constructor/destructor/functions
            gamepad_base();
            virtual ~gamepad_base();
            int check_button(int button_id);
            int check_button_pressed(int button_id);
            int check_button_released(int button_id);
            int gamepad_get_axes_count( );
            std::string get_name( );
            std::string get_system_handler( );
            int gamepad_find_axes_map(int axes_id );
            int gamepad_find_button_map(int button_id );
            bool gamepad_map_axes(int axes_id, int new_axes_id );
            bool gamepad_map_button(int button_id, int newbutton_id );
            float get_axis_value( int axis_id);
            virtual void handle_input();
            bool is_connected( );
            virtual void pure_reset();
            virtual void reset_gamepad();
            virtual void reset_temp_input();
            void set_connected( bool now_connected);
            void set_name( std::string new_name);
            virtual void setup_default_mapping( bool map_buttons = true , bool mapAxis = true );
    };

    class input_manager_base
    {
        protected:
            input_event_container * event_container;
            int manager_id_number;
            int gamepad_current_count;
            std::string input_clipboard_text;
            std::string manager_type;
        public:
            bool debug_input;
            char* file_dropped_path;
            bool exit_requested;

            //gamepad data
            int gamepad_binding[ gp_button_count ];
            gamepad_base * game_pads[gp_max_devices];
            bool gamepad_requires_input;

            std::string inputted_keys;
            bool input_received;


            //keyboard data
            static const int kb_button_count = 256;
            bool kb_backspace_pressed;
            int kb_binding[kb_button_count];
            int kb_binding_alt[kb_button_count];
            std::string kb_binding_name[kb_button_count];

            bool kb_button_previous[ kb_button_count ];
            bool kb_button_down[ kb_button_count ];
            bool kb_button_pressed[ kb_button_count ];
            bool kb_button_released[ kb_button_count ];
            bool kb_button_locked[ kb_button_count ];
            bool kb_button_map[ kb_button_count ];
            bool kb_input_received;
            int kb_last_button;
            bool kb_shift_pressed;

            time_t last_screen_resize;

            //mouse data
            static const int mouse_button_count = 7;
            bool mouse_input_received;
            bool mouse_movement_received;
            int mouse_position_x, mouse_position_y;
            int mouse_previous_position_x, mouse_previous_position_y;
            bool mouse_scroll_inverted;
            bool mouse_wheel_received;
            bool mouse_scrolling_up;
            bool mouse_scrolling_down;
            bool mouse_scrolling_left;
            bool mouse_scrolling_right;

            //mouse buttons
            int mouse_previous_button[mouse_button_count];
            int mouse_down_button[mouse_button_count];
            int mouse_pressed_button[mouse_button_count];
            int mouse_released_button[mouse_button_count];
            int mouse_clicked_button[mouse_button_count];
            std::string mouse_button[mouse_button_count];
            std::vector < std::string >files_dropped_list;

            bool scroll_up;
            bool scroll_down;
            bool window_input_received;

            //Constructor / deconstructor / functions
            input_manager_base();
            virtual ~input_manager_base();

            //Keyboard functions [begin]
            bool any_key_pressed();
            void key_bind_qwerty();
            void key_bind_load();
            void key_bind_save();

            virtual bool clipboard_empty();
            virtual bool clipboard_set( std::string new_clipboard_string);
            virtual std::string clipboard_string();

            bool check_kb_only_down(int button_id = -1 );
            bool check_kb_only_released(int button_id = -1 );
            bool check_kb_down(int button_id=-1);
            bool check_kb_pressed(int button_id=-1);
            bool check_kb_released(int button_id=-1);

            //keyboard functions [ end ]

            //gamepad related functions [ begin ]
            int gamepad_check_button(int gamepad_id, int button_id);
            int gamepad_check_button_down(int gamepad_id, int button_id);
            int gamepad_check_button_pressed(int gamepad_id, int button_id);
            int gamepad_check_button_released(int gamepad_id, int button_id);

            int gamepad_count();
            bool gamepad_connected(int gamepad_id);
            virtual bool gamepad_detect_all();
            virtual bool gamepad_disconnect( int gamepad_id);

            std::string gamepad_detected_name(int gamepad_id);
            std::string gamepad_name(int gamepad_id);
            int gamepad_get_axes_count(int gamepad_id);

            int gamepad_find_axes_map(int gamepad_id, int axes_id);
            int gamepad_find_button_map(int gamepad_id, int button_id);
            int gamepad_find_map(int gamepad_id, int button_id);

            float gamepad_get_axis_value( int gamepad_id, int axis_id);
            gamepad_base * gamepad_get_object(int gamepad_id);

            bool gamepad_map_axes(int gamepad_id, int axes_id, int new_axes_id);
            bool gamepad_map_button(int gamepad_id, int button_id, int newbutton_id);
            virtual bool gamepad_setup(int cId );

            //gamepad related functions [ end ]

            //getters/handlers [ begin ]
            std::string get_type();
            virtual void handle_input(bool dump_event = false, bool is_embedded=false );
            void handle_input_end(bool dump_event = false, bool is_embedded=false );
            void handle_input_start(bool dump_event = false, bool is_embedded=false );

            //geters / handlers [ end ]

            bool has_new_input();
            virtual bool load_input_settings(std::string file_path = "");
            int move_cursor(int cursor, int max_value, int row_max=1);

            //mouse functions [ begin ]
            bool check_mouse_button_clicked(int button_id=-1);
            bool check_mouse_down(int button_id=-1);
            bool check_mouse_pressed(int button_id=-1);
            bool check_mouse_released(int button_id=-1);
            void reset_scroll_values();
            //mouse functions [ end ]

            //Reset functions [ begin ]
            void reset_all_input();
            void reset_temp_input();
            //reset functions [ end ]
    };

    extern int input_manger_id_count;
}

#endif //gpe_input_base_h
