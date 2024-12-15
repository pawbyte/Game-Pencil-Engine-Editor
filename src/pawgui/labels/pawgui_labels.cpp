/*
pawgui_labels.cpp
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

#include "pawgui_labels.h"
namespace pawgui
{
    widget_label_image::widget_label_image(std::string name_param, std::string file_name, std::string description, std::string url_string)
    {
        allowOverSize = true;
        resizeAspect = 0.5;
        widget_type = "labelimage";
        widget_name = name_param;
        descriptionText = description;
        wasClicked = false;
        isHoldingCustomTexture = false;
        webUrl = url_string;
        imgSrc = rsm_gui->texture_add_filename( file_name );
        if( imgSrc!=nullptr )
        {
            widget_box.w = imgSrc->get_width();
            widget_box.h = imgSrc->get_height();
        }
        else
        {
            widget_box.w = 32;
            widget_box.h = 32;
            imgSrc = nullptr;
        }
    }

    widget_label_image::~widget_label_image()
    {

    }

    void widget_label_image::load_label_image(std::string new_file_name)
    {
        imgSrc = rsm_gui->texture_add_filename( new_file_name );
        if( imgSrc!=nullptr )
        {
            widget_box.w = imgSrc->get_width();
            widget_box.h = imgSrc->get_height();
        }
    }

    void widget_label_image::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        view_space = gpe::camera_find( view_space );
        cam = gpe::camera_find( cam );
        widget_basic::process_self(view_space,cam);
        if( view_space!=nullptr && cam!=nullptr )
        {
            if( isHovered && (int)webUrl.size() > 3)
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
    }

    void widget_label_image::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( cam!=nullptr )
        {
            if( imgSrc!=nullptr )
            {
                imgSrc->render_tex_resized( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.w,widget_box.h,nullptr );
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_font_color,true);
            }
            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_font_url_color,true);
            }
            if( isHovered)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_font_url_hovered_color,true);
            }
        }
    }

    void widget_label_image::change_texture( gpe::texture_base * imgIn )
    {
        imgSrc = imgIn;
        if( imgSrc!=nullptr )
        {
            widget_box.w = imgSrc->get_width();
            widget_box.h = imgSrc->get_height();
        }
        else
        {
            widget_box.w = 0;
            widget_box.h = 0;
        }
    }

    void widget_label_image::set_width( int newW )
    {
        //if( newW!=widget_box.w)
        {
            if( newW > 0)
            {
                float newH = 0;
                if( imgSrc!=nullptr && imgSrc->get_width() > 0)
                {
                    if( !allowOverSize && !isFullWidth )
                    {
                        if( newW > imgSrc->get_width() )
                        {
                            newW = imgSrc->get_width();
                        }
                    }

                    widget_box.w = newW * resizeAspect;
                    if( imgSrc!=nullptr && imgSrc->get_width()!=0 )
                    {
                        widget_box.h =  widget_box.w * (float)imgSrc->get_height()/(float)imgSrc->get_width();
                    }
                }
            }
            else if( imgSrc!=nullptr && imgSrc->get_width() > 0)
            {
                widget_box.w = imgSrc->get_width() ;
                widget_box.h = imgSrc->get_height() ;
            }
            else
            {
                widget_box.w = 0;
                widget_box.h = 0;
            }
        }
    }

    void widget_label_image::set_height( int newH)
    {
        if( newH > 0)
        {
            if( imgSrc!=nullptr && !allowOverSize)
            {
                if( newH > imgSrc->get_height() )
                {
                    newH = imgSrc->get_height();
                }
            }
            widget_box.h = newH;
            if( imgSrc!=nullptr )
            {
                if( resizeAspect != 0.f )
                {
                    widget_box.w = resizeAspect * ceil( (float)newH * (float)imgSrc->get_width()/(float)imgSrc->get_height() );
                }
                else
                {
                    widget_box.w = widget_box.h;
                }
            }
        }
        else
        {
            widget_box.w = 0;
            widget_box.h = 0;
        }
    }

    void widget_label_image::set_size( float newW, float newH)
    {
        widget_box.w = newW;
        widget_box.h = newH;
    }

    void widget_label_image::set_name(std::string name_param)
    {
        widget_name = name_param;
    }

    //

    //

    widget_label_text::widget_label_text(std::string name_param, std::string description)
    {
        widget_type = "labeltext";
        widget_type_id = 7;
        widget_name = name_param;
        descriptionText = description;
        if( FONT_LABEL!=nullptr)
        {
            int bWid = 0;
            int bHgt = 0;
            FONT_LABEL->get_metrics(name_param.c_str(), &bWid, &bHgt);
            widget_box.w = bWid;
            widget_box.h = bHgt;
        }
        widget_name = name_param;
        hasLineBreak = true;
    }

    widget_label_text::~widget_label_text()
    {

    }

    std::string widget_label_text::get_data()
    {
        return widget_type+":"+widget_name+"==|||=="+descriptionText;
    }

    std::string widget_label_text::get_plain_string()
    {
        return "\""+widget_name+"\"";
    }

    void widget_label_text::load_data(std::string datastring)
    {
        descriptionText = datastring;
    }

    void widget_label_text::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        widget_basic::process_self(view_space,cam);
        /*
        if( isHovered)
        {
            gpe::cursor_main_controller->cursor_change(GPE_CURSOR_HAND);
        }
        if( isClicked )
        {
            wasClicked = true;
            if( (int)webUrl.size() > 3)
            {
                gpe::external_open_url(webUrl);
            }
        }
        */
        if( isInUse)
        {
            if( gpe::input->check_kb_down(kb_ctrl) && gpe::input->check_kb_released(kb_c) )
            {
                gpe::input->clipboard_set(widget_name.c_str() );
            }
        }
    }

    void widget_label_text::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( (int)widget_name.size() > 0 && view_space!=nullptr && cam!=nullptr )
        {
            gpe::gfs->render_text( widget_box.x-cam->x,widget_box.y-cam->y,widget_name,pawgui::theme_main->main_box_font_color,FONT_LABEL,gpe::fa_left,gpe::fa_top);

            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_font_url_color,true);
            }
        }
    }

    void widget_label_text::set_name(std::string name_param)
    {
        if( FONT_LABEL!=nullptr)
        {
            int bWid = 0;
            int bHgt = 0;
            FONT_LABEL->get_metrics(name_param.c_str(), &bWid, &bHgt);
            widget_box.w = bWid;
            widget_box.h = bHgt;
        }
        widget_name = name_param;
    }

    //
    //

    widget_label_error::widget_label_error(std::string name_param, std::string description)
    {
        widget_type = "labelerror";
        widget_type_id = 7;
        widget_name = name_param;
        descriptionText = description;
        if( FONT_LABEL!=nullptr)
        {
            int bWid = 0;
            int bHgt = 0;
            FONT_LABEL->get_metrics(name_param.c_str(), &bWid, &bHgt);
            widget_box.w = bWid;
            widget_box.h = bHgt;
        }
        widget_name = name_param;
        hasLineBreak = true;
    }

    widget_label_error::~widget_label_error()
    {

    }

    std::string widget_label_error::get_data()
    {
        return widget_type+":"+widget_name+"==|||=="+descriptionText;
    }

    std::string widget_label_error::get_plain_string()
    {
        return "\""+widget_name+"\"";
    }

    void widget_label_error::load_data(std::string datastring)
    {
        descriptionText = datastring;
    }

    void widget_label_error::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        widget_basic::process_self(view_space,cam);
        /*
        if( isHovered)
        {
            gpe::cursor_main_controller->cursor_change(GPE_CURSOR_HAND);
        }
        if( isClicked )
        {
            wasClicked = true;
            if( (int)webUrl.size() > 3)
            {
                gpe::external_open_url(webUrl);
            }
        }
        */
        if( isInUse)
        {
            if( gpe::input->check_kb_down(kb_ctrl) && gpe::input->check_kb_released(kb_c) )
            {
                gpe::input->clipboard_set(widget_name.c_str() );
            }
        }
    }

    void widget_label_error::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( (int)widget_name.size() > 0 && view_space!=nullptr && cam!=nullptr )
        {
            gpe::gfs->render_text( widget_box.x-cam->x,widget_box.y-cam->y,widget_name,pawgui::theme_main->input_error_font_color,FONT_LABEL,gpe::fa_left,gpe::fa_top);

            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_font_highlight_color,true);
            }
        }
    }

    void widget_label_error::set_name(std::string name_param)
    {
        if( FONT_LABEL!=nullptr)
        {
            int bWid = 0;
            int bHgt = 0;
            FONT_LABEL->get_metrics(name_param.c_str(), &bWid, &bHgt);
            widget_box.w = bWid;
            widget_box.h = bHgt;
        }
        widget_name = name_param;
    }

    //

    widget_label_rotated_90::widget_label_rotated_90(std::string name_param, std::string description)
    {
        widget_type = "labeltext";
        widget_type_id = 7;
        widget_name = name_param;
        descriptionText = description;
        if( FONT_LABEL!=nullptr)
        {
            int bWid = 0;
            int bHgt = 0;
            FONT_LABEL->get_metrics(name_param.c_str(), &bWid, &bHgt);
            widget_box.w = bHgt;
            widget_box.h = bWid;
        }
        widget_name = name_param;
        hasLineBreak = true;
    }

    widget_label_rotated_90::~widget_label_rotated_90()
    {

    }

    std::string widget_label_rotated_90::get_data()
    {
        return widget_type+":"+widget_name+"==|||=="+descriptionText;
    }

    std::string widget_label_rotated_90::get_plain_string()
    {
        return "\""+widget_name+"\"";
    }

    void widget_label_rotated_90::load_data(std::string datastring)
    {
        descriptionText = datastring;
    }

    void widget_label_rotated_90::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        widget_basic::process_self(view_space,cam);
        /*
        if( isHovered)
        {
            gpe::cursor_main_controller->cursor_change(GPE_CURSOR_HAND);
        }
        if( isClicked )
        {
            wasClicked = true;
            if( (int)webUrl.size() > 3)
            {
                gpe::external_open_url(webUrl);
            }
        }
        */
        if( isInUse)
        {
            if( gpe::input->check_kb_down(kb_ctrl) && gpe::input->check_kb_released(kb_c) )
            {
                gpe::input->clipboard_set(widget_name.c_str() );
            }
        }
    }

    void widget_label_rotated_90::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( (int)widget_name.size() > 0 && view_space!=nullptr && cam!=nullptr )
        {
            gpe::gfs->render_text_rotated( widget_box.x-cam->x,widget_box.y-cam->y+widget_box.h,widget_name,pawgui::theme_main->main_box_font_color,FONT_LABEL,90);

            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_font_url_color,true);
            }
        }
    }

    void widget_label_rotated_90::set_name(std::string name_param)
    {
        if( FONT_LABEL!=nullptr)
        {
            int bWid = 0;
            int bHgt = 0;
            FONT_LABEL->get_metrics(name_param.c_str(), &bWid, &bHgt);
            widget_box.w = bHgt;
            widget_box.h = bWid;
        }
        widget_name = name_param;
    }

    widget_label_paragraph::widget_label_paragraph(std::string name_param, std::string paragraphIn, std::string descriptionIn)
    {
        widget_type = "paragraph";
        paragraphWidth = 0;
        needsNewLine = true;
        isFullWidth = true;
        autoResizes= true;
        widget_name = name_param;
        paragraphText = paragraphIn;
        descriptionText = descriptionIn;
    }

    widget_label_paragraph::~widget_label_paragraph()
    {
        paragraphLines.clear();
        if( FONT_LABEL!=nullptr )
        {
            FONT_LABEL->clear_cache();
        }
    }

    void widget_label_paragraph::add_line(std::string newLine)
    {
        paragraphLines.push_back(newLine);
        paragraphWidth = 0;
    }

    void widget_label_paragraph::clear_all_lines()
    {
        paragraphLines.clear();
        paragraphWidth = 0;
    }

    std::string widget_label_paragraph::get_paragraph()
    {
        return paragraphText;
    }

    void widget_label_paragraph::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        if( view_space!=nullptr )
        {
            //if( view_space->w - widget_box.x !=widget_box.w || paragraphWidth == 0 )
            {
                update_paragraph();
            }
            widget_basic::process_self(  view_space, cam );
        }
    }

    void widget_label_paragraph::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( view_space!=nullptr && cam!=nullptr )
        {
            if( alignment_h == gpe::fa_center)
            {
                for( int i =0; i < (int)paragraphLines.size(); i++)
                {
                    gpe::gfs->render_text( widget_box.x-cam->x + cam->w/2,widget_box.y-cam->y+(padding_default+defaultFontHeight)*i,paragraphLines[i],pawgui::theme_main->main_box_font_color,FONT_LABEL,gpe::fa_center,gpe::fa_top);
                }
            }
            else if( alignment_h == gpe::fa_right)
            {
                for( int i =0; i < (int)paragraphLines.size(); i++)
                {
                    gpe::gfs->render_text( widget_box.x-cam->x + widget_box.w,widget_box.y-cam->y+(padding_default+defaultFontHeight)*i,paragraphLines[i],pawgui::theme_main->main_box_font_color,FONT_LABEL,gpe::fa_right,gpe::fa_top);
                }
            }
            else
            {
                for( int i =0; i < (int)paragraphLines.size(); i++)
                {
                    gpe::gfs->render_text( widget_box.x-cam->x,widget_box.y-cam->y+(padding_default+defaultFontHeight)*i,paragraphLines[i],pawgui::theme_main->main_box_font_color,FONT_LABEL,gpe::fa_left,gpe::fa_top);
                }
            }

            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+paragraphWidth-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_font_url_color,true);
            }
        }
    }

    void widget_label_paragraph::set_name(std::string name_param)
    {

    }

    void widget_label_paragraph::set_width(int new_width)
    {
        if( widget_box.w!=new_width)
        {
            widget_box.w = new_width;
            update_paragraph();
        }
    }

    void widget_label_paragraph::set_height( int new_height)
    {
        if( widget_box.h!=new_height)
        {
            widget_box.h = new_height;
            update_paragraph();
        }
    }

    void widget_label_paragraph::set_max_width( int nMW)
    {
        if( widget_box.w!=nMW)
        {
            widget_box.w = nMW;
            update_paragraph();
        }
    }

    void widget_label_paragraph::set_max_height( int nMH)
    {
        if( widget_box.h!=nMH)
        {
            widget_box.h = nMH;
            update_paragraph();
        }
    }

    void widget_label_paragraph::update_paragraph()
    {
        if( widget_box.w > 0 )
        {
            clear_all_lines();
            if( (int)paragraphText.size() > 0)
            {
                int maxMessageWidth = 0;
                //int maxMessageHeight = 0;
                defaultFontWidth = 0;
                defaultFontHeight = 0;
                int iSubMessage = 0;
                if( FONT_LABEL!=nullptr)
                {
                    FONT_LABEL->get_metrics("A",&defaultFontWidth, &defaultFontHeight);
                    FONT_LABEL->clear_cache();
                }

                std::vector < std::string > messageSubTitles;

                if( defaultFontWidth > 0 && defaultFontHeight > 0)
                {
                    maxMessageWidth = ( widget_box.w -padding_default*2)/ defaultFontWidth;

                    if( (int)paragraphText.size() > 0)
                    {
                        stg_ex::wrap_string(paragraphText,messageSubTitles,maxMessageWidth,-1);
                    }

                    maxMessageWidth = 0;
                    if( messageSubTitles.size() <= 0)
                    {
                        add_line("");
                        widget_box.h = 0;
                        paragraphWidth = 0;
                    }
                    else
                    {
                        for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
                        {
                            add_line( messageSubTitles.at(iSubMessage) );
                        }
                        widget_box.h = ( (int)messageSubTitles.size() ) *(padding_default+defaultFontHeight);
                    }
                }
            }
            else
            {
                add_line("");
            }
        }
    }

    void widget_label_paragraph::update_text( std::string newParagraph)
    {
        paragraphText = newParagraph;
        update_paragraph();
    }
    widget_label_title::widget_label_title(std::string name_param, std::string description)
    {
        widget_type = "labeltitle";
        needsNewLine = true;
        widget_name = name_param;
        descriptionText = description;
        if( FONT_LABEL_TITLE!=nullptr)
        {
            int bWid = 0;
            int bHgt = 0;
            FONT_LABEL_TITLE->get_metrics(name_param.c_str(), &bWid, &bHgt);
            widget_box.w = bWid;
            widget_box.h = bHgt;
        }
        widget_name = name_param;
        hasLineBreak = true;
    }

    widget_label_title::~widget_label_title()
    {

    }

    std::string widget_label_title::get_data()
    {
        return widget_type+":"+widget_name+"==|||=="+descriptionText;
    }

    std::string widget_label_title::get_plain_string()
    {
        return "\""+widget_name+"\"";
    }

    void widget_label_title::load_data(std::string datastring)
    {
        descriptionText = datastring;
    }

    void widget_label_title::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        widget_basic::process_self(view_space,cam);
        if( isInUse)
        {
            if( gpe::input->check_kb_down(kb_ctrl) && gpe::input->check_kb_released(kb_c) )
            {
                gpe::input->clipboard_set(widget_name.c_str() );
            }
        }
        /*
        if( isHovered)
        {
            gpe::cursor_main_controller->cursor_change(GPE_CURSOR_HAND);
        }
        if( isClicked )
        {
            wasClicked = true;
            if( (int)webUrl.size() > 3)
            {
                gpe::external_open_url(webUrl);
            }
        }
        */
    }

    void widget_label_title::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( (int)widget_name.size() > 0 && view_space!=nullptr && cam!=nullptr )
        {
            gpe::gfs->render_text( widget_box.x-cam->x,widget_box.y+widget_box.h/2-cam->y,widget_name,pawgui::theme_main->main_box_font_color,FONT_LABEL_TITLE,gpe::fa_left,gpe::fa_middle);

            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_font_url_color,true);
            }
        }
    }

    void widget_label_title::set_name(std::string name_param)
    {
        widget_type = "labeltexturl";
        if( FONT_LABEL_TITLE!=nullptr && name_param!=widget_name)
        {
            int bWid = 0;
            int bHgt = 0;
            FONT_LABEL_TITLE->get_metrics(name_param.c_str(), &bWid, &bHgt);
            widget_box.w = bWid;
            widget_box.h = bHgt;
        }
        widget_name = name_param;
    }
}
