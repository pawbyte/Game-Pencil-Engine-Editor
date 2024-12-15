/*
pawgui_sliders.cpp
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

#include "pawgui_sliders.h"

namespace pawgui
{
    widget_slide_xaxis::widget_slide_xaxis( float s_val, float sMin, float sMax )
    {
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 192;
        widget_box.h = 8;
        scrollDisplayPixels = 24;
        currentSliderXPos = 0;
        isHeldOn = false;
        hasMoved = false;
        isOnScrollBox = false;
        mouseXScrolPos = 0;
        //Will let the function do optimizations needed below
        minValue = sMin;
        max_value = sMax;
        myValue = s_val;
        set_data( s_val, sMin, sMax );
    }

    widget_slide_xaxis::~widget_slide_xaxis()
    {

    }

    float widget_slide_xaxis::get_value()
    {
        return myValue;
    }

    bool widget_slide_xaxis::has_moved()
    {
        return hasMoved;
    }

    bool widget_slide_xaxis::is_held()
    {
        return isHeldOn;
    }

    bool widget_slide_xaxis::is_scrolling()
    {
        return isOnScrollBox;
    }

    void widget_slide_xaxis::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
    {
        cam = gpe::camera_find(cam);
        view_space = gpe::camera_find(view_space);
        bool doWork = false;
        //calculates the height needed to be scrolled.
        hasMoved = false;
        isHeldOn = false;

        if( gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,view_space->x+widget_box.x-cam->x,view_space->y+widget_box.y-cam->y,view_space->x+widget_box.x+widget_box.w-cam->x,view_space->y+widget_box.y+widget_box.h-cam->y) )
        {
            if( gpe::input->check_mouse_pressed( mb_left ) )
            {
                isOnScrollBox = true;
                isHeldOn = false;
                hasArrowkeyControl = true;
                hasScrollControl = true;
            }
            else if( gpe::input->check_mouse_down( mb_left ) )
            {
                if( !isOnScrollBox)
                {
                    isHeldOn = true;
                    hasArrowkeyControl = true;
                    hasScrollControl = true;
                }
            }
        }
        else if( gpe::input->check_mouse_released( mb_anybutton ) )
        {
            isOnScrollBox = false;
            isHeldOn = false;
            hasArrowkeyControl = false;
            hasScrollControl = false;
        }

        if(view_space->x+widget_box.x-cam->x <= gpe::input->mouse_position_x && gpe::input->mouse_position_x <= view_space->x+widget_box.x+widget_box.w-cam->x )
        {
            if( gpe::input->check_mouse_down(0 ) && isOnScrollBox)
            {
                mouseXScrolPos = gpe::input->mouse_position_x - view_space->x-widget_box.x-cam->x; //translates mouse coords to viewed space coords
                if( isOnScrollBox)
                {
                    currentSliderXPos = mouseXScrolPos;
                    doWork = true;
                }
            }
        }
        else
        {
            isOnScrollBox = false;
            isHeldOn = false;
        }


        if( gpe::input->check_mouse_released( mb_left)|| gpe::input->check_mouse_released(1))
        {
            if( isOnScrollBox)
            {
                gpe::input->reset_all_input();
            }
            isOnScrollBox = false;
            isHeldOn = false;
        }


        if( currentSliderXPos > widget_box.w )
        {
            currentSliderXPos = widget_box.w;
            doWork = true;
        }

        if( currentSliderXPos < 0)
        {
            currentSliderXPos = 0;
            doWork = true;
        }

        if( doWork )
        {
            set_value( minValue+ ( ( (float)currentSliderXPos)/widget_box.w )*( max_value-minValue ) );
        }

        if( hasArrowkeyControl )
        {
            float scrollRenderPixels = 8;

            if( gpe::input->check_kb_down(kb_left) && !gpe::input->check_kb_pressed(kb_left)  )
            {
                set_value( myValue-scrollRenderPixels );
            }
            else if( gpe::input->check_kb_down(kb_right)&& !gpe::input->check_kb_pressed(kb_right)   )
            {
                set_value( myValue+scrollRenderPixels );
            }
        }
    }

    void widget_slide_xaxis::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( cam!=nullptr && view_space!=nullptr )
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y+4,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y-4,pawgui::theme_main->scroll_box_color,false);
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y+4,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y-4,pawgui::theme_main->scroll_box_border_color,true);

            float scrollRenderPixels = 24.f;
            if( scrollRenderPixels < widget_box.h)
            {
                scrollRenderPixels = widget_box.h;
            }
            float scrollPreviewX = std::min( currentSliderXPos, (float)(widget_box.w-scrollRenderPixels) );

            if( isOnScrollBox )
            {
                gpe::gcanvas->render_rectangle( widget_box.x+scrollPreviewX-cam->x,widget_box.y-cam->y,widget_box.x+scrollPreviewX+scrollRenderPixels-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->scroll_box_camera_highlight_color,false);
            }
            else if( hasArrowkeyControl)
            {
                gpe::gcanvas->render_rectangle( widget_box.x+scrollPreviewX-cam->x,widget_box.y-cam->y,widget_box.x+scrollPreviewX+scrollRenderPixels-cam->x,widget_box.y+widget_box.h-cam->y, gpe::c_red,false );
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x+scrollPreviewX-cam->x,widget_box.y-cam->y,widget_box.x+scrollPreviewX+scrollRenderPixels-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->scroll_box_camera_color,false);
            }
        }
    }

    void widget_slide_xaxis::set_data( float s_val, float sMin, float sMax  )
    {
        minValue = sMin;
        max_value = sMax;
        if( sMin > sMax)
        {
            minValue = sMax;
            max_value = sMin;
        }
        set_value( s_val);
    }

    bool widget_slide_xaxis::set_value( float s_val )
    {
        myValue = s_val;
        if( myValue < minValue)
        {
            myValue = minValue;
        }
        else if( max_value < myValue )
        {
            myValue = max_value;
        }
        currentSliderXPos =  ( widget_box.w )*( ( (float)myValue-(float)minValue) / ( (float)max_value-(float)minValue ) );
        return true;
    }
}
