/*
paw_gui_content_selector.h
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


#ifndef PAW_GUI_CONTENT_SELECTOR_H
#define PAW_GUI_CONTENT_SELECTOR_H

#include "paw_gui_base.h"

typedef enum
{
    GPE_CS_TYPE_INT,     /**< Int */
    GPE_CS_TYPE_float,     /**< float */
    GPE_CS_TYPE_color,     /**< Color */
    GPE_CS_TYPE_STRING,     /**< String */
} GPE_Content_Selector_Type;

class GPE_Content_Selector: public GPE_GeneralGuiElement
{
protected:
    int fontSizeW, fontSizeH;
public:
    int selectorId;
    int selectorType;
    int selectorGroup; //Seperated groups will be used per level essentially
    int storedInt;
    float storeddfloat;
    gpe::color * storedColor;
    std::string storedString;
    gpe::shape_rect fieldElementBox;
    GPE_Content_Selector(std::string name="",std::string description="" );
    ~GPE_Content_Selector();
    std::string get_data();
    void load_data(std::string dataString);
    std::string get_plain_string();
    void set_label(std::string newLabel);
    void set_name(std::string new_name);
    void set_string(std::string newString);
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void set_color_from_rgb( gpe::color * newColor);
    void set_color_from_rgb(std::string newColorStr);
    void set_color_from_hex(std::string newColorStr);
    void set_r(int r);
    void set_g(int g);
    void set_b(int b);
    void set_rgb(int r, int g, int b);
};

extern GPE_Content_Selector * GPE_Content_Selector_Current;
extern int GPE_Content_Selector_State; // -1, not active, 0 active, 1 just set
#endif //PAW_GUI_CONTENT_SELECTOR_H
