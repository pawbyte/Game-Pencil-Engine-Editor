/*
gpe_renderer_package_sdl.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_render_package_sdl.h"

namespace gpe
{
    bool init_sdl_render_package()
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
        renderer_main_sdl = new renderer_system_sdl( window_controller_main_sdl->get_window_id(), settings->defaultWindowWidth, settings->defaultWindowHeight, window_controller_main_sdl->get_sdl_window() );
        renderer_main = renderer_main_sdl;

        if( rph == NULL )
        {
            rph = new render_package_handler();

        }

        render_package * defaultRenderPackage = rph->add_render_package( "sdl" );
        if( gpe::rsm != NULL )
        {
            delete gpe::rsm;
            gpe::rsm = NULL;
            gpe::rsm = new gpe::asset_manager( defaultRenderPackage, "gcm-rsm" );
        }

        window_controller_main_sdl->set_renderer( renderer_main_sdl, false );

        defaultRenderPackage->packageRenderer = renderer_main_sdl;
        defaultRenderPackage->packageTexture = new texture_sdl();
        defaultRenderPackage->packageWindow = window_controller_main_sdl;
        rph->defaultRenderPackageName = defaultRenderPackage->get_package_name();

        error_log->report("-Starting GPE_SDL Artist...");
        gcanvas = new artist_sdl( renderer_main_sdl );
        return true;
    }

    void quit_sdl_render_package()
    {
        error_log->report("Quitting SDL_IMG....");
        IMG_Quit();
    }
}
