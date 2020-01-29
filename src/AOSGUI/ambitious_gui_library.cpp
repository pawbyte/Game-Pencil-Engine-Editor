/*
ambitious_gui_library.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/
#include "ambitious_gui_library.h"


GPE_StatusBar * GPE_Main_Statusbar = NULL;

void update_rectangle(GPE_Rect * rectIn, float nx, float ny, float nw, float nh)
{
    if( rectIn!=NULL)
    {
        rectIn->update_shape( nx, ny, nw, nh);
    }
}


GPE_StatusBar::GPE_StatusBar()
{
    elementBox.h = 24;
    guiListTypeName = "statusbar";
    codeEditorStatusBarString = "";
    insertModeString = "";
}

GPE_StatusBar::~GPE_StatusBar()
{

}

void GPE_StatusBar::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    codeEditorStatusBarString= "";
    insertModeString = "";
}

void GPE_StatusBar::render_self( GPE_Rect * viewedSpace,GPE_Rect *cam)
{
    if( isEnabled&& elementBox.h >0 && FONT_STATUSBAR!=NULL)
    {
        gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Program_Color,false);
        int statusBarStringWidth = 0, statusBarStringHeight = 0;
        int projectNameWidth = 0, projectNameHeight = 0;
        if( (int)statusBarLeftText.size() > 0)
        {
            std::string  projectNameToRender = statusBarLeftText;
            if( file_exists( statusBarLeftText) )
            {
                if( MAIN_GUI_SETTINGS && MAIN_GUI_SETTINGS->useShortProjectNames )
                {
                    projectNameToRender = get_file_noext( get_local_from_global_file(statusBarLeftText) );
                }
            }
            FONT_STATUSBAR->get_metrics(projectNameToRender,&projectNameWidth, &projectNameHeight);

            if( (int)codeEditorStatusBarString.size() > 0)
            {
                FONT_STATUSBAR->get_numbered_metrics(codeEditorStatusBarString,&statusBarStringWidth, &statusBarStringHeight);
                if( elementBox.w > statusBarStringWidth+projectNameWidth +GENERAL_GPE_GUI_PADDING )
                {
                    gfs->render_text( elementBox.x,elementBox.y+elementBox.h/2,projectNameToRender,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_MIDDLE);
                    //gcanvas->render_rectangle( elementBox.x+projectNameWidth,elementBox.y,elementBox.x+elementBox.w,elementBox.y+elementBox.h,GPE_MAIN_THEME->Button_Box_Color,false);
                    gfs->render_text( elementBox.x+projectNameWidth+GENERAL_GPE_GUI_PADDING,elementBox.y+elementBox.h/2,codeEditorStatusBarString,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_MIDDLE);
                }
                else
                {
                    gfs->render_text( elementBox.x+GENERAL_GPE_GUI_PADDING,elementBox.y+elementBox.h/2,codeEditorStatusBarString,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_MIDDLE);
                }
            }
            else if( elementBox.w > projectNameWidth +GENERAL_GPE_GUI_PADDING )
            {
                gfs->render_text( elementBox.x+GENERAL_GPE_GUI_PADDING,elementBox.y+elementBox.h/2,projectNameToRender,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_MIDDLE);
            }
            else
            {
                gfs->render_text( elementBox.x+elementBox.w-GENERAL_GPE_GUI_PADDING/2,elementBox.y+elementBox.h/2,projectNameToRender,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STATUSBAR,FA_RIGHT,FA_MIDDLE);
            }
        }
        else if( (int)codeEditorStatusBarString.size() > 0)
        {
            FONT_STATUSBAR->get_numbered_metrics(codeEditorStatusBarString,&statusBarStringWidth, &statusBarStringHeight);
            gfs->render_text( elementBox.x+GENERAL_GPE_GUI_PADDING,elementBox.y+elementBox.h/2,codeEditorStatusBarString,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STATUSBAR,FA_LEFT,FA_MIDDLE );
        }
        gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Main_Border_Color,true);
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

GPE_TextURL::GPE_TextURL(std::string nameIn, std::string description, std::string urlIn)
{
    webId = "";
    guiListTypeName = "texturl";
    opName = nameIn;
    descriptionText = description;
    wasClicked = false;
    webUrl = urlIn;
    if( GPE_DEFAULT_FONT!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        GPE_DEFAULT_FONT->get_metrics(nameIn, &bWid, &bHgt);
        elementBox.w = bWid;
        elementBox.h = bHgt;
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
    descriptionText = split_first_string(dataString,",,,");
    webUrl = split_first_string(dataString,",,,");
}

void GPE_TextURL::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isHovered)
    {
        gpe->cursor_change("hand");
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

void GPE_TextURL::render_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    if( (int)opName.size() > 0)
    {
        if( wasClicked)
        {
            gfs->render_text( elementBox.x-cam->x+ elementBox.w/2,elementBox.y-cam->y,opName,GPE_MAIN_THEME->Main_Box_Font_URL_Visited_Color,GPE_DEFAULT_FONT, FA_CENTER,FA_TOP);
            gcanvas->render_horizontal_line_color( elementBox.y+elementBox.h-cam->y,elementBox.x-cam->x,elementBox.x+elementBox.w-cam->x,GPE_MAIN_THEME->Main_Box_Font_URL_Visited_Color);
        }
        else if( isHovered)
        {
            gfs->render_text( elementBox.x-cam->x+ elementBox.w/2,elementBox.y-cam->y,opName,GPE_MAIN_THEME->Main_Box_Font_URL_Hovered_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP);
            gcanvas->render_horizontal_line_color( elementBox.y+elementBox.h-cam->y,elementBox.x-cam->x,elementBox.x+elementBox.w-cam->x,GPE_MAIN_THEME->Main_Box_Font_URL_Hovered_Color);
        }
        else
        {
            gfs->render_text( elementBox.x-cam->x + elementBox.w/2,elementBox.y-cam->y,opName,GPE_MAIN_THEME->Main_Box_Font_URL_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP);
            gcanvas->render_horizontal_line_color( elementBox.y+elementBox.h-cam->y,elementBox.x-cam->x,elementBox.x+elementBox.w-cam->x,GPE_MAIN_THEME->Main_Box_Font_URL_Color);
        }
        if( isInUse)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Hovered_Color,true);
        }
    }
}

void GPE_TextURL::set_name(std::string nameIn)
{
    if( GPE_DEFAULT_FONT!=NULL)
    {
        int bWid = 0;
        int bHgt = 0;
        GPE_DEFAULT_FONT->get_metrics(nameIn, &bWid, &bHgt);
        elementBox.w = bWid;
        elementBox.h = bHgt;
    }
    opName = nameIn;
}
