/*
ambitious_gui_library.cpp
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
#include "ambitious_gui_library.h"


GPE_StatusBar * GPE_main_Statusbar = NULL;

GPE_StatusBar::GPE_StatusBar()
{
    widget_box.h = 24;
    guiListTypeName = "statusbar";
    codeEditorStatusBarString = "";
    insertModeString = "";
}

GPE_StatusBar::~GPE_StatusBar()
{

}

void GPE_StatusBar::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    codeEditorStatusBarString= "";
    insertModeString = "";
}

void GPE_StatusBar::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    if( isEnabled&& widget_box.h >0 && FONT_STATUSBAR!=NULL)
    {
        gpe::gcanvas->render_rect( &widget_box,theme_main->program_color,false);
        int statusBarStringWidth = 0, statusBarStringHeight = 0;
        int projectNameWidth = 0, projectNameHeight = 0;
        if( (int)statusBarLeftText.size() > 0)
        {
            std::string  projectNameToRender = statusBarLeftText;
            if( sff_ex::file_exists( statusBarLeftText) )
            {
                if( main_GUI_SETTINGS && main_GUI_SETTINGS->useShortProjectNames )
                {
                    projectNameToRender = stg_ex::get_file_noext( stg_ex::get_local_from_global_file(statusBarLeftText) );
                }
            }
            FONT_STATUSBAR->get_metrics(projectNameToRender,&projectNameWidth, &projectNameHeight);

            if( (int)codeEditorStatusBarString.size() > 0)
            {
                FONT_STATUSBAR->get_numbered_metrics(codeEditorStatusBarString,&statusBarStringWidth, &statusBarStringHeight);
                if( widget_box.w > statusBarStringWidth+projectNameWidth +GENERAL_GPE_GUI_PADDING )
                {
                    gpe::gfs->render_text( widget_box.x,widget_box.y+widget_box.h/2,projectNameToRender,theme_main->main_box_font_color,FONT_STATUSBAR, gpe::fa_left, gpe::fa_middle);
                    //gpe::gcanvas->render_rectangle( widget_box.x+projectNameWidth,widget_box.y,widget_box.x+widget_box.w,widget_box.y+widget_box.h,theme_main->button_box_color,false);
                    gpe::gfs->render_text( widget_box.x+projectNameWidth+GENERAL_GPE_GUI_PADDING,widget_box.y+widget_box.h/2,codeEditorStatusBarString,theme_main->main_box_font_color,FONT_STATUSBAR,gpe::fa_left,gpe::fa_middle);
                }
                else
                {
                    gpe::gfs->render_text( widget_box.x+GENERAL_GPE_GUI_PADDING,widget_box.y+widget_box.h/2,codeEditorStatusBarString,theme_main->main_box_font_color,FONT_STATUSBAR, gpe::fa_left, gpe::fa_middle);
                }
            }
            else if( widget_box.w > projectNameWidth +GENERAL_GPE_GUI_PADDING )
            {
                gpe::gfs->render_text( widget_box.x+GENERAL_GPE_GUI_PADDING,widget_box.y+widget_box.h/2,projectNameToRender,theme_main->main_box_font_color,FONT_STATUSBAR, gpe::fa_left, gpe::fa_middle);
            }
            else
            {
                gpe::gfs->render_text( widget_box.x+widget_box.w-GENERAL_GPE_GUI_PADDING/2,widget_box.y+widget_box.h/2,projectNameToRender,theme_main->main_box_font_color,FONT_STATUSBAR, gpe::fa_right, gpe::fa_middle);
            }
        }
        else if( (int)codeEditorStatusBarString.size() > 0)
        {
            FONT_STATUSBAR->get_numbered_metrics(codeEditorStatusBarString,&statusBarStringWidth, &statusBarStringHeight);
            gpe::gfs->render_text( widget_box.x+GENERAL_GPE_GUI_PADDING,widget_box.y+widget_box.h/2,codeEditorStatusBarString,theme_main->main_box_font_color,FONT_STATUSBAR, gpe::fa_left, gpe::fa_middle );
        }
        gpe::gcanvas->render_rect( &widget_box,theme_main->main_border_color,true);
    }
}

void GPE_StatusBar::reset_statusbar()
{
    codeEditorStatusBarString = "";
    insertModeString = "";
}

void GPE_StatusBar::set_codestring(std::string newCodeString)
{
    codeEditorStatusBarString = newCodeString;
}

void GPE_StatusBar::set_insertmode(std::string newInsertMode)
{
    insertModeString = newInsertMode;
}






//

GPE_TextURL::GPE_TextURL(std::string name_param, std::string description, std::string urlIn)
{
    webId = "";
    guiListTypeName = "texturl";
    opName = name_param;
    descriptionText = description;
    wasClicked = false;
    webUrl = urlIn;
    if( font_default!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        font_default->get_metrics(name_param, &bWid, &bHgt);
        widget_box.w = bWid;
        widget_box.h = bHgt;
    }
}

GPE_TextURL::~GPE_TextURL()
{

}

std::string GPE_TextURL::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+descriptionText+",,,"+webUrl+",,,";
}

std::string GPE_TextURL::get_plain_string()
{
    return "\""+webUrl+"\"";
}

void GPE_TextURL::load_data(std::string dataString)
{
    descriptionText = stg_ex::split_first_string(dataString,",,,");
    webUrl = stg_ex::split_first_string(dataString,",,,");
}

void GPE_TextURL::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
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

void GPE_TextURL::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    if( (int)opName.size() > 0)
    {
        if( wasClicked)
        {
            gpe::gfs->render_text( widget_box.x-cam->x+ widget_box.w/2,widget_box.y-cam->y,opName,theme_main->main_box_font_url_visited_color,font_default, gpe::fa_center,gpe::fa_top);
            gpe::gcanvas->render_horizontal_line_color( widget_box.y+widget_box.h-cam->y,widget_box.x-cam->x,widget_box.x+widget_box.w-cam->x,theme_main->main_box_font_url_visited_color);
        }
        else if( isHovered)
        {
            gpe::gfs->render_text( widget_box.x-cam->x+ widget_box.w/2,widget_box.y-cam->y,opName,theme_main->main_box_font_url_hovered_color,font_default,gpe::fa_center,gpe::fa_top);
            gpe::gcanvas->render_horizontal_line_color( widget_box.y+widget_box.h-cam->y,widget_box.x-cam->x,widget_box.x+widget_box.w-cam->x,theme_main->main_box_font_url_hovered_color);
        }
        else
        {
            gpe::gfs->render_text( widget_box.x-cam->x + widget_box.w/2,widget_box.y-cam->y,opName,theme_main->main_box_font_url_color,font_default,gpe::fa_center,gpe::fa_top);
            gpe::gcanvas->render_horizontal_line_color( widget_box.y+widget_box.h-cam->y,widget_box.x-cam->x,widget_box.x+widget_box.w-cam->x,theme_main->main_box_font_url_color);
        }
        if( isInUse)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->main_box_font_url_hovered_color,true);
        }
    }
}

void GPE_TextURL::set_name(std::string name_param)
{
    if( font_default!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        font_default->get_metrics(name_param, &bWid, &bHgt);
        widget_box.w = bWid;
        widget_box.h = bHgt;
    }
    opName = name_param;
}
