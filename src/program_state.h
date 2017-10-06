/*
program_state.h
This file is part of:
GAME PENCI ENGINE
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

#ifndef PROGRAM_STATE_H
#define PROGRAM_STATE_H

#include "GPE_CIncludes.h"

//State variables
extern int programStateId;
extern int nextProgramState;

//Program states
typedef enum
{
    STATE_NULL,
    STATE_LOGO_DISPLAY,
    STATE_INTRO,
    STATE_TITLE,
    STATE_MAINMENU,
    STATE_GAME_LOADING,
    STATE_GAME_PLAY,
    STATE_CREDITS,
    STATE_CUTSCENE,
    STATE_FLASHBACK,
    STATE_OPTIONS,
    STATE_EXIT
}programStates;


//State status manager
void set_next_state( int newState );

//Program State base class

class ProgramState
{
    public:
        int randomVariable;
        //ProgramState();
        virtual void process_input() = 0;
        virtual void apply_logic() = 0;
        virtual void render() = 0;
        virtual void clean_up() = 0;
        virtual ~ProgramState(){};
    protected:
        ProgramState(){};
};

#endif
