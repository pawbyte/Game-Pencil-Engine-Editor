/*
pawgui_scrollbars.cpp
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

#include "pawgui_scrollbars.h"

namespace pawgui
{
    gpe::animation2d  * main_scrollbar_arrow = nullptr;

    widget_scrollbar_xaxis::widget_scrollbar_xaxis()
    {
        autoCorrect = true;
        widget_box.x = contextRect.x =  fullRect.x = 0;
        widget_box.y = contextRect.y =  fullRect.y = 0;
        widget_box.w = contextRect.w =  fullRect.w = padding_default / 2;
        widget_box.h = contextRect.h =  fullRect.h = padding_default / 2;


        scrollWidth = padding_default;
        scrollPercent = 100;
        scrollPixels = 16;
        scrollXPos = 0;
        scrollDisplayPixels = 0;
        hasMoved = false;
        mouseXScrolPos =0;
        isOnScrollBox = false;
        isHeldOn = false;
    }

    widget_scrollbar_xaxis::~widget_scrollbar_xaxis()
    {

    }

    bool widget_scrollbar_xaxis::calculate_sizing()
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

    int widget_scrollbar_xaxis::get_box_width()
    {
        return widget_box.w;
    }

    int widget_scrollbar_xaxis::get_box_height()
    {
        return widget_box.h;
    }

    bool widget_scrollbar_xaxis::has_moved()
    {
        return hasMoved;
    }

    bool widget_scrollbar_xaxis::is_held()
    {
        return isHeldOn;
    }

    bool widget_scrollbar_xaxis::is_scrolling()
    {
        return isOnScrollBox;
    }

    void widget_scrollbar_xaxis::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        cam = gpe::camera_find(cam);
        view_space = gpe::camera_find(view_space);
        bool doWork = true;

        calculate_sizing();


        float prevPosition = scrollXPos;
        hasMoved = false;
        isHeldOn = false;
        if( doWork)
        {
            if( gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,view_space->x+widget_box.x-cam->x,view_space->y+widget_box.y-cam->y,view_space->x+widget_box.x+widget_box.w-cam->x,view_space->y+widget_box.y+widget_box.h-cam->y) )
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

            if(view_space->x+widget_box.x-cam->x <= gpe::input->mouse_position_x && gpe::input->mouse_position_x <= view_space->x+widget_box.x+widget_box.w-cam->x )
            {
                if( gpe::input->check_mouse_down( mb_left ) && isOnScrollBox)
                {
                    mouseXScrolPos = gpe::input->mouse_position_x - view_space->x-widget_box.x-cam->x; //translates mouse coords to viewed space coords
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

    void widget_scrollbar_xaxis::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        cam = gpe::camera_find(cam);
        view_space = gpe::camera_find(view_space);

        if( cam!=nullptr && view_space!=nullptr)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->scroll_box_color,false);
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->scroll_box_border_color,true);
            int scrollRenderPixels = scrollDisplayPixels;
            if( scrollRenderPixels < widget_box.h)
            {
                scrollRenderPixels = widget_box.h;
            }
            if( isOnScrollBox )
            {
                gpe::gcanvas->render_rectangle( widget_box.x+scrollXPos-cam->x,widget_box.y-cam->y,widget_box.x+scrollXPos+scrollRenderPixels-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->scroll_box_camera_highlight_color,false);
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x+scrollXPos-cam->x,widget_box.y-cam->y,widget_box.x+scrollXPos+scrollRenderPixels-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->scroll_box_camera_color,false);
            }
            /*
            render_animation_special( main_scrollbar_arrow,2,
                                 widget_box.x+widget_box.h/4-cam->x,widget_box.y+widget_box.h/4-cam->y,
                                 widget_box.h/2,widget_box.h/2, pawgui::theme_main->scroll_box_arrow_color );
            render_animation_special( main_scrollbar_arrow,0,widget_box.x+widget_box.w-widget_box.h*3/4-cam->x,  widget_box.y+widget_box.h/4-cam->y,widget_box.h/2,widget_box.h/2,pawgui::theme_main->scroll_box_arrow_color );
            */
        }
    }

    void widget_scrollbar_xaxis::reset_scroller()
    {
        hasMoved = false;
        isOnScrollBox = false;
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 0;
        widget_box.h = 0;
    }

    widget_scrollbar_yaxis::widget_scrollbar_yaxis()
    {
        autoCorrect = true;
        documentActions = false;
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = padding_default/2;
        widget_box.h = padding_default/2;

        contextRect.x = fullRect.x = 0;
        contextRect.y = fullRect.y = 0;
        contextRect.w = fullRect.w = padding_default/2;
        contextRect.h = fullRect.h = padding_default/2;

        scrollHeight = padding_default;
        scrollPercent = 100;
        scrollDisplayPixels = padding_default/2;
        scrollYPos = 0;

        scrollYPos = 0;
        mouseYScrolPos = scrollYPos;
        isOnScrollBox = false;
        hasMoved = false;
        isHeldOn = false;
    }

    widget_scrollbar_yaxis::~widget_scrollbar_yaxis()
    {

    }

    bool widget_scrollbar_yaxis::calculate_sizing()
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

    int widget_scrollbar_yaxis::get_box_width()
    {
        return widget_box.w;
    }

    int widget_scrollbar_yaxis::get_box_height()
    {
        return widget_box.h;
    }

    bool widget_scrollbar_yaxis::has_moved()
    {
        return hasMoved;
    }

    bool widget_scrollbar_yaxis::is_held()
    {
        return isHeldOn;
    }

    bool widget_scrollbar_yaxis::is_scrolling()
    {
        return isOnScrollBox;
    }

    void widget_scrollbar_yaxis::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        cam = gpe::camera_find(cam);
        view_space = gpe::camera_find(view_space);
        bool doWork = true;
        if( view_space!=nullptr && cam!=nullptr )
        {
            doWork = calculate_sizing();
            hasMoved = false;
            isHeldOn = false;
            if( doWork)
            {
                if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,widget_box.x+view_space->x-cam->x,widget_box.y+view_space->y-cam->y,widget_box.x+widget_box.w+view_space->x-cam->x,widget_box.y+widget_box.h+view_space->y-cam->y) )
                {
                    if( gpe::input->check_mouse_pressed(0 ) )
                    {
                        isOnScrollBox = true;
                    }
                }

                if(widget_box.y+view_space->y-cam->y <= gpe::input->mouse_position_y && gpe::input->mouse_position_y <= widget_box.y+widget_box.h+view_space->y-cam->y )
                {
                    if( gpe::input->check_mouse_down( mb_left ) && isOnScrollBox)
                    {
                        mouseYScrolPos = gpe::input->mouse_position_y - view_space->y-widget_box.y+cam->y; //translates mouse coords to viewed space coords
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

    void widget_scrollbar_yaxis::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam  )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( view_space!=nullptr && cam!=nullptr )
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->scroll_box_color,false);
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->scroll_box_border_color,true);
            int scrollRenderPixels = scrollDisplayPixels;
            if( scrollRenderPixels < widget_box.w)
            {
                scrollRenderPixels = widget_box.w;
            }
            if( isOnScrollBox )
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y+scrollYPos-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+scrollYPos+scrollRenderPixels-cam->y,pawgui::theme_main->scroll_box_camera_highlight_color,false);
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y+scrollYPos-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+scrollYPos+scrollRenderPixels-cam->y,pawgui::theme_main->scroll_box_camera_color,false);
            }
            /*
            render_animation_special( main_scrollbar_arrow,1,
                                 widget_box.x+widget_box.w/4-cam->x,widget_box.y+widget_box.w/4-cam->y,
                                 widget_box.w/2,widget_box.w/2,
                                 pawgui::theme_main->scroll_box_arrow_color);
            render_animation_special( main_scrollbar_arrow,3,widget_box.x+widget_box.w/4-cam->x,widget_box.y+widget_box.h-widget_box.w*3/4-cam->y,widget_box.w/2,widget_box.w/2,pawgui::theme_main->scroll_box_arrow_color);
            */
        }
    }

    void widget_scrollbar_yaxis::reset_scroller()
    {
        isOnScrollBox = false;
        hasMoved = false;
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 0;
        widget_box.h = 0;

    }
}
