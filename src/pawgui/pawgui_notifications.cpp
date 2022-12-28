/*
pawgui_notifications.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#include "pawgui_notifications.h"

namespace pawgui
{
    widget_notifcation_base::widget_notifcation_base(std::string name_param, std::string description )
    {
        widget_name = name_param;
        descriptionText = description;
        hovering_over_action = false;
        hovering_over_x  = false;
        remove_requested = false;
        notification_bg_color =  gpe::c_blue->duplicate_color();
        notification_bg_hover_color = gpe::c_navy->duplicate_color();
        notification_font_color = gpe::c_white->duplicate_color();
        notification_font_hover_color = gpe::c_ltgray->duplicate_color();
        text_wid = 0;
        text_height = 0;

        box_action = new gpe::shape_rect(0,0, 16,16 );
        box_close_x = new gpe::shape_rect(0,0, 16,16 );
        if( pawgui::font_textinput != nullptr )
        {
            text_wid = pawgui::font_textinput->get_mono_width();
            text_height = pawgui::font_textinput->get_mono_height();

            box_action->w  = pawgui::font_textinput->get_mono_width();
            box_action->w  = pawgui::font_textinput->get_mono_width();

            box_close_x->w  = pawgui::font_textinput->get_mono_width();
            box_close_x->h = pawgui::font_textinput->get_mono_height();
        }
        else
        {
            text_wid = 16;
            text_height = 16;
        }
    }

    widget_notifcation_base::~widget_notifcation_base()
    {
        text_vector.clear();
        if( notification_bg_color !=nullptr )
        {
            delete notification_bg_color;
            notification_bg_color = nullptr;
        }

        if( notification_bg_hover_color !=nullptr )
        {
            delete notification_bg_hover_color;
            notification_bg_hover_color = nullptr;
        }

        if( notification_font_color !=nullptr )
        {
            delete notification_font_color;
            notification_font_color = nullptr;
        }

        if( notification_font_hover_color !=nullptr )
        {
            delete notification_font_hover_color;
            notification_font_hover_color = nullptr;
        }
    }

    std::string widget_notifcation_base::get_data()
    {
        return "";
    }

    std::string widget_notifcation_base::get_plain_string()
    {
        return "";
    }

    void widget_notifcation_base::load_data(std::string datastring)
    {

    }

    void widget_notifcation_base::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);

        hovering_over_action = false;
        hovering_over_x = false;
        remove_requested = false;

        if( pawgui::font_textinput != nullptr )
        {
            text_wid = pawgui::font_textinput->get_mono_width();
            text_height = pawgui::font_textinput->get_mono_height();

            box_action->w  = pawgui::font_textinput->get_mono_width() *2;
            box_action->h  = pawgui::font_textinput->get_mono_height();

            box_close_x->w  = pawgui::font_textinput->get_mono_width() *2;
            box_close_x->h = pawgui::font_textinput->get_mono_height();
        }
        else
        {
            text_wid = 16;
            text_height = 16;
            box_action->w  = text_wid;
            box_action->h  = text_height;

            box_close_x->w  = text_wid;
            box_close_x->h = text_height;
        }

        box_action->x = widget_box.get_x2() - box_action->w;
        box_action->y = widget_box.get_y2() - box_action->h;

        box_close_x->x = widget_box.get_x2() - box_close_x->w;
        box_close_x->y = widget_box.y;

        widget_basic::process_self( view_space, cam );

        if( is_hovered() )
        {
            if ( gpe::point_between_rect(gpe::input->mouse_position_x - view_space->x + cam->x,gpe::input->mouse_position_y  - view_space->y + cam->y, box_close_x)  )
            {
                hovering_over_x = true;
                if( is_clicked() )
                {
                    remove_requested = true;
                }
            }
            else if ( gpe::point_between_rect(gpe::input->mouse_position_x - view_space->x + cam->x,gpe::input->mouse_position_y - view_space->y + cam->y,box_action ) )
            {
                hovering_over_action = true;
                if( is_clicked() )
                {
                    if( notification_action == "url" || notification_action == "website")
                    {
                        gpe::main_file_url_manager->external_open_url( notification_parameter );
                    }
                }
            }
        }
    }

    void widget_notifcation_base::refresh_notification()
    {
        widget_box.h = 0;
        max_text_width = widget_box.w - 32 - padding_default*2;
        text_vector.clear();
        if( pawgui::font_textinput == nullptr )
        {
            text_wid = 16;
            text_height = 16;
            gpe::error_log->report("Font is nullptr, so can't calculate widths...");
            return;
        }
        text_wid = pawgui::font_textinput->get_mono_width();
        text_height = pawgui::font_textinput->get_mono_height();

        int max_chars_per_line = max_text_width;
        if( max_text_width !=0 && text_wid!=0 )
        {
            max_chars_per_line = max_text_width / text_wid;
        }
        else
        {
            max_chars_per_line = -1;
        }
        stg_ex::wrap_string( descriptionText, text_vector, max_chars_per_line );
        widget_box.h = (int)text_vector.size() * ( text_height+padding_default )+ padding_default;

        gpe::error_log->report("gpe::screen_width , widget_box.w (" + stg_ex::int_to_string( gpe::screen_width ) + "," + stg_ex::int_to_string( widget_box.w ) + ")" );
        gpe::error_log->report("max_text_width , text_wid , max_chars_per_line(" + stg_ex::int_to_string(max_text_width) + "," + stg_ex::int_to_string( text_wid ) + "," + stg_ex::int_to_string(max_chars_per_line) + ")" );
    }

    void widget_notifcation_base::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        gpe::color * current_font_color = notification_font_color;
        if( is_hovered() )
        {
            gpe::gcanvas->render_rect( &widget_box, notification_bg_hover_color, false, 255 );
            current_font_color = notification_font_hover_color;
        }
        else
        {
            gpe::gcanvas->render_rect( &widget_box, notification_bg_color, false, 255 );
        }


        if( pawgui::font_textinput == nullptr )
        {
            return;
        }

        if( hovering_over_x )
        {
            gpe::gcanvas->render_rect( box_close_x, notification_bg_color, false, 255 );
            pawgui::font_textinput->render_text( box_close_x->get_x2(),box_close_x->y, "×", notification_font_hover_color, gpe::fa_right, gpe::fa_top, 255 );
        }
        else
        {
            pawgui::font_textinput->render_text( box_close_x->get_x2(),box_close_x->y, "×", notification_font_color, gpe::fa_right, gpe::fa_top, 255 );
        }

        if( hovering_over_action)
        {
            gpe::gcanvas->render_rect( box_action, notification_bg_color, false, 255 );
            pawgui::font_textinput->render_text( box_action->get_x2(), box_action->get_y2(), "->", notification_font_hover_color, gpe::fa_right, gpe::fa_bottom, 255 );
        }
        else
        {
            pawgui::font_textinput->render_text( box_action->get_x2(), box_action->get_y2(), "->", notification_font_color, gpe::fa_right, gpe::fa_bottom, 255 );
        }

        int render_ypos = widget_box.y+ padding_default;
        for( int i = 0; i <(int)text_vector.size(); i++ )
        {
            pawgui::font_textinput->render_text( widget_box.x + padding_default, render_ypos, text_vector[i], current_font_color, gpe::fa_left, gpe::fa_top, 255 );
            render_ypos += pawgui::font_textinput->get_mono_height()+padding_default ;
        }
    }

    void widget_notifcation_base::set_name(std::string name_param)
    {

    }

    void widget_notifcation_base::set_width(int new_width)
    {
        if( widget_box.w != new_width )
        {
            //return;
        }
        widget_basic::set_width( new_width );
    }

    widget_notfications_holder::widget_notfications_holder()
    {
        notifications_in_view_max  = 3;
        widget_box.w = -1;
    }

    widget_notfications_holder::~widget_notfications_holder()
    {
        widget_notifcation_base * temp_notification = nullptr;
        for( int i = (int)notifications.size() -1; i>= 0; i-- )
        {
            temp_notification = notifications[i];
            if( temp_notification != nullptr )
            {
                delete temp_notification;
                temp_notification = nullptr;
            }
        }
        notifications.clear();
    }

    widget_notifcation_base * widget_notfications_holder::add_notification_simple( std::string name, std::string description)
    {
        widget_notifcation_base * temp_notification = new widget_notifcation_base( name, description );
        notifications.push_back( temp_notification );
        return temp_notification;
    }

    bool widget_notfications_holder::remove_notification( std::string name )
    {
        widget_notifcation_base * temp_notification = nullptr;
        bool notification_being_removed = false;
        for( int i = (int)notifications.size() -1; i>= 0; i-- )
        {
            temp_notification = notifications[i];
            if( temp_notification != nullptr )
            {
                if( temp_notification->get_name() == name )
                {
                    delete temp_notification;
                    temp_notification = nullptr;
                    notifications.erase( notifications.begin() + i );
                    notification_being_removed = true;
                }
            }
        }
        return notification_being_removed;
    }

    void widget_notfications_holder::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        widget_notifcation_base * temp_notification = nullptr;
        int iMin = std::max( 0, (int)notifications.size() - notifications_in_view_max );
        int temp_y_pos = get_ypos();
        int notification_being_removed = -1;

        for( int i = (int)notifications.size() -1; i>=iMin; i-- )
        {
            temp_notification = notifications[i];
            if( temp_notification != nullptr )
            {
                temp_notification->set_coords( get_xpos(), temp_y_pos );
                //temp_notification->set_width( widget_box.w );
                temp_notification->process_self( view_space, cam );
                temp_y_pos += temp_notification->get_height();
                if( temp_notification->remove_requested )
                {
                    notification_being_removed = i;
                }
                //widget_box.h += temp_notification->get_height();
            }
        }

        if( notification_being_removed >=0 && notification_being_removed < (int)notifications.size() )
        {
            temp_notification = notifications[ notification_being_removed];
            if( temp_notification !=nullptr )
            {
                widget_box.h -= temp_notification->get_height();
                delete temp_notification;
                temp_notification = nullptr;
            }
            notifications.erase( notifications.begin() + notification_being_removed );
        }
    }

    void widget_notfications_holder::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        widget_notifcation_base * temp_notification = nullptr;
        int iMin = std::max( 0, (int)notifications.size() - notifications_in_view_max );
        for( int i = (int)notifications.size() -1; i>=iMin; i-- )
        {
            temp_notification = notifications[i];
            if( temp_notification != nullptr )
            {
                temp_notification->render_self( view_space, cam );
            }
        }
    }

    void widget_notfications_holder::set_width(int new_width)
    {
        if( widget_box.w == new_width )
        {
            return;
        }
        widget_basic::set_width( new_width );
        widget_box.h = 0;
        widget_notifcation_base * temp_notification = nullptr;
        int iMin = std::max( 0, (int)notifications.size() - notifications_in_view_max );

        for( int i = (int)notifications.size() -1; i>=0; i-- )
        {
            temp_notification = notifications[i];
            if( temp_notification != nullptr )
            {
                temp_notification->set_width( new_width );
                temp_notification->refresh_notification();
                if( i >= iMin )
                {
                    widget_box.h += temp_notification->get_height();
                }
            }
        }
    }
}
