/*
GPE_Texture_Base.cpp
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

#include "GPE_Texture_Base.h"


GPE_Texture_Base::GPE_Texture_Base()
{
    texId=-1;
    texWid = 0;
    texHeight = 0;
    currentR = 255;
    currentG = 255;
    currentB = 255;

    lastAlphaRendered = 255;
    currentBlendMode = -1;
    fileLocation = "";
    isTransparent = false;
    textureType = "base";
}

GPE_Texture_Base::~GPE_Texture_Base()
{

}

void GPE_Texture_Base::change_alpha( Uint8 alpha )
{

}

void GPE_Texture_Base::change_color(GPE_Color * newColor)
{

}

void GPE_Texture_Base::change_color( Uint8 red, Uint8 green, Uint8 blue )
{

}

bool GPE_Texture_Base::copy_image_source(std::string outDirectoryName)
{
    return false;
}

int GPE_Texture_Base::get_id()
{
    return texId;
}

std::string GPE_Texture_Base::get_filename()
{
    return fileLocation;
}

int GPE_Texture_Base::get_height()
{
    return texHeight;
}

std::string GPE_Texture_Base::get_name()
{
    return name;
}

int GPE_Texture_Base::get_width()
{
    return texWid;
}

std::string GPE_Texture_Base::get_texture_type()
{
    return textureType;
}

void GPE_Texture_Base::load_new_texture( GPE_Renderer_Base * renderer, std::string fileName, int id , bool transparent , bool useLinearScaling )
{

}

void GPE_Texture_Base::prerender_circle( GPE_Renderer_Base * renderer,int rad, GPE_Color * circleColor,  Uint8 alpha ,int id, bool transparent, bool useLinearScaling, bool isOutline )
{

}

void GPE_Texture_Base::prerender_rectangle( GPE_Renderer_Base * renderer,int w, int h, GPE_Color * newColor, int id , bool transparent, bool useLinearScaling, bool isOutline  )
{

}

GPE_Texture_Base * GPE_Texture_Base::produce_clone()
{
    return new GPE_Texture_Base();
}


void GPE_Texture_Base::render_align(  int x, int y, int hAlign, int vAlign, GPE_Rect* clip, GPE_Color * rendColor, int alpha )
{

}

void GPE_Texture_Base::render_align_resized(  int x, int y,int newWidth, int newHeight, int hAlign, int vAlign, GPE_Rect* clip , GPE_Color * rendColor , int alpha)
{

}

void GPE_Texture_Base::GPE_Texture_Base::render_tex(  int x, int y,GPE_Rect* clip, int alpha  )
{

}

void GPE_Texture_Base::render_tex_colored(  int x, int y, GPE_Color * rendColor, int alpha, GPE_Rect* clip )
{

}

void GPE_Texture_Base::render_tex_resized(  int x, int y,float newWidth, float newHeight, GPE_Rect* clip, GPE_Color * rendColor, int alpha)
{

}

void GPE_Texture_Base::render_tex_scaled(  int x, int y,float xScale, float yScale, GPE_Rect* clip, GPE_Color * rendColor, int alpha )
{

}

void GPE_Texture_Base::render_tex_rotated(  int x, int y, float renderAngle,  GPE_Color * renderColor, GPE_Rect* clip , int alpha )
{

}

void GPE_Texture_Base::render_tex_rotated_at_point(  int x, int y, float renderAngle, int pointX, int pointY, GPE_Color * renderColor , GPE_Rect* clip, int alpha )
{

}

void GPE_Texture_Base::render_tex_special(  int x, int y, float renderAngle, int newWidth , int newHeight, GPE_Color * renderColor, GPE_Rect* clip , int alpha )
{

}

void GPE_Texture_Base::render_tex_special_at_point(  int x, int y, float renderAngle, int pointX, int pointY,int newWidth, int newHeight, GPE_Color * renderColor, GPE_Rect* clip, int alpha )
{

}


void GPE_Texture_Base::set_alpha( int alpha)
{

}


void GPE_Texture_Base::set_blend_mode( int newBlendMode)
{

}
