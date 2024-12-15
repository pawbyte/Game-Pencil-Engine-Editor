/*
pawgui_scrollbars.h
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

#ifndef PAWGUI_SCROLLBARS_H
#define PAWGUI_SCROLLBARS_H

#include "../base/pawgui_base.h"

namespace pawgui
{
    extern gpe::animation2d  * main_scrollbar_arrow;

    class widget_scrollbar_xaxis: public widget_basic
    {
        protected:
            bool isHeldOn;
            bool isOnScrollBox;
            float mouseXScrolPos;
            bool hasMoved;
        public:
            bool autoCorrect;
            gpe::shape_rect contextRect;
            gpe::shape_rect fullRect;
            float scrollWidth, scrollPercent, scrollPixels, scrollXPos, scrollDisplayPixels;
            bool isNeeded;
            widget_scrollbar_xaxis();
            ~widget_scrollbar_xaxis();
            bool calculate_sizing();
            int get_box_width();
            int get_box_height();
            bool has_moved();
            bool is_held();
            bool is_scrolling();
            void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr );
            void reset_scroller();
    };

    class widget_scrollbar_yaxis: public widget_basic
    {
        protected:
            bool isHeldOn;
            bool isOnScrollBox;
            float mouseYScrolPos;
            bool hasMoved;
        public:
            bool autoCorrect;
            bool documentActions;
            gpe::shape_rect contextRect;
            gpe::shape_rect fullRect;
            float scrollHeight, scrollPercent, scrollYPos, scrollDisplayPixels;
            bool isNeeded;
            widget_scrollbar_yaxis();
            ~widget_scrollbar_yaxis();
            bool calculate_sizing();
            int get_box_width();
            int get_box_height();
            bool has_moved();
            bool is_held();
            bool is_scrolling();
            void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr );
            void reset_scroller();
    };
}
#endif //PAWGUI_SCROLLBARS_H
