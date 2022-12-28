/*
paw_gui_labels.cpp
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

#include "paw_gui_labels.h"

GPE_Label_Image::GPE_Label_Image(std::string name_param, std::string file_name, std::string description, std::string urlIn)
{
    allowOverSize = true;
    resizeAspect = 1;
    guiListTypeName = "labelimage";
    opName = name_param;
    descriptionText = description;
    wasClicked = false;
    isHoldingCustomTexture = false;
    webUrl = urlIn;
    imgSrc = paw_gui_rsm->texture_add_filename( file_name );
    if( imgSrc!=NULL )
    {
        widget_box.w = imgSrc->get_width();
        widget_box.h = imgSrc->get_height();
    }
    else
    {
        widget_box.w = 32;
        widget_box.h = 32;
        imgSrc = NULL;
    }
}

GPE_Label_Image::~GPE_Label_Image()
{

}

void GPE_Label_Image::load_label_image(std::string new_file_name)
{
    imgSrc = paw_gui_rsm->texture_add_filename( new_file_name );
    if( imgSrc!=NULL )
    {
        widget_box.w = imgSrc->get_width();
        widget_box.h = imgSrc->get_height();
    }
}

void GPE_Label_Image::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    viewedSpace = gpe::camera_find( viewedSpace );
    cam = gpe::camera_find( cam );
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( viewedSpace!=NULL && cam!=NULL )
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

void GPE_Label_Image::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( cam!=NULL )
    {
        if( imgSrc!=NULL )
        {
            imgSrc->render_tex_resized( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.w,widget_box.h,NULL );
        }
        else
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->main_box_font_color,true);
        }
        if( isInUse)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->main_box_font_url_color,true);
        }
        if( isHovered)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->main_box_font_url_hovered_color,true);
        }
    }
}

void GPE_Label_Image::change_texture( gpe::texture_base * imgIn )
{
    imgSrc = imgIn;
    if( imgSrc!=NULL )
    {
        widget_box.w = imgSrc->get_width();
        widget_box.h = imgSrc->get_height();
    }
}

void GPE_Label_Image::set_width( int newW )
{
    //if( newW!=widget_box.w)
    {
        if( newW > 0)
        {
            float newH = 0;
            if( imgSrc!=NULL && imgSrc->get_width() > 0)
            {
                if( !allowOverSize && !isFullWidth )
                {
                    if( newW > imgSrc->get_width() )
                    {
                        newW = imgSrc->get_width();
                    }
                }

                widget_box.w = newW*resizeAspect;
                if( imgSrc!=NULL && imgSrc->get_width()!=0 )
                {
                    widget_box.h =  ( widget_box.w ) *  ( (float) imgSrc->get_height() / (float)imgSrc->get_width() );
                }
            }
        }
        else if( imgSrc!=NULL && imgSrc->get_width() > 0)
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

void GPE_Label_Image::set_height( int newH)
{
    if( newH > 0)
    {
        if( imgSrc!=NULL && !allowOverSize)
        {
            if( newH > imgSrc->get_height() )
            {
                newH = imgSrc->get_height();
            }
        }
        widget_box.h = newH*resizeAspect;
        if( imgSrc!=NULL )
        {
            widget_box.w = resizeAspect * ceil( (float)newH * (float)imgSrc->get_width()/(float)imgSrc->get_height() );
        }
    }
    else
    {
        widget_box.w = 0;
        widget_box.h = 0;
    }
}

void GPE_Label_Image::set_size( float newW, float newH)
{
    widget_box.w = newW;
    widget_box.h = newH;
}

void GPE_Label_Image::set_name(std::string name_param)
{
    opName = name_param;
}

//

//

GPE_Label_Text::GPE_Label_Text(std::string name_param, std::string description)
{
    guiListTypeName = "labeltext";
    guiListTypeId = 7;
    opName = name_param;
    descriptionText = description;
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(name_param.c_str(), &bWid, &bHgt);
        widget_box.w = bWid;
        widget_box.h = bHgt;
    }
    opName = name_param;
    hasLineBreak = true;
}

GPE_Label_Text::~GPE_Label_Text()
{

}

std::string GPE_Label_Text::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+descriptionText;
}

std::string GPE_Label_Text::get_plain_string()
{
    return "\""+opName+"\"";
}

void GPE_Label_Text::load_data(std::string dataString)
{
    descriptionText = dataString;
}

void GPE_Label_Text::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
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
            gpe::input->clipboard_set(opName.c_str() );
        }
    }
}

void GPE_Label_Text::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        gpe::gfs->render_text( widget_box.x-cam->x,widget_box.y-cam->y,opName,theme_main->main_box_font_color,FONT_LABEL,gpe::fa_left,gpe::fa_top);

        if( isInUse)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->main_box_font_url_color,true);
        }
    }
}

void GPE_Label_Text::set_name(std::string name_param)
{
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(name_param.c_str(), &bWid, &bHgt);
        widget_box.w = bWid;
        widget_box.h = bHgt;
    }
    opName = name_param;
}

//
//

GPE_Label_Error::GPE_Label_Error(std::string name_param, std::string description)
{
    guiListTypeName = "labelerror";
    guiListTypeId = 7;
    opName = name_param;
    descriptionText = description;
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(name_param.c_str(), &bWid, &bHgt);
        widget_box.w = bWid;
        widget_box.h = bHgt;
    }
    opName = name_param;
    hasLineBreak = true;
}

GPE_Label_Error::~GPE_Label_Error()
{

}

std::string GPE_Label_Error::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+descriptionText;
}

std::string GPE_Label_Error::get_plain_string()
{
    return "\""+opName+"\"";
}

void GPE_Label_Error::load_data(std::string dataString)
{
    descriptionText = dataString;
}

void GPE_Label_Error::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
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
            gpe::input->clipboard_set(opName.c_str() );
        }
    }
}

void GPE_Label_Error::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        gpe::gfs->render_text( widget_box.x-cam->x,widget_box.y-cam->y,opName,theme_main->input_error_font_color,FONT_LABEL,gpe::fa_left,gpe::fa_top);

        if( isInUse)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->main_box_font_highlight_color,true);
        }
    }
}

void GPE_Label_Error::set_name(std::string name_param)
{
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(name_param.c_str(), &bWid, &bHgt);
        widget_box.w = bWid;
        widget_box.h = bHgt;
    }
    opName = name_param;
}

//

GPE_Label_90Degree::GPE_Label_90Degree(std::string name_param, std::string description)
{
    guiListTypeName = "labeltext";
    guiListTypeId = 7;
    opName = name_param;
    descriptionText = description;
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(name_param.c_str(), &bWid, &bHgt);
        widget_box.w = bHgt;
        widget_box.h = bWid;
    }
    opName = name_param;
    hasLineBreak = true;
}

GPE_Label_90Degree::~GPE_Label_90Degree()
{

}

std::string GPE_Label_90Degree::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+descriptionText;
}

std::string GPE_Label_90Degree::get_plain_string()
{
    return "\""+opName+"\"";
}

void GPE_Label_90Degree::load_data(std::string dataString)
{
    descriptionText = dataString;
}

void GPE_Label_90Degree::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
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
            gpe::input->clipboard_set(opName.c_str() );
        }
    }
}

void GPE_Label_90Degree::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        gpe::gfs->render_text_rotated( widget_box.x-cam->x,widget_box.y-cam->y+widget_box.h,opName,theme_main->main_box_font_color,FONT_LABEL,90);

        if( isInUse)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->main_box_font_url_color,true);
        }
    }
}

void GPE_Label_90Degree::set_name(std::string name_param)
{
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(name_param.c_str(), &bWid, &bHgt);
        widget_box.w = bHgt;
        widget_box.h = bWid;
    }
    opName = name_param;
}

GPE_Label_Paragraph::GPE_Label_Paragraph(std::string name_param, std::string paragraphIn, std::string descriptionIn)
{
    guiListTypeName = "paragraph";
    paragraphWidth = 0;
    needsNewLine = true;
    isFullWidth = true;
    autoResizes= true;
    opName = name_param;
    paragraphText = paragraphIn;
    descriptionText = descriptionIn;
}

GPE_Label_Paragraph::~GPE_Label_Paragraph()
{
    paragraphLines.clear();
    if( FONT_LABEL!=NULL )
    {
        FONT_LABEL->clear_cache();
    }
}

void GPE_Label_Paragraph::add_line(std::string newLine)
{
    paragraphLines.push_back(newLine);
    paragraphWidth = 0;
}

void GPE_Label_Paragraph::clear_all_lines()
{
    paragraphLines.clear();
    paragraphWidth = 0;
}

std::string GPE_Label_Paragraph::get_paragraph()
{
    return paragraphText;
}

void GPE_Label_Paragraph::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    if( viewedSpace!=NULL )
    {
        //if( viewedSpace->w - widget_box.x !=widget_box.w || paragraphWidth == 0 )
        {
            update_paragraph();
        }
        GPE_GeneralGuiElement::process_self(  viewedSpace, cam );
    }
}

void GPE_Label_Paragraph::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( viewedSpace!=NULL && cam!=NULL )
    {
        if( hAlign == gpe::fa_center)
        {
            for( int i =0; i < (int)paragraphLines.size(); i++)
            {
                gpe::gfs->render_text( widget_box.x-cam->x + cam->w/2,widget_box.y-cam->y+(GENERAL_GPE_GUI_PADDING+defaultFontHeight)*i,paragraphLines[i],theme_main->main_box_font_color,FONT_LABEL,gpe::fa_center,gpe::fa_top);
            }
        }
        else if( hAlign == gpe::fa_right)
        {
            for( int i =0; i < (int)paragraphLines.size(); i++)
            {
                gpe::gfs->render_text( widget_box.x-cam->x + widget_box.w,widget_box.y-cam->y+(GENERAL_GPE_GUI_PADDING+defaultFontHeight)*i,paragraphLines[i],theme_main->main_box_font_color,FONT_LABEL,gpe::fa_right,gpe::fa_top);
            }
        }
        else
        {
            for( int i =0; i < (int)paragraphLines.size(); i++)
            {
                gpe::gfs->render_text( widget_box.x-cam->x,widget_box.y-cam->y+(GENERAL_GPE_GUI_PADDING+defaultFontHeight)*i,paragraphLines[i],theme_main->main_box_font_color,FONT_LABEL,gpe::fa_left,gpe::fa_top);
            }
        }

        if( isInUse)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+paragraphWidth-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->main_box_font_url_color,true);
        }
    }
}

void GPE_Label_Paragraph::set_name(std::string name_param)
{

}

void GPE_Label_Paragraph::set_width(int newWidth)
{
    if( widget_box.w!=newWidth)
    {
        widget_box.w = newWidth;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::set_height( int newHeight)
{
    if( widget_box.h!=newHeight)
    {
        widget_box.h = newHeight;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::set_max_width( int nMW)
{
    if( widget_box.w!=nMW)
    {
        widget_box.w = nMW;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::set_max_height( int nMH)
{
    if( widget_box.h!=nMH)
    {
        widget_box.h = nMH;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::update_paragraph()
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
            if( FONT_LABEL!=NULL)
            {
                FONT_LABEL->get_metrics("A",&defaultFontWidth, &defaultFontHeight);
                FONT_LABEL->clear_cache();
            }

            std::vector < std::string > messageSubTitles;

            if( defaultFontWidth > 0 && defaultFontHeight > 0)
            {
                maxMessageWidth = ( widget_box.w -GENERAL_GPE_GUI_PADDING*2)/ defaultFontWidth;

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
                    widget_box.h = ( (int)messageSubTitles.size() ) *(GENERAL_GPE_GUI_PADDING+defaultFontHeight);
                }
            }
        }
        else
        {
            add_line("");
        }
    }
}

void GPE_Label_Paragraph::update_text( std::string newParagraph)
{
    paragraphText = newParagraph;
    update_paragraph();
}
GPE_Label_Title::GPE_Label_Title(std::string name_param, std::string description)
{
    guiListTypeName = "labeltitle";
    needsNewLine = true;
    opName = name_param;
    descriptionText = description;
    if( FONT_LABEL_TITLE!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL_TITLE->get_metrics(name_param.c_str(), &bWid, &bHgt);
        widget_box.w = bWid;
        widget_box.h = bHgt;
    }
    opName = name_param;
    hasLineBreak = true;
}

GPE_Label_Title::~GPE_Label_Title()
{

}

std::string GPE_Label_Title::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+descriptionText;
}

std::string GPE_Label_Title::get_plain_string()
{
    return "\""+opName+"\"";
}

void GPE_Label_Title::load_data(std::string dataString)
{
    descriptionText = dataString;
}

void GPE_Label_Title::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isInUse)
    {
        if( gpe::input->check_kb_down(kb_ctrl) && gpe::input->check_kb_released(kb_c) )
        {
            gpe::input->clipboard_set(opName.c_str() );
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

void GPE_Label_Title::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        gpe::gfs->render_text( widget_box.x-cam->x,widget_box.y+widget_box.h/2-cam->y,opName,theme_main->main_box_font_color,FONT_LABEL_TITLE,gpe::fa_left,gpe::fa_middle);

        if( isInUse)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->main_box_font_url_color,true);
        }
    }
}

void GPE_Label_Title::set_name(std::string name_param)
{
    guiListTypeName = "labeltexturl";
    if( FONT_LABEL_TITLE!=NULL && name_param!=opName)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL_TITLE->get_metrics(name_param.c_str(), &bWid, &bHgt);
        widget_box.w = bWid;
        widget_box.h = bHgt;
    }
    opName = name_param;
}
