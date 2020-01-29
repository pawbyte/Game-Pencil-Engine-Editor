/*
SDL_SurfaceEx.h
This file is part of:
SDL_SurfaceEx
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

-SDL_SurfaceEx <https://www.pawbyte.com/opensource>


*/

#ifndef SDL_SurfaceEx_H
#define SDL_SurfaceEx_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

namespace SDL_SurfaceEx
{
    //Surface Functions
    SDL_Texture * create_texture_from_surface( SDL_Renderer * sdlRenderer, SDL_Surface * surf, int format = SDL_PIXELFORMAT_RGBA8888, SDL_Color * sdlColorKey = NULL, bool destroySurface = false );
    SDL_Surface * create_filled_surface(int w, int h, SDL_Color color, Uint8 alpha = 255);
    SDL_Surface * load_surface_image( const char * filename );

    Uint32 get_pixel32( SDL_Surface *surface, int x, int y );

    Uint8 merge_channel(Uint8 a, Uint8 b, float amount);

    void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );

    void surface_render_horizontal_line_color( SDL_Surface * surface, int y, int x1, int x2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    void surface_render_vertical_line_color( SDL_Surface * surface, int x, int y1, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    bool surface_render_circle_color( SDL_Surface * surface, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255 );

    SDL_Surface * surface_grayscale( SDL_Surface *surface);
    SDL_Surface * surface_invert( SDL_Surface *surface);
    SDL_Surface * surface_merge_color( SDL_Surface *surface, SDL_Color colorMerge, float amount );
    SDL_Surface * surface_recolor( SDL_Surface *surface, SDL_Color colorMerge, float amount );
    SDL_Surface * surface_remove_color( SDL_Surface *surface, SDL_Color colorToRemove );
    SDL_Surface * surface_flip( SDL_Surface *surface, int flags );
};

#endif //SDL_SurfaceEx_H
