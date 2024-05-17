/*
texture_base.cpp
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

#include "gpe_texture_base.h"

namespace gpe
{
    texture_base::texture_base( bool store_pixels )
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
        is_transparent = false;
        texture_type = "base";
        pixel_values_are_stored = store_pixels;
        render_texture_effect = false;
    }

    texture_base::~texture_base()
    {

    }

    void texture_base::change_alpha( uint8_t alpha )
    {

    }

    void texture_base::change_color( color * color_new)
    {

    }

    void texture_base::change_color( uint8_t red, uint8_t green, uint8_t blue )
    {

    }

    bool texture_base::copy_image_source( const std::string& directory_output_name)
    {
        return false;
    }

    int texture_base::get_id()
    {
        return tex_id;
    }

    std::string texture_base::get_filename()
    {
        return file_location;
    }

    int texture_base::get_height()
    {
        return tex_height;
    }

    std::string texture_base::get_name()
    {
        return name;
    }

    uint32_t texture_base::get_pixel_32bit(int x, int y )
    {
        return 0; //up to the child class to implement
    }

    void texture_base::get_pixel_values(int x, int y, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a )
    {
        //up to the child class to implement
    }

    int texture_base::get_width()
    {
        return tex_width;
    }

    std::string texture_base::get_texture_type()
    {
        return texture_type;
    }

    void texture_base::load_new_texture(  const std::string& file_name, int id , bool transparent , bool useLinearScaling, uint8_t colorkeyR, uint8_t colorkeyG, uint8_t colorkeyB )
    {

    }

    bool texture_base::modify_texture_grayscale( )
    {
        return false;
    }

    bool texture_base::modify_texture_invert( )
    {
        return false;
    }

    bool texture_base::modify_texture_merge_color_rgba( color * color_key, float amount )
    {
        return false;
    }

    bool texture_base::modify_texture_recolor_rgba(  color * color_key, float amount )
    {
        return false;
    }

    bool texture_base::modify_texture_remove_color_rgba( color * color_key )
    {
        return false;
    }

    bool texture_base::modify_texture_selective_color( color * selected_color, float difference_allowed )
    {
        return false;
    }

    bool texture_base::modify_texture_selective_color_duo( color * selected_color1, color * selected_color2, float difference_allowed )
    {
        return false;
    }

    bool texture_base::modify_texture_selective_color_trio( color * selected_color1, color * selected_color2, color * selected_color3, float difference_allowed  )
    {
        return false;
    }

    bool texture_base::modify_texture_flip( int flags )
    {
        return false;
    }

    void texture_base::prerender_circle( renderer_base * renderer,int rad, color * circleColor,  uint8_t alpha ,int id, bool transparent, bool useLinearScaling, bool isOutline )
    {

    }

    void texture_base::prerender_triangle( renderer_base * renderer, shape_triangle2d,  color * circleColor,  uint8_t alpha )
    {

    }

    void texture_base::prerender_rectangle( renderer_base * renderer,int w, int h, color * color_new, int id , bool transparent, bool useLinearScaling, bool isOutline  )
    {

    }

    texture_base * texture_base::create_new()
    {
        return new texture_base();
    }


    void texture_base::render_align(  int x, int y, int alignment_h, int alignment_v, shape_rect* clip, color * render_color, int alpha )
    {

    }

    bool texture_base::render_quad( shape_point2d p1 , shape_point2d p2, shape_point2d p3, shape_point2d p4, color * render_color, int alpha, shape_rect * clip  )
    {
        return false;
    }


    void texture_base::render_align_resized(  int x, int y,int new_width, int new_height, int alignment_h, int alignment_v, shape_rect* clip , color * render_color , int alpha)
    {

    }

    void texture_base::texture_base::render_tex(  int x, int y, shape_rect* clip, int alpha  )
    {

    }

    void texture_base::render_tex_colored(  int x, int y, color * render_color, int alpha, shape_rect* clip )
    {

    }

    bool texture_base::render_tex_polygon( int x, int y, int shape_length , int point_count, color * render_color, int alpha, float start_angle )
    {
        return false;
    }

    bool texture_base::render_tex_polygon_clipped( int x, int y, int shape_length , int point_count, color * render_color,shape_rect* clip, int alpha, float start_angle )
    {
        return false;
    }

    void texture_base::render_tex_quad(   shape_point2d p1 , shape_point2d p2, shape_point2d p3, shape_point2d p4, color * render_color, int alpha)
    {

    }

    void texture_base::render_tex_quad(   shape_point2d p1 , shape_point2d p2, shape_point2d p3, shape_point2d p4, color * render_color, int alpha , shape_rect * clip )
    {

    }

    void texture_base::render_tex_resized(  int x, int y,float new_width, float new_height, shape_rect* clip, color * render_color, int alpha)
    {

    }

    void texture_base::render_tex_scaled(  int x, int y,float x_scale, float y_scale, shape_rect* clip, color * render_color, int alpha )
    {

    }

    void texture_base::render_tex_rotated(  int x, int y, float render_angle,  color * render_color, shape_rect* clip , int alpha )
    {

    }

    void texture_base::render_tex_rotated_at_point(  int x, int y, float render_angle, int point_x, int point_y, color * render_color , shape_rect* clip, int alpha )
    {

    }

    void texture_base::render_tex_special(  int x, int y, float render_angle, int new_width , int new_height, color * render_color, shape_rect* clip , int alpha )
    {

    }

    void texture_base::render_tex_special_at_point(  int x, int y, float render_angle, int point_x, int point_y,int new_width, int new_height, color * render_color, shape_rect* clip, int alpha )
    {

    }

    bool texture_base::reset_effect_texture()
    {
        return false;
    }

    int texture_base::save_as_bmp( const std::string& file_location, bool save_effect )
    {
        return 0;
    }

    int texture_base::save_as_png( const std::string& file_location, bool save_effect )
    {
        return 0;
    }

    void texture_base::set_alpha( int alpha)
    {

    }


    void texture_base::set_blend_mode( int blend_mode_new)
    {

    }

    void texture_base::set_pixel_32bit( int x, int y, uint32_t pixel )
    {

    }

    void texture_base::set_pixel_values( int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a )
    {

    }

    bool texture_base::storing_pixel_values()
    {
        return pixel_values_are_stored;
    }

    bool texture_base::update_pixels()
    {
        return false;
    }
}
