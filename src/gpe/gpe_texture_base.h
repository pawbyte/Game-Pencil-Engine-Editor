/*
texture_base.h
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#ifndef gpe_texture_base_h
#define gpe_texture_base_h

#include <string>
#include "gpe_color.h"
#include "gpe_renderer_base.h"
#include "gpe_shapes.h"

namespace gpe
{
    class texture_base
    {
        protected:
            std::string file_location;
            bool is_transparent;
            int tex_width, tex_height;
            std::string name;
            std::string texture_type;
            int tex_id;
            int blend_mode_current;
            int current_color_r, current_color_g, current_color_b;
            bool pixel_values_are_stored;
        public:
            int lastAlphaRendered;
            bool render_texture_effect;
            texture_base( bool store_pixels  = false ); //defaults to false
            virtual ~texture_base();
            virtual void change_alpha( uint8_t alpha );
            virtual void change_color( color * color_new);
            virtual void change_color( uint8_t red, uint8_t green, uint8_t blue );
            virtual bool copy_image_source( const std::string& directory_output_name );
            virtual texture_base * create_new();

            std::string get_filename();
            int get_height();
            int get_id();
            std::string get_name();
            virtual uint32_t get_pixel_32bit(int x, int y );//done by child classes//done by child classes & requires pixels to be stored
            virtual void get_pixel_values(int x, int y, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a );//done by child classes & requires pixels to be stored
            std::string get_texture_type();
            int get_width();

            //Loader functions
            virtual void load_new_texture( const std::string& file_name, int id = -1, bool transparent = true, bool useLinearScaling = false , uint8_t colorkeyR = 255, uint8_t colorkeyG = 0, uint8_t colorkeyB = 255 );

            //Texture modification functions [ Implemented by child classes] [ BEGIN]
            virtual bool modify_texture_grayscale( );
            virtual bool modify_texture_invert( );
            virtual bool modify_texture_merge_color_rgba( color * color_key, float amount );
            virtual bool modify_texture_recolor_rgba(  color * color_key, float amount );
            virtual bool modify_texture_remove_color_rgba( color * color_key );
            virtual bool modify_texture_selective_color( color * selected_color, float difference_allowed = 0.25  );
            virtual bool modify_texture_selective_color_duo( color * selected_color1, color * selected_color2, float difference_allowed = 0.25  );
            virtual bool modify_texture_selective_color_trio( color * selected_color1, color * selected_color2, color * selected_color3, float difference_allowed = 0.25 );
            virtual bool modify_texture_flip( int flags );
            //Texture modification functions [ Implemented by child classes] [ END ]

            //pre-render functions...
            virtual void prerender_circle( renderer_base * renderer, int rad, color * circleColor,  uint8_t alpha = 255,int id = -1, bool transparent = true, bool useLinearScaling = true, bool isOutline = false  );
            virtual void prerender_triangle( renderer_base * renderer, shape_triangle2d,  color * circleColor,  uint8_t alpha = 255 );
            virtual void prerender_rectangle( renderer_base * renderer, int w, int h, color * color_new, int id = -1, bool transparent = true, bool useLinearScaling = true , bool isOutline = false);

            //
            virtual void render_align(  int x, int y, int alignment_h, int alignment_v, shape_rect* clip = nullptr, color * render_color = nullptr, int alpha = 255  );
            virtual void render_align_resized(  int x, int y,int new_width, int new_height, int alignment_h, int alignment_v, shape_rect* clip = nullptr, color * render_color = nullptr, int alpha = 255 );

            virtual void render_tex(  int x, int y, shape_rect* clip = nullptr, int alpha = 255  );
            virtual void render_tex_colored(  int x, int y, color * render_color, int alpha = 255, shape_rect* clip = nullptr  );
            virtual bool render_tex_polygon( int x, int y, int shape_length , int point_count, color * render_color, int alpha = 255, float start_angle = 0 );
            virtual bool render_tex_polygon_clipped( int x, int y, int shape_length , int point_count, color * render_color,shape_rect* clip = nullptr , int alpha = 255, float start_angle = 0 );
            virtual bool render_tex_quad(   shape_point2d p1 , shape_point2d p2, shape_point2d p3, shape_point2d p4, color * render_color, int alpha = 255 );
            virtual bool render_tex_quad_clipped(   shape_point2d p1 , shape_point2d p2, shape_point2d p3, shape_point2d p4, color * render_color, int alpha, shape_rect * clip );
            virtual void render_tex_resized(  int x, int y,float new_width, float new_height, shape_rect* clip = nullptr, color * render_color = nullptr, int alpha = 255 );
            virtual void render_tex_scaled(  int x, int y,float x_scale, float y_scale, shape_rect* clip = nullptr, color * render_color = nullptr, int alpha = 255 );
            virtual void render_tex_rotated(  int x, int y, float render_angle,  color * render_color = nullptr, shape_rect* clip = nullptr , int alpha = 255 );
            virtual void render_tex_rotated_at_point(  int x, int y, float render_angle, int point_x, int point_y, color * render_color = nullptr, shape_rect* clip = nullptr , int alpha = 255 );
            virtual void render_tex_special(  int x, int y, float render_angle, int new_width = -1, int new_height = -1, color * render_color = nullptr, shape_rect* clip = nullptr , int alpha = 255 );
            virtual void render_tex_special_at_point(  int x, int y, float render_angle, int point_x, int point_y,int new_width = -1, int new_height = -1, color * render_color = nullptr, shape_rect* clip = nullptr , int alpha = 255 );


            virtual bool reset_effect_texture();

            virtual void set_alpha( int alpha);
            virtual void set_blend_mode( int blend_mode_new);

            virtual int save_as_bmp( const std::string& file_location , bool save_effect  = false );
            virtual int save_as_png( const std::string& file_location , bool save_effect  = false );

            virtual void set_pixel_32bit( int x, int y, uint32_t pixel ); //requires pixels to be stored
            virtual void set_pixel_values( int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a ); //requires pixels to be stored
            bool storing_pixel_values();
            virtual bool update_pixels(); //requires pixels to be stored
    };
}
#endif //gpe_texture_base_h
