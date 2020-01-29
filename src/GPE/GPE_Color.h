/*
GPE_Color.h
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

#ifndef GPE_COLOR_CLASS_H
#define GPE_COLOR_CLASS_H

#include "GPE_CIncludes.h"

class GPE_Color
{
private:
    Uint8 rVal;
    Uint8 gVal;
    Uint8 bVal;
    Uint8 aVal;
    std::string colorName;
    int hexDecNumb;
public:
    GPE_Color(std::string name,Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a=255);
    GPE_Color(int hexDec);
    bool checkEquality(GPE_Color otherColor);
    GPE_Color * duplicate_color();
    std::string get_name();
    SDL_Color get_sdl_color();
    Uint8 get_r();
    Uint8 get_g();
    Uint8 get_b();
    Uint8 get_a();
    void set_hex(int newHex);
    void change_r(Uint8 r);
    void change_g(Uint8 g);
    void change_b(Uint8 b);
    void change_a(Uint8 a);

    void change_rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    void change_and_verify_rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    ~GPE_Color();
};

#endif
