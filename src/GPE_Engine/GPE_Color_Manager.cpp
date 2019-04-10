/*
GPE_Color_Manager.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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

void GPE_Init_Colors()
{
    if( MASTER_OF_COLORS!=NULL)
    {
        delete MASTER_OF_COLORS;
        MASTER_OF_COLORS= NULL;
    }
    textColor = new GPE_Color("text",228, 228, 228 );
    c_aqua = new GPE_Color(   "aqua", 000,255,255);
    c_black =new GPE_Color(  "black", 0,0,0);
    c_alblack = new GPE_Color("alblack", 27,27,29);
    c_blgray =new GPE_Color( "blgray", 32,32,32);
    c_blue =new GPE_Color(   "blue",000,000,255);
    c_brown =new GPE_Color(  "brown",165,042,042);
    c_dkgray =new GPE_Color( "dkgray",64,64,64);
    c_fuchsia = new GPE_Color("fuschia",255,00,255);
    c_gold = new GPE_Color("gold", 255,215,0);
    c_gray =new GPE_Color(   "gray", 128,128,128);
    c_green =new GPE_Color(  "green",000,128,000);
    c_jetblack = new GPE_Color("jetblack", 52,52,52);
    c_lime =new GPE_Color(   "lime",000,255,000);
    c_ltblue =new GPE_Color( "ltblue",128,128,255);
    c_ltgray =new GPE_Color( "ltgray",211,211,211);
    c_ltgreen =new GPE_Color( "ltgreen", 128,255,128);
    c_ltlime =new GPE_Color( "ltlime", 96,255,96);
    c_ltorange =new GPE_Color( "ltorange",255,145,0);
    c_ltpink =new GPE_Color( "ltpink",255,145,255);
    c_ltpurple = new GPE_Color( "ltpurple", 145,145,255);
    c_ltred =new GPE_Color( "ltred", 255,128,128);
    c_maroon =new GPE_Color( "maroon", 128,000,000);
    c_navy=new GPE_Color(    "navy", 000,000,128);
    c_olive =new GPE_Color(  "olive", 128,128,000);
    c_orange =new GPE_Color( "orange", 255,165,000);
    c_orangered= new GPE_Color("orangered", 255,69,0);
    c_purple =new GPE_Color( "purple", 128,000,128);
    c_red =new GPE_Color(    "red", 255,000,000);
    c_silver =new GPE_Color( "silver", 192,192,192);
    c_teal =new GPE_Color(   "teal", 000,128,128);
    c_violet = new GPE_Color(  "violet", 238,130,238 );
    c_white =new GPE_Color(  "white", 255,255,255);
    c_yellow =new GPE_Color( "yellow", 255,255,000);

    hudColor = c_blgray;
    hudColorHighlight = c_black;

    MASTER_OF_COLORS = new GPE_Color_Master();
    MASTER_OF_COLORS->define_colors();
}

GPE_Color_Master::GPE_Color_Master()
{

}

void GPE_Color_Master::add_color(GPE_Color *colorIn,std::string stringIn)
{
    COLOR_OBJECTS.push_back(colorIn);
    if(biggestStringSize<(int)stringIn.size())
    {
        biggestStringSize=(int)stringIn.size();
    }
}

bool GPE_Color_Master::define_colors()
{
    add_color(c_aqua,"aqua");
    add_color(c_black,"black");
    add_color(c_blue,"blue");
    add_color(c_brown,"brown");
    add_color(c_dkgray,"dkgray");
    add_color(c_fuchsia,"fuchsia");
    add_color(c_gray,"gray");
    add_color(c_green,"green");
    add_color(c_lime,"lime");
    add_color(c_ltgray,"ltgray");
    add_color(c_maroon,"maroon");
    add_color(c_navy,"navy");
    add_color(c_olive,"olive");
    add_color(c_orange,"orange");
    add_color(c_orangered,"orangered");
    add_color(c_purple,"purple");
    add_color(c_red,"red");
    add_color(c_silver,"silver");
    add_color(c_teal,"teal");
    add_color(c_white,"white");
    add_color(c_yellow,"yellow");
    return true;
}


int GPE_Color_Master::get_longest_color_name()
{
    return biggestStringSize;
}

int GPE_Color_Master::get_color_count()
{
    return (int)COLOR_OBJECTS.size();
}

GPE_Color * GPE_Color_Master::get_color(std::string stringInForSearch)
{
    for(int i=0; i<(int)COLOR_OBJECTS.size(); i++)
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


int GPE_Color_Master::find_color_id(std::string stringInForSearch)
{
    for(unsigned i=0; i<(unsigned)COLOR_OBJECTS.size(); i++)
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

GPE_Color *GPE_Color_Master::get_color_from_id(int pos)
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
std::string GPE_Color_Master::get_color_name(int pos)
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
int merge_channel(int a, int b, double amount)
{
    double result = (b * amount) + (a * (1.0f - amount));
    return (int)result;
}

//merges two colors dependent on the amount
GPE_Color merge_color(GPE_Color clOne, GPE_Color clTwo, double amount)
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


