/*
sdl_surface_ex.h
This file is part of:
sdl_surface_ex
https://www.pawbyte.com/opensource
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
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

-sdl_surface_ex <https://www.pawbyte.com/opensource>


*/

#ifndef sdl_surface_ex_h
#define sdl_surface_ex_h

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

namespace sdl_surface_ex
{
    //Surface Functions
    SDL_Texture * create_texture_from_surface( SDL_Renderer * sdlRenderer, SDL_Surface * surf,  int format = SDL_PIXELFORMAT_RGBA8888, bool destroySurface = true );
    SDL_Surface * create_filled_surface_rgba(int w, int h, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, Uint8 alpha = 255);
    SDL_Surface * load_surface_image( const char * filename );

    Uint32 get_pixel32( SDL_Surface *surface, int x, int y );

    Uint8 merge_channel(Uint8 a, Uint8 b, float amount);

    void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );

    void surface_render_horizontal_line_color_rgba( SDL_Surface * surface, int y, int x1, int x2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    void surface_render_vertical_line_color_rgba( SDL_Surface * surface, int x, int y1, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    bool surface_render_circle_color_rgba( SDL_Surface * surface, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255 );

    SDL_Surface * surface_grayscale( SDL_Surface *surface);
    SDL_Surface * surface_invert( SDL_Surface *surface);
    SDL_Surface * surface_merge_color_rgba( SDL_Surface *surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, float amount );
    SDL_Surface * surface_recolor_rgbar( SDL_Surface *surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, float amount );
    SDL_Surface * surface_remove_color_rgba( SDL_Surface *surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b );
    SDL_Surface * surface_flip( SDL_Surface *surface, int flags );
};

#endif //sdl_surface_ex_h
