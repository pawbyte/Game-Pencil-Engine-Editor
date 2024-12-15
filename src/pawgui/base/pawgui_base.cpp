/*
pawgui_base.cpp
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

#include "pawgui_base.h"

namespace pawgui
{
    widget_basic::widget_basic()
    {
        r_packageName = "";
        alignment_h = gpe::fa_left;
        alignment_v = gpe::fa_top;
        dynamic_id = -1;
        needsNewLine = false;
        isFullWidth = false;
        indentationLevel = 0;
        widget_box.x =0;
        widget_box.y = 0;
        widget_box.w = 0;
        widget_box.h = 0;
        maxWidth = 0;
        maxHeight = 0;
        autoResizes = true;
        clickedOutside = false;
        clickedOutside = false;
        hasScrollControl = false;
        hasArrowkeyControl = false;
        isClicked = false;
        isPressed = false;
        isRightClicked = false;
        isEnabled = true;
        isHovered = false;
        widget_type = "";
        widget_type_id = 0;
        widget_name = "";
        descriptionText = "";
        showBorderBox = true;
        hasLineBreak = false;
        isModified = false;
        isInUse = false;
    }

    widget_basic::~widget_basic()
    {

    }

    bool widget_basic::contains_string( std::string s_name )
    {
        if( (int)s_name.size() == 0 )
        {
            return true;
        }
        std::string lowered_string = stg_ex::string_lower( widget_name );

        int foundPosition = lowered_string.find( s_name );

        if ( foundPosition!=(int)std::string::npos )
        {
            return true;
        }
        return false;
    }

    bool widget_basic::is_modified()
    {
        return isModified;
    }

    bool widget_basic::is_inuse()
    {
        return isInUse;
    }

    void widget_basic::switch_inuse(bool newUse)
    {
        isInUse = newUse;
        if( newUse == false )
        {
            hasScrollControl = false;
            hasArrowkeyControl = false;
        }
    }

    void widget_basic::update_box( float nx, float ny, float nw, float nh)
    {
        set_coords( nx, ny);
        set_width( nw);
        set_height( nh );
    }

    void widget_basic::update_paragraph( int foundMaxWidth )
    {

    }

    void widget_basic::disable_self()
    {
        isEnabled = false;
    }

    void widget_basic::enable_self()
    {
        isEnabled = true;
    }

    int widget_basic::get_height()
    {
        return widget_box.h;
    }

    int widget_basic::get_max_height()
    {
        return maxHeight;
    }

    int widget_basic::get_max_width()
    {
        return maxWidth;
    }

    std::string widget_basic::get_name()
    {
        return widget_name;
    }

    std::string widget_basic::get_element_type()
    {
        return widget_type;
    }

    std::string widget_basic::get_data()
    {
        return "";
    }

    std::string widget_basic::get_plain_string()
    {
        return "\""+widget_name+"\"";
    }

    void widget_basic::load_data(std::string datastring)
    {
        datastring = datastring;
    }

    void widget_basic::remove_data(std::string datastring)
    {
        datastring = datastring;
    }

    int widget_basic::get_width()
    {
        return widget_box.w;
    }

    int widget_basic::get_xpos()
    {
        return widget_box.x;

    }

    int widget_basic::get_x2()
    {
        return widget_box.x+widget_box.w;
    }

    int widget_basic::get_ypos()
    {
        return widget_box.y;
    }

    int widget_basic::get_y2()
    {
        return widget_box.y+widget_box.h;
    }

    bool widget_basic::is_clicked()
    {
        return isClicked;
    }

    bool widget_basic::is_rightclicked()
    {
        return isRightClicked;
    }


    bool widget_basic::is_enabled()
    {
        return isEnabled;
    }

    bool widget_basic::is_full_width()
    {
        return isFullWidth;
    }

    bool widget_basic::is_hovered()
    {
        return isHovered;
    }

    void widget_basic::move_pos(int x_new, int y_new)
    {
        if( x_new!=0)
        {
            widget_box.x += x_new;
        }
        if(y_new!=0)
        {
            widget_box.y += y_new;
        }
    }

    void widget_basic::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
    {
        bool wasHovered = isHovered;
        windowInView = false;
        isClicked = false;
        isPressed = false;
        isRightClicked = false;
        isHovered = false;
        clickedOutside = false;
        if( !isEnabled)
        {
            return;
        }
        if( isFullWidth )
        {
            needsNewLine = true;
        }

        gpe::render_package * guir_package = gpe::rph->get_render_package_from_name( r_packageName );
        if( guir_package !=nullptr && guir_package->packageWindow!=nullptr )
        {
            windowInView = guir_package->packageWindow->has_focus();
        }

        if( !windowInView )
        {
            return;
        }

        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if(view_space!=nullptr && cam!=nullptr)
        {
            if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,view_space->x,view_space->y,view_space->x+view_space->w,view_space->y+view_space->h) )
            {
                if ( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,widget_box.x+view_space->x-cam->x,widget_box.y+view_space->y-cam->y,widget_box.x+widget_box.w+view_space->x-cam->x,widget_box.y+widget_box.h+view_space->y-cam->y) )
                {
                    isHovered = true;
                    if( main_overlay_system!=nullptr)
                    {
                        if( (int)descriptionText.size()>0 )
                        {
                            main_overlay_system->update_tooltip(descriptionText);
                        }
                        else
                        {
                            main_overlay_system->update_tooltip(widget_name);
                        }
                    }
                    if( gpe::input->check_mouse_pressed( mb_left ) )
                    {
                        isPressed = true;
                        isInUse = true;
                    }
                    else if( gpe::input->check_mouse_released( mb_left))
                    {
                        isClicked = true;
                        isInUse = true;
                    }
                    else if( gpe::input->check_mouse_pressed( mb_right) )
                    {
                        isRightClicked = true;
                        isInUse = true;
                    }
                }
                else if( gpe::input->check_mouse_pressed( mb_anybutton ) || gpe::input->check_mouse_released( mb_anybutton ) )
                {
                    clickedOutside = true;
                    isInUse = false;
                    hasScrollControl = false;
                    hasArrowkeyControl = false;
                }
            }
            else if( gpe::input->check_mouse_pressed( mb_anybutton )  || gpe::input->check_mouse_released(  mb_anybutton ) )
            {
                clickedOutside = true;
                isInUse = false;
                hasScrollControl = false;
                hasArrowkeyControl = false;
            }
        }
    }

    void widget_basic::prerender_self( )
    {

    }

    void widget_basic::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {

    }

    bool widget_basic::requires_newline()
    {
        return needsNewLine;
    }

    void widget_basic::scale_height(float scaleH)
    {
        set_height( widget_box.h * scaleH);
    }

    void widget_basic::scale_width(float scaleW)
    {
        set_width( widget_box.w* scaleW);
    }

    void widget_basic::scale_object(float scaleValue )
    {
        scale_width( scaleValue);
        scale_height( scaleValue);
    }

    void widget_basic::set_full_width( bool makeFullWidth )
    {
        if( makeFullWidth)
        {
            needsNewLine = true;
        }
        isFullWidth = makeFullWidth;
    }

    void widget_basic::set_name(std::string new_name)
    {
        widget_name = new_name;
    }

    void widget_basic::set_coords(int x_new, int y_new)
    {
        if( x_new!=-1)
        {
            widget_box.x = x_new;
        }
        if(y_new!=-1)
        {
            widget_box.y = y_new;
        }
    }

    void widget_basic::set_max_width( int nMW)
    {
        maxWidth = nMW;
        if( maxWidth > 0 && widget_box.w > maxWidth)
        {
            widget_box.w = maxWidth;
        }
    }

    void widget_basic::set_max_height( int nMH)
    {
        maxHeight = nMH;
        if( maxHeight > 0 && widget_box.h > maxHeight)
        {
            widget_box.h = maxHeight;
        }
    }

    void widget_basic::set_clicked( bool clickedVal)
    {
        isClicked = clickedVal;
    }


    void widget_basic::set_rightclicked( bool clickedVal)
    {
        isRightClicked = clickedVal;
    }

    void widget_basic::set_height(int new_height)
    {
        widget_box.h = new_height;
        if(  new_height < -1 )
        {
            widget_box.h = -1;
        }
        widget_box.set_h( new_height);
        /*if(widget_box.y+widget_box.h > gpe::screen_height)
        {
            widget_box.h = gpe::screen_height-widget_box.y;
        }*/
    }

    void widget_basic::set_hovered(bool hoverVal)
    {
        isHovered = hoverVal;
    }

    void widget_basic::set_width(int newWid )
    {
        widget_box.w = newWid;
        if(  newWid < -1 )
        {
            widget_box.w = -1;
        }
        widget_box.set_w( newWid);
    }

    void update_action_message(std::string newMessage)
    {
        action_message_text = newMessage;
    }

    void update_popup_info(std::string newMessage, int selectedOpId, bool popIsContext)
    {
        action_message_text = newMessage;
        action_message_id = selectedOpId;
        action_is_context = popIsContext;
    }
}
