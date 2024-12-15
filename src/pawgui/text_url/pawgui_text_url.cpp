/*
pawgui_text_url.cpp
This file is part of:
PAW GUI
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

#include "pawgui_text_url.h"

namespace pawgui
{
    widget_text_url::widget_text_url(std::string name_param, std::string description, std::string url_string)
    {
        webId = "";
        widget_type = "texturl";
        widget_name = name_param;
        descriptionText = description;
        wasClicked = false;
        webUrl = url_string;
        if( gpe::font_default!=nullptr)
        {
            int bWid = 0;
            int bHgt = 0;
            gpe::font_default->get_metrics(name_param, &bWid, &bHgt);
            widget_box.w = bWid;
            widget_box.h = bHgt;
        }
    }

    widget_text_url::~widget_text_url()
    {

    }

    std::string widget_text_url::get_data()
    {
        return widget_type+":"+widget_name+"==|||=="+descriptionText+",,,"+webUrl+",,,";
    }

    std::string widget_text_url::get_plain_string()
    {
        return "\""+webUrl+"\"";
    }

    void widget_text_url::load_data(std::string datastring)
    {
        descriptionText = stg_ex::split_first_string(datastring,",,,");
        webUrl = stg_ex::split_first_string(datastring,",,,");
    }

    void widget_text_url::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        widget_basic::process_self(view_space,cam);
        if( isHovered)
        {
            gpe::cursor_main_controller->cursor_change("hand");
        }

        if( isInUse &&( gpe::input->check_kb_down( kb_enter ) || gpe::input->check_kb_down( kb_space )  ) )
        {
            isClicked = true;
        }
        if( isClicked )
        {
            wasClicked = true;
            if( (int)webUrl.size() > 3)
            {
                gpe::external_open_url(webUrl);
            }
        }
    }

    void widget_text_url::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        if( (int)widget_name.size() > 0)
        {
            if( wasClicked)
            {
                gpe::gfs->render_text( widget_box.x-cam->x+ widget_box.w/2,widget_box.y-cam->y,widget_name,pawgui::theme_main->main_box_font_url_visited_color,gpe::font_default, gpe::fa_center,gpe::fa_top);
                gpe::gcanvas->render_horizontal_line_color( widget_box.y+widget_box.h-cam->y,widget_box.x-cam->x,widget_box.x+widget_box.w-cam->x,pawgui::theme_main->main_box_font_url_visited_color);
            }
            else if( isHovered)
            {
                gpe::gfs->render_text( widget_box.x-cam->x+ widget_box.w/2,widget_box.y-cam->y,widget_name,pawgui::theme_main->main_box_font_url_hovered_color,gpe::font_default,gpe::fa_center,gpe::fa_top);
                gpe::gcanvas->render_horizontal_line_color( widget_box.y+widget_box.h-cam->y,widget_box.x-cam->x,widget_box.x+widget_box.w-cam->x,pawgui::theme_main->main_box_font_url_hovered_color);
            }
            else
            {
                gpe::gfs->render_text( widget_box.x-cam->x + widget_box.w/2,widget_box.y-cam->y,widget_name,pawgui::theme_main->main_box_font_url_color,gpe::font_default,gpe::fa_center,gpe::fa_top);
                gpe::gcanvas->render_horizontal_line_color( widget_box.y+widget_box.h-cam->y,widget_box.x-cam->x,widget_box.x+widget_box.w-cam->x,pawgui::theme_main->main_box_font_url_color);
            }
            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_font_url_hovered_color,true);
            }
        }
    }

    void widget_text_url::set_name(std::string name_param)
    {
        if( gpe::font_default!=nullptr)
        {
            int bWid = 0;
            int bHgt = 0;
            gpe::font_default->get_metrics(name_param, &bWid, &bHgt);
            widget_box.w = bWid;
            widget_box.h = bHgt;
        }
        widget_name = name_param;
    }
}
