/*
GPE_Color_Manager.h
This file is part of:
GAME PENCIL ENGINE
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

#ifndef COLOR_CONTROL_H
#define COLOR_CONTROL_H

//The headers
#include "GPE_CIncludes.h"
#include "GPE_Color.h"
#include "GPE_Constants.h"
#include <string>
#include <vector>
//The color of the font
extern GPE_Color * textColor;

extern GPE_Color * c_aqua;
extern GPE_Color * c_alblack;
extern GPE_Color * c_black;
extern GPE_Color * c_blgray;
extern GPE_Color * c_brown;
extern GPE_Color * c_blue;
extern GPE_Color * c_dkgray;
extern GPE_Color * c_fuchsia;
extern GPE_Color * c_gold;
extern GPE_Color * c_gray;
extern GPE_Color * c_green;
extern GPE_Color * c_jetblack;
extern GPE_Color * c_lime;
extern GPE_Color * c_ltblue;
extern GPE_Color * c_ltgray;
extern GPE_Color * c_ltgreen;
extern GPE_Color * c_ltlime;
extern GPE_Color * c_ltorange;
extern GPE_Color * c_ltpink;
extern GPE_Color * c_ltpurple;
extern GPE_Color * c_ltred;
extern GPE_Color * c_maroon;
extern GPE_Color * c_navy;
extern GPE_Color * c_olive;
extern GPE_Color * c_orange;
extern GPE_Color * c_orangered;
extern GPE_Color * c_purple;
extern GPE_Color * c_red;
extern GPE_Color * c_silver;
extern GPE_Color * c_teal;
extern GPE_Color * c_violet;
extern GPE_Color * c_white;
extern GPE_Color * c_yellow;

extern GPE_Color * hudColor;
extern GPE_Color * hudColorHighlight;

//color functions
void activate_colors();
bool colorEquals(GPE_Color cOne, GPE_Color cTwo);
int merge_channel(int a, int b, float amount);
GPE_Color merge_color(GPE_Color clOne, GPE_Color clTwo, float amount);
class colorMaster
{
    private:
        std::vector <GPE_Color *> COLOR_OBJECTS;
        int biggestStringSize;
    public:
        colorMaster();
        void addColor(GPE_Color *colorIn,std::string stringIn);
        int getBiggestColorSize();
        int getListSize();
        GPE_Color * getColor(std::string stringInForSearch);
        int getColorPos(std::string stringInForSearch);
        GPE_Color *getColorAt(int pos);
        std::string getColorNameAt(int pos);
};

//Color Loading
void load_colors();


#endif
