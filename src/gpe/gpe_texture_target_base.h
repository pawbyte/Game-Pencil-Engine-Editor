/*
texture_target_base.h
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

#ifndef gpe_texture_target_base_h
#define gpe_texture_target_base_h

#include "gpe_texture_base.h"

namespace gpe
{
    class texture_target_base: public texture_base
    {
        public:
            texture_target_base( bool store_pixels = false ); //defaults to false
            virtual ~texture_target_base();
            texture_base * create_new();
            virtual uint32_t get_pixel_32bit(int x, int y );//done by child classes
            virtual void get_pixel_values(int x, int y, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a );//done by child classes
            //Texture modification functions [ Implemented by child classes] [ BEGIN]
            virtual bool modify_texture_grayscale( );
            virtual bool modify_texture_invert( );
            virtual bool modify_texture_merge_color_rgba( color * color_key, float amount );
            virtual bool modify_texture_recolor_rgba(  color * color_key, float amount );
            virtual bool modify_texture_remove_color_rgba( color * color_key );
            virtual bool modify_texture_selective_color( color * selected_color, float difference_allowed = 0.25  );
            virtual bool modify_texture_selective_color_duo( color * selected_color1, color * selected_color2, float difference_allowed =0.25  );
            virtual bool modify_texture_selective_color_trio( color * selected_color1, color * selected_color2, color * selected_color3, float difference_allowed = 0.25 );
            virtual bool modify_texture_flip( int flags );
            //Texture modification functions [ Implemented by child classes] [ END ]

            virtual void render_overlay(  renderer_base * renderer,int x, int y, gpe::shape_rect* clip = nullptr, int alpha = 255  );
            virtual void render_overlay_scaled(  renderer_base * renderer,int x, int y,float x_scale, float y_scale, gpe::shape_rect* clip = nullptr, color * render_color = nullptr, int alpha = 255 );

            virtual void resize_target( renderer_base * renderer,int w, int h, int id = -1,bool useLinearScaling = true  );
            virtual int save_as_bmp( const std::string& file_location );
            virtual int save_as_png( const std::string& file_location );
            virtual void set_pixel_32bit( int x, int y, uint32_t pixel ); //requires pixels to be stored
            virtual void set_pixel_values( int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a ); //requires pixels to be stored
            virtual bool update_pixels(); //requires pixels to be stored
    };
}

#endif //gpe_texture_target_base_h
