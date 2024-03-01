/*
pawgui_iconpack.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#include "pawgui_iconpack.h"

enum
{
    icon_pack_single_image = 0,
    icon_pack_image_sheet = 1,
    icon_pack_font = 2,
    icon_pack_none = 3, //Leave it as none for now, unsure of other possible types
};

namespace pawgui
{

    widget_iconpack::widget_iconpack( int icon_pack_style, std::string icons_directory, std::string icons_file_type, int font_size, bool effect_required  )
    {

    }

    widget_iconpack::~widget_iconpack()
    {

    }

    bool widget_iconpack::add_icon( std::string i_name, std::string f_name, int sheet_position )
    {
        return false;
    }

    bool widget_iconpack::icon_exists( std::string name )
    {
        return false;
    }


    void widget_iconpack::clear_pack()
    {

    }

    void widget_iconpack::clear_sheet()
    {

    }

    std::string widget_iconpack::get_file_type()
    {
        return "";
    }

    std::string widget_iconpack::get_file_directory()
    {
        return "";
    }

    int widget_iconpack::get_sheet_size()
    {
        return 0;
    }

    int widget_iconpack::get_size()
    {
        return 0;
    }

    bool widget_iconpack::is_sheet_setup()
    {
        return false;
    }

    void widget_iconpack::setup_sheet( int frame_count, int aw, int ah, int sofx, int sofy, int hPad, int vPad )
    {

    }

    void widget_iconpack::setup_sheet_animation(  int aw, int ah, int sofx, int sofy, int hPad, int vPad )
    {

    }

}
