/*
GPE_State_Manager.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

#include "GPE_State_Manager.h"

//Program state object
ProgramState *currentState = NULL;


void change_state()
{
    //If the state needs to be changed
    if( nextProgramState != STATE_NULL )
    {
        //Delete the current state
        if( nextProgramState != STATE_EXIT )
        {
            delete currentState;
        }
         //Change the state
        switch( nextProgramState )
        {
            case STATE_LOGO_DISPLAY:
            break;
            default:
         //       nextProgramState=STATE_INTRO;
         //       currentState = new GPE_Editor_State();
            break;
        }

        //Change the current state ID
        programStateId = nextProgramState;

        //NULL the next state ID
        nextProgramState = STATE_NULL;
    }
}
