/*
pawgui_content_selector.cpp
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
#include "pawgui_content_selector.h"

namespace pawgui
{
    widget_content_selector * main_content_selector = nullptr;
    int widget_content_selector_state = -1;

    widget_content_selector::widget_content_selector(std::string name,std::string description)
    {
        widget_name = name;
        set_label( name );
        descriptionText = description;
        widget_type = "contentselector";
        widget_type_id = 2;
        storedInt = -1;
        storeddfloat = -1;
        storedstring = "";
        storedColor = new gpe::color("customColor", 128,128, 128 );

        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 196;
        widget_box.h = 32;
        fieldElementBox.x = widget_box.x+128;
        fieldElementBox.y = widget_box.y;
        fieldElementBox.w = widget_box.w-128;
        fieldElementBox.h = widget_box.h;


    }

    widget_content_selector::~widget_content_selector()
    {

    }

    std::string widget_content_selector::get_data()
    {
        return ""; //WIPNOTFUNCTIONALYET
    }

    void widget_content_selector::load_data(std::string datastring)
    {

    }

    std::string widget_content_selector::get_plain_string()
    {
        return ""; //WIPNOTFUNCTIONALYET
    }

    void widget_content_selector::set_label(std::string newLabel)
    {

    }

    void widget_content_selector::set_name(std::string new_name)
    {

    }

    void widget_content_selector::set_string(std::string newstring)
    {

    }

    void widget_content_selector::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
    {
        fieldElementBox.x = widget_box.x+widget_box.w-32;
        fieldElementBox.y = widget_box.y;
        fieldElementBox.w = 32;
        fieldElementBox.h = widget_box.h;

        if( widget_box.w > 512)
        {
            fieldElementBox.x = widget_box.x+widget_box.w-128;
            fieldElementBox.w = 128;
        }

        if( widget_box.w > 384)
        {
            fieldElementBox.x = widget_box.x+widget_box.w-96;
            fieldElementBox.w = 96;
        }
        else if( widget_box.w > 256)
        {
            fieldElementBox.x = widget_box.x+widget_box.w-64;
            fieldElementBox.w = 64;
        }
        widget_basic::process_self(view_space,cam);

        if( isEnabled)
        {

            view_space = gpe::camera_find(view_space);
            cam = gpe::camera_find(cam);
        }

        //
        if( isInUse &&( gpe::input->check_kb_down( kb_enter ) || gpe::input->check_kb_down( kb_space )  ) )
        {
            isClicked = true;
        }
        if( isClicked)
        {
            //get_color_from_popup(popUpstring,storedColor);
            if( widget_content_selector_state == -1)
            {
                widget_content_selector_state = 0;
                main_content_selector = this;
                gpe::cursor_main_controller->cursor_change( "hand" );
            }
            else
            {
                widget_content_selector_state = -1;
                main_content_selector = nullptr;
                gpe::cursor_main_controller->cursor_change( "arrow");
            }
            isClicked = false;
        }
        else if( widget_content_selector_state == 0)
        {
            gpe::cursor_main_controller->cursor_change( "hand" );
        }
    }

    void widget_content_selector::render_self( gpe::shape_rect * view_space , gpe::shape_rect * cam  )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( isEnabled && cam!=nullptr)
        {
            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y, pawgui::theme_main->main_box_highlight_color, false );
                if( (int)widget_name.size() > 0)
                {
                    gpe::gfs->render_text_clipped( widget_box.x-cam->x+widget_box.h+padding_default,widget_box.y+widget_box.h/2-cam->y,widget_name+":",pawgui::theme_main->main_box_font_highlight_color,font_textinput,gpe::fa_left,gpe::fa_middle );
                }
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y, pawgui::theme_main->main_box_color, false );

                if( (int)widget_name.size() > 0 )
                {
                    gpe::gfs->render_text_clipped( widget_box.x-cam->x+widget_box.h+padding_default,widget_box.y+widget_box.h/2-cam->y,widget_name+":",pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_left,gpe::fa_middle );
                }
            }

            if( eyedropper_texture!=nullptr)
            {
                eyedropper_texture->render_tex_resized( widget_box.x-cam->x + widget_box.h/4, widget_box.y-cam->y + widget_box.h/4, widget_box.h/2, widget_box.h/2,  nullptr, pawgui::theme_main->main_box_font_color );
            }
            if( selectorType == content_selector_type::cs_type_int )
            {
                gpe::gfs->render_text_clipped( widget_box.x-cam->x+widget_box.w,widget_box.y+widget_box.h/2-cam->y, stg_ex::int_to_string(storedInt),pawgui::theme_main->main_box_font_highlight_color,font_textinput,gpe::fa_right,gpe::fa_middle );

            }
            else if( selectorType == content_selector_type::cs_type_float )
            {
                gpe::gfs->render_text_clipped( widget_box.x-cam->x+widget_box.w,widget_box.y+widget_box.h/2-cam->y,stg_ex::float_to_string(storeddfloat),pawgui::theme_main->main_box_font_highlight_color,font_textinput,gpe::fa_right,gpe::fa_middle );
            }
            else if( selectorType == content_selector_type::cs_type_color )
            {
                gpe::gcanvas->render_rectangle( fieldElementBox.x-cam->x,fieldElementBox.y-cam->y,fieldElementBox.x+fieldElementBox.w-cam->x,fieldElementBox.y+fieldElementBox.h-cam->y,storedColor,false);
                //gpe::gcanvas->render_vertical_line_color( widget_box.x-cam->x,widget_box.y-cam->y, widget_box.y+widget_box.h-cam->y, pawgui::theme_main->main_border_color );
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y, pawgui::theme_main->main_border_color, true );
            }
            else if(  selectorType== content_selector_type::cs_type_string )
            {
                if( (int)storedstring.size() > 0  )
                {
                    gpe::gfs->render_text_clipped( widget_box.x-cam->x+widget_box.w,widget_box.y+widget_box.h/2-cam->y,storedstring,pawgui::theme_main->main_box_font_highlight_color,font_textinput,gpe::fa_right,gpe::fa_middle );
                }
                else
                {
                    gpe::gfs->render_text_clipped( widget_box.x-cam->x+widget_box.w,widget_box.y+widget_box.h/2-cam->y,"undefined",pawgui::theme_main->main_box_font_highlight_color,font_textinput,gpe::fa_right,gpe::fa_middle );
                }
            }
            else
            {
                gpe::gfs->render_text_clipped( widget_box.x-cam->x+widget_box.w,widget_box.y+widget_box.h/2-cam->y,"[invalid]",pawgui::theme_main->main_box_font_highlight_color,font_textinput,gpe::fa_right,gpe::fa_middle );
            }
        }
    }

    void widget_content_selector::set_color_from_rgb( gpe::color *color_new)
    {

    }

    void widget_content_selector::set_color_from_rgb(std::string color_newStr)
    {

    }

    void widget_content_selector::set_color_from_hex(std::string color_newStr)
    {

    }

    void widget_content_selector::set_r(int r)
    {

    }

    void widget_content_selector::set_g(int g)
    {

    }

    void widget_content_selector::set_b(int b)
    {

    }

    void widget_content_selector::set_rgb(int r, int g, int b)
    {

    }
}
