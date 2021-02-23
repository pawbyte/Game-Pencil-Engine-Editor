/*
gpe_renderer_package_raylib.cpp
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

#include "gpe_render_package_raylib.h"

namespace gpe
{
    bool init_raylib_render_package()
    {
        if( renderer_main!=NULL )
        {
            delete renderer_main;
            renderer_main = NULL;
        }
        if( gcanvas != NULL )
        {
            delete gcanvas;
            gcanvas = NULL;
        }
        renderer_main_raylib = new renderer_system_raylib( window_controller_main_raylib->get_window_id(), settings->defaultWindowWidth, settings->defaultWindowHeight );
        renderer_main = renderer_main_raylib;

        if( rph == NULL )
        {
            rph = new render_package_handler();

        }

        render_package * defaultr_package = rph->add_render_package( "raylib" );
        if( gpe::rsm != NULL )
        {
            delete gpe::rsm;
            gpe::rsm = NULL;
            gpe::rsm = new gpe::asset_manager( defaultr_package, "gcm-rsm-raylib" );
        }

        window_controller_main_raylib->set_renderer( renderer_main_raylib, false );

        defaultr_package->packageRenderer = renderer_main_raylib;
        defaultr_package->packageTexture = new texture_raylib();
        defaultr_package->packageWindow = window_controller_main_raylib;
        rph->defaultr_packageName = defaultr_package->get_package_name();

        error_log->report("-Starting gpe_raylib artist...");
        gcanvas = new artist_raylib( renderer_main_raylib );
        return true;
    }

    void quit_raylib_render_package()
    {
        error_log->report("Quitting gpe_raylib render package....");
    }
}
