/*
GPE_Renderer_Base.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/
#include "GPE_Renderer_Base.h"

GPE_Renderer_Base::GPE_Renderer_Base()
{

}

GPE_Renderer_Base::GPE_Renderer_Base(int rId, int widthStart, int heightStart )
{
    renderId = rId;
    rWidth = widthStart;
    rHeight = heightStart;
}

GPE_Renderer_Base::~GPE_Renderer_Base()
{

}

void GPE_Renderer_Base::clean_up()
{

}

void GPE_Renderer_Base::clear_renderer( bool windowIsMinimized )
{

}


//Check if anything's wrong with the window
bool GPE_Renderer_Base::error_check()
{
    return false;
}

int GPE_Renderer_Base::get_blend_mode()
{
    return renderBlendMode;
}

std::string GPE_Renderer_Base::get_renderer_name()
{
    return rendererName;
}

std::string GPE_Renderer_Base::get_renderer_type()
{
    return renderType;
}

//Some Shape Stuff
void GPE_Renderer_Base::render_horizontal_line(int y, int x1, int x2)
{

}

void GPE_Renderer_Base::render_horizontal_line_color( int y, int x1, int x2, Uint8 r, Uint8 g, Uint8 b, Uint8 a )
{

}

bool GPE_Renderer_Base::render_circle_color( Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a )
{

}

void GPE_Renderer_Base::reset_input()
{

}

void GPE_Renderer_Base::reset_viewpoint()
{

}

void GPE_Renderer_Base::resize_renderer(int newW, int newH )
{
    rWidth = newW;
    rHeight =newH;
}

std::string GPE_Renderer_Base::save_screenshot(std::string screenShotLocation )
{
    return "";
}

bool GPE_Renderer_Base::screen_was_cleared()
{
    return screenClearedOnFrame;
}

void GPE_Renderer_Base::set_render_blend_mode( int newBlendMode )
{

}

void GPE_Renderer_Base::set_viewpoint(GPE_Rect * newViewPoint )
{

}

void GPE_Renderer_Base::update_renderer( bool windowIsMinimized )
{

}
