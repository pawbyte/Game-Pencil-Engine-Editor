/*
gpe_color.h
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

#ifndef gpe_color_h
#define gpe_color_h

#include <cstdint>
#include <string>

namespace gpe
{
    class color
    {
        private:
            uint8_t rVal;
            uint8_t gVal;
            uint8_t bVal;
            uint8_t aVal;
            std::string colorName;
            int hexDecNumb;
        public:
            color(std::string name,uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a=255);
            color(int hexDec);
            ~color();

            bool checkEquality(color otherColor);
            color * duplicate_color();
            std::string get_name();
            uint8_t get_r();
            uint8_t get_g();
            uint8_t get_b();
            uint8_t get_a();
            void set_hex(int newHex);
            void change_r(uint8_t r);
            void change_g(uint8_t g);
            void change_b(uint8_t b);
            void change_a(uint8_t a);

            void change_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
            void change_and_verify_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
            void copy_color( color * other_color );
    };
}

#endif //gpe_color_h
