/*
paw_gui_sliders.cpp
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

#include "paw_gui_sliders.h"


GPE_Slider_XAxis::GPE_Slider_XAxis( float sVal, float sMin, float sMax )
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
    myValue = sVal;
    set_data( sVal, sMin, sMax );
}

GPE_Slider_XAxis::~GPE_Slider_XAxis()
{

}

float GPE_Slider_XAxis::get_value()
{
    return myValue;
}

bool GPE_Slider_XAxis::has_moved()
{
    return hasMoved;
}

bool GPE_Slider_XAxis::is_held()
{
    return isHeldOn;
}

bool GPE_Slider_XAxis::is_scrolling()
{
    return isOnScrollBox;
}

void GPE_Slider_XAxis::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam)
{
    cam = gpe::camera_find(cam);
    viewedSpace = gpe::camera_find(viewedSpace);
    bool doWork = false;
    //calculates the height needed to be scrolled.
    hasMoved = false;
    isHeldOn = false;

    if( gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,viewedSpace->x+widget_box.x-cam->x,viewedSpace->y+widget_box.y-cam->y,viewedSpace->x+widget_box.x+widget_box.w-cam->x,viewedSpace->y+widget_box.y+widget_box.h-cam->y) )
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

    if(viewedSpace->x+widget_box.x-cam->x <= gpe::input->mouse_position_x && gpe::input->mouse_position_x <= viewedSpace->x+widget_box.x+widget_box.w-cam->x )
    {
        if( gpe::input->check_mouse_down(0 ) && isOnScrollBox)
        {
            mouseXScrolPos = gpe::input->mouse_position_x - viewedSpace->x-widget_box.x-cam->x; //translates mouse coords to viewed space coords
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

void GPE_Slider_XAxis::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( cam!=NULL && viewedSpace!=NULL )
    {
        gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y+4,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y-4,theme_main->scroll_box_color,false);
        gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y+4,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y-4,theme_main->scroll_box_border_color,true);

        float scrollRenderPixels = 24.f;
        if( scrollRenderPixels < widget_box.h)
        {
            scrollRenderPixels = widget_box.h;
        }
        float scrollPreviewX = std::min( currentSliderXPos, (float)(widget_box.w-scrollRenderPixels) );

        if( isOnScrollBox )
        {
            gpe::gcanvas->render_rectangle( widget_box.x+scrollPreviewX-cam->x,widget_box.y-cam->y,widget_box.x+scrollPreviewX+scrollRenderPixels-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->scroll_box_camera_highlight_color,false);
        }
        else if( hasArrowkeyControl)
        {
            gpe::gcanvas->render_rectangle( widget_box.x+scrollPreviewX-cam->x,widget_box.y-cam->y,widget_box.x+scrollPreviewX+scrollRenderPixels-cam->x,widget_box.y+widget_box.h-cam->y, gpe::c_red,false );
        }
        else
        {
            gpe::gcanvas->render_rectangle( widget_box.x+scrollPreviewX-cam->x,widget_box.y-cam->y,widget_box.x+scrollPreviewX+scrollRenderPixels-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->scroll_box_camera_color,false);
        }
    }
}

void GPE_Slider_XAxis::set_data( float sVal, float sMin, float sMax  )
{
    minValue = sMin;
    max_value = sMax;
    if( sMin > sMax)
    {
        minValue = sMax;
        max_value = sMin;
    }
    set_value( sVal);
}

bool GPE_Slider_XAxis::set_value( float sVal )
{
    myValue = sVal;
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
