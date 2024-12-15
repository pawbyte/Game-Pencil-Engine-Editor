/*
sdl2_surface_ex.h
This file is part of:
SDL_SurfaceEx
https://www.pawbyte.com/sdl2_surface_ex
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
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

-sdl2_surface_ex <https://www.pawbyte.com/sdl2_surface_ex>


*/

#ifndef sdl_surface_ex_h
#define sdl_surface_ex_h

#include "SDL.h"
#include "SDL_image.h"
#include <vector>

namespace sdl2_surface_ex
{
    const int version_major = 1;
    const int version_minor = 5;
    const int version_patch_level = 0;

    enum
    {
        grayscale_all_shades = 0,
        grayscale_dark_shades_only = 1,
        grayscale_light_shades_only = 2,
        grayscale_medium_shades_only = 3,
        grayscale_medium_and_light_only = 4,
        grayscale_medium_and_dark_shades_only = 5,
        grayscale_light_and_dark_shades_only = 6,
        grayscale_no_shades = 7,
    };

    //Surface Functions
    SDL_Texture * create_texture_from_surface( SDL_Renderer * sdlRenderer, SDL_Surface * surf,  int format = SDL_PIXELFORMAT_RGBA8888 );
    SDL_Surface * create_clear_surface(int w, int h, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, Uint8 alpha = 255);
    SDL_Surface * create_filled_surface_rgba(int w, int h, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, Uint8 alpha = 255);
    SDL_Surface * load_surface_image( const char * filename );

    Uint32 get_pixel32( SDL_Surface *surface, int x, int y );
    uint32_t get_pixel32_t( SDL_Surface *surface, int x, int y );

    Uint8 merge_channel(Uint8 a, Uint8 b, float amount);
    uint8_t merge_channel_t(uint8_t a, uint8_t b, float amount);

    void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );
    void put_pixel32_t( SDL_Surface *surface, int x, int y, uint32_t pixel );


    void surface_render_horizontal_line_color_rgba( SDL_Surface * surface, int y, int x1, int x2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    void surface_render_vertical_line_color_rgba( SDL_Surface * surface, int x, int y1, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    bool surface_render_circle_color_rgba( SDL_Surface * surface, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255 );

    bool rgb_to_hsv(float r, float g, float b, float *h, float *s, float *v );

    SDL_Surface * surface_flip( SDL_Surface *surface, int flags );

    SDL_Surface * surface_grayscale( SDL_Surface *surface);
    SDL_Surface * surface_invert( SDL_Surface *surface);
    SDL_Surface * surface_merge_color_rgba( SDL_Surface *surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, float amount );

    SDL_Surface * surface_modify_2bit( SDL_Surface * surface, SDL_Color c1, SDL_Color c2,  SDL_Color c3, SDL_Color c4  );
    SDL_Surface * surface_modify_mono_chrome( SDL_Surface * surface, SDL_Color c1, SDL_Color c2  );

    SDL_Surface * surface_modify_only_color( SDL_Surface * surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, float difference_allowed = 0.25, int grayscale_technique = grayscale_all_shades  );
    SDL_Surface * surface_modify_remove_background( SDL_Surface * surface, float difference_allowed = 0.25, int grayscale_technique = grayscale_all_shades  );
    SDL_Surface * surface_modify_remove_color( SDL_Surface * surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, float difference_allowed = 0.25, int grayscale_technique = grayscale_all_shades  );
    SDL_Surface * surface_modify_selective_color( SDL_Surface * surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, float difference_allowed = 0.25, int grayscale_technique = grayscale_all_shades  );
    SDL_Surface * surface_modify_selective_color_list( SDL_Surface * surface, std::vector <SDL_Color > &colorList, float difference_allowed = 0.25, int grayscale_technique = grayscale_all_shades , int primary_color = -1, float primary_color_different_allowed
                                                       = 0.35, bool sv_matters = false );

    SDL_Surface * surface_recolor_rgba( SDL_Surface *surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, float amount );
    SDL_Surface * surface_remove_color_rgba( SDL_Surface *surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b );
    SDL_Surface * surface_sephia( SDL_Surface *surface );

    bool hue_within_threshold( float value, float base, float difference );
    bool within_threshold( float value, float base, float difference );

};

#endif //sdl_surface_ex_h
