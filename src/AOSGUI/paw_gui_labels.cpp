/*
paw_gui_labels.cpp
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

#include "paw_gui_labels.h"

GPE_Label_Image::GPE_Label_Image(GPE_Texture * imgIn,std::string nameIn, std::string description, std::string urlIn)
{
    allowOverSize = true;
    resizeAspect = 1;
    guiListTypeName = "labelimage";
    opName = nameIn;
    descriptionText = description;
    wasClicked = false;
    isHoldingCustomTexture = false;
    webUrl = urlIn;
    imgSrc = imgIn;
    if( imgSrc!=NULL )
    {
        elementBox.w = imgSrc->get_width();
        elementBox.h = imgSrc->get_height();
    }
    else
    {
        elementBox.w = 32;
        elementBox.h = 32;
        imgSrc = NULL;
    }
}

GPE_Label_Image::~GPE_Label_Image()
{

}

void GPE_Label_Image::load_label_image(std::string fileNameIn)
{
    if( isHoldingCustomTexture)
    {
        if( imgSrc==NULL )
        {
            imgSrc = new GPE_Texture();
        }
    }
    else
    {
        imgSrc = new GPE_Texture();
        isHoldingCustomTexture = true;
    }
    if( imgSrc!=NULL )
    {
        imgSrc->load_new_texture(fileNameIn);
        elementBox.w = imgSrc->get_width();
        elementBox.h = imgSrc->get_height();
    }
}

void GPE_Label_Image::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    viewedSpace = GPE_find_camera( viewedSpace );
    cam = GPE_find_camera( cam );
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( viewedSpace!=NULL && cam!=NULL )
    {
        if( isHovered && (int)webUrl.size() > 3)
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
        }
        if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
        {
            isClicked = true;
        }
        if( isClicked )
        {
            wasClicked = true;
            if( (int)webUrl.size() > 3)
            {
                GPE_OpenURL(webUrl);
            }
        }
    }
}

void GPE_Label_Image::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && cam!=NULL )
    {
        if( imgSrc!=NULL )
        {
            imgSrc->render_tex_resized( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.w,elementBox.h,NULL );
        }
        else
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_Color,true);
        }
        if( isInUse)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
        }
        if( isHovered)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Hovered_Color,true);
        }
    }
}

void GPE_Label_Image::set_image( GPE_Texture * imgIn )
{
    imgSrc = imgIn;
    if( imgSrc!=NULL )
    {
        elementBox.w = imgSrc->get_width();
        elementBox.h = imgSrc->get_height();
    }
}

void GPE_Label_Image::set_width( int newW )
{
    //if( newW!=elementBox.w)
    {
        if( newW > 0)
        {
            double newH = 0;
            if( imgSrc!=NULL && imgSrc->get_width() > 0)
            {
                if( !allowOverSize && !isFullWidth )
                {
                    if( newW > imgSrc->get_width() )
                    {
                        newW = imgSrc->get_width();
                    }
                }

                elementBox.w = newW*resizeAspect;
                if( imgSrc!=NULL && imgSrc->get_width()!=0 )
                {
                    elementBox.h =  ( elementBox.w ) *  ( (double) imgSrc->get_height() / (double)imgSrc->get_width() );
                }
            }
        }
        else if( imgSrc!=NULL && imgSrc->get_width() > 0)
        {
            elementBox.w = imgSrc->get_width() ;
            elementBox.h = imgSrc->get_height() ;
        }
        else
        {
            elementBox.w = 0;
            elementBox.h = 0;
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
        elementBox.h = newH*resizeAspect;
        if( imgSrc!=NULL )
        {
            elementBox.w = resizeAspect * ceil( (double)newH * (double)imgSrc->get_width()/(double)imgSrc->get_height() );
        }
    }
    else
    {
        elementBox.w = 0;
        elementBox.h = 0;
    }
}

void GPE_Label_Image::set_size( double newW, double newH)
{
    elementBox.w = newW;
    elementBox.h = newH;
}

void GPE_Label_Image::set_name(std::string nameIn)
{
    opName = nameIn;
}

//

//

GPE_Label_Text::GPE_Label_Text(std::string nameIn, std::string description)
{
    guiListTypeName = "labeltext";
    guiListTypeId = 7;
    opName = nameIn;
    descriptionText = descriptionText;
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        elementBox.w = bWid;
        elementBox.h = bHgt;
    }
    opName = nameIn;
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

void GPE_Label_Text::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    /*
    if( isHovered)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
    }
    if( isClicked )
    {
        wasClicked = true;
        if( (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }
    }
    */
    if( isInUse)
    {
        if( input->check_keyboard_down(kb_ctrl) && input->check_keyboard_released(kb_c) )
        {
            SDL_SetClipboardText(opName.c_str() );
        }
    }
}

void GPE_Label_Text::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        gfs->render_text( elementBox.x-cam->x,elementBox.y-cam->y,opName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_LABEL,FA_LEFT,FA_TOP);

        if( isInUse)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
        }
    }
}

void GPE_Label_Text::set_name(std::string nameIn)
{
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        elementBox.w = bWid;
        elementBox.h = bHgt;
    }
    opName = nameIn;
}

//
//

GPE_Label_Error::GPE_Label_Error(std::string nameIn, std::string description)
{
    guiListTypeName = "labelerror";
    guiListTypeId = 7;
    opName = nameIn;
    descriptionText = descriptionText;
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        elementBox.w = bWid;
        elementBox.h = bHgt;
    }
    opName = nameIn;
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

void GPE_Label_Error::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    /*
    if( isHovered)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
    }
    if( isClicked )
    {
        wasClicked = true;
        if( (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }
    }
    */
    if( isInUse)
    {
        if( input->check_keyboard_down(kb_ctrl) && input->check_keyboard_released(kb_c) )
        {
            SDL_SetClipboardText(opName.c_str() );
        }
    }
}

void GPE_Label_Error::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        gfs->render_text( elementBox.x-cam->x,elementBox.y-cam->y,opName,GPE_MAIN_THEME->Input_Error_Font_Color,FONT_LABEL,FA_LEFT,FA_TOP);

        if( isInUse)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_Highlight_Color,true);
        }
    }
}

void GPE_Label_Error::set_name(std::string nameIn)
{
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        elementBox.w = bWid;
        elementBox.h = bHgt;
    }
    opName = nameIn;
}

//

GPE_Label_90Degree::GPE_Label_90Degree(std::string nameIn, std::string description)
{
    guiListTypeName = "labeltext";
    guiListTypeId = 7;
    opName = nameIn;
    descriptionText = descriptionText;
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        elementBox.w = bHgt;
        elementBox.h = bWid;
    }
    opName = nameIn;
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

void GPE_Label_90Degree::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    /*
    if( isHovered)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
    }
    if( isClicked )
    {
        wasClicked = true;
        if( (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }
    }
    */
    if( isInUse)
    {
        if( input->check_keyboard_down(kb_ctrl) && input->check_keyboard_released(kb_c) )
        {
            SDL_SetClipboardText(opName.c_str() );
        }
    }
}

void GPE_Label_90Degree::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        gfs->render_text_rotated( elementBox.x-cam->x,elementBox.y-cam->y+elementBox.h,opName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_LABEL,90);

        if( isInUse)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
        }
    }
}

void GPE_Label_90Degree::set_name(std::string nameIn)
{
    if( FONT_LABEL!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        elementBox.w = bHgt;
        elementBox.h = bWid;
    }
    opName = nameIn;
}

GPE_Label_Paragraph::GPE_Label_Paragraph(std::string nameIn, std::string paragraphIn, std::string descriptionIn)
{
    guiListTypeName = "paragraph";
    paragraphWidth = 0;
    needsNewLine = true;
    isFullWidth = true;
    autoResizes= true;
    opName = nameIn;
    paragraphText = paragraphIn;
    descriptionText = descriptionIn;
}

GPE_Label_Paragraph::~GPE_Label_Paragraph()
{
    paragraphLines.clear();
    if( FONT_PARAGRAGH!=NULL )
    {
        FONT_PARAGRAGH->clear_cache();
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

void GPE_Label_Paragraph::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    if( viewedSpace!=NULL )
    {
        //if( viewedSpace->w - elementBox.x !=elementBox.w || paragraphWidth == 0 )
        {
            update_paragraph();
        }
        GPE_GeneralGuiElement::process_self(  viewedSpace, cam );
    }
}

void GPE_Label_Paragraph::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && viewedSpace!=NULL && cam!=NULL )
    {
        if( hAlign == FA_CENTER)
        {
            for( int i =0; i < (int)paragraphLines.size(); i++)
            {
                gfs->render_text( elementBox.x-cam->x + cam->w/2,elementBox.y-cam->y+(GENERAL_GPE_PADDING+defaultFontHeight)*i,paragraphLines[i],GPE_MAIN_THEME->Main_Box_Font_Color,FONT_PARAGRAGH,FA_CENTER,FA_TOP);
            }
        }
        else if( hAlign == FA_RIGHT)
        {
            for( int i =0; i < (int)paragraphLines.size(); i++)
            {
                gfs->render_text( elementBox.x-cam->x + elementBox.w,elementBox.y-cam->y+(GENERAL_GPE_PADDING+defaultFontHeight)*i,paragraphLines[i],GPE_MAIN_THEME->Main_Box_Font_Color,FONT_PARAGRAGH,FA_RIGHT,FA_TOP);
            }
        }
        else
        {
            for( int i =0; i < (int)paragraphLines.size(); i++)
            {
                gfs->render_text( elementBox.x-cam->x,elementBox.y-cam->y+(GENERAL_GPE_PADDING+defaultFontHeight)*i,paragraphLines[i],GPE_MAIN_THEME->Main_Box_Font_Color,FONT_PARAGRAGH,FA_LEFT,FA_TOP);
            }
        }

        if( isInUse)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+paragraphWidth-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
        }
    }
}

void GPE_Label_Paragraph::set_name(std::string nameIn)
{

}

void GPE_Label_Paragraph::set_width(int newWidth)
{
    if( elementBox.w!=newWidth)
    {
        elementBox.w = newWidth;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::set_height( int newHeight)
{
    if( elementBox.h!=newHeight)
    {
        elementBox.h = newHeight;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::set_max_width( int nMW)
{
    if( elementBox.w!=nMW)
    {
        elementBox.w = nMW;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::set_max_height( int nMH)
{
    if( elementBox.h!=nMH)
    {
        elementBox.h = nMH;
        update_paragraph();
    }
}

void GPE_Label_Paragraph::update_paragraph()
{
    if( elementBox.w > 0 )
    {
        clear_all_lines();
        if( (int)paragraphText.size() > 0)
        {
            int maxMessageWidth = 0;
            //int maxMessageHeight = 0;
            defaultFontWidth = 0;
            defaultFontHeight = 0;
            int iSubMessage = 0;
            if( FONT_PARAGRAGH!=NULL)
            {
                FONT_PARAGRAGH->get_metrics("A",&defaultFontWidth, &defaultFontHeight);
                FONT_PARAGRAGH->clear_cache();
            }

            std::vector < std::string > messageSubTitles;

            if( defaultFontWidth > 0 && defaultFontHeight > 0)
            {
                maxMessageWidth = ( elementBox.w -GENERAL_GPE_PADDING*2)/ defaultFontWidth;

                if( (int)paragraphText.size() > 0)
                {
                    wrap_string(paragraphText,messageSubTitles,maxMessageWidth,-1);
                }

                maxMessageWidth = 0;
                if( messageSubTitles.size() <= 0)
                {
                    add_line("");
                    elementBox.h = 0;
                    paragraphWidth = 0;
                }
                else
                {
                    for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
                    {
                        add_line( messageSubTitles.at(iSubMessage) );
                    }
                    elementBox.h = ( (int)messageSubTitles.size() ) *(GENERAL_GPE_PADDING+defaultFontHeight);
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
GPE_Label_Title::GPE_Label_Title(std::string nameIn, std::string description)
{
    guiListTypeName = "labeltitle";
    needsNewLine = true;
    opName = nameIn;
    descriptionText = descriptionText;
    if( FONT_LABEL_TITLE!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL_TITLE->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        elementBox.w = bWid;
        elementBox.h = bHgt;
    }
    opName = nameIn;
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

void GPE_Label_Title::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isInUse)
    {
        if( input->check_keyboard_down(kb_ctrl) && input->check_keyboard_released(kb_c) )
        {
            SDL_SetClipboardText(opName.c_str() );
        }
    }
    /*
    if( isHovered)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
    }
    if( isClicked )
    {
        wasClicked = true;
        if( (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }
    }
    */
}

void GPE_Label_Title::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw && (int)opName.size() > 0 && viewedSpace!=NULL && cam!=NULL )
    {
        gfs->render_text( elementBox.x-cam->x,elementBox.y+elementBox.h/2-cam->y,opName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_LABEL_TITLE,FA_LEFT,FA_MIDDLE);

        if( isInUse)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
        }
    }
}

void GPE_Label_Title::set_name(std::string nameIn)
{
    guiListTypeName = "labeltexturl";
    if( FONT_LABEL_TITLE!=NULL && nameIn!=opName)
    {
        int bWid = 0;
        int bHgt = 0;
        FONT_LABEL_TITLE->get_metrics(nameIn.c_str(), &bWid, &bHgt);
        elementBox.w = bWid;
        elementBox.h = bHgt;
    }
    opName = nameIn;
}
