/*
paw_gui_scrollbars.cpp
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

#include "paw_gui_scrollbars.h"

gpe::animaton2d  * GPE_SCROLLBAR_ARROW = NULL;

GPE_ScrollBar_XAxis::GPE_ScrollBar_XAxis()
{
    autoCorrect = true;
    widget_box.x = contextRect.x =  fullRect.x = 0;
    widget_box.y = contextRect.y =  fullRect.y = 0;
    widget_box.w = contextRect.w =  fullRect.w = GENERAL_GPE_GUI_PADDING / 2;
    widget_box.h = contextRect.h =  fullRect.h = GENERAL_GPE_GUI_PADDING / 2;


    scrollWidth = GENERAL_GPE_GUI_PADDING;
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
            scrollPercent =  (float)contextRect.w/(float)fullRect.w;
            scrollDisplayPixels = (float)(widget_box.w)*scrollPercent;
            scrollXPos = (float)(contextRect.x)/(float)(fullRect.w)*(float)widget_box.w;
        }
        else
        {
            scrollPercent = 1;
            scrollDisplayPixels = widget_box.w;
            scrollXPos = 0;
            contextRect.x = 0;
            return false;
        }
    }
    return true;
}

int GPE_ScrollBar_XAxis::get_box_width()
{
    return widget_box.w;
}

int GPE_ScrollBar_XAxis::get_box_height()
{
    return widget_box.h;
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

void GPE_ScrollBar_XAxis::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    cam = gpe::camera_find(cam);
    viewedSpace = gpe::camera_find(viewedSpace);
    bool doWork = true;

    calculate_sizing();


    float prevPosition = scrollXPos;
    hasMoved = false;
    isHeldOn = false;
    if( doWork)
    {
        if( gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,viewedSpace->x+widget_box.x-cam->x,viewedSpace->y+widget_box.y-cam->y,viewedSpace->x+widget_box.x+widget_box.w-cam->x,viewedSpace->y+widget_box.y+widget_box.h-cam->y) )
        {
            if( gpe::input->check_mouse_pressed( mb_left ) )
            {
                isOnScrollBox = true;
                isHeldOn = false;
            }
            else if( gpe::input->check_mouse_down( mb_left ) )
            {
                if( !isOnScrollBox)
                {
                    isHeldOn = true;
                }
            }
        }
        else if( gpe::input->check_mouse_released( mb_anybutton ) )
        {
            isOnScrollBox = false;
            isHeldOn = false;
        }

        if(viewedSpace->x+widget_box.x-cam->x <= gpe::input->mouse_position_x && gpe::input->mouse_position_x <= viewedSpace->x+widget_box.x+widget_box.w-cam->x )
        {
            if( gpe::input->check_mouse_down( mb_left ) && isOnScrollBox)
            {
                mouseXScrolPos = gpe::input->mouse_position_x - viewedSpace->x-widget_box.x-cam->x; //translates mouse coords to viewed space coords
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

        if( gpe::input->check_mouse_released( mb_left)|| gpe::input->check_mouse_released(1))
        {
            if( isOnScrollBox)
            {
                gpe::input->reset_all_input();
            }
            isOnScrollBox = false;
            isHeldOn = false;
        }

        if( autoCorrect)
        {
            if( scrollXPos > widget_box.w- scrollDisplayPixels)
            {
                scrollXPos = widget_box.w- scrollDisplayPixels;
            }

            if( scrollXPos < 0)
            {
                scrollXPos = 0;
            }
        }

        prevPosition = contextRect.x;
        contextRect.x =  ( scrollXPos/widget_box.w )*fullRect.w;

        if( contextRect.x!=prevPosition)
        {
            hasMoved = true;
        }
    }
    else
    {
        scrollPercent = 1;
        scrollDisplayPixels = widget_box.w;
        scrollXPos = 0;
        contextRect.x = 0;
        //hasMoved = true;
    }

}

void GPE_ScrollBar_XAxis::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    cam = gpe::camera_find(cam);
    viewedSpace = gpe::camera_find(viewedSpace);

    if( cam!=NULL && viewedSpace!=NULL)
    {
        gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->scroll_box_color,false);
        gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->scroll_box_border_color,true);
        int scrollRenderPixels = scrollDisplayPixels;
        if( scrollRenderPixels < widget_box.h)
        {
            scrollRenderPixels = widget_box.h;
        }
        if( isOnScrollBox )
        {
            gpe::gcanvas->render_rectangle( widget_box.x+scrollXPos-cam->x,widget_box.y-cam->y,widget_box.x+scrollXPos+scrollRenderPixels-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->scroll_box_camera_highlight_color,false);
        }
        else
        {
            gpe::gcanvas->render_rectangle( widget_box.x+scrollXPos-cam->x,widget_box.y-cam->y,widget_box.x+scrollXPos+scrollRenderPixels-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->scroll_box_camera_color,false);
        }
        /*
        render_animation_special( GPE_SCROLLBAR_ARROW,2,
                             widget_box.x+widget_box.h/4-cam->x,widget_box.y+widget_box.h/4-cam->y,
                             widget_box.h/2,widget_box.h/2, theme_main->scroll_box_arrow_color );
        render_animation_special( GPE_SCROLLBAR_ARROW,0,widget_box.x+widget_box.w-widget_box.h*3/4-cam->x,  widget_box.y+widget_box.h/4-cam->y,widget_box.h/2,widget_box.h/2,theme_main->scroll_box_arrow_color );
        */
    }
}

void GPE_ScrollBar_XAxis::reset_scroller()
{
    hasMoved = false;
    isOnScrollBox = false;
    widget_box.x = 0;
    widget_box.y = 0;
    widget_box.w = 0;
    widget_box.h = 0;
}

GPE_ScrollBar_YAxis::GPE_ScrollBar_YAxis()
{
    autoCorrect = true;
    documentActions = false;
    widget_box.x = 0;
    widget_box.y = 0;
    widget_box.w = GENERAL_GPE_GUI_PADDING/2;
    widget_box.h = GENERAL_GPE_GUI_PADDING/2;

    contextRect.x = fullRect.x = 0;
    contextRect.y = fullRect.y = 0;
    contextRect.w = fullRect.w = GENERAL_GPE_GUI_PADDING/2;
    contextRect.h = fullRect.h = GENERAL_GPE_GUI_PADDING/2;

    scrollHeight = GENERAL_GPE_GUI_PADDING;
    scrollPercent = 100;
    scrollDisplayPixels = GENERAL_GPE_GUI_PADDING/2;
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
            scrollDisplayPixels = (widget_box.h)*scrollPercent;
            scrollYPos = ( contextRect.y/fullRect.h )*widget_box.h;
        }
        else
        {
            scrollPercent = 1;
            scrollDisplayPixels = widget_box.h;
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
    return widget_box.w;
}

int GPE_ScrollBar_YAxis::get_box_height()
{
    return widget_box.h;
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

void GPE_ScrollBar_YAxis::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    cam = gpe::camera_find(cam);
    viewedSpace = gpe::camera_find(viewedSpace);
    bool doWork = true;
    if( viewedSpace!=NULL && cam!=NULL )
    {
        doWork = calculate_sizing();
        hasMoved = false;
        isHeldOn = false;
        if( doWork)
        {
            if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,widget_box.x+viewedSpace->x-cam->x,widget_box.y+viewedSpace->y-cam->y,widget_box.x+widget_box.w+viewedSpace->x-cam->x,widget_box.y+widget_box.h+viewedSpace->y-cam->y) )
            {
                if( gpe::input->check_mouse_pressed(0 ) )
                {
                    isOnScrollBox = true;
                }
            }

            if(widget_box.y+viewedSpace->y-cam->y <= gpe::input->mouse_position_y && gpe::input->mouse_position_y <= widget_box.y+widget_box.h+viewedSpace->y-cam->y )
            {
                if( gpe::input->check_mouse_down( mb_left ) && isOnScrollBox)
                {
                    mouseYScrolPos = gpe::input->mouse_position_y - viewedSpace->y-widget_box.y+cam->y; //translates mouse coords to viewed space coords
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

            if( gpe::input->check_mouse_released( mb_left)|| gpe::input->check_mouse_released(1))
            {
                if( isOnScrollBox)
                {
                    gpe::input->reset_all_input();
                }
                isOnScrollBox = false;
                isHeldOn = false;
            }
            float prevPosition = contextRect.y;
            //if( scrollPercent)
            {
                if( scrollYPos+scrollDisplayPixels > widget_box.h )
                {
                    scrollYPos = widget_box.h- scrollDisplayPixels;
                }

                if( scrollYPos < 0)
                {
                    scrollYPos = 0;
                }
            }
            contextRect.y =  ( scrollYPos/widget_box.h )*fullRect.h;

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
            scrollDisplayPixels = widget_box.h;
            scrollYPos = 0;
            contextRect.y = 0;
        }
    }
}

void GPE_ScrollBar_YAxis::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam  )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( viewedSpace!=NULL && cam!=NULL )
    {
        gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->scroll_box_color,false);
        gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->scroll_box_border_color,true);
        int scrollRenderPixels = scrollDisplayPixels;
        if( scrollRenderPixels < widget_box.w)
        {
            scrollRenderPixels = widget_box.w;
        }
        if( isOnScrollBox )
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y+scrollYPos-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+scrollYPos+scrollRenderPixels-cam->y,theme_main->scroll_box_camera_highlight_color,false);
        }
        else
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y+scrollYPos-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+scrollYPos+scrollRenderPixels-cam->y,theme_main->scroll_box_camera_color,false);
        }
        /*
        render_animation_special( GPE_SCROLLBAR_ARROW,1,
                             widget_box.x+widget_box.w/4-cam->x,widget_box.y+widget_box.w/4-cam->y,
                             widget_box.w/2,widget_box.w/2,
                             theme_main->scroll_box_arrow_color);
        render_animation_special( GPE_SCROLLBAR_ARROW,3,widget_box.x+widget_box.w/4-cam->x,widget_box.y+widget_box.h-widget_box.w*3/4-cam->y,widget_box.w/2,widget_box.w/2,theme_main->scroll_box_arrow_color);
        */
    }
}

void GPE_ScrollBar_YAxis::reset_scroller()
{
    isOnScrollBox = false;
    hasMoved = false;
    widget_box.x = 0;
    widget_box.y = 0;
    widget_box.w = 0;
    widget_box.h = 0;

}