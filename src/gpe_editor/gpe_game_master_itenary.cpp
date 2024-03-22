/*
gpe_game_master_itenary.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

 ___                ___
|   \ _ __      | _ )    | |_ __
| |_) / ` \ \ /\ / /   \| | | | _/  \
|  _/ (_| |\ V  V /| |_) | |_| | ||  _/
|_|   \__,_| \_/\_/ |____/ \__, |\__\___|
                           |___/
Created By PawByte
Attribution required at start of program and in credits.
SDL 2.0.9 used for this version...
*/

#include "gpe_game_master_itenary.h"

bool init_gpe_master_itenary( int argc, char* args[] )
{
    return init_gpe_editor(argc, args);
}

bool quit_gpe_master_itenary()
{
    gpe::error_log->report("Deleting Editor....");
    quit_gpe_editor();

    gpe::error_log->report("Deleting Gui...");
    pawgui::quit_gui();
    return true;
}
