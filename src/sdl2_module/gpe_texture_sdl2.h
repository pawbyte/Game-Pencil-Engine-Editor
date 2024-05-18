/*
GPE_Texture.h
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

#ifndef gpe_texture_sdl2
#define gpe_texture_sdl2

#include "../gpe/gpe_branch.h"
#include "../gpe/gpe_color_manager.h"
#include "../gpe/gpe_common_includes.h"
#include "../gpe/gpe_color.h"
#include "../gpe/gpe_constants.h"
#include "../gpe/gpe_error_logger.h"
#include "../gpe/gpe_globals.h"
#include "../gpe/gpe_texture_base.h"
#include "../gpe/internal_libs/stg_ex.h"
#include "gpe_renderer_sdl2.h"
#include "thirdparty/sdl2_surface_ex.h"

namespace gpe
{
    class texture_sdl2: public texture_base
    {
        protected:
            SDL_Texture * texImg;
            //SDL_Texture * texImg_effect;
            //SDL_Surface * sdlTexSurface;
            //SDL_Surface * sdlTexSurface_effect;
            bool is_prerendered;
        public:
            texture_sdl2(bool store_pixels  = false ); //defaults to false
            ~texture_sdl2();
            void change_alpha( Uint8 alpha );
            void change_color( color * color_new);
            void change_color( Uint8 red, Uint8 green, Uint8 blue );
            void change_texture(SDL_Texture * newTexture);
            bool copy_image_source(std::string directory_output_name);
            texture_base * create_new();

            virtual uint32_t get_pixel_32bit(int x, int y );
            virtual void get_pixel_values(int x, int y, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a );

            renderer_system_sdl2 * get_gpe_renderer_sdl2(renderer_base * renderer);
            SDL_Renderer * get_sdl2_renderer(renderer_base * renderer);
            SDL_Texture * get_sdl2_texture();

            void load_new_texture( const std::string& file_name, int id = -1, bool transparent = true, bool useLinearScaling = false, uint8_t colorkeyR = 255, uint8_t colorkeyG = 0, uint8_t colorkeyB = 255 );

            virtual bool modify_texture_grayscale( );
            virtual bool modify_texture_invert( );
            virtual bool modify_texture_merge_color_rgba( color * color_key, float amount );
            virtual bool modify_texture_recolor_rgba(  color * color_key, float amount );
            virtual bool modify_texture_remove_color_rgba( color * color_key );
            virtual bool modify_texture_selective_color( color * selected_color, float difference_allowed = 0.25  );
            virtual bool modify_texture_selective_color_duo( color * selected_color1, color * selected_color2, float difference_allowed = 0.25  );
            virtual bool modify_texture_selective_color_trio( color * selected_color1, color * selected_color2, color * selected_color3, float difference_allowed = 0.25 );
            virtual bool modify_texture_flip( int flags );

            void prerender_circle( renderer_base * renderer, int rad, color * circleColor,  Uint8 alpha = 255,int id = -1, bool transparent = true, bool useLinearScaling = true , bool isOutline = false );
            void prerender_triangle( renderer_base * renderer, shape_triangle2d,  color * circleColor,  uint8_t alpha = 255 );
            void prerender_rectangle( renderer_base * renderer, int w, int h, color * color_new, int id = -1, bool transparent = true, bool useLinearScaling = true  , bool isOutline = false);

            void render_align(  int x, int y, int alignment_h, int alignment_v, gpe::shape_rect* clip = NULL, color * render_color = NULL, int alpha = 255  );
            void render_align_resized(  int x, int y,int new_width, int new_height, int alignment_h, int alignment_v, gpe::shape_rect* clip = NULL, color * render_color = NULL, int alpha = 255 );
            void render_tex(  int x, int y, gpe::shape_rect* clip = NULL, int alpha = 255  );
            void render_tex_colored(  int x, int y, color * render_color, int alpha = 255, gpe::shape_rect* clip = NULL  );


            bool render_tex_quad(   shape_point2d p1 , shape_point2d p2, shape_point2d p3, shape_point2d p4, color * render_color, int alpha = 255 );
            bool render_tex_quad_clipped(   shape_point2d p1 , shape_point2d p2, shape_point2d p3, shape_point2d p4, color * render_color, int alpha, shape_rect * clip );

            void render_tex_resized(  int x, int y,float new_width, float new_height, gpe::shape_rect* clip = NULL, color * render_color = NULL, int alpha = 255 );
            void render_tex_scaled(  int x, int y,float x_scale, float y_scale, gpe::shape_rect* clip = NULL, color * render_color = NULL, int alpha = 255 );
            void render_tex_rotated(  int x, int y, float render_angle,  color * render_color = NULL, gpe::shape_rect* clip = NULL , int alpha = 255 );
            void render_tex_rotated_at_point(  int x, int y, float render_angle, int point_x, int point_y, color * render_color = NULL, gpe::shape_rect* clip = NULL , int alpha = 255 );
            void render_tex_special(  int x, int y, float render_angle, int new_width = -1, int new_height = -1, color * render_color = NULL, gpe::shape_rect* clip = NULL , int alpha = 255 );
            void render_tex_special_at_point(  int x, int y, float render_angle, int point_x, int point_y,int new_width = -1, int new_height = -1, color * render_color = NULL, gpe::shape_rect* clip = NULL , int alpha = 255 );

            virtual bool reset_effect_texture();
            virtual int save_as_bmp( const std::string& file_location, bool save_effect = false );
            virtual int save_as_png( const std::string& file_location, bool save_effect = false );

            void set_alpha( int alpha);
            void set_blend_mode( int blend_mode_new);
            virtual void set_pixel_32bit( int x, int y, uint32_t pixel ); //requires pixels to be stored
            virtual void set_pixel_values( int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a ); //requires pixels to be stored
            virtual bool update_pixels(); //requires pixels to be stored

    };
}

#endif //GPE_TEXTURE_sdl2_h
