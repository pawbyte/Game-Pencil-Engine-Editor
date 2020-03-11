/*
paw_gui_sliders.h
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

#ifndef PAW_GUI_SLIDERS_H
#define PAW_GUI_SLIDERS_H

#include "paw_gui_base.h"

class GPE_Slider_XAxis: public GPE_GeneralGuiElement
{
protected:
    bool isOnScrollBox;
    float mouseXScrolPos;
    bool hasMoved;
    bool isHeldOn;
    float myValue;
    float minValue;
    float max_value;
    float currentSliderXPos;
    float scrollDisplayPixels;
public:
    bool isNeeded;
    GPE_Slider_XAxis( float sVal  = 50, float sMin = 0, float sMax = 100 );
    ~GPE_Slider_XAxis();
    float get_value();
    bool has_moved();
    bool is_held();
    bool is_scrolling();
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void reset_scroller();
    void set_data( float sVal  = 50, float sMin = 0, float sMax = 100 );
    bool set_value( float sVal );
};

class GPE_Slider_YAxis: public GPE_GeneralGuiElement
{
protected:
    bool isOnScrollBox;
    float mouseYScrolPos;
    bool hasMoved;
    bool isHeldOn;
    float scrollDisplayPixels;
public:
    bool isNeeded;
    GPE_Slider_YAxis( float sVal  = 50, float sMin = 0, float sMax = 100 );
    ~GPE_Slider_YAxis();
    float get_value();
    float get_box_width();
    float get_box_height();
    bool has_moved();
    bool is_held();
    bool is_scrolling();
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void reset_scroller();
    void set_data( float sVal  = 50, float sMin = 0, float sMax = 100 );
    bool set_value( float sVal );
};

#endif
