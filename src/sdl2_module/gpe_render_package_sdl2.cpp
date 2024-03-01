/*
gpe_renderer_package_sdl2.cpp
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

-Game Pencil Engine  - SDL2 Module <https://gamepencil.net/sdl2-module/>


*/

#include "gpe_render_package_sdl2.h"

namespace gpe
{
    bool init_sdl2_render_package()
    {
        error_log->report("-Starting SDL2_Render_Package...");

        error_log->report("-Deleting renderer_main...");

        if( renderer_main!=NULL )
        {
            delete renderer_main;
            renderer_main = NULL;
        }

        error_log->report("-Deleting gcanvas...");
        if( gcanvas != NULL )
        {
            delete gcanvas;
            gcanvas = NULL;
        }

        error_log->report("-Creating renderer_main_sdl2...");

        renderer_main_sdl2 = new renderer_system_sdl2( window_controller_main_sdl2->get_window_id(), settings->defaultWindowWidth, settings->defaultWindowHeight, window_controller_main_sdl2->get_sdl2_window() );
        renderer_main = renderer_main_sdl2;

        error_log->report("-Creating rph...");

        if( rph == NULL )
        {
            rph = new render_package_handler();

        }

        error_log->report("-Adding sdl to  rph...");

        render_package * defaultr_package = rph->add_render_package( "sdl" );
        if( gpe::rsm != NULL )
        {
            delete gpe::rsm;
            gpe::rsm = NULL;
            gpe::rsm = new gpe::asset_manager( defaultr_package, "gcm-rsm" );
        }

        error_log->report("-Creating SDL2 based window...");
        window_controller_main_sdl2->set_renderer( renderer_main_sdl2, false );

        error_log->report("-Creating renderer_main_sdl2...");

        defaultr_package->packageRenderer = renderer_main_sdl2;

        error_log->report("-Creating texture_sdl2...");

        defaultr_package->packageTexture = new texture_sdl2();

        error_log->report("-Creating animation2d_sdl2...");

        defaultr_package->packageAnimation2d = new animation2d_sdl2();

        error_log->report("-Changing default window to window_controller_main_sdl2...");


        defaultr_package->packageWindow = window_controller_main_sdl2;
        error_log->report("-Changing rph->defaultr_packageName...");

        rph->defaultr_packageName = defaultr_package->get_package_name();

        error_log->report("-Starting GPE_SDL Artist...");
        gcanvas = new artist_sdl2( renderer_main_sdl2 );
        return true;
    }

    void quit_sdl2_render_package()
    {
        error_log->report("Quitting SDL_IMG....");
        IMG_Quit();
    }
}
