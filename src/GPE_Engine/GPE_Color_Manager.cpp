/*
GPE_Color_Manager.cpp
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

#include "GPE_Color_Manager.h"

GPE_Color * textColor = NULL;

GPE_Color * c_aqua = NULL;
GPE_Color * c_black = NULL;
GPE_Color * c_alblack = NULL;
GPE_Color * c_blgray = NULL;
GPE_Color * c_brown = NULL;
GPE_Color * c_blue = NULL;
GPE_Color * c_dkgray = NULL;
GPE_Color * c_fuchsia = NULL;
GPE_Color * c_gold = NULL;
GPE_Color * c_gray = NULL;
GPE_Color * c_green = NULL;
GPE_Color * c_jetblack = NULL;
GPE_Color * c_lime = NULL;
GPE_Color * c_ltblue = NULL;
GPE_Color * c_ltgray = NULL;
GPE_Color * c_ltgreen = NULL;
GPE_Color * c_ltlime = NULL;
GPE_Color * c_ltorange = NULL;
GPE_Color * c_ltpink = NULL;
GPE_Color * c_ltpurple = NULL;
GPE_Color * c_ltred = NULL;
GPE_Color * c_maroon = NULL;
GPE_Color * c_navy = NULL;
GPE_Color * c_olive = NULL;
GPE_Color * c_orange = NULL;
GPE_Color * c_orangered = NULL;
GPE_Color * c_purple = NULL;
GPE_Color * c_red = NULL;
GPE_Color * c_silver = NULL;
GPE_Color * c_teal = NULL;
GPE_Color * c_violet = NULL;
GPE_Color * c_white = NULL;
GPE_Color * c_yellow = NULL;

GPE_Color * hudColor = NULL;
GPE_Color * hudColorHighlight = NULL;

void activate_colors()
{
    textColor = new GPE_Color(255, 255, 255 );

    c_aqua =new GPE_Color(   000,255,255);
    c_black =new GPE_Color(  0,0,0);
    c_alblack = new GPE_Color(27,27,29);
    c_blgray =new GPE_Color( 32,32,32);
    c_blue =new GPE_Color(   000,000,255);
    c_brown =new GPE_Color(  165,042,042);
    c_dkgray =new GPE_Color( 64,64,64);
    c_fuchsia = new GPE_Color(255,00,255);
    c_gold = new GPE_Color(255,215,0);
    c_gray =new GPE_Color(   128,128,128);
    c_green =new GPE_Color(  000,128,000);
    c_jetblack = new GPE_Color(52,52,52);
    c_lime =new GPE_Color(   000,255,000);
    c_ltblue =new GPE_Color( 128,128,255);
    c_ltgray =new GPE_Color( 211,211,211);
    c_ltgreen =new GPE_Color( 128,255,128);
    c_ltlime =new GPE_Color( 96,255,96);
    c_ltorange =new GPE_Color( 255,145,0);
    c_ltpink =new GPE_Color( 255,145,255);
    c_ltpurple = new GPE_Color( 145,145,255);
    c_ltred =new GPE_Color( 255,128,128);
    c_maroon =new GPE_Color( 128,000,000);
    c_navy=new GPE_Color(    000,000,128);
    c_olive =new GPE_Color(  128,128,000);
    c_orange =new GPE_Color( 255,165,000);
    c_orangered= new GPE_Color(255,69,0);
    c_purple =new GPE_Color( 128,000,128);
    c_red =new GPE_Color(    255,000,000);
    c_silver =new GPE_Color( 192,192,192);
    c_teal =new GPE_Color(   000,128,128);
    c_violet = new GPE_Color(  238,130,238 );
    c_white =new GPE_Color(  255,255,255);
    c_yellow =new GPE_Color( 255,255,000);

    hudColor = c_blgray;
    hudColorHighlight = c_black;

}

colorMaster::colorMaster()
{

}

void colorMaster::addColor(GPE_Color *colorIn,std::string stringIn)
{
    COLOR_OBJECTS.push_back(colorIn);
    if(biggestStringSize<(int)stringIn.size())
    {
        biggestStringSize=(int)stringIn.size();
    }
}

int colorMaster::getBiggestColorSize()
{
    return biggestStringSize;
}

int colorMaster::getListSize()
{
    return (int)COLOR_OBJECTS.size();
}

GPE_Color * colorMaster::getColor(std::string stringInForSearch)
{
    for(int i=0;i<(int)COLOR_OBJECTS.size();i++)
    {
        if(COLOR_OBJECTS[i]!=NULL)
        {
             if(COLOR_OBJECTS[i]->get_name()==stringInForSearch)
             {
                 return COLOR_OBJECTS[i];
             }
        }
    }
    return c_white;
}


int colorMaster::getColorPos(std::string stringInForSearch)
{
    for(unsigned i=0;i<(unsigned)COLOR_OBJECTS.size();i++)
    {
        if(COLOR_OBJECTS[i]!=NULL)
        {
             if(COLOR_OBJECTS[i]->get_name()==stringInForSearch)
             {
                 return i;
             }
        }
    }
    return 0;
}

GPE_Color *colorMaster::getColorAt(int pos)
{
    if((pos<(int)COLOR_OBJECTS.size())&&(pos>0))
    {
        return COLOR_OBJECTS[pos];
    }
    else
    {
        return c_white;
    }
}
std::string colorMaster::getColorNameAt(int pos)
{
    if((pos<(int)COLOR_OBJECTS.size())&&(pos>0))
    {
        return COLOR_OBJECTS[pos]->get_name();
    }
    else
    {
        return "Error: Color Not Found";
    }
}

bool colorEquals(GPE_Color ccOne, GPE_Color ccTwo)
{
    return ccOne.checkEquality(ccTwo);
}

//merges two channels dependent on the amount
int merge_channel(int a, int b, float amount)
{
    float result = (b * amount) + (a * (1 - amount));
    return int(result);
}

//merges two colors dependent on the amount
GPE_Color merge_color(GPE_Color clOne, GPE_Color clTwo, float amount)
{
    if(colorEquals(clOne,clTwo))
    {
        return clOne;
    }
    else
    {
        if(amount<0)
        {
            return clOne;
        }
        else
        {
            if(amount>1)
            {
                return clTwo;
            }
            else
            {
                GPE_Color  * result = NULL;
                /*
                result.r = merge_channel(clOne.r, clTwo.r, amount);
                result.g = merge_channel(clOne.g, clTwo.g, amount);
                result.b = merge_channel(clOne.b, clTwo.b, amount);
                */
                //result.a = merge_channel(a.a, b.a, amount);
                return *result;
            }
        }
    }
}


