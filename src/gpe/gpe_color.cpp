/*
gpe_color.cpp
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

#include "gpe_color.h"

namespace gpe
{
    color::color(std::string name, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        colorName = name;
        rVal=r;
        gVal=g;
        bVal=b;
        aVal=a;
    }
    color::color(int hexDec)
    {
        hexDecNumb = hexDec;
    }

    color::~color()
    {

    }

    bool color::checkEquality(color otherColor)
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

    color * color::duplicate_color()
    {
        color * duplicatedColor = new color(colorName,rVal,gVal,bVal,aVal);
        return duplicatedColor;
    }

    std::string color::get_name()
    {
        return colorName;
    }



    void color::set_hex(int newHex)
    {
        hexDecNumb = newHex;
    }

    uint8_t color::get_r()
    {
        return rVal;
    }

    uint8_t color::get_g()
    {
        return gVal;
    }

    uint8_t color::get_b()
    {
        return bVal;
    }

    uint8_t color::get_a()
    {
        return aVal;
    }

    void color::change_r(uint8_t r)
    {
        rVal=r;
    }

    void color::change_g(uint8_t g)
    {
        gVal=g;
    }

    void color::change_b(uint8_t b)
    {
        bVal=b;
    }
    void color::change_a(uint8_t a)
    {
        aVal=a;
    }

    void color::change_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        rVal=r;
        gVal=g;
        bVal=b;
        aVal=a;
    }

    void color::change_and_verify_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {

        rVal= r;
        gVal= g;
        bVal = b;
        aVal = a;
    }

    void color::copy_color( color * other_color )
    {
        if( other_color == NULL )
        {
            return;
        }
        rVal = other_color->get_r();
        gVal = other_color->get_g();
        bVal = other_color->get_b();
        aVal = other_color->get_a();
    }
}
