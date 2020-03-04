/*
renderer_base.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#include "gpe_renderer_base.h"

namespace gpe
{
    renderer_base * renderer_main = NULL;
    renderer_base::renderer_base()
    {

    }

    renderer_base::renderer_base(int rId, int widthStart, int heightStart )
    {
        renderId = rId;
        rWidth = widthStart;
        rHeight = heightStart;
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


    //Check if anything's wrong with the window
    bool renderer_base::error_check()
    {
        return false;
    }

    int renderer_base::get_blend_mode()
    {
        return renderBlendMode;
    }

    std::string renderer_base::get_renderer_name()
    {
        return rendererName;
    }

    std::string renderer_base::get_renderer_type()
    {
        return renderType;
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

    }

    void renderer_base::reset_input()
    {

    }

    void renderer_base::reset_viewpoint()
    {

    }

    void renderer_base::resize_renderer(int newW, int newH )
    {
        rWidth = newW;
        rHeight =newH;
    }

    std::string renderer_base::save_screenshot(std::string file_location )
    {
        return "";
    }

    bool renderer_base::screen_was_cleared()
    {
        return screenClearedOnFrame;
    }

    void renderer_base::set_render_blend_mode( int newBlendMode )
    {

    }

    void renderer_base::set_viewpoint( gpe::shape_rect * newViewPoint )
    {

    }

    void renderer_base::update_renderer( bool windowIsMinimized )
    {

    }
}
