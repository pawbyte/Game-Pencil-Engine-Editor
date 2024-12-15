/*
pawgui_content_selector.h
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PAWGUI contributors ( Contributors Page )

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

-PAWGUI <https://www.pawbyte.com/pawgui>
*/


#ifndef PAWGUI_CONTENT_SELECTOR_H
#define PAWGUI_CONTENT_SELECTOR_H

#include "../base/pawgui_base.h"

namespace pawgui
{
    enum class content_selector_type
    {
        cs_type_int = 0,     /**< Int */
        cs_type_float = 1,     /**< float */
        cs_type_color = 2,     /**< Color */
        cs_type_string = 3,     /**< string */
    };

    class widget_content_selector: public widget_basic
    {
        protected:
            int font_sizeW, font_sizeH;
        public:
            int selectorId;
            content_selector_type selectorType;
            int selectorGroup; //Seperated groups will be used per level essentially
            int storedInt;
            float storeddfloat;
            gpe::color * storedColor;
            std::string storedstring;
            gpe::shape_rect fieldElementBox;
            widget_content_selector(std::string name="",std::string description="" );
            ~widget_content_selector();
            std::string get_data();
            void load_data(std::string datastring);
            std::string get_plain_string();
            void set_label(std::string newLabel);
            void set_name(std::string new_name);
            void set_string(std::string newstring);
            void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void set_color_from_rgb( gpe::color * color_new);
            void set_color_from_rgb(std::string color_newStr);
            void set_color_from_hex(std::string color_newStr);
            void set_r(int r);
            void set_g(int g);
            void set_b(int b);
            void set_rgb(int r, int g, int b);
    };

    extern widget_content_selector * main_content_selector;
    extern int widget_content_selector_state; // -1, not active, 0 active, 1 just set

}
#endif //PAWGUI_CONTENT_SELECTOR_H
