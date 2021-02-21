/*
gpe_input_base.cpp
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

#include <iostream>
#include "gpe_globals.h"
#include "gpe_input_base.h"
#include "../other_libs/stg_ex.h"

namespace gpe
{
    int input_manger_id_count = 0;

    gamepad_base::gamepad_base()
    {
        gamepad_id_number = 0;
        system_handler = "";
        gp_system_axes_count = 0;
        device_extension = "";
        device_connected = false;
        mapping = 0;
        timestamp = 0;
        touch_mouse_position_x = 0;
        touch_mouse_position_y = 0;
        for(int but=0; but <gp_button_count; but++)
        {
            button_previous[but] = false;
            button_button[but] = false;
            button_pressed[but] = false;
            button_released[but] = false;
            button_locked[but] = false;
            button_map[but] = -1;
        }


        for (int tempAxes=0; tempAxes < gp_axes_count; tempAxes++)
        {
            axes_map[ tempAxes ] = tempAxes;
            axes_value[ tempAxes ] = 0;
        }

        setup_default_mapping( );

    }

    gamepad_base::~gamepad_base()
    {

    }

    bool gamepad_base::is_connected()
    {
        return device_connected;
    }

    int gamepad_base::check_button(int button_id)
    {
        if( device_connected )
        {
            if(  button_id >=0 && button_id < gp_button_count)
            {
                if( button_map[button_id] >= 0)
                {
                    return button_button[ button_map[button_id] ] > 0;
                }
            }
            else if( button_id == gp_anybutton )
            {
                for( int iAny_button = gp_button_count-1; iAny_button >=0; iAny_button-- )
                {
                    if( button_button[ button_map[iAny_button] ] > 0 )
                    {
                        return true;
                    }
                }
                return false;
            }
            else if( button_id == gp_nobutton )
            {
                for(int iNo_button = gp_button_count-1; iNo_button >=0; iNo_button-- )
                {
                    if( button_button[ button_map[iNo_button] ] > 0 )
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
    }

    int gamepad_base::check_button_pressed(int button_id)
    {
        if( device_connected )
        {
            if( button_id >=0 && button_id < gp_button_count)
            {
                if( button_map[button_id] >= 0 )
                {
                    return button_pressed[ button_map[button_id] ] > 0;
                }
            }
            else if( button_id == gp_anybutton )
            {
                for(int iAny_button = gp_button_count-1; iAny_button >=0; iAny_button-- )
                {
                    if( button_pressed[ button_map[iAny_button] ] > 0 )
                    {
                        return true;
                    }
                }
                return false;
            }
            else if( button_id == gp_nobutton )
            {
                for( int iNo_button = gp_button_count-1; iNo_button >=0; iNo_button-- )
                {
                    if( button_pressed[ button_map[iNo_button] ] > 0 )
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
    }

    int gamepad_base::check_button_released(int button_id)
    {
        if( device_connected )
        {
            if( button_id >=0 && button_id < gp_button_count)
            {
                if( button_map[ button_id] >= 0 )
                {
                    return button_released[ button_map[button_id] ];
                }
            }
            else if( button_id == gp_anybutton )
            {
                for( int iAny_button = gp_button_count-1; iAny_button >=0; iAny_button-- )
                {
                    if( button_released[ button_map[iAny_button] ] > 0 )
                    {
                        return true;
                    }
                }
                return false;
            }
            else if( button_id == gp_nobutton )
            {
                for( int iNo_button = gp_button_count-1; iNo_button >=0; iNo_button-- )
                {
                    if( button_released[ button_map[iNo_button] ] > 0 )
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
    }

    int gamepad_base::gamepad_get_axes_count( )
    {
        return gp_system_axes_count;
    }

    int gamepad_base::gamepad_find_axes_map( int axes_id )
    {
        if(  axes_id >=0 && axes_id < gp_axes_count )
        {
            return axes_map[axes_id];
        }
        return -1;
    }

    int gamepad_base::gamepad_find_button_map( int button_id )
    {
        if(  button_id >=0 && button_id < gp_button_count )
        {
            return axes_map[button_id];
        }
        return -1;
    }

    bool gamepad_base::gamepad_map_axes(int axes_id, int new_axes_id )
    {
        if(  axes_id >=0 && axes_id < gp_axes_count)
        {
            if( new_axes_id >=0 && new_axes_id < gp_axes_count )
            {
                button_map[axes_id] = new_axes_id;
                return true;
            }
        }
        return false;
    }

    bool gamepad_base::gamepad_map_button(int button_id, int newbutton_id)
    {
        if(  button_id >=0 && button_id < gp_button_count)
        {
            if(  newbutton_id >=0 && newbutton_id < gp_button_count)
            {
                button_map[button_id] = newbutton_id;
                return true;
            }
        }
        return false;
    }

    float gamepad_base::get_axis_value( int axis_id)
    {
        if( axis_id >=0 && axis_id < gp_axes_count )
        {
            return axes_value[ axis_id];
        }
        return 0;
    }

    std::string gamepad_base::get_name( )
    {
        return device_name;
    }

    std::string gamepad_base::get_system_handler( )
    {
        return system_handler;
    }

    //gamepad Input related functions/objects/variables as of Version 1.12 [ END ]
    void gamepad_base::handle_input()
    {

    }

    void gamepad_base::pure_reset()
    {
        for( int but=0; but< gp_button_count; but++)
        {
            button_previous[ but ] = false;
            button_button[ but ] = false;
            button_pressed[ but ] = false;
            button_released[ but] = false;
            button_locked[ but ] = false;
        }

        for (int tempAxes=0; tempAxes <gp_axes_count; tempAxes++)
        {
            axes_value[ tempAxes ] = 0;
        }
    }

    void gamepad_base::reset_gamepad()
    {
        for(int but=0; but<gp_button_count; but++)
        {
            button_pressed[ but ] = button_button[ but ] && !button_previous[but];
            button_released[ but] = !button_button[ but ] && button_previous[ but ];
        }
    }

    void gamepad_base::reset_temp_input()
    {
        for(int but=0; but<gp_button_count; but++)
        {
            button_pressed[ but ] = false;
            button_released[ but] = false;
        }
    }

    void gamepad_base::set_connected( bool now_connected )
    {
        device_connected = now_connected;
    }

    void gamepad_base::set_name( std::string new_name)
    {
        device_name = new_name;
    }

    void gamepad_base::setup_default_mapping( bool map_buttons, bool mapAxis)
    {
        if( map_buttons)
        {
            for(int but=0; but <gp_button_count; but++)
            {
                button_map[but] = -1;
            }
        }

        if( mapAxis )
        {
            for (int temp_axis = 0; temp_axis < gp_axes_count; temp_axis++)
            {
                axes_map[ temp_axis ] = -1;
            }
        }
    }

    input_manager_base::input_manager_base()
    {
        event_container = new input_event_container();
        manager_id_number = input_manger_id_count;
        input_manger_id_count++;
        manager_type = "input_base";
        debug_input = true;
        gamepad_requires_input = true;
        time(&last_screen_resize);
        exit_requested = false;
        mouse_scroll_inverted = false;
        kb_backspace_pressed = false;
        kb_shift_pressed = false;
        kb_last_button = -1;
        scroll_up = false;
        scroll_down = false;
        gamepad_current_count = 0;

        //Creates the gamepads
        for( int i_gamepad = 0; i_gamepad < gp_max_devices; i_gamepad++)
        {
            game_pads[ i_gamepad ] = new gamepad_base();
        }

        for (int key=0; key<kb_button_count; key++)
        {
            kb_button_previous[key] = false;
            kb_button_down[key] = false;
            kb_button_pressed[key] = false;
            kb_button_released[key] = false;
            kb_button_locked[key] = false;
            kb_binding[key] = kb_binding_alt[key] = -1;
            kb_binding_name[key] ="";
        }

        for (int mb=0; mb<mouse_button_count; mb++)
        {
            mouse_previous_button[mb] = false;
            mouse_down_button[mb] = false;
            mouse_pressed_button[mb] = false;
            mouse_released_button[mb] = false;
        }

        mouse_position_x = mouse_previous_position_x = 0;
        mouse_position_x = mouse_previous_position_y = 0;

        mouse_scrolling_up = false;
        mouse_scrolling_down = false;
        mouse_scrolling_left = false;
        mouse_scrolling_right = false;


        kb_binding_name[kb_esc] = "ESC";
        kb_binding_name[kb_space] = "SPACE";
        kb_binding_name[kb_up] = "UP";
        kb_binding_name[kb_left] = "LEFT";
        kb_binding_name[kb_right] = "RIGHT";
        kb_binding_name[kb_down] = "DOWN";
        kb_binding_name[kb_ctrl] = "CTRL";
        kb_binding_name[kb_shift] = "SHIFT";
        kb_binding_name[kb_a] = "a";
        kb_binding_name[kb_b] = "b";
        kb_binding_name[kb_c] = "c";
        kb_binding_name[kb_d] = "d";
        kb_binding_name[kb_e] = "e";
        kb_binding_name[kb_f] = "f";
        kb_binding_name[kb_g] = "g";
        kb_binding_name[kb_h] = "h";
        kb_binding_name[kb_i] = "i";
        kb_binding_name[kb_j] = "j";
        kb_binding_name[kb_k] = "k";
        kb_binding_name[kb_l] = "l";
        kb_binding_name[kb_m] = "m";
        kb_binding_name[kb_n] = "n";
        kb_binding_name[kb_o] = "o";
        kb_binding_name[kb_p] = "p";
        kb_binding_name[kb_q] = "q";
        kb_binding_name[kb_r] = "r";
        kb_binding_name[kb_s] = "s";
        kb_binding_name[kb_t] = "t";
        kb_binding_name[kb_u] = "u";
        kb_binding_name[kb_v] = "v";
        kb_binding_name[kb_w] = "w";
        kb_binding_name[kb_x] = "x";
        kb_binding_name[kb_y] = "u";
        kb_binding_name[kb_z] = "z";
        kb_binding_name[kb_tab] = "tab";
        kb_binding_name[kb_f1] = "F1";
        kb_binding_name[kb_f2] = "F2";
        kb_binding_name[kb_f3] = "F3";
        kb_binding_name[kb_f4] = "F4";
        kb_binding_name[kb_f5] = "F5";
        kb_binding_name[kb_f6] = "F6";
        kb_binding_name[kb_f7] = "F7";
        kb_binding_name[kb_f8] = "F8";
        kb_binding_name[kb_f9] = "F9";
        kb_binding_name[kb_f10] = "F10";
        kb_binding_name[kb_f11] = "F11";
        kb_binding_name[kb_f12] = "F12";
        kb_binding_name[kb_plus] = "=";
        kb_binding_name[kb_comma] = "<";
        kb_binding_name[kb_minus] = "_";
        kb_binding_name[kb_period] = ">";
        mouse_position_x=0;
        mouse_position_y=0;

        mouse_button[0] = "lmb";
        mouse_button[1] = "lmb";
        mouse_button[2] = "rmb";
        mouse_button[3] = "wheel_up";
        mouse_button[4] = "wheel_down";
        mouse_button[5] = "mbx1";
        mouse_button[6] = "mbx2";

        input_received = true;
        kb_input_received = false;
        mouse_input_received = false;
        mouse_movement_received = false;
        mouse_wheel_received = false;
        window_input_received = false;
        key_bind_qwerty();

        load_input_settings();
        gamepad_detect_all( );
    }

    input_manager_base::~input_manager_base()
    {

    }


    bool input_manager_base::any_key_pressed()
    {
        for (int key=0; key<kb_button_count; key++)
        {
            if ( kb_button_pressed[key] && !kb_button_locked[key])
            {
                return true;
            }
        }
        return false;
    }


    bool input_manager_base::check_kb_down(int button_id)
    {
        if(button_id>=0 && button_id<kb_button_count )
        {
            return kb_button_down[button_id];
        }
        return false;
    }

    bool input_manager_base::check_kb_only_down(int button_id  )
    {
        if( button_id>=0 && button_id < kb_button_count )
        {
            if( kb_button_down[button_id] )
            {
                for( int i = 0; i < kb_button_count; i++ )
                {
                    if( i!= button_id )
                    {
                        if( kb_button_down[i] )
                        {
                            return false;
                        }
                    }
                }
                return true;
            }
        }
        return false;
    }

    bool input_manager_base::check_kb_only_released(int button_id  )
    {
        if( button_id>=0 && button_id < kb_button_count )
        {
            if( kb_button_released[button_id] )
            {
                for( int i = 0; i < kb_button_count; i++ )
                {
                    if( i!= button_id )
                    {
                        if( kb_button_down[i] == true )
                        {
                            return false;
                        }
                        if( kb_button_pressed[i] == true )
                        {
                            return false;
                        }
                        else if( kb_button_released[i] == true )
                        {
                            return false;
                        }
                    }
                }
                return true;
            }
        }
        return false;
    }

    bool input_manager_base::check_kb_pressed(int button_id)
    {
        if(button_id>=0 && button_id<kb_button_count )
        {
            return kb_button_pressed[button_id];
        }
        return false;
    }

    bool input_manager_base::check_kb_released(int button_id)
    {
        if(button_id>=0 && button_id<kb_button_count )
        {
            return kb_button_released[button_id];
        }
        return false;
    }

    bool input_manager_base::check_mouse_button_clicked(int button_id)
    {
        if(button_id>=0 && button_id<mouse_button_count )
        {
            if(mouse_clicked_button[button_id]>1)
            {
                return true;
            }
        }
        return false;
    }

    bool input_manager_base::check_mouse_down(int button_id)
    {
        if(button_id>=0 && button_id<mouse_button_count )
        {
            return mouse_down_button[button_id];
        }
        else
        {
            bool resultFound = false;
            for(int i=0; i < mouse_button_count; i++)
            {
                if(mouse_down_button[i] )
                {
                    resultFound = true;
                    return resultFound;
                }
            }
            return resultFound;
        }
        return false;
    }

    bool input_manager_base::check_mouse_pressed(int button_id)
    {
        if(button_id>=0 && button_id<mouse_button_count )
        {
            return mouse_pressed_button[button_id];
        }
        else
        {
            bool resultFound = false;
            for(int i=0; i < mouse_button_count; i++)
            {
                if(mouse_pressed_button[i] )
                {
                    resultFound = true;
                    return resultFound;
                }
            }
            return resultFound;
        }
        return false;
    }

    bool input_manager_base::check_mouse_released(int button_id)
    {
        if( button_id>=0 && button_id< mouse_button_count )
        {
            return mouse_released_button[button_id];
        }
        else
        {
            for(int i=0; i < mouse_button_count; i++)
            {
                if( mouse_released_button[i] ==true )
                {
                    return true;
                }
            }
        }
        return false;
    }

    //clipboard funcions
    bool input_manager_base::clipboard_empty()
    {
        //handled in child classes
        return true;
    }

    bool input_manager_base::clipboard_set( std::string new_clipboard_string)
    {
        //handled in child classes
        return false;
    }

    std::string input_manager_base::clipboard_string()
    {
        //handled in child classes
        return "";
    }

    int input_manager_base::gamepad_check_button (int gamepad_id, int button_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->check_button(button_id);
        }
        else if( gamepad_id == gp_anycontroller )
        {
            for( int iConPos = gp_max_devices -1; iConPos >=0; iConPos-- )
            {
                if( game_pads[iConPos]->check_button(button_id) )
                {
                    return true;
                }
            }
        }
        return false;
    }

    int input_manager_base::gamepad_check_button_down (int gamepad_id, int button_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->check_button(button_id);
        }
        else if( gamepad_id == gp_anycontroller )
        {
            for( int iConPos = gp_max_devices -1; iConPos >=0; iConPos-- )
            {
                if( game_pads[iConPos]->check_button(button_id) )
                {
                    return true;
                }
            }
        }
        return false;
    }

    int input_manager_base::gamepad_check_button_pressed (int gamepad_id, int button_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->check_button_pressed(button_id);
        }
        else if( gamepad_id == gp_anycontroller )
        {
            for( int iConPos = gp_max_devices -1; iConPos >=0; iConPos-- )
            {
                if( game_pads[iConPos]->check_button_pressed(button_id) )
                {
                    return true;
                }
            }
        }
        return false;
    }

    int input_manager_base::gamepad_check_button_released (int gamepad_id, int button_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->check_button_released(button_id);
        }
        else if( gamepad_id == gp_anycontroller )
        {
            for( int iConPos = gp_max_devices -1; iConPos >=0; iConPos-- )
            {
                if( game_pads[iConPos]->check_button_released(button_id) )
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool input_manager_base::gamepad_connected (int gamepad_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->is_connected();
        }
        return -1;
    }

    int input_manager_base::gamepad_count()
    {
        return gamepad_current_count;
    }

    bool input_manager_base::gamepad_detect_all()
    {
        if( debug_input )
        {
            error_log->report("Performing Controller Check...");
        }

        gamepad_current_count = 0;
        for( int i_gamepad = 0; i_gamepad < gp_max_devices; i_gamepad++ )
        {
            game_pads[ i_gamepad ]->set_connected( false );
            game_pads[ i_gamepad ]->set_name("");
            game_pads[ i_gamepad ]->pure_reset();
        }
        return false;
    }

    std::string input_manager_base::gamepad_detected_name(int gamepad_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->get_name();
        }
        return "";
    };

    bool input_manager_base::gamepad_disconnect( int gamepad_id )
    {
        if( debug_input )
        {
            error_log->report("Attempting to disconnect gamepad["+ stg_ex::int_to_string(gamepad_id )+"]..." );
        }
        return false;
    }

    int input_manager_base::gamepad_find_axes_map ( int gamepad_id, int axes_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->gamepad_find_axes_map( axes_id);
        }
        return -1;
    }

    int input_manager_base::gamepad_find_map (int gamepad_id, int button_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->gamepad_find_button_map( button_id);
        }
        return -1;
    }

    gamepad_base * input_manager_base::gamepad_get_object (int gamepad_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id];
        }
        return NULL;
    };

    int input_manager_base::gamepad_get_axes_count ( int gamepad_id )
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->gamepad_get_axes_count();
        }
        return -1;
    };

    float input_manager_base::gamepad_get_axis_value( int gamepad_id, int axis_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->get_axis_value(  axis_id );
        }
        return 0;
    }

    std::string input_manager_base::gamepad_name (int gamepad_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->get_name();
        }
        return "";
    }

    int input_manager_base::gamepad_find_button_map ( int gamepad_id, int button_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->gamepad_find_button_map( button_id);
        }
        return -1;
    }

    bool input_manager_base::gamepad_map_axes (int gamepad_id, int axes_id, int new_axes_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->gamepad_map_axes(  axes_id, new_axes_id);
        }
        else if( gamepad_id == gp_anycontroller )
        {
            bool successfulMapping = false;
            for( int iConPos = gp_max_devices -1; iConPos >=0; iConPos-- )
            {
                if( game_pads[iConPos]->gamepad_map_axes(  axes_id, new_axes_id) )
                {
                    successfulMapping = true;
                }
            }
            return successfulMapping;
        }
        return false;
    };

    bool input_manager_base::gamepad_map_button (int gamepad_id, int button_id, int newbutton_id)
    {
        if( gamepad_id >=0 && gamepad_id < gp_max_devices )
        {
            return game_pads[gamepad_id]->gamepad_map_button(  button_id, newbutton_id);
        }
        else if( gamepad_id == gp_anycontroller )
        {
            bool successfulMapping = false;
            for( int iConPos = gp_max_devices -1; iConPos >=0; iConPos-- )
            {
                if( game_pads[iConPos]->gamepad_map_button(  button_id, newbutton_id) )
                {
                    successfulMapping = true;
                }
            }
            return successfulMapping;
        }
        return false;
    };

    bool input_manager_base::gamepad_setup( int gamepad_id )
    {
        if( debug_input)
        {
            error_log->report("Attempting to setup gamepad["+ stg_ex::int_to_string(gamepad_id )+"]..." );
        }
        return false;
    }

    std::string input_manager_base::get_type()
    {
        return manager_type;
    }

    void input_manager_base::handle_input(bool dump_event, bool is_embedded )
    {
        handle_input_start( dump_event, is_embedded);
        handle_input_end( dump_event, is_embedded);
    }

    void input_manager_base::handle_input_end(bool dump_event , bool is_embedded )
    {
        if( mouse_position_x!= mouse_previous_position_x || mouse_position_y!= mouse_previous_position_y )
        {
            mouse_input_received = mouse_movement_received = true;
            input_received = true;
        }

        if( window_controller_main->is_resized() )
        {
            window_input_received = true;
            //screen_width = window_controller_main->get_window_width();
            //screen_height = window_controller_main->get_window_height();
            if(  window_controller_main->is_resized() )
            {
                time(&last_screen_resize);
            }
            //reset_all_input();
        }

        if( window_controller_main->window_closed)
        {
            exit_requested = true;
        }

        for (int key=0; key<kb_button_count; key++)
        {
            kb_button_pressed[key] = kb_button_down[key] && !kb_button_previous[key];
            kb_button_released[key] = !kb_button_down[key] && kb_button_previous[key];
        }

        for (int mb=0; mb<mouse_button_count; mb++)
        {
            mouse_pressed_button[mb] = mouse_down_button[mb] && !mouse_previous_button[mb];
            mouse_released_button[mb] = !mouse_down_button[mb] && mouse_previous_button[mb];
        }
    }

    void input_manager_base::handle_input_start(bool dump_event, bool is_embedded)
    {
        renderer_main->reset_input();
        input_received = false;
        kb_input_received = false;
        mouse_input_received = false;
        mouse_movement_received = false;
        mouse_wheel_received = false;
        window_input_received = false;

        mouse_scrolling_left = false;
        mouse_scrolling_right = false;
        mouse_scrolling_up = false;
        mouse_scrolling_down = false;

        if( window_controller_main !=NULL )
        {
            window_controller_main->reset_input();
        }

        int key = 0;
        int cbutton = 0;
        for (key=0; key<kb_button_count; key++)
        {
            kb_button_previous[key] = kb_button_down[key];
            kb_button_pressed[key] = false;
            kb_button_released[key] = false;
            if( key!=kb_ctrl && key!=kb_alt)
            {
                //down[key] = false;
            }
        }

        for (int mb=0; mb<mouse_button_count; mb++)
        {
            mouse_previous_button[mb] = mouse_down_button[mb];
            mouse_pressed_button[mb] = false;
            mouse_released_button[mb] = false;
            mouse_clicked_button[mb] = 0;
        }
        renderer_main->reset_input();
        kb_backspace_pressed = false;
        exit_requested = false;
        //kb_shift_pressed = false;
        files_dropped_list.clear();
    }

    void input_manager_base::key_bind_qwerty()
    {
        kb_binding_name[kb_a] = "A";
        kb_binding_name[kb_b] = "B";
        kb_binding_name[kb_c] = "C";
        kb_binding_name[kb_d] = "D";
        kb_binding_name[kb_e] = "E";
        kb_binding_name[kb_f] = "F";
        kb_binding_name[kb_g] = "G";
        kb_binding_name[kb_h] = "H";
        kb_binding_name[kb_i] = "I";
        kb_binding_name[kb_j] = "J";
        kb_binding_name[kb_k] = "K";
        kb_binding_name[kb_l] = "L";
        kb_binding_name[kb_m] = "M";
        kb_binding_name[kb_n] = "N";
        kb_binding_name[kb_o] = "O";
        kb_binding_name[kb_p] = "P";
        kb_binding_name[kb_q] = "Q";
        kb_binding_name[kb_r] = "R";
        kb_binding_name[kb_s] = "S";
        kb_binding_name[kb_t] = "T";
        kb_binding_name[kb_u] = "U";
        kb_binding_name[kb_v] = "V";
        kb_binding_name[kb_w] = "W";
        kb_binding_name[kb_x] = "X";
        kb_binding_name[kb_y] = "Y";
        kb_binding_name[kb_z] = "Z";

        kb_binding_name[kb_comma] = "<";
        kb_binding_name[kb_period] = ">";

        kb_binding_name[kb_f1] = "F1";
        kb_binding_name[kb_f2] = "F2";
        kb_binding_name[kb_f3] = "F3";
        kb_binding_name[kb_f4] = "F4";
        kb_binding_name[kb_f5] = "F5";
        kb_binding_name[kb_f6] = "F6";
        kb_binding_name[kb_f7] = "F7";
        kb_binding_name[kb_f8] = "F8";
        kb_binding_name[kb_f9] = "F9";
        kb_binding_name[kb_f10] = "F10";
        kb_binding_name[kb_f11] = "F11";
        kb_binding_name[kb_f12] = "F12";
    }

    bool input_manager_base::load_input_settings(std::string file_path )
    {
        std::string input_settingsFile = app_directory_name + "gpe_input_settings.txt";
        if( main_file_url_manager->file_exists(file_path) )
        {
            input_settingsFile = input_settingsFile;
        }
        if( main_file_url_manager->file_exists(input_settingsFile) )
        {
            std::ifstream gameResourceFileIn( input_settingsFile.c_str() );
            if (gameResourceFileIn.is_open() )
            {
                int equalPos = 0;
                std::string firstChar="";
                std::string key_string="";
                std::string valstring="";
                std::string subValstring="";
                std::string currLine="";
                std::string currLineToBeProcessed;

                int i = 0;
                float foundFileVersion = 0;
                while ( gameResourceFileIn.good() )
                {
                    getline (gameResourceFileIn,currLine); //gets the next line of the file
                    currLineToBeProcessed = stg_ex::trim_left_inplace(currLine);
                    currLineToBeProcessed = stg_ex::trim_right_inplace(currLineToBeProcessed);

                    if( foundFileVersion <=0)
                    {
                        //Empty Line skipping is only allowed at the top of the file
                        if(!currLineToBeProcessed.empty() )
                        {
                            //Comment skipping is only allowed at the top of the file
                            if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                            {
                                //searches for an equal character and parses through the variable
                                equalPos=currLineToBeProcessed.find_first_of("=");
                                if(equalPos!=(int)std::string::npos)
                                {
                                    //if the equalPos is present, then parse on through and carryon
                                    key_string = currLineToBeProcessed.substr(0,equalPos);
                                    valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                    if( key_string=="Version")
                                    {
                                        foundFileVersion = stg_ex::string_to_float(valstring);
                                    }
                                }
                            }
                        }
                    }
                    else if( foundFileVersion <= 2)
                    {
                        //Begin processing the file.
                        if(!currLineToBeProcessed.empty() )
                        {
                            equalPos=currLineToBeProcessed.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                key_string = currLineToBeProcessed.substr(0,equalPos);
                                valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                                if( key_string=="deput_input")
                                {
                                    debug_input = stg_ex::string_to_bool( valstring);
                                }
                            }
                        }
                    }
                }
                if( foundFileVersion  > 0 )
                {
                    return true;
                }
            }
        }
        return false;
    }


    bool input_manager_base::has_new_input()
    {
        if( window_controller_main->is_resized() )
        {
            return true;
        }
        return input_received;
    }

    void input_manager_base::reset_all_input()
    {
        scroll_up = false;
        scroll_down = false;
        mouse_scrolling_down = false;
        mouse_scrolling_up = false;
        mouse_scrolling_left = false;
        mouse_scrolling_right = false;
        /*mouse_position_x = -1;
        mouse_position_y = -1;*/
        for (int key=0; key<kb_button_count; key++)
        {
            kb_button_pressed[key] = false;
            kb_button_down[key] = false;
            kb_button_previous[key] = false;
            kb_button_released[key] = false;
        }
        for (int mb=0; mb<mouse_button_count; mb++)
        {
            mouse_pressed_button[mb] = false;
            mouse_down_button[mb] = false;
            mouse_previous_button[mb] = false;
            mouse_released_button[mb] = false;
        }
        inputted_keys = "";
        kb_backspace_pressed= false;
        kb_shift_pressed = false;
        input_received = true;
    }


    void input_manager_base::reset_scroll_values()
    {
        scroll_up = false;
        scroll_down = false;
    }

    void input_manager_base::reset_temp_input()
    {
        scroll_up = false;
        scroll_down = false;
        mouse_scrolling_down = false;
        mouse_scrolling_up = false;
        mouse_scrolling_left = false;
        mouse_scrolling_right = false;
        /*mouse_position_x = -1;
        mouse_position_y = -1;*/
        for (int key=0; key<kb_button_count; key++)
        {
            kb_button_pressed[key] = false;
            kb_button_released[key] = false;
        }
        for (int mb=0; mb<mouse_button_count; mb++)
        {
            mouse_pressed_button[mb] = false;
            mouse_released_button[mb] = false;
        }
        inputted_keys = "";
        kb_backspace_pressed= false;
        kb_shift_pressed = false;

        gamepad_base * tempGC = NULL;

        for( int i = 0; i < gp_max_devices; i++ )
        {
            tempGC = game_pads[i];
            tempGC->reset_temp_input();
        }
    }

    int input_manager_base::move_cursor(int cursor, int max_value, int row_max)
    {
        if ( kb_button_pressed[kb_up] && !kb_button_locked[kb_up])
        {
            if(cursor>=row_max)
                cursor-=row_max;
        }
        if ( kb_button_pressed[kb_down] && !kb_button_locked[kb_down])
        {
            if(cursor<max_value-row_max )
                cursor+=row_max;
        }
        if ( kb_button_pressed[kb_right] && !kb_button_locked[kb_right])
        {
            if(cursor<max_value)
                cursor+=1;
        }
        if ( kb_button_pressed[kb_left] && !kb_button_locked[kb_left])
        {
            if(cursor>0)
                cursor-=1;
        }
        if(cursor<0)
        {
            cursor=0;
        }
        if(cursor>=max_value)
        {
            cursor=max_value-1;
        }
        return cursor;
    }
}
