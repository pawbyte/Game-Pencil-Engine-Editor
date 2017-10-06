/*
GPE_Color.cpp
This file is part of:
GAME PENCI ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

#include "GPE_Color.h"

GPE_Color::GPE_Color()
{
    colorName = "white";
    rVal=255;
    gVal=255;
    bVal=255;
    aVal=255;
}

GPE_Color::GPE_Color(std::string name, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    colorName = name;
    rVal=r;
    gVal=g;
    bVal=b;
    aVal=a;
}

GPE_Color::GPE_Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    colorName = "unnamed";
    rVal=r;
    gVal=g;
    bVal=b;
    aVal=a;
}

bool GPE_Color::checkEquality(GPE_Color otherColor)
{
    if(rVal==otherColor.rVal)
    {
        if(gVal==otherColor.gVal)
        {
            if(bVal==otherColor.bVal)
            {
                if(aVal==otherColor.aVal)
                {
                    return true;
                }

            }
        }
    }
    return false;
}

GPE_Color * GPE_Color::duplicate_color()
{
    GPE_Color * duplicatedColor = new GPE_Color(colorName,rVal,gVal,bVal,aVal);
    return duplicatedColor;
}

std::string GPE_Color::get_name()
{
    return colorName;
}

GPE_Color::GPE_Color(int hexDec)
{
    hexDecNumb = hexDec;
}

void GPE_Color::set_hex(int newHex)
{
    hexDecNumb = newHex;
}

SDL_Color GPE_Color::get_sdl_color()
{
    SDL_Color retColor = {rVal, gVal, bVal, aVal};
    return retColor;
}

Uint8 GPE_Color::get_r()
{
    return rVal;
}

Uint8 GPE_Color::get_g()
{
    return gVal;
}

Uint8 GPE_Color::get_b()
{
    return bVal;
}

void GPE_Color::change_r(Uint8 r)
{
    rVal=r;
}

void GPE_Color::change_g(Uint8 g)
{
    gVal=g;
}

void GPE_Color::change_b(Uint8 b)
{
    bVal=b;
}

void GPE_Color::change_rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    rVal=r;
    gVal=g;
    bVal=b;
    aVal=a;
}

GPE_Color::~GPE_Color()
{

}
