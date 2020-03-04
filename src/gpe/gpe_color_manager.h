/*
gpe_color_manager.h
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

#ifndef gpe_color_manager_h
#define gpe_color_manager_h

//The headers
#include "gpe_common_includes.h"
#include "gpe_color.h"
#include "gpe_constants.h"
#include <string>
#include <vector>

namespace gpe
{
    //The color of the font
    extern color * textColor;

    extern color * c_aqua;
    extern color * c_alblack;
    extern color * c_black;
    extern color * c_blgray;
    extern color * c_brown;
    extern color * c_blue;
    extern color * c_dkgray;
    extern color * c_fuchsia;
    extern color * c_gold;
    extern color * c_gray;
    extern color * c_green;
    extern color * c_jetblack;
    extern color * c_lime;
    extern color * c_ltblue;
    extern color * c_ltgray;
    extern color * c_ltgreen;
    extern color * c_ltlime;
    extern color * c_ltorange;
    extern color * c_ltpink;
    extern color * c_ltpurple;
    extern color * c_ltred;
    extern color * c_maroon;
    extern color * c_navy;
    extern color * c_olive;
    extern color * c_orange;
    extern color * c_orangered;
    extern color * c_purple;
    extern color * c_red;
    extern color * c_silver;
    extern color * c_teal;
    extern color * c_violet;
    extern color * c_white;
    extern color * c_yellow;

    extern color * hudColor;
    extern color * hudColorHighlight;

    //color functions
    void GPE_Init_colors();

    class color_master
    {
    private:
        std::vector <color *> COLOR_OBJECTS;
        int biggestStringSize;
    public:
        color_master();
        void add_color( color *colorIn,std::string stringIn);
        bool color_equals(color cOne, color cTwo);

        bool define_colors();
        int find_color_id(std::string stringInForSearch);
        int get_longest_color_name();
        int get_color_count();
        color * get_color(std::string stringInForSearch);
        color * get_color_from_id(int pos);
        std::string get_color_name(int pos);

        void hex_to_rgb(std::string hexCode,int &r, int &g, int &b);
        int merge_channel(int a, int b, float amount);
        color merge_color(color clOne, color clTwo, float amount);
        std::string rgb_to_hex(int r, int g, int b);
    };

    extern color_master * color_system;
}

#endif //gpe_color_manager_h
