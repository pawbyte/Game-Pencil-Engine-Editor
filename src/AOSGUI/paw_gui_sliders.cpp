/*
paw_gui_sliders.cpp
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

#include "paw_gui_sliders.h"


GPE_Slider_XAxis::GPE_Slider_XAxis( float sVal, float sMin, float sMax )
{
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 192;
    elementBox.h = 8;
    scrollDisplayPixels = 24;
    currentSliderXPos = 0;
    isHeldOn = false;
    hasMoved = false;
    isOnScrollBox = false;
    mouseXScrolPos = 0;
    //Will let the function do optimizations needed below
    minValue = sMin;
    maxValue = sMax;
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

void GPE_Slider_XAxis::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    bool doWork = false;
    //calculates the height needed to be scrolled.
    hasMoved = false;
    isHeldOn = false;

    if( point_within(input->mouse_x,input->mouse_y,viewedSpace->x+elementBox.x-cam->x,viewedSpace->y+elementBox.y-cam->y,viewedSpace->x+elementBox.x+elementBox.w-cam->x,viewedSpace->y+elementBox.y+elementBox.h-cam->y) )
    {
        if( input->check_mouse_pressed( mb_left ) )
        {
            isOnScrollBox = true;
            isHeldOn = false;
            hasArrowkeyControl = true;
            hasScrollControl = true;
        }
        else if( input->check_mouse_down( mb_left ) )
        {
            if( !isOnScrollBox)
            {
                isHeldOn = true;
                hasArrowkeyControl = true;
                hasScrollControl = true;
            }
        }
    }
    else if( input->check_mouse_released( mb_anybutton ) )
    {
        isOnScrollBox = false;
        isHeldOn = false;
        hasArrowkeyControl = false;
        hasScrollControl = false;
    }

    if(viewedSpace->x+elementBox.x-cam->x <= input->mouse_x && input->mouse_x <= viewedSpace->x+elementBox.x+elementBox.w-cam->x )
    {
        if( input->check_mouse_down(0 ) && isOnScrollBox)
        {
            mouseXScrolPos = input->mouse_x - viewedSpace->x-elementBox.x-cam->x; //translates mouse coords to viewed space coords
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


    if( input->check_mouse_released( mb_left)|| input->check_mouse_released(1))
    {
        if( isOnScrollBox)
        {
            input->reset_all_input();
        }
        isOnScrollBox = false;
        isHeldOn = false;
    }


    if( currentSliderXPos > elementBox.w )
    {
        currentSliderXPos = elementBox.w;
        doWork = true;
    }

    if( currentSliderXPos < 0)
    {
        currentSliderXPos = 0;
        doWork = true;
    }

    if( doWork )
    {
        set_value( minValue+ ( ( (float)currentSliderXPos)/elementBox.w )*( maxValue-minValue ) );
    }

    if( hasArrowkeyControl )
    {
        float scrollRenderPixels = 8;

        if( input->check_keyboard_down(kb_left) && !input->check_keyboard_pressed(kb_left)  )
        {
            set_value( myValue-scrollRenderPixels );
        }
        else if( input->check_keyboard_down(kb_right)&& !input->check_keyboard_pressed(kb_right)   )
        {
            set_value( myValue+scrollRenderPixels );
        }
    }
}

void GPE_Slider_XAxis::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam!=NULL && viewedSpace!=NULL )
    {
        gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y+4,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y-4,GPE_MAIN_THEME->Scroll_Box_Color,false);
        gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y+4,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y-4,GPE_MAIN_THEME->Scroll_Box_Border_Color,true);

        float scrollRenderPixels = 24.f;
        if( scrollRenderPixels < elementBox.h)
        {
            scrollRenderPixels = elementBox.h;
        }
        float scrollPreviewX = std::min( currentSliderXPos, (float)(elementBox.w-scrollRenderPixels) );

        if( isOnScrollBox )
        {
            gcanvas->render_rectangle( elementBox.x+scrollPreviewX-cam->x,elementBox.y-cam->y,elementBox.x+scrollPreviewX+scrollRenderPixels-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Camera_Highlight_Color,false);
        }
        else if( hasArrowkeyControl)
        {
            gcanvas->render_rectangle( elementBox.x+scrollPreviewX-cam->x,elementBox.y-cam->y,elementBox.x+scrollPreviewX+scrollRenderPixels-cam->x,elementBox.y+elementBox.h-cam->y, c_red,false );
        }
        else
        {
            gcanvas->render_rectangle( elementBox.x+scrollPreviewX-cam->x,elementBox.y-cam->y,elementBox.x+scrollPreviewX+scrollRenderPixels-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Camera_Color,false);
        }
    }
}

void GPE_Slider_XAxis::set_data( float sVal, float sMin, float sMax  )
{
    minValue = sMin;
    maxValue = sMax;
    if( sMin > sMax)
    {
        minValue = sMax;
        maxValue = sMin;
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
    else if( maxValue < myValue )
    {
        myValue = maxValue;
    }
    currentSliderXPos =  ( elementBox.w )*( ( (float)myValue-(float)minValue) / ( (float)maxValue-(float)minValue ) );
    return true;
}
