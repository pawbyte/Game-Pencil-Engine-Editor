/*
GPE_Input.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

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

GPE_InputManager_GameController::GPE_InputManager_GameController()
{
    controllerType = NULL;
    controllerExtension = "";
    isConnected = false;
    mapping = 0;
    timestamp = 0;
    touch_mouse_x = 0;
    touch_mouse_y = 0;
    for(int but=0; but <button_count; but++)
    {
        prevHit[but] = false;
        butIsDown[but] = false;
        butIsPressed[but] = false;
        butIsReleased[but] = false;
        butIsLocked[but] = false;
        buttonsMap[but] = but;
    }

    for (int tempAxes=0; tempAxes <axes_count; tempAxes++)
    {
        axesMap[ tempAxes ] = tempAxes;
        axesValues[ tempAxes ] = 0;
    }

}

GPE_InputManager_GameController::~GPE_InputManager_GameController()
{

}

bool GPE_InputManager_GameController::check_connection()
{
	return isConnected;
}

int GPE_InputManager_GameController::check_button(int buttonId)
{
	if( isConnected )
	{
		if(  buttonId >=0 && buttonId < button_count)
		{
			return butIsDown[ buttonsMap[buttonId] ] > 0;
		}
		else if( buttonId == gc_anybutton )
		{
			for( int iAnyButton = button_count-1; iAnyButton >=0; iAnyButton-- )
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
			for(int iNoButton = button_count-1; iNoButton >=0; iNoButton-- )
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

int GPE_InputManager_GameController::check_button_pressed(int buttonId)
{
	if( isConnected )
	{
		if( buttonId >=0 && buttonId < button_count)
		{
			return butIsPressed[ buttonsMap[buttonId] ] > 0;
		}
		else if( buttonId == gc_anybutton )
		{
			for(int iAnyButton = button_count-1; iAnyButton >=0; iAnyButton-- )
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
			for( int iNoButton = button_count-1; iNoButton >=0; iNoButton-- )
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

int GPE_InputManager_GameController::check_button_released(int buttonId)
{
	if( isConnected )
	{
		if( buttonId >=0 && buttonId < button_count)
		{
			return butIsReleased[ buttonsMap[buttonId] ];
		}
		else if( buttonId == gc_anybutton )
		{
			for( int iAnyButton = button_count-1; iAnyButton >=0; iAnyButton-- )
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
			for( int iNoButton = button_count-1; iNoButton >=0; iNoButton-- )
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

int GPE_InputManager_GameController::gamepad_get_axes_count( )
{
	return system_axes_count;
}

int GPE_InputManager_GameController::find_gamepad_axes_mapping( int axesId )
{
	if(  axesId >=0 && axesId < axes_count )
	{
		return axesMap[axesId];
	}
	return -1;
}

int GPE_InputManager_GameController::find_gamepad_button_mapping( int buttonId )
{
	if(  buttonId >=0 && buttonId < button_count )
	{
		return axesMap[buttonId];
	}
	return -1;
}

bool GPE_InputManager_GameController::gamepad_map_axes(int axesId, int newAxesId )
{
	if(  axesId >=0 && axesId < axes_count)
	{
		if( newAxesId >=0 && newAxesId < axes_count )
		{
			buttonsMap[axesId] = newAxesId;
			return true;
		}
	}
	return false;
}

bool GPE_InputManager_GameController::gamepad_map_button(int buttonId, int newButtonId)
{
	if(  buttonId >=0 && buttonId < button_count)
	{
		if(  newButtonId >=0 && newButtonId < button_count)
		{
			buttonsMap[buttonId] = newButtonId;
			return true;
		}
	}
	return false;
}

std::string GPE_InputManager_GameController::get_name( )
{
    return deviceName;
}
//GameController Input related functions/objects/variables as of Version 1.12 [ END ]

void GPE_InputManager_GameController::pure_reset()
{
	for( int but=0; but<button_count; but++)
	{
		prevHit[ buttonsMap[but] ] = false;
		butIsDown[ buttonsMap[but] ] = false;
		butIsPressed[ buttonsMap[but] ] = false;
		butIsReleased[ buttonsMap[but] ] = false;
		butIsLocked[ buttonsMap[but] ] = false;
	}
	for (int tempAxes=0; tempAxes <axes_count; tempAxes++)
	{
		axesValues[ tempAxes ] = 0;
	}
}

void GPE_InputManager_GameController::reset_controller()
{
	for(int but=0; but<button_count; but++)
	{
		butIsPressed[ buttonsMap[but] ] = butIsDown[ buttonsMap[but] ] && !prevHit[ buttonsMap[but] ];
		butIsReleased[ buttonsMap[but] ] = !butIsDown[ buttonsMap[but] ] && prevHit[ buttonsMap[but] ];
	}
}

void GPE_InputManager_GameController::set_connected( bool isNowConnected )
{
    isConnected = isNowConnected;
}

void GPE_InputManager_GameController::set_name( std::string newName)
{
    deviceName = newName;
}

GPE_InputManager::GPE_InputManager()
{
    //Creates the game controllers
    for( int iController = 0; iController < max_controllers; iController++)
    {
         game_controllers[iController] = new GPE_InputManager_GameController();
    }
    programStartTicks = 0;
    time(&lastTimeScreenResized);
    forceRedrawNeeded = true;
    done = false;
    programJustStarted = true;
    backSpaceIsPressed = false;
    shiftKeyIsPressed = false;
    last_key = 0;
    scroll_up = false;
    scroll_down = false;

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
	detect_controllers();
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

void GPE_InputManager::handle(bool dump_event, bool isEmbedded)
{
    MAIN_RENDERER->reset_input();
    inputHappenedInFrame = false;
    keyInputReceivedInFrame = false;
    mouseInputReceivedInFrame = false;
    mouseMovementInputReceivedInFrame = false;
    mouseWheelInputReceivedInFrame= false;
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

    for (int key=0; key<key_count; key++)
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
    unsigned tries = 10; /* use UINT_MAX for no effective limit */

    if (SDL_WaitEventTimeout(&event, frame_wait))
    {
        do
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
                        for (int key=0; key<key_count; key++)
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
                break;

                case SDL_WINDOWEVENT:
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
        }while (SDL_PollEvent(&event));
    }

    SDL_GetMouseState(&mouse_x, &mouse_y);
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

bool GPE_InputManager::detect_controllers()
{
    int nJoysticks = SDL_NumJoysticks();
    int nGameControllers = 0;
    int currentControllerId = 0;
    SDL_GameController * foundSDLController = NULL;
    std::string foundControllerName = "";
    for (int i = 0; i < nJoysticks; i++)
    {
        if (SDL_IsGameController(i))
        {
            foundSDLController = SDL_GameControllerOpen(i);
            if( nGameControllers < max_controllers && foundSDLController!=NULL )
            {
                game_controllers[nGameControllers]->assignedSDLController = foundSDLController;
                foundControllerName = SDL_GameControllerName(foundSDLController);
                game_controllers[nGameControllers]->set_name( foundControllerName );
                record_error("Game Controller found:    ["+foundControllerName+"].");
            }
            nGameControllers++;
        }
    }
    if( nGameControllers > 0)
    {
        return true;
    }
    record_error("No Game Controllers found...");
    return false;
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
    windowEventHappendInFrame = false;
    WINDOW_WAS_JUST_RESIZED = false;

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
                    else if( released[i] == true )
                    {
                        return false;
                    }
                }
            }
            return true;
        }
        else
        {
            //record_error("Key "+int_to_string(buttonId)+"is not released...");
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

bool GPE_InputManager::gamepad_connected (int controllerPos)
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos]->check_connection();
    }
    return -1;
};

std::string GPE_InputManager::gamepad_detected_name(int controllerPos)
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos]->get_name();
    }
    return "";
};


std::string GPE_InputManager::gamepad_name (int controllerPos)
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos]->get_name();
    }
    return "";
};

GPE_InputManager_GameController * GPE_InputManager::gamepad_object (int controllerPos)
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos];
    }
    return NULL;
};

int GPE_InputManager::gamepad_get_axes_count ( int controllerPos )
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos]->gamepad_get_axes_count();;
    }
    return -1;
};

int GPE_InputManager::find_gamepad_mapping (int controllerPos, int buttonId)
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos]->find_gamepad_button_mapping( buttonId);
    }
    return -1;
};

int GPE_InputManager::check_gamepad (int controllerPos, int buttonId)
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos]->check_button(buttonId);
    }
    else if( controllerPos == gc_anycontroller )
    {
        for( int iConPos = max_controllers -1; iConPos >=0; iConPos-- )
        {
            if( game_controllers[iConPos]->check_button(buttonId) )
            {
                return true;
            }
        }
    }
    return false;
}

int GPE_InputManager::check_gamepad_down (int controllerPos, int buttonId)
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos]->check_button(buttonId);
    }
    else if( controllerPos == gc_anycontroller )
    {
        for( int iConPos = max_controllers -1; iConPos >=0; iConPos-- )
        {
            if( game_controllers[iConPos]->check_button(buttonId) )
            {
                return true;
            }
        }
    }
    return false;
};

int GPE_InputManager::check_gamepad_pressed (int controllerPos, int buttonId)
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos]->check_button_pressed(buttonId);
    }
    else if( controllerPos == gc_anycontroller )
    {
        for( int iConPos = max_controllers -1; iConPos >=0; iConPos-- )
        {
            if( game_controllers[iConPos]->check_button_pressed(buttonId) )
            {
                return true;
            }
        }
    }
    return false;
};

int GPE_InputManager::check_gamepad_released (int controllerPos, int buttonId)
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos]->check_button_released(buttonId);
    }
    else if( controllerPos == gc_anycontroller )
    {
        for( int iConPos = max_controllers -1; iConPos >=0; iConPos-- )
        {
            if( game_controllers[iConPos]->check_button_released(buttonId) )
            {
                return true;
            }
        }
    }
    return false;
};

int GPE_InputManager::find_gamepad_axes_mapping ( int controllerPos, int axesId)
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos]->find_gamepad_axes_mapping( axesId);
    }
    return NULL;
};


int GPE_InputManager::find_gamepad_button_mapping ( int controllerPos, int buttonId)
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos]->find_gamepad_button_mapping( buttonId);
    }
    return -1;
};

bool GPE_InputManager::gamepad_map_axes (int controllerPos, int axesId, int newAxesId)
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos]->gamepad_map_axes(  axesId, newAxesId);
    }
    else if( controllerPos == gc_anycontroller )
    {
        bool successfulMapping = false;
        for( int iConPos = max_controllers -1; iConPos >=0; iConPos-- )
        {
            if( game_controllers[iConPos]->gamepad_map_axes(  axesId, newAxesId) )
            {
                successfulMapping = true;;
            }
        }
        return successfulMapping;
    }
    return false;
};


bool GPE_InputManager::gamepad_map_button (int controllerPos, int buttonId, int newButtonId)
{
    if( controllerPos >=0 && controllerPos < max_controllers )
    {
        return game_controllers[controllerPos]->gamepad_map_button(  buttonId, newButtonId);
    }
    else if( controllerPos == gc_anycontroller )
    {
        bool successfulMapping = false;
        for( int iConPos = max_controllers -1; iConPos >=0; iConPos-- )
        {
            if( game_controllers[iConPos]->gamepad_map_button(  buttonId, newButtonId) )
            {
                successfulMapping = true;;
            }
        }
        return successfulMapping;
    }
    return false;
};

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
