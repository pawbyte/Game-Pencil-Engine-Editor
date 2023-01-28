/*
gpe_texture_target_sdl.h
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

#ifndef gpe_texture_target_sdl_h
#define gpe_texture_target_sdl_h


#include "gpe_renderer_sdl.h"
#include "../gpe/gpe_texture_target_base.h"

namespace gpe
{
    class texture_target_sdl: public texture_target_base
    {
        protected:
            SDL_Texture * sdlTargetTexture;
        public:
            texture_target_sdl();
            ~texture_target_sdl();
            void change_alpha( Uint8 alpha );
            void change_color( color * color_new);
            void change_color( Uint8 red, Uint8 green, Uint8 blue );

            renderer_system_sdl * get_gpe_renderer_sdl(renderer_base * renderer);
            SDL_Renderer * get_sdl_renderer(renderer_base * renderer);
            SDL_Texture * get_sdl_texture();

            texture_base * create_new();
            void render_overlay(  renderer_base * renderer,int x, int y, gpe::shape_rect* clip = NULL, int alpha = 255  );
            void render_overlay_scaled(  renderer_base * renderer,int x, int y,float x_scale, float y_scale, gpe::shape_rect* clip = NULL, color * render_color = NULL, int alpha = 255 );

            void resize_target( renderer_base * renderer,int w, int h, int id = -1,bool useLinearScaling = true  );
            void set_blend_mode( int blend_mode_new);
    };
}
#endif //gpe_texture_target_sdl_h
