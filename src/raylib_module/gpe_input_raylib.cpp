/*
gpe_input_raylib.cpp
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

#include "gpe_input_raylib.h"

namespace gpe
{
    gamepad_raylib::gamepad_raylib()
    {
        gamepad_id_number = -1;
        system_handler = "raylib";
    }

    gamepad_raylib::~gamepad_raylib()
    {

    }


    void gamepad_raylib::handle_input()
    {
        gamepad_base::handle_input();
    }

    void gamepad_raylib::pure_reset()
    {
        gamepad_base::pure_reset();

    }

    void gamepad_raylib::reset_gamepad()
    {
        gamepad_base::reset_gamepad();
    }

    void gamepad_raylib::reset_temp_input()
    {
        gamepad_base::reset_temp_input();
    }

    void gamepad_raylib::setup_default_mapping( bool map_buttons, bool mapAxis)
    {
        gamepad_base::setup_default_mapping( map_buttons, mapAxis );
        if( map_buttons)
        {
            //Maps the default controller buttons
             button_map[gp_but0] = GAMEPAD_BUTTON_RIGHT_FACE_DOWN;
             button_map[gp_but1] = GAMEPAD_BUTTON_RIGHT_FACE_LEFT;
             button_map[gp_but2] = GAMEPAD_BUTTON_RIGHT_FACE_RIGHT;
             button_map[gp_but3] = GAMEPAD_BUTTON_RIGHT_FACE_UP;

             button_map[gp_left] = GAMEPAD_BUTTON_LEFT_FACE_LEFT;
             button_map[gp_right] = GAMEPAD_BUTTON_LEFT_FACE_RIGHT;
             button_map[gp_up] = GAMEPAD_BUTTON_LEFT_FACE_UP;
             button_map[gp_down] = GAMEPAD_BUTTON_LEFT_FACE_DOWN;

             button_map[gp_l1] = GAMEPAD_BUTTON_LEFT_TRIGGER_1;
             button_map[gp_l2] = GAMEPAD_BUTTON_LEFT_TRIGGER_2;
             button_map[gp_l3] = GAMEPAD_BUTTON_LEFT_THUMB;

             button_map[gp_r1] = GAMEPAD_BUTTON_RIGHT_TRIGGER_1;
             button_map[gp_r2] = GAMEPAD_BUTTON_RIGHT_TRIGGER_2;
             button_map[gp_r3] = GAMEPAD_BUTTON_RIGHT_THUMB;

             button_map[gp_home] = GAMEPAD_BUTTON_MIDDLE;
             button_map[gp_select] = GAMEPAD_BUTTON_MIDDLE_LEFT;
             button_map[gp_start] = GAMEPAD_BUTTON_MIDDLE_RIGHT;
        }

        if( mapAxis )
        {
            axes_map[ gp_joystick_left_x ] = GAMEPAD_AXIS_LEFT_X;
            axes_map[ gp_joystick_left_y ] = GAMEPAD_AXIS_LEFT_Y;

            axes_map[ gp_joystick_right_x ] = GAMEPAD_AXIS_RIGHT_X;
            axes_map[ gp_joystick_right_y ] = GAMEPAD_AXIS_RIGHT_Y;

            axes_map[ gp_joystick_trigger_left ] = GAMEPAD_AXIS_LEFT_TRIGGER;
            axes_map[ gp_joystick_trigger_right ] = GAMEPAD_AXIS_RIGHT_TRIGGER;
        }
    }

    input_manager_raylib::input_manager_raylib()
    {
        manager_type = "raylib";
        for( int i_gamepad = 0; i_gamepad < gp_max_devices; i_gamepad++ )
        {
            game_pads_raylib[i_gamepad] = new gamepad_raylib();
            game_pads_raylib[i_gamepad]->gamepad_id_number = i_gamepad;
        }
        key_bind_qwerty();
    }

    input_manager_raylib::~input_manager_raylib()
    {

    }

    bool input_manager_raylib::clipboard_empty()
    {
        std::string clip_text = GetClipboardText();
        return !(clip_text.size() > 0);
    }

    bool input_manager_raylib::clipboard_set( std::string new_clipboard_string)
    {
        SetClipboardText( new_clipboard_string.c_str() );
        return true;
    }

    std::string input_manager_raylib::clipboard_string()
    {
        return GetClipboardText();
    }

    bool input_manager_raylib::gamepad_detect_all()
    {
        input_manager_base::gamepad_detect_all();

        int currentControllerId = 0;
        gamepad_current_count = 0;

        for ( int i_gamepad = 0; i_gamepad <= GAMEPAD_PLAYER4; i_gamepad++ )
        {
            if( IsGamepadAvailable( i_gamepad) )
            {
                gamepad_setup( i_gamepad );
                gamepad_current_count++;
            }
            else
            {
                game_pads_raylib[i_gamepad]->reset_gamepad();
            }
        }

        if( gamepad_current_count > 0)
        {
            return true;
        }

        if( debug_input )
        {
            error_log->report("No Game Controllers found...");
        }
        return false;
    }

    bool input_manager_raylib::gamepad_disconnect( int gamepad_id )
    {
        input_manager_base::gamepad_disconnect( gamepad_id );
        gamepad_raylib * temp_raylib_gamepad = nullptr;
        for( int i = gp_max_devices - 1; i >=0; i--)
        {
            if( game_pads[i]->get_system_handler() == "raylib" )
            {
                temp_raylib_gamepad->pure_reset();
                temp_raylib_gamepad->set_connected( false );
                if( gamepad_current_count > 0)
                {
                    gamepad_current_count--;
                }
            }
            return true;
        }
        if( debug_input)
        {
            error_log->report("gamepad["+ stg_ex::int_to_string(gamepad_id )+"] disconnected..." );
        }
        return false;
    }

    bool input_manager_raylib::gamepad_setup( int gamepad_id )
    {
        if( gamepad_id < GAMEPAD_PLAYER1 || gamepad_id > GAMEPAD_PLAYER4 )
        {
            if( debug_input )
            {
                error_log->report("Joystick ["+ stg_ex::int_to_string(gamepad_id)+"] is out of range!" );
            }
            return false;
        }

        if ( IsGamepadAvailable(gamepad_id) )
        {
            if( debug_input )
            {
                error_log->report("Finding gamepad...");
            }
            gamepad_base * tgamepad = game_pads[ gamepad_id ];
            if( tgamepad->get_system_handler() == "raylib" )
            {
                if( tgamepad != nullptr )
                {
                    gamepad_raylib * tgamepad = (gamepad_raylib*) tgamepad;
                    if( debug_input )
                    {
                        error_log->report("Finding Joystick ID...");
                    }
                    tgamepad->set_connected( true );
                    tgamepad->set_name( GetGamepadName( gamepad_id ) );

                    if( debug_input )
                    {
                        error_log->report("Mapping buttons...");
                    }
                    tgamepad->setup_default_mapping();
                    if( debug_input )
                    {
                        error_log->report("New gamepad has been setup.");
                    }
                    gamepad_current_count++;
                    return true;
                }
            }
            else if( debug_input )
            {
                error_log->report("Unable to open gamepad ("+ stg_ex::int_to_string( gamepad_id )+").");
                return false;
            }
        }
        else if( debug_input )
        {
            error_log->report("Failed...");
            game_pads_raylib[ gamepad_id]->set_connected( false );
            return false;
        }
        return true;
    }

    void input_manager_raylib::handle_input(bool dump_event, bool is_embedded )
    {
        handle_input_start( dump_event, is_embedded );

        int fControllerInputId = 0;
        gamepad_raylib *  tempController = nullptr;
        float  tempAxisValue  = 0.0;

        mouse_wheel_received  = false;
        mouse_scrolling_left  = false;
        mouse_scrolling_right = false;
        mouse_scrolling_up = false;
        mouse_scrolling_down = false;

        bool key_detected_down = false, key_detected_up = false;
        for( int i_key = 0; i_key < kb_button_count; i_key++ )
        {
            key_detected_down = false;
            key_detected_up = false;

            if( kb_binding[ i_key] >= 0 )
            {
                if( IsKeyPressed ( kb_binding[i_key] ) )
                {
                    key_detected_down = true;
                }
            }
            if(  kb_binding_alt[i_key]  >= 0 )
            {
                if(  IsKeyPressed ( kb_binding_alt[i_key] ) )
                {
                    key_detected_down = true;
                }
            }

            if( key_detected_down )
            {
                kb_input_received = true;
                kb_button_pressed[i_key] =  true;
                kb_button_down[i_key] =  true;
                kb_last_button = i_key;

                if( i_key == kb_shift)
                {
                    kb_shift_pressed = true;
                }

                if( i_key == kb_backspace)
                {
                    kb_backspace_pressed = true;
                }
            }
            key_detected_down = false;


            if( kb_binding[ i_key] >= 0 )
            {
                if( IsKeyReleased ( kb_binding[i_key] ) )
                {
                    key_detected_up = true;
                }
            }
            if(  kb_binding_alt[i_key]  >= 0 )
            {
                if(  IsKeyReleased ( kb_binding_alt[i_key] ) )
                {
                    key_detected_up = true;
                }
            }

            if( key_detected_up )
            {
                kb_input_received = true;
                kb_button_pressed[i_key] =  false;
                kb_button_down[i_key] =  false;
                kb_button_locked[i_key] = false;
                kb_last_button = i_key;

                if( i_key == kb_shift)
                {
                    kb_shift_pressed = false;
                }

                if( i_key == kb_backspace)
                {
                    kb_backspace_pressed = false;
                }
            }
            key_detected_up = false;
        }


        for( int i_mouse_button = MOUSE_LEFT_BUTTON; i_mouse_button <= MOUSE_MIDDLE_BUTTON; i_mouse_button++ )
        {
            if( IsMouseButtonPressed( i_mouse_button) )
            {
                input_received = true;
                mouse_input_received = true;

                mouse_down_button[i_mouse_button] = true;
                mouse_released_button[i_mouse_button] = false;

            }
            if( IsMouseButtonUp( i_mouse_button) )
            {
                mouse_down_button[ i_mouse_button ] = false;
                mouse_pressed_button[ i_mouse_button] = false;
            }
        }

        mouse_position_x = GetMouseX();
        mouse_position_y = GetMouseY();

        if( mouse_position_x!= mouse_previous_position_x || mouse_position_y != mouse_previous_position_y )
        {
            mouse_movement_received = true;
            mouse_input_received = true;
        }

        float mouse_wheel_movement = GetMouseWheelMove();
        if( mouse_wheel_movement != 0.f )
        {
            mouse_wheel_received = true;
            mouse_input_received = true;

            if( mouse_wheel_movement > 0.f )
            {
                if( mouse_scroll_inverted )
                {
                    mouse_scrolling_down = true;
                }
                else
                {
                    mouse_scrolling_up = true;
                }
            }
            else if ( mouse_wheel_movement < 0.f )
            {
                if( mouse_scroll_inverted )
                {
                    mouse_scrolling_up = true;
                }
                else
                {
                    mouse_scrolling_down = true;
                }
            }
        }

        handle_input_end( dump_event, is_embedded );
    }

    void input_manager_raylib::key_bind_qwerty()
    {
        for (int key=0; key<kb_button_count; key++)
        {
            kb_binding[ key] = -1;
            kb_binding_alt[ key] = -1;
        }

        kb_binding[kb_esc] = kb_binding_alt[kb_esc] ==  KEY_ESCAPE;
        kb_binding[kb_enter] = kb_binding_alt[kb_enter] = KEY_ENTER;
        kb_binding[kb_up] = kb_binding[kb_up] = KEY_UP;
        kb_binding[kb_down] = kb_binding[kb_down] =KEY_DOWN;
        kb_binding[kb_left] = kb_binding[kb_left] =KEY_LEFT;
        kb_binding[kb_right] = kb_binding[kb_right] =KEY_RIGHT;
        kb_binding[kb_space] = kb_binding_alt[kb_space] = KEY_SPACE;
        kb_binding[kb_page_down] = kb_binding_alt[kb_page_down] = KEY_PAGE_DOWN;
        kb_binding[kb_page_up] = kb_binding_alt[kb_page_up] = KEY_PAGE_UP;
        kb_binding[kb_a] = kb_binding_alt[kb_a] = KEY_A;
        kb_binding[kb_b] = kb_binding_alt[kb_b] = KEY_B;
        kb_binding[kb_c] = kb_binding_alt[kb_c] = KEY_C;
        kb_binding[kb_d] = kb_binding_alt[kb_d] = KEY_D;
        kb_binding[kb_e] = kb_binding_alt[kb_e] = KEY_E;
        kb_binding[kb_f] = kb_binding_alt[kb_f] = KEY_F;
        kb_binding[kb_g] = kb_binding_alt[kb_g] = KEY_G;
        kb_binding[kb_h] = kb_binding_alt[kb_h] = KEY_H;
        kb_binding[kb_i] = kb_binding_alt[kb_i] = KEY_I;
        kb_binding[kb_j] = kb_binding_alt[kb_j] = KEY_J;
        kb_binding[kb_k] = kb_binding_alt[kb_k] = KEY_K;
        kb_binding[kb_l] = kb_binding_alt[kb_l] = KEY_L;
        kb_binding[kb_m] = kb_binding_alt[kb_m] = KEY_M;
        kb_binding[kb_n] = kb_binding_alt[kb_n] = KEY_N;
        kb_binding[kb_o] = kb_binding_alt[kb_o] = KEY_O;
        kb_binding[kb_p] = kb_binding_alt[kb_p] = KEY_P;
        kb_binding[kb_q] = kb_binding_alt[kb_q] = KEY_Q;
        kb_binding[kb_r] = kb_binding_alt[kb_r] = KEY_R;
        kb_binding[kb_s] = kb_binding_alt[kb_s] = KEY_S;
        kb_binding[kb_t] = kb_binding_alt[kb_t] = KEY_T;
        kb_binding[kb_u] = kb_binding_alt[kb_u] = KEY_U;
        kb_binding[kb_v] = kb_binding_alt[kb_v] = KEY_V;
        kb_binding[kb_w] = kb_binding_alt[kb_w] = KEY_W;
        kb_binding[kb_x] = kb_binding_alt[kb_x] = KEY_X;
        kb_binding[kb_y] = kb_binding_alt[kb_y] = KEY_Y;
        kb_binding[kb_z] = kb_binding_alt[kb_z] = KEY_Z;
        kb_binding[kb_minus] = kb_binding_alt[kb_minus] = KEY_MINUS;
        kb_binding[kb_plus] = kb_binding_alt[kb_plus] = KEY_EQUAL;

        kb_binding[kb_1] = kb_binding_alt[kb_1] = KEY_ONE;
        kb_binding[kb_2] = kb_binding_alt[kb_2] = KEY_TWO;
        kb_binding[kb_3] = kb_binding_alt[kb_3] = KEY_THREE;
        kb_binding[kb_4] = kb_binding_alt[kb_4] = KEY_FOUR;
        kb_binding[kb_5] = kb_binding_alt[kb_5] = KEY_FIVE;
        kb_binding[kb_6] = kb_binding_alt[kb_6] = KEY_SIX;
        kb_binding[kb_7] = kb_binding_alt[kb_7] = KEY_SEVEN;
        kb_binding[kb_8] = kb_binding_alt[kb_8] = KEY_EIGHT;
        kb_binding[kb_9] = kb_binding_alt[kb_9] = KEY_NINE;
        kb_binding[kb_0] = kb_binding_alt[kb_0] = KEY_ZERO;

        kb_binding_alt[kb_1] = KEY_KP_1;
        kb_binding_alt[kb_2] = KEY_KP_2;
        kb_binding_alt[kb_3] = KEY_KP_3;
        kb_binding_alt[kb_4] = KEY_KP_4;
        kb_binding_alt[kb_5] = KEY_KP_5;
        kb_binding_alt[kb_6] = KEY_KP_6;
        kb_binding_alt[kb_7] = KEY_KP_7;
        kb_binding_alt[kb_8] = KEY_KP_8;
        kb_binding_alt[kb_9] = KEY_KP_9;
        kb_binding_alt[kb_0] = KEY_KP_0;
        //
        kb_binding[kb_f1] = kb_binding_alt[kb_f1] = KEY_F1;
        kb_binding[kb_f2] = kb_binding_alt[kb_f2] = KEY_F2;
        kb_binding[kb_f3] = kb_binding_alt[kb_f3] = KEY_F3;
        kb_binding[kb_f4] = kb_binding_alt[kb_f4] = KEY_F4;
        kb_binding[kb_f5] = kb_binding_alt[kb_f5] = KEY_F5;
        kb_binding[kb_f6] = kb_binding_alt[kb_f6] = KEY_F6;
        kb_binding[kb_f7] = kb_binding_alt[kb_f7] = KEY_F7;
        kb_binding[kb_f8] = kb_binding_alt[kb_f8] = KEY_F8;
        kb_binding[kb_f9] = kb_binding_alt[kb_f9] = KEY_F9;
        kb_binding[kb_f10] = kb_binding_alt[kb_f10] = KEY_F10;
        kb_binding[kb_f11] = kb_binding_alt[kb_f11] = KEY_F11;
        kb_binding[kb_f12] = kb_binding_alt[kb_f12] = KEY_F12;

        kb_binding[kb_comma] = KEY_COMMA;
        kb_binding_alt[kb_comma] = KEY_COMMA;
        kb_binding[kb_period]  = KEY_PERIOD;
        kb_binding_alt[kb_period] = KEY_PERIOD;

        kb_binding[kb_alt] = KEY_LEFT_ALT;
        kb_binding_alt[kb_alt] = KEY_RIGHT_ALT;
        kb_binding[kb_backspace] = KEY_BACKSPACE;
        kb_binding_alt[kb_backspace] = KEY_BACKSPACE;

        kb_binding[kb_ctrl] = KEY_LEFT_CONTROL;
        kb_binding_alt[kb_ctrl] = KEY_RIGHT_CONTROL;

        kb_binding[kb_shift] = KEY_LEFT_SHIFT;
        kb_binding_alt[kb_shift] = KEY_RIGHT_SHIFT;

        kb_binding[kb_delete] = KEY_DELETE ;
        kb_binding_alt[kb_delete] = KEY_DELETE;

        kb_binding[kb_print_screen] = KEY_PRINT_SCREEN;
        kb_binding_alt[kb_print_screen] = KEY_PRINT_SCREEN;

        kb_binding[kb_pause] = KEY_PAUSE;
        kb_binding_alt[kb_pause] = KEY_PAUSE;

        kb_binding[kb_tab] = kb_binding_alt[kb_tab] = KEY_TAB;
    }

    void input_manager_raylib::key_bind_load()
    {

    }

    void input_manager_raylib::key_bind_save()
    {

    }

    bool input_manager_raylib::load_input_settings(std::string file_path )
    {
        return input_manager_base::load_input_settings( file_path );
    }

    bool init_raylib_input_system()
    {
        error_log->report("Starting raylib_module input system...");
        if( input != nullptr )
        {
            delete input;
            input = nullptr;
        }
        input = new input_manager_raylib();
        error_log->report("Input swapped from base to raylib_module class...");
        return ( input!= nullptr);
    }

    void quit_raylib_input_system()
    {
        if( input != nullptr && input->get_type() == "raylib")
        {
            delete input;
            input = new input_manager_base();
        }
    }

}
