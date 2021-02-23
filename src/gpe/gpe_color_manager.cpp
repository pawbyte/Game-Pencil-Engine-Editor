/*
gpe_color_manager.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

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
#include "gpe_color_manager.h"

namespace gpe
{
    color_master * color_system = nullptr;

    color * c_aqua = nullptr;
    color * c_black = nullptr;
    color * c_alblack = nullptr;
    color * c_blgray = nullptr;
    color * c_brown = nullptr;
    color * c_blue = nullptr;
    color * c_dkgray = nullptr;
    color * c_fuchsia = nullptr;
    color * c_gold = nullptr;
    color * c_gray = nullptr;
    color * c_green = nullptr;
    color * c_jetblack = nullptr;
    color * c_lime = nullptr;
    color * c_ltblue = nullptr;
    color * c_ltgray = nullptr;
    color * c_ltgreen = nullptr;
    color * c_ltlime = nullptr;
    color * c_ltorange = nullptr;
    color * c_ltpink = nullptr;
    color * c_ltpurple = nullptr;
    color * c_ltred = nullptr;
    color * c_maroon = nullptr;
    color * c_navy = nullptr;
    color * c_olive = nullptr;
    color * c_orange = nullptr;
    color * c_orangered = nullptr;
    color * c_purple = nullptr;
    color * c_red = nullptr;
    color * c_silver = nullptr;
    color * c_teal = nullptr;
    color * c_violet = nullptr;
    color * c_white = nullptr;
    color * c_yellow = nullptr;

    color * color_hud = nullptr;
    color * color_hud_highlight = nullptr;

    void init_colors()
    {
        if( color_system!=nullptr)
        {
            delete color_system;
            color_system= nullptr;
        }
        c_aqua = new color(   "aqua", 000,255,255);
        c_black =new color(  "black", 0,0,0);
        c_alblack = new color("alblack", 27,27,29);
        c_blgray =new color( "blgray", 32,32,32);
        c_blue =new color(   "blue",000,000,255);
        c_brown =new color(  "brown",165,042,042);
        c_dkgray =new color( "dkgray",64,64,64);
        c_fuchsia = new color("fuschia",255,00,255);
        c_gold = new color("gold", 255,215,0);
        c_gray =new color(   "gray", 128,128,128);
        c_green =new color(  "green",000,128,000);
        c_jetblack = new color("jetblack", 52,52,52);
        c_lime =new color(   "lime",000,255,000);
        c_ltblue =new color( "ltblue",128,128,255);
        c_ltgray =new color( "ltgray",228,228,228);
        c_ltgreen =new color( "ltgreen", 128,255,128);
        c_ltlime =new color( "ltlime", 96,255,96);
        c_ltorange =new color( "ltorange",255,145,0);
        c_ltpink =new color( "ltpink",255,145,255);
        c_ltpurple = new color( "ltpurple", 145,145,255);
        c_ltred =new color( "ltred", 255,128,128);
        c_maroon =new color( "maroon", 128,000,000);
        c_navy=new color(    "navy", 000,000,128);
        c_olive =new color(  "olive", 128,128,000);
        c_orange =new color( "orange", 255,165,000);
        c_orangered= new color("orangered", 255,69,0);
        c_purple =new color( "purple", 128,000,128);
        c_red =new color(    "red", 255,000,000);
        c_silver =new color( "silver", 192,192,192);
        c_teal =new color(   "teal", 000,128,128);
        c_violet = new color(  "violet", 238,130,238 );
        c_white =new color(  "white", 255,255,255);
        c_yellow =new color( "yellow", 255,255,000);

        color_hud = c_blgray;
        color_hud_highlight = c_black;

        color_system = new color_master();
        color_system->define_colors();
    }

    color_master::color_master()
    {

    }

    void color_master::add_color( color *color_in,std::string string_input)
    {
        color_directory_list.push_back(color_in);
        if(largest_string_size<(int)string_input.size())
        {
            largest_string_size=(int)string_input.size();
        }
    }

    bool color_master::define_colors()
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


    int color_master::get_longest_color_name()
    {
        return largest_string_size;
    }

    int color_master::get_color_count()
    {
        return (int)color_directory_list.size();
    }

    color * color_master::get_color(std::string string_in_search)
    {
        for(int i=0; i<(int)color_directory_list.size(); i++)
        {
            if(color_directory_list[i]!=nullptr)
            {
                if(color_directory_list[i]->get_name()==string_in_search)
                {
                    return color_directory_list[i];
                }
            }
        }
        return c_white;
    }


    int color_master::find_color_id(std::string string_in_search)
    {
        for(unsigned i=0; i<(unsigned)color_directory_list.size(); i++)
        {
            if(color_directory_list[i]!=nullptr)
            {
                if(color_directory_list[i]->get_name()==string_in_search)
                {
                    return i;
                }
            }
        }
        return 0;
    }

    color *color_master::get_color_from_id(int pos)
    {
        if((pos<(int)color_directory_list.size())&&(pos>0))
        {
            return color_directory_list[pos];
        }
        else
        {
            return c_white;
        }
    }

    std::string color_master::get_color_name(int pos)
    {
        if((pos<(int)color_directory_list.size())&&(pos>0))
        {
            return color_directory_list[pos]->get_name();
        }
        else
        {
            return "Error: Color Not Found";
        }
    }

    void color_master::hex_to_rgb(std::string hexCode,int &r, int &g, int &b)
    {
        if( (int)hexCode.size() >= 3)
        {
            if(hexCode[0] == '#')
            {
                hexCode.erase(0,1);
            }
            if( (int)hexCode.size() == 3)
            {
                hexCode = hexCode.substr(0,1)+hexCode.substr(0,1)+hexCode.substr(1,1)+hexCode.substr(1,1)+hexCode.substr(2,1)+hexCode.substr(2,1);
            }

            if( (int)hexCode.size()==6)
            {
                int rrrrr =0;
                int ggggg =0;
                int bbbbb =0;
                std::istringstream(hexCode.substr(0,2)) >> std::hex >> rrrrr;
                std::istringstream(hexCode.substr(2,2)) >> std::hex >> ggggg;
                std::istringstream(hexCode.substr(4,2)) >> std::hex >> bbbbb;
                r = rrrrr;
                g = ggggg;
                b = bbbbb;
            }
            else
            {
                //invalid hex given
                r = 0;
                g = 0;
                b = 0;
            }
        }
    }

    bool color_master::color_equals(color ccOne, color ccTwo)
    {
        return ccOne.checkEquality(ccTwo);
    }

    //merges two channels dependent on the amount
    int color_master::merge_channel(int a, int b, float amount)
    {
        float result = (b * amount) + (a * (1.0f - amount));
        return (int)result;
    }

    //merges two colors dependent on the amount
    color color_master::merge_color(color clOne, color clTwo, float amount)
    {
        if( color_equals(clOne,clTwo))
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
                    color  * result = nullptr;

                    result->change_r( merge_channel(clOne.get_r(), clTwo.get_r(), amount) );
                    result->change_g( merge_channel(clOne.get_g(), clTwo.get_g(), amount) );
                    result->change_b( merge_channel(clOne.get_b(), clTwo.get_b(), amount) );
                    //result.a = merge_channel(a.a, b.a, amount);
                    return *result;
                }
            }
        }
        return clOne;
    }

    std::string color_master::rgb_to_hex(int r, int g, int b)
    {
        char buf[256];
        sprintf(buf, "%.2X%.2X%.2X", r, g, b);
        return std::string(buf);
    }

}
