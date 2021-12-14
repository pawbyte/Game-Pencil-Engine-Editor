/*
gpe_input_sdl.cpp
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

#include "gpe_input_sdl.h"

namespace gpe
{
    gamepad_sdl::gamepad_sdl()
    {
        joy_sdl_id = -1;
        assigned_sdl_controller = NULL;
        system_handler = "sdl";
    }

    gamepad_sdl::~gamepad_sdl()
    {

    }


    void gamepad_sdl::handle_input()
    {
        gamepad_base::handle_input();
    }

    void gamepad_sdl::pure_reset()
    {
        gamepad_base::pure_reset();
        if( assigned_sdl_controller!=NULL )
        {
            //SDL_gamepadClose( assigned_sdl_controller );
            //assigned_sdl_controller = NULL;
            if (SDL_JoystickGetAttached(assigned_sdl_controller) )
            {
                SDL_JoystickClose( assigned_sdl_controller);
            }
            assigned_sdl_controller = NULL;
        }
    }

    void gamepad_sdl::reset_gamepad()
    {
        gamepad_base::reset_gamepad();
    }

    void gamepad_sdl::reset_temp_input()
    {
        gamepad_base::reset_temp_input();
    }

    void gamepad_sdl::setup_default_mapping( bool map_buttons, bool mapAxis)
    {
        gamepad_base::setup_default_mapping( map_buttons, mapAxis );
        if( map_buttons)
        {
            //Maps the default controller buttons
             button_map[gp_but0] = SDL_CONTROLLER_BUTTON_A;
             button_map[gp_but1] = SDL_CONTROLLER_BUTTON_B;
             button_map[gp_but2] = SDL_CONTROLLER_BUTTON_X;
             button_map[gp_but3] = SDL_CONTROLLER_BUTTON_Y;

             button_map[gp_left] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
             button_map[gp_right] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
             button_map[gp_up] = SDL_CONTROLLER_BUTTON_DPAD_UP;
             button_map[gp_down] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;

             button_map[gp_l1] = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
             button_map[gp_r1] = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;

             button_map[gp_l3] = SDL_CONTROLLER_BUTTON_LEFTSTICK;
             button_map[gp_r3] = SDL_CONTROLLER_BUTTON_RIGHTSTICK;

             button_map[gp_home] = SDL_CONTROLLER_BUTTON_GUIDE;
             button_map[gp_select] = SDL_CONTROLLER_BUTTON_BACK;
             button_map[gp_start] = SDL_CONTROLLER_BUTTON_START;
        }

        if( mapAxis )
        {
            axes_map[ gp_joystick_left_x ] = SDL_CONTROLLER_AXIS_LEFTX;
            axes_map[ gp_joystick_left_y ] = SDL_CONTROLLER_AXIS_LEFTY;

            axes_map[ gp_joystick_right_x ] = SDL_CONTROLLER_AXIS_RIGHTX;
            axes_map[ gp_joystick_right_y ] = SDL_CONTROLLER_AXIS_RIGHTY;

            axes_map[ gp_joystick_trigger_left ] = SDL_CONTROLLER_AXIS_TRIGGERLEFT;
            axes_map[ gp_joystick_trigger_right ] = SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
        }
    }

    input_manager_sdl::input_manager_sdl()
    {
        SDL_GameControllerAddMapping("gamepaddb.txt");
        SDL_StartTextInput();
        manager_type = "sdl";
        for( int i_gamepad = 0; i_gamepad < gp_max_devices; i_gamepad++ )
        {
            game_pads_sdl[i_gamepad] = new gamepad_sdl();
        }
        key_bind_qwerty();
    }

    input_manager_sdl::~input_manager_sdl()
    {

    }

    bool input_manager_sdl::clipboard_empty()
    {
        return !SDL_HasClipboardText();
    }

    bool input_manager_sdl::clipboard_set( std::string new_clipboard_string)
    {
        SDL_SetClipboardText( new_clipboard_string.c_str() );
    }

    std::string input_manager_sdl::clipboard_string()
    {
        return SDL_GetClipboardText();
    }

    void input_manager_sdl::convert_event_input( )
    {
        if( event_container == NULL )
        {
            error_log->report("Unable to convert event container, it's null!");
            return;
        }

        if( sdl_input_event.type == SDL_WINDOWEVENT )
        {
            event_container->reset_event();

            event_container->event_time = sdl_input_event.window.timestamp;
            event_container->window_id = sdl_input_event.window.windowID;

            switch( sdl_input_event.window.event )
            {
                //Get new dimensions and repaint on window size change
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                case SDL_WINDOWEVENT_RESIZED:
                    event_container->event_type = input_event_type::window_resized_event;
                    event_container->window_resize_w = sdl_input_event.window.data1;
                    event_container->window_resize_h = sdl_input_event.window.data2;
                break;

                case SDL_WINDOWEVENT_MINIMIZED:
                    event_container->event_type = input_event_type::window_minimized_event;
                break;

                case SDL_WINDOWEVENT_MAXIMIZED:
                    event_container->event_type = input_event_type::window_maximized_event;
                break;

                case SDL_WINDOWEVENT_ENTER:
                    event_container->event_type = input_event_type::window_entered_event;
                break;

                case SDL_WINDOWEVENT_LEAVE:
                    event_container->event_type = input_event_type::window_leave_event;
                break;

                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    event_container->event_type = input_event_type::window_focus_gained_event;
                break;

                case SDL_WINDOWEVENT_FOCUS_LOST:
                    event_container->event_type = input_event_type::window_focus_lost_event;
                break;

                case SDL_WINDOWEVENT_EXPOSED:
                    event_container->event_type = input_event_type::window_exposed_event;
                break;

                case SDL_WINDOWEVENT_HIDDEN:
                    event_container->event_type = input_event_type::window_hidden_event;
                break;

                case SDL_WINDOWEVENT_CLOSE:
                    event_container->event_type = input_event_type::window_closed_event;
                break;

                case SDL_WINDOWEVENT_SHOWN:
                case SDL_WINDOWEVENT_RESTORED:
                    event_container->event_type = input_event_type::window_shown_event;
                break;
            }
        }
    }


    bool input_manager_sdl::gamepad_detect_all()
    {
        input_manager_base::gamepad_detect_all();

        int currentControllerId = 0;
        gamepad_current_count = 0;

        int foundJoyStickCount = SDL_NumJoysticks();

        if( debug_input )
        {
            if( debug_input )
            {
                error_log->report("Attempting to process ["+ stg_ex::int_to_string( foundJoyStickCount )+"] SDL_JoySticks...");
            }
        }

        for ( int i_gamepad = 0; i_gamepad < foundJoyStickCount; i_gamepad++ )
        {
            gamepad_setup( i_gamepad );
        }

        if( gamepad_current_count > 0)
        {
            return true;
        }
        if( debug_input )
        {
            error_log->report("    No Game Controllers found...");
        }
        return false;
    }

    bool input_manager_sdl::gamepad_disconnect( int gamepad_id )
    {
        input_manager_base::gamepad_disconnect( gamepad_id );
        SDL_ClearError();
        gamepad_sdl * temp_sdl_gamepad = NULL;
        if ( SDL_IsGameController( gamepad_id ) )
        {
            for( int i = gp_max_devices - 1; i >=0; i--)
            {
                if( game_pads[i]->get_system_handler() == "sdl" )
                {
                    temp_sdl_gamepad = (gamepad_sdl*) game_pads[i];
                    if( temp_sdl_gamepad!=NULL && temp_sdl_gamepad->joy_sdl_id == gamepad_id )
                    {
                        temp_sdl_gamepad->pure_reset();
                        temp_sdl_gamepad->set_connected( false );
                        if( gamepad_current_count > 0)
                        {
                            gamepad_current_count--;
                        }
                        temp_sdl_gamepad->joy_sdl_id  = -1;
                    }
                }
            }
            if( debug_input )
            {
                error_log->report(SDL_GetError() );
            }
            return true;
        }
        else if( debug_input )
        {
            error_log->report(SDL_GetError() );
        }
        if( debug_input)
        {
            error_log->report("gamepad["+ stg_ex::int_to_string(gamepad_id )+"] disconnected..." );
        }
    }

    bool input_manager_sdl::gamepad_setup( int gamepad_id )
    {
        SDL_ClearError();
        int jStickCount = SDL_NumJoysticks();
        if( jStickCount < 0 )
        {
            if( debug_input )
            {
                error_log->report("Joystick count ["+ stg_ex::int_to_string(jStickCount)+"] is out of range!" );
                error_log->report( SDL_GetError( ) );
            }
            //Attempt to fix our problem...
            if( SDL_WasInit( SDL_INIT_JOYSTICK ) )
            {
                SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
            }
            SDL_InitSubSystem( SDL_INIT_JOYSTICK );
            return false;
        }


        if( gamepad_id >= jStickCount )
        {
            if( debug_input )
            {
                error_log->report("Joystick ["+ stg_ex::int_to_string(gamepad_id)+"] is out of range!" );
            }
            return false;
        }

        if ( SDL_IsGameController(gamepad_id) )
        {
            SDL_GameController * foundSDLController = SDL_GameControllerOpen( gamepad_id );
            if( foundSDLController )
            {
                if( gamepad_current_count >=0 && gamepad_current_count < gp_max_devices-1 )
                {
                    if( debug_input )
                    {
                        error_log->report("Finding gamepad...");
                    }
                    gamepad_base * tgamepad = game_pads_sdl[ gamepad_current_count ];
                    if( tgamepad->get_system_handler() == "sdl" )
                    {
                        gamepad_sdl * temp_sdl_gamepad = (gamepad_sdl*) tgamepad;
                        if( debug_input )
                        {
                            error_log->report("Finding Joystick ID...");
                        }
                        SDL_Joystick * jStick = SDL_GameControllerGetJoystick( foundSDLController );

                        if( jStick !=NULL )
                        {
                            temp_sdl_gamepad->assigned_sdl_controller = jStick;
                            temp_sdl_gamepad->set_connected( true );
                        }
                        else
                        {
                            temp_sdl_gamepad->assigned_sdl_controller = NULL;
                            temp_sdl_gamepad->set_connected( false );
                            if( debug_input )
                            {
                                error_log->report( SDL_GetError() );
                            }
                            return false;
                        }

                        temp_sdl_gamepad->joy_sdl_id = gamepad_current_count;

                        if( debug_input )
                        {
                            error_log->report("Setting name...");
                        }
                        tgamepad->set_name( SDL_JoystickNameForIndex( gamepad_id ) );

                        if( debug_input )
                        {
                            error_log->report("Name set successfully...");
                        }
                        if( !gamepad_requires_input )
                        {
                            tgamepad->set_connected( true );
                        }
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
                else if( debug_input)
                {
                    error_log->report("Failed to start gamepad["+ stg_ex::int_to_string(gamepad_id )+"]..." );
                    error_log->report(SDL_GetError() );
                    return false;
                }
            }
            else if( debug_input )
            {
                error_log->report("Unable to open gamepad ("+ stg_ex::int_to_string( gamepad_id )+"):    ["+SDL_GetError()+"].");
                return false;
            }
        }
        else if( debug_input )
        {
            error_log->report("Failed...");
            error_log->report(SDL_GetError( ));
            return false;
        }
        return true;
    }

    void input_manager_sdl::handle_input(bool dump_event, bool is_embedded )
    {
        SDL_GameController * foundSDLController = NULL;
        SDL_JoystickUpdate();

        handle_input_start( dump_event, is_embedded );


        std::vector < int > addControllerList;
        std::vector < int > removeControllerList;

        /* Check for events */
        int cbutton = -1;
        int frame_wait = 2; /* anything less than 16 ms is good */
        unsigned tries = 3; /* use UINT_MAX for no effective limit */
        int fControllerInputId = 0;
        gamepad_sdl *  tempController = NULL;
        float  tempAxisValue  = 0.0;

        while (SDL_PollEvent( &sdl_input_event) )
        {
            switch(sdl_input_event.type)
            {
                case SDL_WINDOWEVENT:
                    input_received = true;
                    kb_input_received = false;
                    //Window related events
                    if( rph!=NULL )
                    {
                        convert_event_input();
                        rph->process_event( event_container );
                        //reset_all_input();
                    }
                break;

                case SDL_QUIT:
                    input_received = true;
                    kb_input_received = true;
                    if( is_embedded==false)
                    {
                        //User requests quit
                        exit_requested = true;
                    }
                break;


                case (SDL_DROPFILE):
                    // In case if dropped file
                    file_dropped_path = sdl_input_event.drop.file;
                    // Shows directory of dropped file
                    /*
                    SDL_ShowSimpleMessageBox(
                        SDL_MESSAGEBOX_ERROR,
                        "File dropped on window",
                        file_dropped_path,
                        renderer_main->get_window()
                    );
                    */
                    files_dropped_list.push_back( file_dropped_path );
                    SDL_free( file_dropped_path );    // Free file_dropped_path memory
                    input_received = true;
                    kb_input_received = true;
                break;

                //Game Controller Input
                case SDL_CONTROLLERDEVICEADDED:
                    //For reasons, we will just add to them a list and manage em at the end
                    addControllerList.push_back( sdl_input_event.cdevice.which );
                break;

                case SDL_CONTROLLERDEVICEREMOVED:
                    //For reasons, we will just add to them a list and manage em at the end
                    removeControllerList.push_back( sdl_input_event.cdevice.which );
                break;

                case SDL_CONTROLLERBUTTONDOWN:
                    fControllerInputId = sdl_input_event.cdevice.which;
                    if( debug_input )
                    {
                        error_log->report("_button[" + stg_ex::int_to_string( sdl_input_event.cbutton.button) + "] on controller [" + stg_ex::int_to_string(fControllerInputId  )+"]  fired...");
                    }
                    if( fControllerInputId >= 0 && fControllerInputId < gp_max_devices )
                    {
                        tempController = game_pads_sdl[ fControllerInputId ];
                        if( gamepad_requires_input || tempController->is_connected() == false )
                        {
                            tempController->set_connected( true );
                        }
                        for (  cbutton = 0;  cbutton < gp_button_count; cbutton++ )
                        {
                            if ( sdl_input_event.cbutton.button == tempController->button_map[cbutton] )
                            {
                                tempController->button_button[ tempController->button_map[cbutton] ] = true;
                                tempController->button_pressed[ tempController->button_map[cbutton] ] = true;
                            }
                        }
                    }
                break;

                case SDL_CONTROLLERBUTTONUP:
                    fControllerInputId = sdl_input_event.cdevice.which;
                    if( debug_input )
                    {
                        error_log->report("_button[" + stg_ex::int_to_string( sdl_input_event.cbutton.button) + "] on controller [" + stg_ex::int_to_string( fControllerInputId )+"] released...");
                    }
                    if( fControllerInputId >= 0 && fControllerInputId < gp_max_devices )
                    {
                        tempController = game_pads_sdl[ fControllerInputId ];
                        for (  cbutton = 0;  cbutton < gp_button_count; cbutton++)
                        {
                            if ( sdl_input_event.cbutton.button == tempController->button_map[cbutton] )
                            {
                                tempController->button_button[ tempController->button_map[cbutton] ] = false;
                                tempController->button_pressed[ tempController->button_map[cbutton] ] = false;
                            }
                        }
                    }
                break;

                case SDL_CONTROLLERAXISMOTION:
                    fControllerInputId = sdl_input_event.cdevice.which;
                    if( fControllerInputId >= 0 && fControllerInputId < gp_max_devices )
                    {
                        tempController = game_pads_sdl[ fControllerInputId ];
                        if( gamepad_requires_input || tempController->is_connected() == false )
                        {
                            tempController->set_connected( true );
                        }
                        //if( sdl_input_event.caxis.axis >= 0 && sdl_input_event.caxis.axis < gp_axes_count )
                        {
                            tempAxisValue = sdl_input_event.caxis.value / gp_deadzone;
                            //Less than the dead zone
                            if( tempAxisValue < -1.f )
                            {
                                tempAxisValue = -1.f;
                            }
                            //Right of dead zone
                            else if( tempAxisValue > 1.f )
                            {
                                tempAxisValue =  1.f;
                            }
                            tempController->axes_value[sdl_input_event.caxis.axis] = tempAxisValue;
                        }
                    }
                break;

                // grab symbol keys
                case SDL_KEYDOWN:
                    input_received = true;
                    kb_input_received = true;
                    //handle_modifers( sdl_input_event.key->keysym.mod );
                    if( sdl_input_event.key.keysym.sym == SDLK_BACKSPACE)
                    {
                        kb_backspace_pressed = true;
                        kb_button_down[kb_backspace] = true;
                        kb_button_pressed[kb_backspace] = true;
                    }
                    else if( sdl_input_event.key.keysym.sym == SDLK_LSHIFT || sdl_input_event.key.keysym.sym == SDLK_RSHIFT)
                    {
                        kb_shift_pressed = true;
                        kb_button_down[kb_shift] = true;
                        kb_button_pressed[kb_shift] = true;
                    }
                    else if( sdl_input_event.key.keysym.scancode==SDL_SCANCODE_LCTRL || sdl_input_event.key.keysym.scancode==SDL_SCANCODE_RCTRL)
                    {
                        kb_button_down[kb_ctrl] = true;
                        kb_button_pressed[kb_ctrl] = true;
                    }
                    else if( sdl_input_event.key.keysym.scancode==SDL_SCANCODE_LALT || sdl_input_event.key.keysym.scancode==SDL_SCANCODE_RALT)
                    {
                        kb_button_down[kb_alt] = true;
                        kb_button_pressed[kb_alt] = true;
                    }
                    else
                    {
                        for ( int key=0; key<kb_button_count; key++)
                        {
                            if (sdl_input_event.key.keysym.sym == kb_binding[key] || sdl_input_event.key.keysym.sym == kb_binding_alt[key])
                            {
                                kb_button_down[key] = true;
                                kb_button_pressed[key] = true;
                            }
                        }
                    }
                break;

                case SDL_KEYUP:
                    input_received = true;
                    kb_input_received = true;
                    if( sdl_input_event.key.keysym.scancode==SDL_SCANCODE_LCTRL || sdl_input_event.key.keysym.scancode==SDL_SCANCODE_RCTRL)
                    {
                        kb_button_down[kb_ctrl] = false;
                        kb_button_pressed[kb_ctrl] = false;
                        kb_button_locked[kb_ctrl] = false;
                    }
                    else if( sdl_input_event.key.keysym.scancode==SDL_SCANCODE_LALT || sdl_input_event.key.keysym.scancode==SDL_SCANCODE_RALT)
                    {
                        kb_button_down[kb_alt] = false;
                        kb_button_pressed[kb_alt] = false;
                        kb_button_locked[kb_alt] = true;
                    }
                    else if( sdl_input_event.key.keysym.sym == SDLK_LSHIFT || sdl_input_event.key.keysym.sym == SDLK_RSHIFT)
                    {
                        kb_shift_pressed = false;
                        kb_button_down[kb_shift] = false;
                        kb_button_pressed[kb_shift] = false;
                        kb_button_locked[kb_shift] = true;
                    }
                    else if( sdl_input_event.key.keysym.sym == SDLK_BACKSPACE)
                    {
                        kb_backspace_pressed = false;
                        kb_button_down[kb_backspace] = false;
                        kb_button_pressed[kb_backspace] = false;
                        kb_button_locked[kb_shift] = true;
                    }
                    else
                    {
                        for (int key=0; key<kb_button_count; key++)
                        {
                            if (sdl_input_event.key.keysym.sym == kb_binding[key] || sdl_input_event.key.keysym.sym == kb_binding_alt[key])
                            {
                                kb_button_down[key] = false;
                                kb_button_pressed[key] = false;
                                //released[key] = true;
                                kb_button_locked[key] = false;
                            }
                        }
                    }
                    kb_last_button = sdl_input_event.key.keysym.sym;
                break;

                case SDL_TEXTINPUT:
                    //Not copy or pasting
                    //if( !( ( sdl_input_event.text.text[ 0 ] == 'c' || sdl_input_event.text.text[ 0 ] == 'C' ) && ( sdl_input_event.text.text[ 0 ] == 'v' || sdl_input_event.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
                {
                    //Append character
                    inputted_keys= sdl_input_event.text.text;
                    input_received = true;
                    kb_input_received = true;
                }
                break;

                case SDL_MOUSEBUTTONDOWN:
                    input_received = true;
                    kb_input_received = true;
                    switch(sdl_input_event.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            mouse_down_button[0] = 1;
                            mouse_clicked_button[0] = sdl_input_event.button.clicks;
                            mouse_released_button[0] = false;
                        break;

                        case SDL_BUTTON_RIGHT:
                            mouse_down_button[1] = 1;
                            mouse_clicked_button[1] = sdl_input_event.button.clicks;
                            mouse_released_button[1] = false;
                        break;

                        case SDL_BUTTON_MIDDLE:
                            mouse_down_button[2] = 1;
                            mouse_clicked_button[2] = sdl_input_event.button.clicks;
                            mouse_released_button[2] = false;
                        break;

                        default:

                        break;
                    }
                break;

                case SDL_MOUSEBUTTONUP:
                    input_received = true;
                    kb_input_received = false;
                    switch(sdl_input_event.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            mouse_down_button[0] = false;
                            mouse_pressed_button[0] = false;
                            //mouse_released_button[0] = true;
                        break;
                        case SDL_BUTTON_RIGHT:
                            mouse_down_button[1] = false;
                            mouse_pressed_button[1] = false;
                            //mouse_released_button[1] = true;
                        break;
                        case SDL_BUTTON_MIDDLE:
                            mouse_down_button[2] = false;
                            mouse_pressed_button[2] = false;
                            //mouse_released_button[2] = true;
                        break;
                        case SDL_BUTTON_X1:
                            mouse_down_button[3] = false;
                            mouse_pressed_button[3] = false;
                            //mouse_released_button[3] = true;
                        break;
                        case SDL_BUTTON_X2:
                            mouse_down_button[4] = false;
                            mouse_pressed_button[4] = false;
                            //mouse_released_button[4] = true;
                        break;
                        default:

                        break;
                    }
                break;

                case SDL_MOUSEWHEEL:
                    input_received = true;
                    kb_input_received = true;
                    mouse_wheel_received = true;
                    if( mouse_scroll_inverted )
                    {
                        if( sdl_input_event.wheel.x > 0)
                        {
                            mouse_scrolling_left = true;
                        }
                        else if( sdl_input_event.wheel.x < 0)
                        {
                            mouse_scrolling_right = true;
                        }

                        if( sdl_input_event.wheel.y > 0)
                        {
                            mouse_scrolling_down = true;
                        }
                        else if( sdl_input_event.wheel.y < 0)
                        {
                            mouse_scrolling_up = true;
                        }
                    }
                    else
                    {
                        if( sdl_input_event.wheel.x > 0)
                        {
                            mouse_scrolling_right = true;
                        }
                        else if( sdl_input_event.wheel.x < 0)
                        {
                            mouse_scrolling_left = true;
                        }

                        if( sdl_input_event.wheel.y > 0)
                        {
                            mouse_scrolling_up = true;
                        }
                        else if( sdl_input_event.wheel.y < 0)
                        {
                            mouse_scrolling_down = true;
                        }
                    }
                break;

                default:
                break;
                };
        }

        //remove disconnected game controllers
        if( (int)removeControllerList.size() > 0 )
        {
            for( int i = (int)removeControllerList.size() - 1; i >= 0; i-- )
            {
                gamepad_disconnect( removeControllerList[i] );
            }
            if( gamepad_count()==0 && SDL_WasInit( SDL_INIT_GAMECONTROLLER ) )
            {
                SDL_QuitSubSystem( SDL_INIT_GAMECONTROLLER );
                SDL_InitSubSystem( SDL_INIT_GAMECONTROLLER );
            }
        }

        //Adds new connected controllers
        if( (int)addControllerList.size() > 0 )
        {
            if( gamepad_count()== 0 )
            {
                if( SDL_WasInit( SDL_INIT_JOYSTICK ) )
                {
                    SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
                }
                SDL_InitSubSystem( SDL_INIT_JOYSTICK );
            }

            for( int i = 0; i < (int)addControllerList.size(); i++ )
            {
                //gamepad_setup( addControllerList[i], true);
            }
            gamepad_detect_all();
        }

        SDL_GetMouseState(&mouse_position_x, &mouse_position_y);

        handle_input_end( dump_event, is_embedded );
    }

    void input_manager_sdl::key_bind_qwerty()
    {
        kb_binding[kb_esc] = SDLK_ESCAPE;
        kb_binding[kb_enter] = SDLK_RETURN;
        kb_binding[kb_up] = SDLK_UP;
        kb_binding[kb_down] = SDLK_DOWN;
        kb_binding[kb_left] = SDLK_LEFT;
        kb_binding[kb_right] = SDLK_RIGHT;
        kb_binding[kb_space] = kb_binding_alt[kb_space] = SDLK_SPACE;
        kb_binding[kb_page_down] = kb_binding_alt[kb_page_down] = SDLK_PAGEDOWN;
        kb_binding[kb_page_up] = kb_binding_alt[kb_page_up] = SDLK_PAGEUP;
        kb_binding[kb_a] = kb_binding_alt[kb_a] = SDLK_a;
        kb_binding[kb_b] = kb_binding_alt[kb_b] = SDLK_b;
        kb_binding[kb_c] = kb_binding_alt[kb_c] = SDLK_c;
        kb_binding[kb_d] = kb_binding_alt[kb_d] = SDLK_d;
        kb_binding[kb_e] = kb_binding_alt[kb_e] = SDLK_e;
        kb_binding[kb_f] = kb_binding_alt[kb_f] = SDLK_f;
        kb_binding[kb_g] = kb_binding_alt[kb_g] = SDLK_g;
        kb_binding[kb_h] = kb_binding_alt[kb_h] = SDLK_h;
        kb_binding[kb_i] = kb_binding_alt[kb_i] = SDLK_i;
        kb_binding[kb_j] = kb_binding_alt[kb_j] = SDLK_j;
        kb_binding[kb_k] = kb_binding_alt[kb_k] = SDLK_k;
        kb_binding[kb_l] = kb_binding_alt[kb_l] = SDLK_l;
        kb_binding[kb_m] = kb_binding_alt[kb_m] = SDLK_m;
        kb_binding[kb_n] = kb_binding_alt[kb_n] = SDLK_n;
        kb_binding[kb_o] = kb_binding_alt[kb_o] = SDLK_o;
        kb_binding[kb_p] = kb_binding_alt[kb_p] = SDLK_p;
        kb_binding[kb_q] = kb_binding_alt[kb_q] = SDLK_q;
        kb_binding[kb_r] = kb_binding_alt[kb_r] = SDLK_r;
        kb_binding[kb_s] = kb_binding_alt[kb_s] = SDLK_s;
        kb_binding[kb_t] = kb_binding_alt[kb_t] = SDLK_t;
        kb_binding[kb_u] = kb_binding_alt[kb_u] = SDLK_u;
        kb_binding[kb_v] = kb_binding_alt[kb_v] = SDLK_v;
        kb_binding[kb_w] = kb_binding_alt[kb_w] = SDLK_w;
        kb_binding[kb_x] = kb_binding_alt[kb_x] = SDLK_x;
        kb_binding[kb_y] = kb_binding_alt[kb_y] = SDLK_y;
        kb_binding[kb_z] = kb_binding_alt[kb_z] = SDLK_z;
        kb_binding[kb_minus] = SDLK_MINUS;
        kb_binding_alt[kb_minus] = SDLK_UNDERSCORE;
        kb_binding[kb_plus] = SDLK_EQUALS;
        kb_binding_alt[kb_plus] = SDLK_BACKQUOTE;

        kb_binding_alt[kb_esc] = SDLK_ESCAPE;
        kb_binding_alt[kb_enter] = SDLK_RETURN2;

        kb_binding_alt[kb_up] = SDLK_UP;
        kb_binding_alt[kb_down] = SDLK_DOWN;
        kb_binding_alt[kb_left] = SDLK_LEFT;
        kb_binding_alt[kb_right] = SDLK_RIGHT;

        kb_binding[kb_1] = kb_binding_alt[kb_1] = SDLK_1;
        kb_binding[kb_2] = kb_binding_alt[kb_2] = SDLK_2;
        kb_binding[kb_3] = kb_binding_alt[kb_3] = SDLK_3;
        kb_binding[kb_4] = kb_binding_alt[kb_4] = SDLK_4;
        kb_binding[kb_5] = kb_binding_alt[kb_5] = SDLK_5;
        kb_binding[kb_6] = kb_binding_alt[kb_6] = SDLK_6;
        kb_binding[kb_7] = kb_binding_alt[kb_7] = SDLK_7;
        kb_binding[kb_8] = kb_binding_alt[kb_8] = SDLK_8;
        kb_binding[kb_9] = kb_binding_alt[kb_9] = SDLK_9;
        kb_binding[kb_0] = kb_binding_alt[kb_0] = SDLK_0;

        //
        kb_binding[kb_f1] = kb_binding_alt[kb_f1] = SDLK_F1;
        kb_binding[kb_f2] = kb_binding_alt[kb_f2] = SDLK_F2;
        kb_binding[kb_f3] = kb_binding_alt[kb_f3] = SDLK_F3;
        kb_binding[kb_f4] = kb_binding_alt[kb_f4] = SDLK_F4;
        kb_binding[kb_f5] = kb_binding_alt[kb_f5] = SDLK_F5;
        kb_binding[kb_f6] = kb_binding_alt[kb_f6] = SDLK_F6;
        kb_binding[kb_f7] = kb_binding_alt[kb_f7] = SDLK_F7;
        kb_binding[kb_f8] = kb_binding_alt[kb_f8] = SDLK_F8;
        kb_binding[kb_f9] = kb_binding_alt[kb_f9] = SDLK_F9;
        kb_binding[kb_f10] = kb_binding_alt[kb_f10] = SDLK_F10;
        kb_binding[kb_f11] = kb_binding_alt[kb_f11] = SDLK_F11;
        kb_binding[kb_f12] = kb_binding_alt[kb_f12] = SDLK_F12;
        kb_binding[kb_comma] = SDLK_LESS;
        kb_binding_alt[kb_comma] = SDLK_COMMA;
        kb_binding[kb_period]  = SDLK_GREATER;
        kb_binding_alt[kb_period] = SDLK_PERIOD;

        kb_binding[kb_alt] = SDLK_LALT;
        kb_binding_alt[kb_alt] = SDLK_RALT;
        kb_binding[kb_backspace] = SDLK_BACKSPACE;
        kb_binding_alt[kb_backspace] = -1;

        kb_binding[kb_ctrl] = SDLK_LCTRL;
        kb_binding_alt[kb_ctrl] = SDLK_RCTRL;
        kb_binding[kb_shift] = SDLK_LSHIFT;
        kb_binding_alt[kb_shift] = SDLK_RSHIFT;
        kb_binding[kb_delete] = SDLK_DELETE;
        kb_binding_alt[kb_delete] = SDLK_DELETE;
        kb_binding[kb_delete] = SDLK_PRINTSCREEN;
        kb_binding_alt[kb_delete] = SDLK_SYSREQ;

        kb_binding[kb_tab] = kb_binding_alt[kb_tab] = SDLK_TAB;
    }

    bool input_manager_sdl::load_input_settings(std::string file_path )
    {
        return input_manager_base::load_input_settings( file_path );
    }

    bool init_sdl_input_system()
    {
        error_log->report("Starting SDL input system...");
        if( input != NULL )
        {
            delete input;
            input = NULL;
        }
        input = new input_manager_sdl();
        error_log->report("Input swapped from base to sdl class...");
        return true;
    }

    void quit_sdl_input_system()
    {
        if( input != NULL && input->get_type() == "sdl")
        {
            delete input;
            input = new input_manager_base();
        }
    }

}
