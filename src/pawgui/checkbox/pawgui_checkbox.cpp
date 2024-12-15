/*
pawgui_checkbox.cpp
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

#include "pawgui_checkbox.h"

namespace pawgui
{
    widget_checkbox::widget_checkbox(std::string name, std::string descriptor, bool already_checked )
    {
        widget_type = "checkbox";
        widget_type_id = 1;
        widget_name = name;
        descriptionText = descriptor;

        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = clickBoxW = 24;
        widget_box.h = clickBoxH = 24;
        labelBoxW = 0;
        labelBoxH = 0;
        if( (int)widget_name.size()>0 && gpe::font_default!=nullptr )
        {
            gpe::font_default->get_metrics(widget_name,&labelBoxW, &labelBoxH);
            widget_box.w+= labelBoxW;
        }
        //widget_box.w+=+padding_default;
        isClicked = already_checked;
    }

    widget_checkbox::~widget_checkbox()
    {

    }

    std::string widget_checkbox::get_data()
    {
        return widget_type+":"+widget_name+"==|||=="+ stg_ex::int_to_string(isClicked);
    }

    void widget_checkbox::load_data(std::string datastring)
    {
        isClicked = stg_ex::string_to_bool(datastring);
    }


    void widget_checkbox::prerender_self(  )
    {
        //widget_box.w = clickBoxW;
        //widget_box.h = clickBoxH;
    }

    void widget_checkbox::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        isHovered = false;

        bool past_clicked = isClicked;
        if(view_space!=nullptr && cam!=nullptr)
        {
            widget_basic::process_self( view_space, cam);
            if( windowInView == false )
            {
                return;
            }
            if( isClicked )
            {
                isClicked = !past_clicked;
            }
            else if( (isInUse || isHovered ) &&( gpe::input->check_kb_pressed( kb_enter ) || gpe::input->check_kb_pressed( kb_space )  ) )
            {
                isClicked = !past_clicked;
            }
            else
            {
                isClicked = past_clicked;
            }
        }
        else
        {
            isClicked = past_clicked;
        }
    }

    void widget_checkbox::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( cam!=nullptr)
        {
            if( isInUse )
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+clickBoxW-cam->x,widget_box.y+clickBoxH-cam->y,pawgui::theme_main->button_box_highlight_color,false);
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+clickBoxW-cam->x,widget_box.y+clickBoxH-cam->y,pawgui::theme_main->button_border_highlight_color,true);
            }
            else if( isHovered)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+clickBoxW-cam->x,widget_box.y+clickBoxH-cam->y,pawgui::theme_main->button_box_selected_color,false);
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+clickBoxW-cam->x,widget_box.y+clickBoxH-cam->y,pawgui::theme_main->button_border_highlight_color,true);
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+clickBoxW-cam->x,widget_box.y+clickBoxH-cam->y,pawgui::theme_main->main_border_color,false);
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+clickBoxW-cam->x,widget_box.y+clickBoxH-cam->y,pawgui::theme_main->main_box_font_color,true);
            }
            if( isClicked && checkmark_texture!=nullptr )
            {
                checkmark_texture->render_tex_resized( widget_box.x-cam->x,widget_box.y-cam->y,clickBoxW,clickBoxH,nullptr, pawgui::theme_main->checkbox_color );
            }
            gpe::gfs->render_text( widget_box.x+clickBoxW+padding_default-cam->x,widget_box.y+widget_box.h/2-cam->y,widget_name,pawgui::theme_main->main_box_font_color,FONT_LABEL,gpe::fa_left,gpe::fa_middle,255);
        }
    }

    void widget_checkbox::set_checked( bool newCheckState )
    {
        isClicked = newCheckState;
    }

    void widget_checkbox::set_checkbox_size(int nBoxSize, bool resizewidget_box)
    {
        if( resizewidget_box)
        {
            widget_box.w = nBoxSize+labelBoxW;
        }
        labelBoxW = labelBoxH = nBoxSize;
    }
}
