/*
texture_target_base.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_texture_target_base.h"

namespace gpe
{
    texture_target_base::texture_target_base( bool store_pixels )
    {
        tex_id=-1;
        tex_width = 0;
        tex_height = 0;
        current_color_r = 255;
        current_color_g = 255;
        current_color_b = 255;

        lastAlphaRendered = 255;
        blend_mode_current = -1;
        file_location = "";
        texture_type = "target-base";
        is_transparent = false;
        pixel_values_are_stored = store_pixels;
    }

    texture_target_base::~texture_target_base()
    {

    }

    texture_base * texture_target_base::create_new()
    {
        return new texture_target_base();
    }

    uint32_t texture_target_base::get_pixel_32bit(int x, int y )
    {
        return 0; //done by child classes
    }

    void texture_target_base::get_pixel_values(int x, int y, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a )
    {
        //done by child classes
    }

    bool texture_target_base::modify_texture_grayscale( )
    {
        return false;
    }

    bool texture_target_base::modify_texture_invert( )
    {
        return false;
    }

    bool texture_target_base::modify_texture_merge_color_rgba( color * color_key, float amount )
    {
        return false;
    }

    bool texture_target_base::modify_texture_recolor_rgba(  color * color_key, float amount )
    {
        return false;
    }

    bool texture_target_base::modify_texture_remove_color_rgba( color * color_key )
    {
        return false;
    }

    bool texture_target_base::modify_texture_selective_color( color * selected_color, float difference_allowed )
    {
        return false;
    }

    bool texture_target_base::modify_texture_selective_color_duo( color * selected_color1, color * selected_color2, float difference_allowed )
    {
        return false;
    }

    bool texture_target_base::modify_texture_selective_color_trio( color * selected_color1, color * selected_color2, color * selected_color3, float difference_allowed  )
    {
        return false;
    }

    bool texture_target_base::modify_texture_flip( int flags )
    {
        return false;
    }

    void texture_target_base::render_overlay(  renderer_base * renderer,int x, int y, gpe::shape_rect* clip , int alpha  )
    {

    }

    void texture_target_base::render_overlay_scaled(  renderer_base * renderer,int x, int y,float x_scale, float y_scale, gpe::shape_rect* clip, color * render_color, int alpha )
    {

    }

    void texture_target_base::resize_target( renderer_base * renderer,int w, int h, int id ,bool useLinerScaling  )
    {

    }

    int texture_target_base::save_as_bmp( const std::string& file_location )
    {
        return 0;
    }

    int texture_target_base::save_as_png( const std::string& file_location )
    {
        return 0;
    }

    void texture_target_base::set_pixel_32bit( int x, int y, uint32_t pixel )
    {

    }

    void texture_target_base::set_pixel_values( int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a )
    {

    }

    bool texture_target_base::update_pixels()
    {
        return false;
    }
}
