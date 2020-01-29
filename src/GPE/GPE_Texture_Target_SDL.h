/*
GPE_Texture_Target_SDL.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef GPE_TEXTURE_TARGET_SDL_H
#define GPE_TEXTURE_TARGET_SDL_H


#include "GPE_Renderer_SDL.h"
#include "GPE_Texture_Target_Base.h"

class GPE_Texture_Target_SDL: public GPE_Texture_Target_Base
{
    SDL_Texture * sdlTargetTexture;
public:
    GPE_Texture_Target_SDL();
    ~GPE_Texture_Target_SDL();
    void change_alpha( Uint8 alpha );
    void change_color(GPE_Color * newColor);
    void change_color( Uint8 red, Uint8 green, Uint8 blue );

    GPE_Renderer_SDL * get_gpe_renderer_sdl(GPE_Renderer_Base * renderer);
    SDL_Renderer * get_sdl_renderer(GPE_Renderer_Base * renderer);
    SDL_Texture * get_sdl_texture();

    GPE_Texture_Base * produce_clone();
    void render_overlay(  GPE_Renderer_Base * renderer,int x, int y,GPE_Rect* clip = NULL, int alpha = 255  );
    void render_overlay_scaled(  GPE_Renderer_Base * renderer,int x, int y,float xScale, float yScale, GPE_Rect* clip = NULL, GPE_Color * rendColor = NULL, int alpha = 255 );

    void resize_target( GPE_Renderer_Base * renderer,int w, int h, int id = -1,bool useLinearScaling = true  );
    void set_blend_mode( int newBlendMode);
};

#endif //GPE_Texture_Target_SDL::
