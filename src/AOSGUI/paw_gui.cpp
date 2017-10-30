/*
ambitious_gui_library.cpp
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
#include "paw_gui.h"

bool PAW_GUI_START()
{
    MAIN_GUI_SETTINGS = new GPE_GUI_Settings();
    MAIN_OVERLAY = new GPE_Overlay_System();
    MAIN_SEARCH_CONTROLLER = new PAW_GUI_SEARCH_CONTROLLER();
    if( MAIN_GUI_SETTINGS!=NULL && MAIN_OVERLAY!=NULL && MAIN_SEARCH_CONTROLLER!=NULL )
    {
        return true;
    }
    return false;
}

bool PAW_GUI_END()
{
    if( MAIN_OVERLAY !=NULL)
    {
        delete MAIN_OVERLAY;
        MAIN_OVERLAY = NULL;
    }

    if( MAIN_SEARCH_CONTROLLER !=NULL)
    {
        delete MAIN_SEARCH_CONTROLLER;
        MAIN_SEARCH_CONTROLLER = NULL;
    }
    if( MAIN_GUI_SETTINGS !=NULL)
    {
        delete MAIN_GUI_SETTINGS;
        MAIN_GUI_SETTINGS = NULL;
    }
}
