/*
paw_gui_scrollbars.cpp
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

#include "paw_gui_scrollbars.h"

GPE_Animation  * GPE_SCROLLBAR_ARROW = NULL;

GPE_ScrollBar_XAxis::GPE_ScrollBar_XAxis()
{
    autoCorrect = true;
    elementBox.x = contextRect.x =  fullRect.x = 0;
    elementBox.y = contextRect.y =  fullRect.y = 0;
    elementBox.w = contextRect.w =  fullRect.w = 32;
    elementBox.h = contextRect.h =  fullRect.h = 8;


    scrollWidth = 32;
    scrollPercent = 100;
    scrollPixels = 16;
    scrollXPos = 0;
    scrollDisplayPixels = 0;
    hasMoved = false;
    mouseXScrolPos =0;
    isOnScrollBox = false;
    isHeldOn = false;
}

GPE_ScrollBar_XAxis::~GPE_ScrollBar_XAxis()
{

}

bool GPE_ScrollBar_XAxis::calculate_sizing()
{
    //calculates the height needed to be scrolled.
    if( fullRect.w!=0)
    {
        if( contextRect.w < fullRect.w)
        {
            scrollPercent =  (double)contextRect.w/(double)fullRect.w;
            scrollDisplayPixels = (double)(elementBox.w)*scrollPercent;
            scrollXPos = (double)(contextRect.x)/(double)(fullRect.w)*(double)elementBox.w;
        }
        else
        {
            scrollPercent = 1;
            scrollDisplayPixels = elementBox.w;
            scrollXPos = 0;
            contextRect.x = 0;
            return false;
        }
    }
    return true;
}

int GPE_ScrollBar_XAxis::get_box_width()
{
    return elementBox.w;
}

int GPE_ScrollBar_XAxis::get_box_height()
{
    return elementBox.h;
}

bool GPE_ScrollBar_XAxis::has_moved()
{
    return hasMoved;
}

bool GPE_ScrollBar_XAxis::is_held()
{
    return isHeldOn;
}

bool GPE_ScrollBar_XAxis::is_scrolling()
{
    return isOnScrollBox;
}

void GPE_ScrollBar_XAxis::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    bool doWork = true;

    calculate_sizing();


    double prevPosition = scrollXPos;
    hasMoved = false;
    isHeldOn = false;
    if( doWork)
    {
        if( point_within(input->mouse_x,input->mouse_y,viewedSpace->x+elementBox.x-cam->x,viewedSpace->y+elementBox.y-cam->y,viewedSpace->x+elementBox.x+elementBox.w-cam->x,viewedSpace->y+elementBox.y+elementBox.h-cam->y) )
        {
            if( input->check_mouse_pressed( mb_left ) )
            {
                isOnScrollBox = true;
                isHeldOn = false;
            }
            else if( input->check_mouse_down( mb_left ) )
            {
                if( !isOnScrollBox)
                {
                    isHeldOn = true;
                }
            }
        }
        else if( input->check_mouse_released( mb_anybutton ) )
        {
            isOnScrollBox = false;
            isHeldOn = false;
        }

        if(viewedSpace->x+elementBox.x-cam->x <= input->mouse_x && input->mouse_x <= viewedSpace->x+elementBox.x+elementBox.w-cam->x )
        {
            if( input->check_mouse_down( mb_left ) && isOnScrollBox)
            {
                mouseXScrolPos = input->mouse_x - viewedSpace->x-elementBox.x-cam->x; //translates mouse coords to viewed space coords
                if( isOnScrollBox)
                {
                    scrollXPos = mouseXScrolPos;
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

        if( autoCorrect)
        {
            if( scrollXPos > elementBox.w- scrollDisplayPixels)
            {
                scrollXPos = elementBox.w- scrollDisplayPixels;
            }

            if( scrollXPos < 0)
            {
                scrollXPos = 0;
            }
        }

        prevPosition = contextRect.x;
        contextRect.x =  ( scrollXPos/elementBox.w )*fullRect.w;

        if( contextRect.x!=prevPosition)
        {
            hasMoved = true;
        }
    }
    else
    {
        scrollPercent = 1;
        scrollDisplayPixels = elementBox.w;
        scrollXPos = 0;
        contextRect.x = 0;
        //hasMoved = true;
    }

}

void GPE_ScrollBar_XAxis::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam, bool forceRedraw )
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);

    if( cam!=NULL && viewedSpace!=NULL)
    {
        gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Color,false);
        gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Border_Color,true);
        int scrollRenderPixels = scrollDisplayPixels;
        if( scrollRenderPixels < elementBox.h)
        {
            scrollRenderPixels = elementBox.h;
        }
        if( isOnScrollBox )
        {
            gcanvas->render_rectangle( elementBox.x+scrollXPos-cam->x,elementBox.y-cam->y,elementBox.x+scrollXPos+scrollRenderPixels-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Camera_Highlight_Color,false);
        }
        else
        {
            gcanvas->render_rectangle( elementBox.x+scrollXPos-cam->x,elementBox.y-cam->y,elementBox.x+scrollXPos+scrollRenderPixels-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Camera_Color,false);
        }
        /*
        render_animation_special( GPE_SCROLLBAR_ARROW,2,
                             elementBox.x+elementBox.h/4-cam->x,elementBox.y+elementBox.h/4-cam->y,
                             elementBox.h/2,elementBox.h/2, GPE_MAIN_THEME->Scroll_Box_Arrow_Color );
        render_animation_special( GPE_SCROLLBAR_ARROW,0,elementBox.x+elementBox.w-elementBox.h*3/4-cam->x,  elementBox.y+elementBox.h/4-cam->y,elementBox.h/2,elementBox.h/2,GPE_MAIN_THEME->Scroll_Box_Arrow_Color );
        */
    }
}

void GPE_ScrollBar_XAxis::reset_scroller()
{
    hasMoved = false;
    isOnScrollBox = false;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 0;
    elementBox.h = 0;
}

GPE_ScrollBar_YAxis::GPE_ScrollBar_YAxis()
{
    autoCorrect = true;
    documentActions = false;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 8;
    elementBox.h = 16;

    contextRect.x = fullRect.x = 0;
    contextRect.y = fullRect.y = 0;
    contextRect.w = fullRect.w = 8;
    contextRect.h = fullRect.h = 32;

    scrollHeight = 32;
    scrollPercent = 100;
    scrollDisplayPixels = 16;
    scrollYPos = 0;

    scrollYPos = 0;
    mouseYScrolPos = scrollYPos;
    isOnScrollBox = false;
    hasMoved = false;
    isHeldOn = false;
}

GPE_ScrollBar_YAxis::~GPE_ScrollBar_YAxis()
{

}

bool GPE_ScrollBar_YAxis::calculate_sizing()
{
    //calculates the height needed to be scrolled.
    if( fullRect.h!=0)
    {
        if( contextRect.h < fullRect.h)
        {
            scrollPercent =  contextRect.h/fullRect.h;
            scrollDisplayPixels = (elementBox.h)*scrollPercent;
            scrollYPos = ( contextRect.y/fullRect.h )*elementBox.h;
        }
        else
        {
            scrollPercent = 1;
            scrollDisplayPixels = elementBox.h;
            scrollYPos = 0;
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

int GPE_ScrollBar_YAxis::get_box_width()
{
    return elementBox.w;
}

int GPE_ScrollBar_YAxis::get_box_height()
{
    return elementBox.h;
}

bool GPE_ScrollBar_YAxis::has_moved()
{
    return hasMoved;
}

bool GPE_ScrollBar_YAxis::is_held()
{
    return isHeldOn;
}

bool GPE_ScrollBar_YAxis::is_scrolling()
{
    return isOnScrollBox;
}

void GPE_ScrollBar_YAxis::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    bool doWork = true;
    if( viewedSpace!=NULL && cam!=NULL )
    {
        doWork = calculate_sizing();
        hasMoved = false;
        isHeldOn = false;
        if( doWork)
        {
            if( point_between(input->mouse_x,input->mouse_y,elementBox.x+viewedSpace->x-cam->x,elementBox.y+viewedSpace->y-cam->y,elementBox.x+elementBox.w+viewedSpace->x-cam->x,elementBox.y+elementBox.h+viewedSpace->y-cam->y) )
            {
                if( input->check_mouse_pressed(0 ) )
                {
                    isOnScrollBox = true;
                }
            }

            if(elementBox.y+viewedSpace->y-cam->y <= input->mouse_y && input->mouse_y <= elementBox.y+elementBox.h+viewedSpace->y-cam->y )
            {
                if( input->check_mouse_down( mb_left ) && isOnScrollBox)
                {
                    mouseYScrolPos = input->mouse_y - viewedSpace->y-elementBox.y+cam->y; //translates mouse coords to viewed space coords
                    if( isOnScrollBox)
                    {
                        scrollYPos = mouseYScrolPos;
                    }
                }
                else
                {
                    isOnScrollBox = false;
                    isHeldOn = false;
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
            double prevPosition = contextRect.y;
            //if( scrollPercent)
            {
                if( scrollYPos+scrollDisplayPixels > elementBox.h )
                {
                    scrollYPos = elementBox.h- scrollDisplayPixels;
                }

                if( scrollYPos < 0)
                {
                    scrollYPos = 0;
                }
            }
            contextRect.y =  ( scrollYPos/elementBox.h )*fullRect.h;

            //if( autoCorrect)
            {
                if( contextRect.y+contextRect.h > fullRect.h )
                {
                    contextRect.y = fullRect.h- contextRect.h;
                }

                if( contextRect.y < 0)
                {
                    contextRect.y = 0;
                }
            }
            if( contextRect.y!=prevPosition)
            {
                hasMoved = true;
            }
        }
        else
        {
            scrollPercent = 1;
            scrollDisplayPixels = elementBox.h;
            scrollYPos = 0;
            contextRect.y = 0;
        }
    }
}

void GPE_ScrollBar_YAxis::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam , bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( viewedSpace!=NULL && cam!=NULL )
    {
        gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Color,false);
        gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Scroll_Box_Border_Color,true);
        int scrollRenderPixels = scrollDisplayPixels;
        if( scrollRenderPixels < elementBox.w)
        {
            scrollRenderPixels = elementBox.w;
        }
        if( isOnScrollBox )
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y+scrollYPos-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+scrollYPos+scrollRenderPixels-cam->y,GPE_MAIN_THEME->Scroll_Box_Camera_Highlight_Color,false);
        }
        else
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y+scrollYPos-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+scrollYPos+scrollRenderPixels-cam->y,GPE_MAIN_THEME->Scroll_Box_Camera_Color,false);
        }
        /*
        render_animation_special( GPE_SCROLLBAR_ARROW,1,
                             elementBox.x+elementBox.w/4-cam->x,elementBox.y+elementBox.w/4-cam->y,
                             elementBox.w/2,elementBox.w/2,
                             GPE_MAIN_THEME->Scroll_Box_Arrow_Color);
        render_animation_special( GPE_SCROLLBAR_ARROW,3,elementBox.x+elementBox.w/4-cam->x,elementBox.y+elementBox.h-elementBox.w*3/4-cam->y,elementBox.w/2,elementBox.w/2,GPE_MAIN_THEME->Scroll_Box_Arrow_Color);
        */
    }
}

void GPE_ScrollBar_YAxis::reset_scroller()
{
    isOnScrollBox = false;
    hasMoved = false;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 0;
    elementBox.h = 0;

}
