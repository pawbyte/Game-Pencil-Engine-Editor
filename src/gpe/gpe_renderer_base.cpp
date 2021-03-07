/*
renderer_base.cpp
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

#include "gpe_renderer_base.h"
#include <iostream>

namespace gpe
{
    renderer_base * renderer_main = nullptr;
    renderer_base::renderer_base()
    {
        render_sub_rectangle = new shape_rect(0,0,0,0 );
        previously_scaled = false;
        renderer_scaling = false;
        use_integer_scaling = false;
        scale_percentage_x = 0;
        scale_percentage_y = 0;
        scale_source_width = 0;
        scale_source_height = 0;
    }

    renderer_base::renderer_base(int rId, int widthStart, int heightStart )
    {
        render_sub_rectangle = new shape_rect(0,0,0,0 );
        render_id = rId;
        r_width = widthStart;
        r_height = heightStart;
        renderer_scaling = false;
        previously_scaled = false;
        use_integer_scaling = false;
        scale_percentage_x = 0;
        scale_percentage_y = 0;
        scale_source_width = 0;
        scale_source_height = 0;
    }

    renderer_base::~renderer_base()
    {

    }

    void renderer_base::clean_up()
    {

    }

    void renderer_base::clear_renderer( bool windowIsMinimized )
    {

    }

    bool renderer_base::disable_scaling()
    {
        return false;
    }

    bool renderer_base::enable_scaling()
    {
        return false;
    }


    //Check if anything's wrong with the window
    bool renderer_base::error_check()
    {
        return false;
    }

    bool renderer_base::file_perform_effect_color_erase( std::string file_location, color * color_to_erase )
    {
        return false;
    }

    bool renderer_base::file_perform_effect_color_invert( std::string file_location)
    {
        return false;
    }

    bool renderer_base::file_perform_effect_grayscale(std::string file_location )
    {
        return false;
    }

    int renderer_base::get_blend_mode()
    {
        return render_blend_mode;
    }

    std::string renderer_base::get_renderer_name()
    {
        return r_name;
    }

    std::string renderer_base::get_renderer_type()
    {
        return r_type;
    }

    int renderer_base::get_scale_width()
    {
        return scale_source_width;
    }

    int renderer_base::get_scale_height()
    {
        return scale_source_height;
    }

    bool renderer_base::get_scaled_previous()
    {
        return previously_scaled;
    }

    float renderer_base::get_scale_x()
    {
        return scale_percentage_x;
    }

    float renderer_base::get_scale_y()
    {
        return scale_percentage_y;
    }

    bool renderer_base::is_integer_scaling()
    {
        return use_integer_scaling;
    }

    bool renderer_base::is_scaled()
    {
        return renderer_scaling;
    }

    //Some Shape Stuff
    void renderer_base::render_horizontal_line(int y, int x1, int x2)
    {

    }

    void renderer_base::render_horizontal_line_color( int y, int x1, int x2, uint8_t r, uint8_t g, uint8_t b, uint8_t a )
    {

    }

    bool renderer_base::render_circle_color( int x, int y, int rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a )
    {
        return false;
    }

    void renderer_base::reset_input()
    {

    }

    void renderer_base::reset_viewpoint()
    {

    }

    void renderer_base::resize_renderer(int newW, int newH )
    {
        r_width = newW;
        r_height = newH;
    }

    std::string renderer_base::save_screenshot(std::string file_location )
    {
        return "";
    }

    bool renderer_base::scale_renderer( int s_width, int s_height , bool scale_int )
    {
        return false;
    }

    bool renderer_base::scale_renderer_factor( float s_width, float s_height, bool scale_int )
    {
        return false;
    }

    bool renderer_base::screen_was_cleared()
    {
        return cleared_this_frame;
    }

    void renderer_base::set_render_blend_mode( int blend_mode_new )
    {

    }

    void renderer_base::set_viewpoint( gpe::shape_rect * newViewPoint )
    {

    }

    void renderer_base::update_renderer( bool windowIsMinimized )
    {
        std::cout << "Render base class rendered another frame...." << ").\n";
    }
}
