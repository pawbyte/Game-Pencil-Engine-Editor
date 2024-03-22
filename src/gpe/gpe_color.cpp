/*
gpe_color.cpp
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

#include "gpe_color.h"

namespace gpe
{
    color::color(std::string name, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        color_name = name;
        r_val=r;
        g_val=g;
        b_val=b;
        a_val=a;
    }
    color::color(int hex_dec_number )
    {
        color_hex_number = hex_dec_number;
    }

    color::~color()
    {

    }

    bool color::checkEquality(color other_color)
    {
        if(r_val==other_color.r_val)
        {
            if(g_val==other_color.g_val)
            {
                if(b_val==other_color.b_val)
                {
                    if(a_val==other_color.a_val)
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
        color * duplicatedColor = new color(color_name,r_val,g_val,b_val,a_val);
        return duplicatedColor;
    }

    std::string color::get_name()
    {
        return color_name;
    }



    void color::set_hex(int hex_dec_number)
    {
        color_hex_number = hex_dec_number;
    }

    uint8_t color::get_r()
    {
        return r_val;
    }

    uint8_t color::get_g()
    {
        return g_val;
    }

    uint8_t color::get_b()
    {
        return b_val;
    }

    uint8_t color::get_a()
    {
        return a_val;
    }

    void color::change_r(uint8_t r)
    {
        r_val=r;
    }

    void color::change_g(uint8_t g)
    {
        g_val=g;
    }

    void color::change_b(uint8_t b)
    {
        b_val=b;
    }
    void color::change_a(uint8_t a)
    {
        a_val=a;
    }

    void color::change_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        r_val=r;
        g_val=g;
        b_val=b;
        a_val=a;
    }

    void color::change_and_verify_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {

        r_val= r;
        g_val= g;
        b_val = b;
        a_val = a;
    }

    void color::copy_color( color * other_color )
    {
        if( other_color == nullptr )
        {
            return;
        }
        r_val = other_color->get_r();
        g_val = other_color->get_g();
        b_val = other_color->get_b();
        a_val = other_color->get_a();
    }
}
