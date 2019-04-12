/*
GPE_Input.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#include <iostream>
#include "GPE_CIncludes.h"
#include "GPE_Globals.h"
#include "GPE_String_Ex.h"
#include "GPE_Input.h"

GPE_GameController::GPE_GameController()
{
    sdlJoyId = -1;
    assignedSDLController = NULL;
    gc_system_axes_count = 0;
    controllerType = -1;
    controllerExtension = "";
    isConnected = false;
    mapping = 0;
    timestamp = 0;
    touch_mouse_x = 0;
    touch_mouse_y = 0;
    for(int but=0; but <gc_button_count; but++)
    {
        prevHit[but] = false;
        butIsDown[but] = false;
        butIsPressed[but] = false;
        butIsReleased[but] = false;
        butIsLocked[but] = false;
        buttonsMap[but] = -1;
    }


    for (int tempAxes=0; tempAxes < gc_axes_count; tempAxes++)
    {
        axesMap[ tempAxes ] = tempAxes;
        axesValues[ tempAxes ] = 0;
    }

    setup_default_mapping( );

}

GPE_GameController::~GPE_GameController()
{

}

bool GPE_GameController::is_connected()
{
    return isConnected;
}

int GPE_GameController::check_button(int buttonId)
{
    if( isConnected )
    {
        if(  buttonId >=0 && buttonId < gc_button_count)
        {
            if( buttonsMap[buttonId] >= 0)
            {
                return butIsDown[ buttonsMap[buttonId] ] > 0;
            }
        }
        else if( buttonId == gc_anybutton )
        {
            for( int iAnyButton = gc_button_count-1; iAnyButton >=0; iAnyButton-- )
            {
                if( butIsDown[ buttonsMap[iAnyButton] ] > 0 )
                {
                    return true;
                }
            }
            return false;
        }
        else if( buttonId == gc_nobutton )
        {
            for(int iNoButton = gc_button_count-1; iNoButton >=0; iNoButton-- )
            {
                if( butIsDown[ buttonsMap[iNoButton] ] > 0 )
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

int GPE_GameController::check_button_pressed(int buttonId)
{
    if( isConnected )
    {
        if( buttonId >=0 && buttonId < gc_button_count)
        {
            if( buttonsMap[buttonId] >= 0 )
            {
                return butIsPressed[ buttonsMap[buttonId] ] > 0;
            }
        }
        else if( buttonId == gc_anybutton )
        {
            for(int iAnyButton = gc_button_count-1; iAnyButton >=0; iAnyButton-- )
            {
                if( butIsPressed[ buttonsMap[iAnyButton] ] > 0 )
                {
                    return true;
                }
            }
            return false;
        }
        else if( buttonId == gc_nobutton )
        {
            for( int iNoButton = gc_button_count-1; iNoButton >=0; iNoButton-- )
            {
                if( butIsPressed[ buttonsMap[iNoButton] ] > 0 )
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

int GPE_GameController::check_button_released(int buttonId)
{
    if( isConnected )
    {
        if( buttonId >=0 && buttonId < gc_button_count)
        {
            if( buttonsMap[ buttonId] >= 0 )
            {
                return butIsReleased[ buttonsMap[buttonId] ];
            }
        }
        else if( buttonId == gc_anybutton )
        {
            for( int iAnyButton = gc_button_count-1; iAnyButton >=0; iAnyButton-- )
            {
                if( butIsReleased[ buttonsMap[iAnyButton] ] > 0 )
                {
                    return true;
                }
            }
            return false;
        }
        else if( buttonId == gc_nobutton )
        {
            for( int iNoButton = gc_button_count-1; iNoButton >=0; iNoButton-- )
            {
                if( butIsReleased[ buttonsMap[iNoButton] ] > 0 )
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

int GPE_GameController::gamecontroller_get_axes_count( )
{
    return gc_system_axes_count;
}

int GPE_GameController::find_gamecontroller_axes_mapping( int axesId )
{
    if(  axesId >=0 && axesId < gc_axes_count )
    {
        return axesMap[axesId];
    }
    return -1;
}

int GPE_GameController::find_gamecontroller_button_mapping( int buttonId )
{
    if(  buttonId >=0 && buttonId < gc_button_count )
    {
        return axesMap[buttonId];
    }
    return -1;
}

bool GPE_GameController::gamecontroller_map_axes(int axesId, int newAxesId )
{
    if(  axesId >=0 && axesId < gc_axes_count)
    {
        if( newAxesId >=0 && newAxesId < gc_axes_count )
        {
            buttonsMap[axesId] = newAxesId;
            return true;
        }
    }
    return false;
}

bool GPE_GameController::gamecontroller_map_button(int buttonId, int newButtonId)
{
    if(  buttonId >=0 && buttonId < gc_button_count)
    {
        if(  newButtonId >=0 && newButtonId < gc_button_count)
        {
            buttonsMap[buttonId] = newButtonId;
            return true;
        }
    }
    return false;
}

double GPE_GameController::get_axis_value( int axisNumber)
{
    if( axisNumber >=0 && axisNumber < gc_axes_count )
    {
        return axesValues[ axisNumber];
    }
    return 0;
}

std::string GPE_GameController::get_name( )
{
    return deviceName;
}

//GameController Input related functions/objects/variables as of Version 1.12 [ END ]
void GPE_GameController::handle_input()
{

}

void GPE_GameController::pure_reset()
{
    if( assignedSDLController!=NULL )
    {
        //SDL_GameControllerClose( assignedSDLController );
        //assignedSDLController = NULL;
        if (SDL_JoystickGetAttached(assignedSDLController) )
        {
            SDL_JoystickClose( assignedSDLController);
        }
        assignedSDLController = NULL;
    }
    for( int but=0; but< gc_button_count; but++)
    {
        prevHit[ but ] = false;
        butIsDown[ but ] = false;
        butIsPressed[ but ] = false;
        butIsReleased[ but] = false;
        butIsLocked[ but ] = false;
    }

    for (int tempAxes=0; tempAxes <gc_axes_count; tempAxes++)
    {
        axesValues[ tempAxes ] = 0;
    }
}

void GPE_GameController::reset_controller()
{
    for(int but=0; but<gc_button_count; but++)
    {
        butIsPressed[ but ] = butIsDown[ but ] && !prevHit[but];
        butIsReleased[ but] = !butIsDown[ but ] && prevHit[ but ];
    }
}

void GPE_GameController::set_connected( bool isNowConnected )
{
    isConnected = isNowConnected;
}

void GPE_GameController::set_name( std::string newName)
{
    deviceName = newName;
}

void GPE_GameController::setup_default_mapping( bool mapButtons, bool mapAxis)
{
    if( mapButtons)
    {
        for(int but=0; but <gc_button_count; but++)
        {
            buttonsMap[but] = -1;
        }

        //Maps the default controller buttons
         buttonsMap[gc_but0] = SDL_CONTROLLER_BUTTON_A;
         buttonsMap[gc_but1] = SDL_CONTROLLER_BUTTON_B;
         buttonsMap[gc_but2] = SDL_CONTROLLER_BUTTON_X;
         buttonsMap[gc_but3] = SDL_CONTROLLER_BUTTON_Y;

         buttonsMap[gc_left] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
         buttonsMap[gc_right] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
         buttonsMap[gc_up] = SDL_CONTROLLER_BUTTON_DPAD_UP;
         buttonsMap[gc_down] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;

         buttonsMap[gc_l1] = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
         buttonsMap[gc_r1] = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;

         buttonsMap[gc_l3] = SDL_CONTROLLER_BUTTON_LEFTSTICK;
         buttonsMap[gc_r3] = SDL_CONTROLLER_BUTTON_RIGHTSTICK;

         buttonsMap[gc_home] = SDL_CONTROLLER_BUTTON_GUIDE;
         buttonsMap[gc_select] = SDL_CONTROLLER_BUTTON_BACK;
         buttonsMap[gc_start] = SDL_CONTROLLER_BUTTON_START;
    }

    if( mapAxis )
    {
        for (int tempAxes=0; tempAxes < gc_axes_count; tempAxes++)
        {
            axesMap[ tempAxes ] = -1;
        }
        axesMap[ gc_joystick_left_x ] = SDL_CONTROLLER_AXIS_LEFTX;
        axesMap[ gc_joystick_left_y ] = SDL_CONTROLLER_AXIS_LEFTY;

        axesMap[ gc_joystick_right_x ] = SDL_CONTROLLER_AXIS_RIGHTX;
        axesMap[ gc_joystick_right_y ] = SDL_CONTROLLER_AXIS_RIGHTY;

        axesMap[ gc_joystick_trigger_left ] = SDL_CONTROLLER_AXIS_TRIGGERLEFT;
        axesMap[ gc_joystick_trigger_right ] = SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
    }
}

GPE_InputManager::GPE_InputManager()
{
    debugInput = false;
    requireControllerInputToConnect = true;
    programStartTicks = 0;
    time(&lastTimeScreenResized);
    forceRedrawNeeded = true;
    done = false;
    programJustStarted = true;
    invertScrollDirection = false;
    backSpaceIsPressed = false;
    shiftKeyIsPressed = false;
    last_key = 0;
    scroll_up = false;
    scroll_down = false;
    SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
    currentControllerCount = 0;
    //Creates the game controllers
    for( int iController = 0; iController < gc_max_devices; iController++)
    {
        game_controllers[iController] = new GPE_GameController();
    }


    for (int key=0; key<key_count; key++)
    {
        prev[key] = false;
        down[key] = false;
        pressed[key] = false;
        released[key] = false;
        lock[key] = false;
        binding[key] = binding_alt[key] = -1;
        binding_name[key] ="";
    }
    for (int key=0; key<joy_key_count; key++)
    {
        joy_binding[key] = key;
        joy_pressed[key] = false;
    }

    for (int mb=0; mb<mouse_button_count; mb++)
    {
        prevMouseButton[mb] = false;
        downMouseButton[mb] = false;
        pressedMouseButton[mb] = false;
        releasedMouseButton[mb] = false;
    }

    mouseScrollingUp = false;
    mouseScrollingDown = false;
    mouseScrollingLeft = false;
    mouseScrollingRight = false;


    binding_name[kb_esc] = "ESC";
    binding_name[kb_space] = "SPACE";
    binding_name[kb_up] = "UP";
    binding_name[kb_left] = "LEFT";
    binding_name[kb_right] = "RIGHT";
    binding_name[kb_down] = "DOWN";
    binding_name[kb_ctrl] = "CTRL";
    binding_name[kb_shift] = "SHIFT";
    binding_name[kb_a] = "A";
    binding_name[kb_b] = "B";
    binding_name[kb_c] = "C";
    binding_name[kb_d] = "D";
    binding_name[kb_e] = "E";
    binding_name[kb_f] = "F";
    binding_name[kb_g] = "G";
    binding_name[kb_h] = "H";
    binding_name[kb_i] = "I";
    binding_name[kb_j] = "J";
    binding_name[kb_k] = "K";
    binding_name[kb_l] = "L";
    binding_name[kb_m] = "M";
    binding_name[kb_n] = "N";
    binding_name[kb_o] = "O";
    binding_name[kb_p] = "P";
    binding_name[kb_q] = "Q";
    binding_name[kb_r] = "R";
    binding_name[kb_s] = "S";
    binding_name[kb_t] = "T";
    binding_name[kb_u] = "U";
    binding_name[kb_v] = "V";
    binding_name[kb_w] = "W";
    binding_name[kb_x] = "X";
    binding_name[kb_y] = "Y";
    binding_name[kb_z] = "Z";
    binding_name[kb_tab] = "TAB";
    binding_name[kb_f1] = "F1";
    binding_name[kb_f2] = "F2";
    binding_name[kb_f3] = "F3";
    binding_name[kb_f4] = "F4";
    binding_name[kb_f5] = "F5";
    binding_name[kb_f6] = "F6";
    binding_name[kb_f7] = "F7";
    binding_name[kb_f8] = "F8";
    binding_name[kb_f9] = "F9";
    binding_name[kb_f10] = "F10";
    binding_name[kb_f11] = "F11";
    binding_name[kb_f12] = "F12";
    binding_name[kb_plus] = "=";
    binding_name[kb_comma] = "<";
    binding_name[kb_minus] = "_";
    binding_name[kb_period] = ">";
    mouse_x=0;
    mouse_y=0;

    mouse_button[0] = "LMB";
    mouse_button[1] = "LMB";
    mouse_button[2] = "RMB";
    mouse_button[3] = "Wheel Up";
    mouse_button[4] = "Wheel Down";
    mouse_button[5] = "MBX1";
    mouse_button[6] = "MBX2";

    SDL_StartTextInput();
    inputHappenedInFrame = true;
    keyInputReceivedInFrame = false;
    mouseInputReceivedInFrame = false;
    mouseMovementInputReceivedInFrame = false;
    mouseWheelInputReceivedInFrame = false;
    windowEventHappendInFrame = false;
    defaultQwertyKeyBindings();

    load_input_settings();
    detect_gamecontrollers( );
}

GPE_InputManager::~GPE_InputManager()
{

}

void GPE_InputManager::defaultQwertyKeyBindings()
{
    binding[kb_esc] = SDLK_ESCAPE;
    binding[kb_enter] = SDLK_RETURN;
    binding[kb_up] = SDLK_UP;
    binding[kb_down] = SDLK_DOWN;
    binding[kb_left] = SDLK_LEFT;
    binding[kb_right] = SDLK_RIGHT;
    binding[kb_space] = binding_alt[kb_space] = SDLK_SPACE;
    binding[kb_page_down] = binding_alt[kb_page_down] = SDLK_PAGEDOWN;
    binding[kb_page_up] = binding_alt[kb_page_up] = SDLK_PAGEUP;
    binding[kb_a] = binding_alt[kb_a] = SDLK_a;
    binding[kb_b] = binding_alt[kb_b] = SDLK_b;
    binding[kb_c] = binding_alt[kb_c] = SDLK_c;
    binding[kb_d] = binding_alt[kb_d] = SDLK_d;
    binding[kb_e] = binding_alt[kb_e] = SDLK_e;
    binding[kb_f] = binding_alt[kb_f] = SDLK_f;
    binding[kb_g] = binding_alt[kb_g] = SDLK_g;
    binding[kb_h] = binding_alt[kb_h] = SDLK_h;
    binding[kb_i] = binding_alt[kb_i] = SDLK_i;
    binding[kb_j] = binding_alt[kb_j] = SDLK_j;
    binding[kb_k] = binding_alt[kb_k] = SDLK_k;
    binding[kb_l] = binding_alt[kb_l] = SDLK_l;
    binding[kb_m] = binding_alt[kb_m] = SDLK_m;
    binding[kb_n] = binding_alt[kb_n] = SDLK_n;
    binding[kb_o] = binding_alt[kb_o] = SDLK_o;
    binding[kb_p] = binding_alt[kb_p] = SDLK_p;
    binding[kb_q] = binding_alt[kb_q] = SDLK_q;
    binding[kb_r] = binding_alt[kb_r] = SDLK_r;
    binding[kb_s] = binding_alt[kb_s] = SDLK_s;
    binding[kb_t] = binding_alt[kb_t] = SDLK_t;
    binding[kb_u] = binding_alt[kb_u] = SDLK_u;
    binding[kb_v] = binding_alt[kb_v] = SDLK_v;
    binding[kb_w] = binding_alt[kb_w] = SDLK_w;
    binding[kb_x] = binding_alt[kb_x] = SDLK_x;
    binding[kb_y] = binding_alt[kb_y] = SDLK_y;
    binding[kb_z] = binding_alt[kb_z] = SDLK_z;
    binding[kb_minus] = SDLK_MINUS;
    binding_alt[kb_minus] = SDLK_UNDERSCORE;
    binding[kb_plus] = SDLK_EQUALS;
    binding_alt[kb_plus] = SDLK_BACKQUOTE;

    binding_name[kb_a] = "A";
    binding_name[kb_b] = "B";
    binding_name[kb_c] = "C";
    binding_name[kb_d] = "D";
    binding_name[kb_e] = "E";
    binding_name[kb_f] = "F";
    binding_name[kb_g] = "G";
    binding_name[kb_h] = "H";
    binding_name[kb_i] = "I";
    binding_name[kb_j] = "J";
    binding_name[kb_k] = "K";
    binding_name[kb_l] = "L";
    binding_name[kb_m] = "M";
    binding_name[kb_n] = "N";
    binding_name[kb_o] = "O";
    binding_name[kb_p] = "P";
    binding_name[kb_q] = "Q";
    binding_name[kb_r] = "R";
    binding_name[kb_s] = "S";
    binding_name[kb_t] = "T";
    binding_name[kb_u] = "U";
    binding_name[kb_v] = "V";
    binding_name[kb_w] = "W";
    binding_name[kb_x] = "X";
    binding_name[kb_y] = "Y";
    binding_name[kb_z] = "Z";

    binding_name[kb_comma] = "<";
    binding_name[kb_period] = ">";

    binding_name[kb_f1] = "F1";
    binding_name[kb_f2] = "F2";
    binding_name[kb_f3] = "F3";
    binding_name[kb_f4] = "F4";
    binding_name[kb_f5] = "F5";
    binding_name[kb_f6] = "F6";
    binding_name[kb_f7] = "F7";
    binding_name[kb_f8] = "F8";
    binding_name[kb_f9] = "F9";
    binding_name[kb_f10] = "F10";
    binding_name[kb_f11] = "F11";
    binding_name[kb_f12] = "F12";

    binding_alt[kb_esc] = SDLK_ESCAPE;
    binding_alt[kb_enter] = SDLK_RETURN2;

    binding_alt[kb_up] = SDLK_UP;
    binding_alt[kb_down] = SDLK_DOWN;
    binding_alt[kb_left] = SDLK_LEFT;
    binding_alt[kb_right] = SDLK_RIGHT;

    binding[kb_1] = binding_alt[kb_1] = SDLK_1;
    binding[kb_2] = binding_alt[kb_2] = SDLK_2;
    binding[kb_3] = binding_alt[kb_3] = SDLK_3;
    binding[kb_4] = binding_alt[kb_4] = SDLK_4;
    binding[kb_5] = binding_alt[kb_5] = SDLK_5;
    binding[kb_6] = binding_alt[kb_6] = SDLK_6;
    binding[kb_7] = binding_alt[kb_7] = SDLK_7;
    binding[kb_8] = binding_alt[kb_8] = SDLK_8;
    binding[kb_9] = binding_alt[kb_9] = SDLK_9;
    binding[kb_0] = binding_alt[kb_0] = SDLK_0;

    //
    binding[kb_f1] = binding_alt[kb_f1] = SDLK_F1;
    binding[kb_f2] = binding_alt[kb_f2] = SDLK_F2;
    binding[kb_f3] = binding_alt[kb_f3] = SDLK_F3;
    binding[kb_f4] = binding_alt[kb_f4] = SDLK_F4;
    binding[kb_f5] = binding_alt[kb_f5] = SDLK_F5;
    binding[kb_f6] = binding_alt[kb_f6] = SDLK_F6;
    binding[kb_f7] = binding_alt[kb_f7] = SDLK_F7;
    binding[kb_f8] = binding_alt[kb_f8] = SDLK_F8;
    binding[kb_f9] = binding_alt[kb_f9] = SDLK_F9;
    binding[kb_f10] = binding_alt[kb_f10] = SDLK_F10;
    binding[kb_f11] = binding_alt[kb_f11] = SDLK_F11;
    binding[kb_f12] = binding_alt[kb_f12] = SDLK_F12;
    binding[kb_comma] = SDLK_LESS;
    binding_alt[kb_comma] = SDLK_COMMA;
    binding[kb_period]  = SDLK_GREATER;
    binding_alt[kb_period] = SDLK_PERIOD;

    binding[kb_alt] = SDLK_LALT;
    binding_alt[kb_alt] = SDLK_RALT;
    binding[kb_backspace] = SDLK_BACKSPACE;
    binding_alt[kb_backspace] = -1;

    binding[kb_ctrl] = SDLK_LCTRL;
    binding_alt[kb_ctrl] = SDLK_RCTRL;
    binding[kb_shift] = SDLK_LSHIFT;
    binding_alt[kb_shift] = SDLK_RSHIFT;
    binding[kb_delete] = SDLK_DELETE;
    binding_alt[kb_delete] = SDLK_DELETE;
    binding[kb_delete] = SDLK_PRINTSCREEN;
    binding_alt[kb_delete] = SDLK_SYSREQ;

    binding[kb_tab] = binding_alt[kb_tab] = SDLK_TAB;
}

void GPE_InputManager::handle_input(bool dump_event, bool isEmbedded )
{
    SDL_GameController * foundSDLController = NULL;
    MAIN_RENDERER->reset_input();
    inputHappenedInFrame = false;
    keyInputReceivedInFrame = false;
    mouseInputReceivedInFrame = false;
    mouseMovementInputReceivedInFrame = false;
    mouseWheelInputReceivedInFrame = false;
    windowEventHappendInFrame = false;

    SDL_Event event;
    bool windowWasPrevSize = WINDOW_WAS_JUST_RESIZED;
    /*
    if( WINDOW_WAS_JUST_RESIZED)
    {
        time_t currentTimeNow;
        time(&currentTimeNow);
        if( difftime(currentTimeNow,lastTimeScreenResized ) >=0.125 )
        {
            WINDOW_WAS_JUST_RESIZED = false;
            inputHappenedInFrame = true;
            forceRedrawNeeded = true;
        }
    }*/
    WINDOW_WAS_JUST_RESIZED = false;
    mouseScrollingLeft = false;
    mouseScrollingRight = false;
    mouseScrollingUp = false;
    mouseScrollingDown = false;
    SCREEN_WIDTH = MAIN_RENDERER->get_window_width();
    SCREEN_HEIGHT= MAIN_RENDERER->get_window_height();
    int prevMouseX = mouse_x;
    int prevMouseY = mouse_y;
    int key = 0;
    int cbutton = 0;
    for (key=0; key<key_count; key++)
    {
        prev[key] = down[key];
        pressed[key] = false;
        released[key] = false;
        if( key!=kb_ctrl && key!=kb_alt)
        {
            //down[key] = false;
        }
    }

    for (int mb=0; mb<mouse_button_count; mb++)
    {
        prevMouseButton[mb] = downMouseButton[mb];
        pressedMouseButton[mb] = false;
        releasedMouseButton[mb] = false;
        doubleClickedMouseButton[mb] = 0;
    }
    MAIN_RENDERER->reset_input();
    backSpaceIsPressed = false;
    done = false;
    //shiftKeyIsPressed = false;
    //SDL_StartTextInput();
    fileDroppedList.clear();
    /* Check for events */
    int frame_wait = 2; /* anything less than 16 ms is good */
    unsigned tries = 3; /* use UINT_MAX for no effective limit */
    int fControllerInputId = 0;
    GPE_GameController *  tempController = NULL;
    SDL_JoystickUpdate();
    double tempAxisValue = 0.0;
    std::vector < int > addControllerList;
    std::vector < int > removeControllerList;

    //if (SDL_WaitEventTimeout(&event, frame_wait))
    {
        //do
        while (SDL_PollEvent(&event) )
        {
            switch(event.type)
            {
            case (SDL_DROPFILE):
                // In case if dropped file
                dropped_filedir = event.drop.file;
                // Shows directory of dropped file
                /*
                SDL_ShowSimpleMessageBox(
                    SDL_MESSAGEBOX_ERROR,
                    "File dropped on window",
                    dropped_filedir,
                    MAIN_RENDERER->get_window()
                );
                */
                fileDroppedList.push_back(dropped_filedir);
                SDL_free(dropped_filedir);    // Free dropped_filedir memory
                inputHappenedInFrame = true;
                keyInputReceivedInFrame = true;
                forceRedrawNeeded = true;
            break;

            //Game Controller Input
            case SDL_CONTROLLERDEVICEADDED:
                //For reasons, we will just add to them a list and manage em at the end
                addControllerList.push_back( event.cdevice.which );
            break;

            case SDL_CONTROLLERDEVICEREMOVED:
                //For reasons, we will just add to them a list and manage em at the end
                removeControllerList.push_back( event.cdevice.which );
            break;

            case SDL_CONTROLLERBUTTONDOWN:
                fControllerInputId = event.cdevice.which;
                if( debugInput )
                {
                    GPE_Report("Button[" +int_to_string( event.cbutton.button) + "] on controller [" + int_to_string(fControllerInputId  )+"]  fired...");
                }
                if( fControllerInputId >= 0 && fControllerInputId < gc_max_devices )
                {
                    tempController = game_controllers[ fControllerInputId ];
                    if( requireControllerInputToConnect || tempController->is_connected() == false )
                    {
                        tempController->set_connected( true );
                    }
                    for (  cbutton = 0;  cbutton < gc_button_count; cbutton++ )
                    {
                        if ( event.cbutton.button == tempController->buttonsMap[cbutton] )
                        {
                            tempController->butIsDown[ tempController->buttonsMap[cbutton] ] = true;
                            tempController->butIsPressed[ tempController->buttonsMap[cbutton] ] = true;
                        }
                    }
                }
            break;

            case SDL_CONTROLLERBUTTONUP:
                fControllerInputId = event.cdevice.which;
                if( debugInput )
                {
                    GPE_Report("Button[" +int_to_string( event.cbutton.button) + "] on controller [" + int_to_string( fControllerInputId )+"] released...");
                }
                if( fControllerInputId >= 0 && fControllerInputId < gc_max_devices )
                {
                    tempController = game_controllers[ fControllerInputId ];
                    for (  cbutton = 0;  cbutton < gc_button_count; cbutton++)
                    {
                        if ( event.cbutton.button == tempController->buttonsMap[cbutton] )
                        {
                            tempController->butIsDown[ tempController->buttonsMap[cbutton] ] = false;
                            tempController->butIsPressed[ tempController->buttonsMap[cbutton] ] = false;
                        }
                    }
                }
            break;

            case SDL_CONTROLLERAXISMOTION:
                fControllerInputId = event.cdevice.which;
                if( fControllerInputId >= 0 && fControllerInputId < gc_max_devices )
                {
                    tempController = game_controllers[ fControllerInputId ];
                    if( requireControllerInputToConnect || tempController->is_connected() == false )
                    {
                        tempController->set_connected( true );
                    }
                    //if( event.caxis.axis >= 0 && event.caxis.axis < gc_axes_count )
                    {
                        tempAxisValue = event.caxis.value / gc_deadzone;
                        //Less than the dead zone
                        if( tempAxisValue < -1.d )
                        {
                            tempAxisValue = -1.d;
                        }
                        //Right of dead zone
                        else if( tempAxisValue > 1.d )
                        {
                            tempAxisValue =  1.d;
                        }
                        tempController->axesValues[event.caxis.axis] = tempAxisValue;
                    }
                }
            break;

            // grab symbol keys
            case SDL_KEYDOWN:
                inputHappenedInFrame = true;
                keyInputReceivedInFrame = true;
                forceRedrawNeeded = true;
                //handle_modifers( event.key->keysym.mod );
                if( event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    backSpaceIsPressed = true;
                    down[kb_backspace] = true;
                    pressed[kb_backspace] = true;
                }
                else if( event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT)
                {
                    shiftKeyIsPressed = true;
                    down[kb_shift] = true;
                    pressed[kb_shift] = true;
                }
                else if( event.key.keysym.scancode==SDL_SCANCODE_LCTRL || event.key.keysym.scancode==SDL_SCANCODE_RCTRL)
                {
                    down[kb_ctrl] = true;
                    pressed[kb_ctrl] = true;
                }
                else if( event.key.keysym.scancode==SDL_SCANCODE_LALT || event.key.keysym.scancode==SDL_SCANCODE_RALT)
                {
                    down[kb_alt] = true;
                    pressed[kb_alt] = true;
                }
                else
                {
                    for ( key=0; key<key_count; key++)
                    {
                        if (event.key.keysym.sym == binding[key] || event.key.keysym.sym == binding_alt[key])
                        {
                            down[key] = true;
                            pressed[key] = true;
                        }
                    }
                }
            break;

            case SDL_KEYUP:
                inputHappenedInFrame = true;
                keyInputReceivedInFrame = true;
                forceRedrawNeeded = true;
                if( event.key.keysym.scancode==SDL_SCANCODE_LCTRL || event.key.keysym.scancode==SDL_SCANCODE_RCTRL)
                {
                    down[kb_ctrl] = false;
                    pressed[kb_ctrl] = false;
                    lock[kb_ctrl] = false;
                }
                else if( event.key.keysym.scancode==SDL_SCANCODE_LALT || event.key.keysym.scancode==SDL_SCANCODE_RALT)
                {
                    down[kb_alt] = false;
                    pressed[kb_alt] = false;
                    lock[kb_alt] = true;
                }
                else if( event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT)
                {
                    shiftKeyIsPressed = false;
                    down[kb_shift] = false;
                    pressed[kb_shift] = false;
                    lock[kb_shift] = true;
                }
                else if( event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    backSpaceIsPressed = false;
                    down[kb_backspace] = false;
                    pressed[kb_backspace] = false;
                    lock[kb_shift] = true;
                }
                else
                {
                    for (int key=0; key<key_count; key++)
                    {
                        if (event.key.keysym.sym == binding[key] || event.key.keysym.sym == binding_alt[key])
                        {
                            down[key] = false;
                            pressed[key] = false;
                            //released[key] = true;
                            lock[key] = false;
                        }
                    }
                }
                last_key = event.key.keysym.sym;
            break;

            case SDL_TEXTINPUT:
                //Not copy or pasting
                //if( !( ( event.text.text[ 0 ] == 'c' || event.text.text[ 0 ] == 'C' ) && ( event.text.text[ 0 ] == 'v' || event.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
            {
                //Append character
                inkeys= event.text.text;
                inputHappenedInFrame = true;
                keyInputReceivedInFrame = true;
                forceRedrawNeeded = true;
            }
            break;

            case SDL_MOUSEBUTTONDOWN:
                inputHappenedInFrame = true;
                keyInputReceivedInFrame = true;
                forceRedrawNeeded = true;
                switch(event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    downMouseButton[0] = 1;
                    doubleClickedMouseButton[0] = event.button.clicks;
                    releasedMouseButton[0] = false;
                    break;
                case SDL_BUTTON_RIGHT:
                    downMouseButton[1] = 1;
                    doubleClickedMouseButton[1] = event.button.clicks;
                    releasedMouseButton[1] = false;
                    break;
                case SDL_BUTTON_MIDDLE:
                    downMouseButton[2] = 1;
                    doubleClickedMouseButton[2] = event.button.clicks;
                    releasedMouseButton[2] = false;
                    break;

                default:

                    break;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                inputHappenedInFrame = true;
                keyInputReceivedInFrame = false;
                forceRedrawNeeded = true;
                switch(event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    downMouseButton[0] = false;
                    pressedMouseButton[0] = false;
                    //releasedMouseButton[0] = true;
                    break;
                case SDL_BUTTON_RIGHT:
                    downMouseButton[1] = false;
                    pressedMouseButton[1] = false;
                    //releasedMouseButton[1] = true;
                    break;
                case SDL_BUTTON_MIDDLE:
                    downMouseButton[2] = false;
                    pressedMouseButton[2] = false;
                    //releasedMouseButton[2] = true;
                    break;
                case SDL_BUTTON_X1:
                    downMouseButton[3] = false;
                    pressedMouseButton[3] = false;
                    //releasedMouseButton[3] = true;
                    break;
                case SDL_BUTTON_X2:
                    downMouseButton[4] = false;
                    pressedMouseButton[4] = false;
                    //releasedMouseButton[4] = true;
                    break;
                default:

                    break;
                }
                break;

            case SDL_MOUSEWHEEL:
                inputHappenedInFrame = true;
                keyInputReceivedInFrame = true;
                forceRedrawNeeded = true;
                mouseWheelInputReceivedInFrame = true;
                if( invertScrollDirection )
                {
                    if( event.wheel.x > 0)
                    {
                        mouseScrollingLeft = true;
                    }
                    else if( event.wheel.x < 0)
                    {
                        mouseScrollingRight = true;
                    }

                    if( event.wheel.y > 0)
                    {
                        mouseScrollingDown = true;
                    }
                    else if( event.wheel.y < 0)
                    {
                        mouseScrollingUp = true;
                    }
                }
                else
                {
                    if( event.wheel.x > 0)
                    {
                        mouseScrollingRight = true;
                    }
                    else if( event.wheel.x < 0)
                    {
                        mouseScrollingLeft = true;
                    }

                    if( event.wheel.y > 0)
                    {
                        mouseScrollingUp = true;
                    }
                    else if( event.wheel.y < 0)
                    {
                        mouseScrollingDown = true;
                    }
                }
                break;

            case SDL_WINDOWEVENT:
                reset_all_input();
                inputHappenedInFrame = true;
                keyInputReceivedInFrame = true;
                forceRedrawNeeded = true;
                //Window related events
                MAIN_RENDERER->handle_events(event);
                break;

            case SDL_QUIT:
                inputHappenedInFrame = true;
                keyInputReceivedInFrame = true;
                forceRedrawNeeded = true;
                if( isEmbedded==false)
                {
                    //User requests quit
                    done = true;
                }
                break;
            default:
                break;
            };
        }
        //);
    }

    if( (int)removeControllerList.size() > 0 )
    {
        for( int i = (int)removeControllerList.size() - 1; i >= 0; i-- )
        {
            disconnect_gamecontroller( removeControllerList[i] );
        }
        if( gamecontroller_count()==0 && SDL_WasInit( SDL_INIT_GAMECONTROLLER ) )
        {
            SDL_QuitSubSystem( SDL_INIT_GAMECONTROLLER );
            SDL_InitSubSystem( SDL_INIT_GAMECONTROLLER );
        }
    }

    if( (int)addControllerList.size() > 0 )
    {
        if( gamecontroller_count()== 0 )
        {
            if( SDL_WasInit( SDL_INIT_JOYSTICK ) )
            {
                SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
            }
            SDL_InitSubSystem( SDL_INIT_JOYSTICK );
        }

        for( int i = 0; i < (int)addControllerList.size(); i++ )
        {
            //setup_new_gamecontroller( addControllerList[i], true);
        }
        detect_gamecontrollers();
    }

    SDL_GetMouseState(&mouse_x, &mouse_y);
    //detect_controllers( false );
    //inkeys = "";
    if( mouse_x!=prevMouseX || mouse_y!=prevMouseY)
    {
        mouseInputReceivedInFrame = mouseMovementInputReceivedInFrame = true;
        inputHappenedInFrame = true;
        forceRedrawNeeded = true;
    }

    if( WINDOW_WAS_JUST_RESIZED)
    {
        windowEventHappendInFrame = true;
        SCREEN_WIDTH = MAIN_RENDERER->get_window_width();
        SCREEN_HEIGHT= MAIN_RENDERER->get_window_height();
        if( !windowWasPrevSize || MAIN_RENDERER->recentResizeHappened )
        {
            time(&lastTimeScreenResized);
        }
        reset_all_input();
        /*MAIN_RENDERER->clear_renderer();
        MAIN_RENDERER->update_renderer();*/
    }
    if( MAIN_RENDERER->windowClosed)
    {
        done = true;
    }
    for (int key=0; key<key_count; key++)
    {
        pressed[key] = down[key] && !prev[key];
        released[key] = !down[key] && prev[key];
    }
    for (int mb=0; mb<mouse_button_count; mb++)
    {
        pressedMouseButton[mb] = downMouseButton[mb] && !prevMouseButton[mb];
        releasedMouseButton[mb] = !downMouseButton[mb] && prevMouseButton[mb];
    }
    //SDL_StopTextInput();
}

bool GPE_InputManager::detect_gamecontrollers()
{
    if( debugInput )
    {
        GPE_Report("Performing Controller Check...");
    }

    int i = 0;
    currentControllerCount = 0;
    for( i = 0; i < gc_max_devices; i++ )
    {
        game_controllers[i]->set_connected( false );
        game_controllers[i]->set_name("");
        game_controllers[i]->pure_reset();
    }

    int currentControllerId = 0;
    for ( i = 0; i < SDL_NumJoysticks(); ++i )
    {
        setup_new_gamecontroller( i );
    }

    if( currentControllerCount > 0)
    {
        return true;
    }

    if( debugInput )
    {
        GPE_Report("    No Game Controllers found...");
    }
    return false;
}

bool GPE_InputManager::load_input_settings(std::string alternatePath )
{
    std::string inputSettingsFile = APP_DIRECTORY_NAME + "gpe_input_settings.txt";
    if( file_exists(alternatePath) )
    {
        inputSettingsFile = inputSettingsFile;
    }
    if( file_exists(inputSettingsFile) )
    {
        std::ifstream gameResourceFileIn( inputSettingsFile.c_str() );
        if (gameResourceFileIn.is_open() )
        {
            int equalPos = 0;
            std::string firstChar="";
            std::string keyString="";
            std::string valString="";
            std::string subValString="";
            std::string currLine="";
            std::string currLineToBeProcessed;

            int i = 0;
            double foundFileVersion = 0;
            while ( gameResourceFileIn.good() )
            {
                getline (gameResourceFileIn,currLine); //gets the next line of the file
                currLineToBeProcessed = trim_left_inplace(currLine);
                currLineToBeProcessed = trim_right_inplace(currLineToBeProcessed);

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
                                keyString = currLineToBeProcessed.substr(0,equalPos);
                                valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                if( keyString=="Version")
                                {
                                    foundFileVersion = string_to_double(valString);
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
                            keyString = currLineToBeProcessed.substr(0,equalPos);
                            valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                            if( keyString=="ResourceName")
                            {
                                debugInput = string_to_bool( valString);
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
bool GPE_InputManager::disconnect_gamecontroller( int cId )
{
    SDL_ClearError();
    if( debugInput )
    {
        GPE_Report("Attempting to disconnect gamecontroller["+ int_to_string(cId )+"]..." );
    }

    if ( SDL_IsGameController(cId) )
    {
        for( int i = gc_max_devices - 1; i >=0; i--)
        {
            if( game_controllers[i]->sdlJoyId == cId )
            {
                game_controllers[i]->pure_reset();
                game_controllers[i]->set_connected( false );
                if( currentControllerCount > 0)
                {
                    currentControllerCount--;
                }
                game_controllers[i]->sdlJoyId  = -1;
            }
        }
        if( debugInput )
        {
            GPE_Report(SDL_GetError() );
        }
    }
    else if( debugInput )
    {
        GPE_Report(SDL_GetError() );
    }
    if( debugInput)
    {
        GPE_Report("gamecontroller["+ int_to_string(cId )+"] disconnected..." );
    }
}

bool GPE_InputManager::setup_new_gamecontroller(int cId )
{
    SDL_ClearError();
    if( debugInput)
    {
        GPE_Report("Attempting to setup gamecontroller["+ int_to_string(cId )+"]..." );
    }
    int jStickCount = SDL_NumJoysticks();
    if( jStickCount < 0 )
    {
        if( debugInput )
        {
            GPE_Report("Joystick count ["+int_to_string(jStickCount)+"] is out of range!" );
            GPE_Report( SDL_GetError( ) );
        }
        //Attempt to fix our problem...
        if( SDL_WasInit( SDL_INIT_JOYSTICK ) )
        {
            SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
        }
        SDL_InitSubSystem( SDL_INIT_JOYSTICK );
        return false;
    }

    if( cId >= jStickCount )
    {
        if( debugInput )
        {
            GPE_Report("Joystick ["+int_to_string(cId)+"] is out of range!" );
        }
        return false;
    }

    if ( SDL_IsGameController(cId) )
    {
        SDL_GameController * foundSDLController = SDL_GameControllerOpen( cId );
        if( foundSDLController )
        {
            if( currentControllerCount >=0 && currentControllerCount < gc_max_devices-1 )
            {
                if( debugInput )
                {
                    GPE_Report("Finding gamecontroller...");
                }
                GPE_GameController * tgamecontroller = game_controllers[ currentControllerCount ];

                if( debugInput )
                {
                    GPE_Report("Finding Joystick ID...");
                }
                SDL_Joystick * jStick = SDL_GameControllerGetJoystick( foundSDLController );

                if( jStick )
                {
                    tgamecontroller->assignedSDLController = jStick;
                }
                else
                {
                    tgamecontroller->assignedSDLController = NULL;
                    tgamecontroller->set_connected( false );
                    if( debugInput )
                    {
                        GPE_Report( SDL_GetError() );
                    }
                    return false;
                }

                tgamecontroller->sdlJoyId = currentControllerCount;

                if( debugInput )
                {
                    GPE_Report("Setting name...");
                }
                tgamecontroller->set_name( SDL_JoystickNameForIndex( cId ) );

                if( debugInput )
                {
                    GPE_Report("Name set successfully...");
                }
                if( !requireControllerInputToConnect )
                {
                    tgamecontroller->set_connected( true );
                }
                if( debugInput )
                {
                    GPE_Report("Mapping buttons...");
                }
                tgamecontroller->setup_default_mapping();
                if( debugInput )
                {
                    GPE_Report("New GameController has been setup.");
                }
                currentControllerCount++;
                return true;
            }
            else if( debugInput)
            {
                GPE_Report("Failed to start gamecontroller["+ int_to_string(cId )+"]..." );
                GPE_Report(SDL_GetError() );
            }
        }
        else if( debugInput )
        {
            GPE_Report("Unable to open gamecontroller ("+int_to_string( cId )+"):    ["+SDL_GetError()+"].");
        }
    }
    else if( debugInput )
    {
        GPE_Report("Failed...");
        GPE_Report(SDL_GetError( ));
    }
    return false;
}

int GPE_InputManager::gamecontroller_count()
{
    return currentControllerCount;
}

bool GPE_InputManager::input_received()
{
    if( programJustStarted || WINDOW_WAS_JUST_RESIZED)
    {
        return true;
    }
    return inputHappenedInFrame;
}

void GPE_InputManager::reset_all_input()
{
    forceRedrawNeeded = true;
    scroll_up = false;
    scroll_down = false;
    mouseScrollingDown = false;
    mouseScrollingUp = false;
    mouseScrollingLeft = false;
    mouseScrollingRight = false;
    /*mouse_x = -1;
    mouse_y = -1;*/
    for (int key=0; key<key_count; key++)
    {
        pressed[key] = false;
        down[key] = false;
        prev[key] = false;
        released[key] = false;
    }
    for (int mb=0; mb<mouse_button_count; mb++)
    {
        pressedMouseButton[mb] = false;
        downMouseButton[mb] = false;
        prevMouseButton[mb] = false;
        releasedMouseButton[mb] = false;
    }
    inkeys = "";
    backSpaceIsPressed= false;
    shiftKeyIsPressed = false;
    inputHappenedInFrame = true;
}

void GPE_InputManager::resetScroll()
{
    scroll_up = false;
    scroll_down = false;
}

bool GPE_InputManager::anyKeyIsPress()
{
    for (int key=0; key<key_count; key++)
    {
        if ( pressed[key] && !lock[key])
        {
            return true;
        }
    }
    return false;
}

bool GPE_InputManager::check_mouse_doubleclicked(int buttonId)
{
    if(buttonId>=0 && buttonId<mouse_button_count )
    {
        if(doubleClickedMouseButton[buttonId]>1)
        {
            return true;
        }
    }
    return false;
}

bool GPE_InputManager::check_keyboard_only_down(int buttonId  )
{
    if( buttonId>=0 && buttonId < key_count )
    {
        if( down[buttonId] )
        {
            for( int i = 0; i < key_count; i++ )
            {
                if( i!= buttonId )
                {
                    if( down[i] )
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

bool GPE_InputManager::check_keyboard_only_released(int buttonId  )
{
    if( buttonId>=0 && buttonId < key_count )
    {
        if( released[buttonId] )
        {
            for( int i = 0; i < key_count; i++ )
            {
                if( i!= buttonId )
                {
                    if( down[i] == true )
                    {
                        return false;
                    }
                    if( pressed[i] == true )
                    {
                        return false;
                    }
                    else if( released[i] == true )
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

bool GPE_InputManager::check_keyboard_down(int buttonId)
{
    if(buttonId>=0 && buttonId<key_count )
    {
        return down[buttonId];
    }
    return false;
}

bool GPE_InputManager::check_keyboard_pressed(int buttonId)
{
    if(buttonId>=0 && buttonId<key_count )
    {
        return pressed[buttonId];
    }
    return false;
}

bool GPE_InputManager::check_keyboard_released(int buttonId)
{
    if(buttonId>=0 && buttonId<key_count )
    {
        return released[buttonId];
    }
    return false;
}

bool GPE_InputManager::gamecontroller_connected (int gamecontrollerPos)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->is_connected();
    }
    return -1;
};

std::string GPE_InputManager::gamecontroller_detected_name(int gamecontrollerPos)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->get_name();
    }
    return "";
};


std::string GPE_InputManager::gamecontroller_name (int gamecontrollerPos)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->get_name();
    }
    return "";
};

GPE_GameController * GPE_InputManager::gamecontroller_object (int gamecontrollerPos)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos];
    }
    return NULL;
};

int GPE_InputManager::gamecontroller_get_axes_count ( int gamecontrollerPos )
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->gamecontroller_get_axes_count();
    }
    return -1;
};

int GPE_InputManager::find_gamecontroller_mapping (int gamecontrollerPos, int buttonId)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->find_gamecontroller_button_mapping( buttonId);
    }
    return -1;
};

int GPE_InputManager::check_gamecontroller (int gamecontrollerPos, int buttonId)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->check_button(buttonId);
    }
    else if( gamecontrollerPos == gc_anycontroller )
    {
        for( int iConPos = gc_max_devices -1; iConPos >=0; iConPos-- )
        {
            if( game_controllers[iConPos]->check_button(buttonId) )
            {
                return true;
            }
        }
    }
    return false;
}

int GPE_InputManager::check_gamecontroller_down (int gamecontrollerPos, int buttonId)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->check_button(buttonId);
    }
    else if( gamecontrollerPos == gc_anycontroller )
    {
        for( int iConPos = gc_max_devices -1; iConPos >=0; iConPos-- )
        {
            if( game_controllers[iConPos]->check_button(buttonId) )
            {
                return true;
            }
        }
    }
    return false;
};

int GPE_InputManager::check_gamecontroller_pressed (int gamecontrollerPos, int buttonId)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->check_button_pressed(buttonId);
    }
    else if( gamecontrollerPos == gc_anycontroller )
    {
        for( int iConPos = gc_max_devices -1; iConPos >=0; iConPos-- )
        {
            if( game_controllers[iConPos]->check_button_pressed(buttonId) )
            {
                return true;
            }
        }
    }
    return false;
};

int GPE_InputManager::check_gamecontroller_released (int gamecontrollerPos, int buttonId)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->check_button_released(buttonId);
    }
    else if( gamecontrollerPos == gc_anycontroller )
    {
        for( int iConPos = gc_max_devices -1; iConPos >=0; iConPos-- )
        {
            if( game_controllers[iConPos]->check_button_released(buttonId) )
            {
                return true;
            }
        }
    }
    return false;
};

int GPE_InputManager::find_gamecontroller_axes_mapping ( int gamecontrollerPos, int axesId)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->find_gamecontroller_axes_mapping( axesId);
    }
    return -1;
};


int GPE_InputManager::find_gamecontroller_button_mapping ( int gamecontrollerPos, int buttonId)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->find_gamecontroller_button_mapping( buttonId);
    }
    return -1;
};

bool GPE_InputManager::gamecontroller_map_axes (int gamecontrollerPos, int axesId, int newAxesId)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->gamecontroller_map_axes(  axesId, newAxesId);
    }
    else if( gamecontrollerPos == gc_anycontroller )
    {
        bool successfulMapping = false;
        for( int iConPos = gc_max_devices -1; iConPos >=0; iConPos-- )
        {
            if( game_controllers[iConPos]->gamecontroller_map_axes(  axesId, newAxesId) )
            {
                successfulMapping = true;
            }
        }
        return successfulMapping;
    }
    return false;
};


bool GPE_InputManager::gamecontroller_map_button (int gamecontrollerPos, int buttonId, int newButtonId)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->gamecontroller_map_button(  buttonId, newButtonId);
    }
    else if( gamecontrollerPos == gc_anycontroller )
    {
        bool successfulMapping = false;
        for( int iConPos = gc_max_devices -1; iConPos >=0; iConPos-- )
        {
            if( game_controllers[iConPos]->gamecontroller_map_button(  buttonId, newButtonId) )
            {
                successfulMapping = true;
            }
        }
        return successfulMapping;
    }
    return false;
};

double GPE_InputManager::gamecontroller_get_axis_value( int gamecontrollerPos, int axisNumber)
{
    if( gamecontrollerPos >=0 && gamecontrollerPos < gc_max_devices )
    {
        return game_controllers[gamecontrollerPos]->get_axis_value(  axisNumber );
    }
    return 0;
}

bool GPE_InputManager::check_mouse_down(int buttonId)
{
    if(buttonId>=0 && buttonId<mouse_button_count )
    {
        return downMouseButton[buttonId];
    }
    else
    {
        bool resultFound = false;
        for(int i=0; i < mouse_button_count; i++)
        {
            if(downMouseButton[i] )
            {
                resultFound = true;
                return resultFound;
            }
        }
        return resultFound;
    }
    return false;
}

bool GPE_InputManager::check_mouse_pressed(int buttonId)
{
    if(buttonId>=0 && buttonId<mouse_button_count )
    {
        return pressedMouseButton[buttonId];
    }
    else
    {
        bool resultFound = false;
        for(int i=0; i < mouse_button_count; i++)
        {
            if(pressedMouseButton[i] )
            {
                resultFound = true;
                return resultFound;
            }
        }
        return resultFound;
    }
    return false;
}

bool GPE_InputManager::check_mouse_released(int buttonId)
{
    if( buttonId>=0 && buttonId< mouse_button_count )
    {
        return releasedMouseButton[buttonId];
    }
    else
    {
        for(int i=0; i < mouse_button_count; i++)
        {
            if( releasedMouseButton[i] ==true )
            {
                return true;
            }
        }
    }
    return false;
}

int GPE_InputManager::move_cursor(int cursor, int maxValue, int rowMax)
{
    if ( pressed[kb_up] && !lock[kb_up])
    {
        if(cursor>=rowMax)
            cursor-=rowMax;
    }
    if ( pressed[kb_down] && !lock[kb_down])
    {
        if(cursor<maxValue-rowMax )
            cursor+=rowMax;
    }
    if ( pressed[kb_right] && !lock[kb_right])
    {
        if(cursor<maxValue)
            cursor+=1;
    }
    if ( pressed[kb_left] && !lock[kb_left])
    {
        if(cursor>0)
            cursor-=1;
    }
    if(cursor<0)
    {
        cursor=0;
    }
    if(cursor>=maxValue)
    {
        cursor=maxValue-1;
    }
    return cursor;
}
