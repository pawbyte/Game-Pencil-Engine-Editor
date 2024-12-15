/*
pawgui_color_picker.h
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )
Copyright (c) 2024 P.Industries

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

Contributors:
- PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>
- P Industries <https://p.industries>
*/


#ifndef PAWGUI_COLOR_PICKER_H
#define PAWGUI_COLOR_PICKER_H

#include "../base/pawgui_base.h"

namespace pawgui
{
    extern gpe::texture_base * texture_color_picker_gradient;

    class gpe_widget_color_picker: public widget_basic
    {
        protected:
            gpe::color * storedColor;
            int font_sizeW, font_sizeH;
        public:
            gpe::shape_rect fieldElementBox;
            std::string inputLabel;
            bool showLabel;
            gpe_widget_color_picker(std::string startName="",std::string description="", int r = 255, int g = 255, int b = 255 );
            ~gpe_widget_color_picker();
            std::string get_data();
            void load_data(std::string datastring);
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
            void set_string(std::string newstring);
            void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void set_color_from_rgb( gpe::color *color_new);
            void set_color_from_rgb(std::string color_newStr);
            void set_color_from_hex(std::string color_newStr);
            void set_r(int r);
            void set_g(int g);
            void set_b(int b);
            void set_rgb(int r, int g, int b);
    };

    bool get_color_from_popup(std::string popUpCaption = "", gpe::color * currColor = nullptr);
}

#endif //PAWGUI_COLOR_PICKER_H
