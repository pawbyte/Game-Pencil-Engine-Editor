/*
paw_gui_color_picker.h
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


#ifndef PAW_GUI_color_PICKER_H
#define PAW_GUI_color_PICKER_H

#include "paw_gui_base.h"

extern gpe::texture_base * texture_color_picker_gradient;

class gpe_widget_color_picker: public GPE_GeneralGuiElement
{
protected:
    gpe::color * storedColor;
    int fontSizeW, fontSizeH;
public:
    gpe::shape_rect fieldElementBox;
    std::string inputLabel;
    bool showLabel;
    gpe_widget_color_picker(std::string startName="",std::string description="", int r = 255, int g = 255, int b = 255 );
    ~gpe_widget_color_picker();
    std::string get_data();
    void load_data(std::string dataString);
    gpe::color * get_color();
    gpe::color * get_duplicate_color();
    std::string get_hex_string();
    std::string get_rgb_string();
    std::string get_plain_string();
    int get_r();
    int get_g();
    int get_b();
    void set_label(std::string newLabel);
    void set_name(std::string new_name);
    void set_string(std::string newString);
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void set_color_from_rgb( gpe::color *newColor);
    void set_color_from_rgb(std::string newColorStr);
    void set_color_from_hex(std::string newColorStr);
    void set_r(int r);
    void set_g(int g);
    void set_b(int b);
    void set_rgb(int r, int g, int b);
};

bool GPE_Change_color_PopUp(std::string popUpCaption = "", gpe::color * currColor = NULL);

#endif
