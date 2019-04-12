/*
paw_gui_sliders.h
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

#ifndef PAW_GUI_SLIDERS_H
#define PAW_GUI_SLIDERS_H

#include "paw_gui_base.h"

class GPE_Slider_XAxis: public GPE_GeneralGuiElement
{
protected:
    bool isOnScrollBox;
    int mouseXScrolPos;
    bool hasMoved;
    bool isHeldOn;
    int myValue;
    int minValue;
    int maxValue;
    int currentSliderXPos;
    int scrollDisplayPixels;
public:
    bool isNeeded;
    GPE_Slider_XAxis( int sVal  = 50, int sMin = 0, int sMax = 100 );
    ~GPE_Slider_XAxis();
    int get_value();
    bool has_moved();
    bool is_held();
    bool is_scrolling();
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    void reset_scroller();
    void set_data( int sVal  = 50, int sMin = 0, int sMax = 100 );
    bool set_value( int sVal );
};

class GPE_Slider_YAxis: public GPE_GeneralGuiElement
{
protected:
    bool isOnScrollBox;
    int mouseYScrolPos;
    bool hasMoved;
    bool isHeldOn;
    int scrollDisplayPixels;
public:
    bool isNeeded;
    GPE_Slider_YAxis( int sVal  = 50, int sMin = 0, int sMax = 100 );
    ~GPE_Slider_YAxis();
    int get_value();
    int get_box_width();
    int get_box_height();
    bool has_moved();
    bool is_held();
    bool is_scrolling();
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    void reset_scroller();
    void set_data( int sVal  = 50, int sMin = 0, int sMax = 100 );
    bool set_value( int sVal );
};

#endif
