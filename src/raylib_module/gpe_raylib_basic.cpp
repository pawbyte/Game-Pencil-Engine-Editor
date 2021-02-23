/*
gpe_raylib_basic.cpp
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

#include "gpe_raylib_basic.h"

namespace gpe
{
    bool using_raylib_system_underneath;

    //initialize raylib
    bool init_raylib_main_system()
    {
        //Initialize all raylib subsystems
        error_log->report("--Starting raylib_module...");

        error_log->report("--raylib_module systems started...");

        //raylib is so simple it appears we don't need to do anything here just yet.
        using_raylib_system_underneath = true;
        SetConfigFlags(FLAG_MSAA_4X_HINT);
        return true;
    }

    bool raylib_is_initted()
    {
        return using_raylib_system_underneath;
    }

    void quit_raylib_main_system()
    {
        CloseWindow();  //Should be done from window class, but if somehow forgotten, we do it again since its over :-)
        using_raylib_system_underneath = false;
    }
}
