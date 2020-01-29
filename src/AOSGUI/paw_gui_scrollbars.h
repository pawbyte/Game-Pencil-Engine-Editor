/*
paw_gui_scrollbars.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef paw_gui_scrollbars_H
#define paw_gui_scrollbars_H

#include "paw_gui_base.h"

extern GPE_Animation  * GPE_SCROLLBAR_ARROW;

class GPE_ScrollBar_XAxis: public GPE_GeneralGuiElement
{
protected:
    bool isHeldOn;
    bool isOnScrollBox;
    float mouseXScrolPos;
    bool hasMoved;
public:
    bool autoCorrect;
    GPE_Rect contextRect;
    GPE_Rect fullRect;
    float scrollWidth, scrollPercent, scrollPixels, scrollXPos, scrollDisplayPixels;
    bool isNeeded;
    GPE_ScrollBar_XAxis();
    ~GPE_ScrollBar_XAxis();
    bool calculate_sizing();
    int get_box_width();
    int get_box_height();
    bool has_moved();
    bool is_held();
    bool is_scrolling();
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL );
    void reset_scroller();
};

class GPE_ScrollBar_YAxis: public GPE_GeneralGuiElement
{
protected:
    bool isHeldOn;
    bool isOnScrollBox;
    float mouseYScrolPos;
    bool hasMoved;
public:
    bool autoCorrect;
    bool documentActions;
    GPE_Rect contextRect;
    GPE_Rect fullRect;
    float scrollHeight, scrollPercent, scrollYPos, scrollDisplayPixels;
    bool isNeeded;
    GPE_ScrollBar_YAxis();
    ~GPE_ScrollBar_YAxis();
    bool calculate_sizing();
    int get_box_width();
    int get_box_height();
    bool has_moved();
    bool is_held();
    bool is_scrolling();
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL );
    void reset_scroller();
};

#endif
