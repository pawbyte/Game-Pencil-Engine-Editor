/*
pawgui_sliders.h
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

#ifndef PAWGUI_SLIDERS_H
#define PAWGUI_SLIDERS_H

#include "../base/pawgui_base.h"


namespace pawgui
{
    class widget_slide_xaxis: public widget_basic
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
            widget_slide_xaxis( float s_val  = 50, float sMin = 0, float sMax = 100 );
            ~widget_slide_xaxis();
            float get_value();
            bool has_moved();
            bool is_held();
            bool is_scrolling();
            void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void reset_scroller();
            void set_data( float s_val  = 50, float sMin = 0, float sMax = 100 );
            bool set_value( float s_val );
    };

    class widget_slider_yaxis: public widget_basic
    {
        protected:
            bool isOnScrollBox;
            float mouseYScrolPos;
            bool hasMoved;
            bool isHeldOn;
            float scrollDisplayPixels;
        public:
            bool isNeeded;
            widget_slider_yaxis( float s_val  = 50, float sMin = 0, float sMax = 100 );
            ~widget_slider_yaxis();
            float get_value();
            float get_box_width();
            float get_box_height();
            bool has_moved();
            bool is_held();
            bool is_scrolling();
            void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void reset_scroller();
            void set_data( float s_val  = 50, float sMin = 0, float sMax = 100 );
            bool set_value( float s_val );
    };
}
#endif //PAWGUI_SLIDERS_H
