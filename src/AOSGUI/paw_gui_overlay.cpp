/*
paw_gui_overlay.cpp
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
#include "paw_gui_overlay.h"

GPE_Overlay_System * MAIN_OVERLAY = NULL;

GPE_Overlay_System::GPE_Overlay_System()
{
    tempUserMessageTitle = "";
    tempUserMessageSubtitle = "";
    tempUserMessageText = "";
    temporayMessageDisplayTimer = 0;
    temporayMessageDisplayStartTimer = 0;
    tempTopLeftMessage = false;
    previousScreenshot = NULL;
    toolTipString = "";
    toolTipTexture = NULL;
    toolTipX = 0;
    toolTipY = 0;
}

GPE_Overlay_System::~GPE_Overlay_System()
{
    if( previousScreenshot!=NULL)
    {
        delete previousScreenshot;
        previousScreenshot = NULL;
    }
    if( toolTipTexture!=NULL)
    {
        delete toolTipTexture;
        toolTipTexture = NULL;
    }
}

bool GPE_Overlay_System::overhead_used()
{
    if( tipTipJustUpdated)
    {
        return true;
    }
    return false;
}

void GPE_Overlay_System::take_frozen_screenshot()
{
    if( previousScreenshot==NULL)
    {
        previousScreenshot = gpeph->get_new_texture();
    }
    if( previousScreenshot!=NULL && GPE_MAIN_RENDERER!=NULL)
    {
        GPE_MAIN_RENDERER->save_screenshot( get_user_settings_folder() +"frozen_screenshot.png");
        previousScreenshot->load_new_texture( GPE_MAIN_RENDERER,get_user_settings_folder() +"frozen_screenshot.png");
    }
}

void GPE_Overlay_System::update_tooltip(std::string newTip)
{
    if( newTip!=toolTipString)
    {
        toolTipString = newTip;
        if( toolTipTexture==NULL)
        {
            toolTipTexture = gpeph->get_new_texture();
        }
        if( toolTipTexture!=NULL)
        {
            //toolTipTexture->loadFromRenderedText(GPE_MAIN_RENDERER,newTip,GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT);
        }
        tipTipJustUpdated = true;
    }
}

void GPE_Overlay_System::update_temporary_message(std::string mTitle, std::string mSubTitle, std::string mText, float messDuration, bool topLeftMessage)
{
    if( messDuration >0 &&  ( (int)mTitle.size()>0 || (int)mSubTitle.size()>0 || (int)mText.size()>0 ) )
    {
        tempUserMessageTitle = mTitle;
        tempUserMessageSubtitle = mSubTitle;
        tempUserMessageText = mText;
        tempTopLeftMessage = topLeftMessage;
        temporayMessageDisplayStartTimer = temporayMessageDisplayTimer = messDuration*FPS_CAP;
    }
    else
    {
        tempUserMessageTitle = "";
        tempUserMessageSubtitle = "";
        tempUserMessageText = "";
        temporayMessageDisplayStartTimer = temporayMessageDisplayTimer = 0;
    }
}

void GPE_Overlay_System::render_frozen_screenshot()
{
    if( GPE_MAIN_RENDERER!=NULL &&  previousScreenshot!=NULL)
    {
        previousScreenshot->render_tex_resized(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    }
}

void GPE_Overlay_System::render_temporary_message()
{
    /*
    if( temporayMessageDisplayTimer > 0 && temporayMessageDisplayStartTimer > 0 )
    {
        int noticeAlphaValue = (temporayMessageDisplayTimer/temporayMessageDisplayStartTimer)*255;
        if( tempTopLeftMessage)
        {
            gcanvas->render_rectangle( 0,0,256,96,GPE_MAIN_THEME->PopUp_Box_Color,false, noticeAlphaValue);
            gcanvas->render_rectangle( 0,0,256,96,GPE_MAIN_THEME->PopUp_Box_Border_Color,true,noticeAlphaValue);
            render_text( 0,GENERAL_GPE_GUI_PADDING,tempUserMessageTitle,GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_LEFT,FA_TOP,noticeAlphaValue);
            render_text( 0,GENERAL_GPE_GUI_PADDING*2+GPE_AVERAGE_LINE_HEIGHT,tempUserMessageSubtitle,GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_LEFT,FA_TOP,noticeAlphaValue);
            render_text( 0,GENERAL_GPE_GUI_PADDING*3+GPE_AVERAGE_LINE_HEIGHT*2,tempUserMessageText,GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_LEFT,FA_TOP,noticeAlphaValue);

        }
        else
        {
            gcanvas->render_rectangle( SCREEN_WIDTH/2-256,SCREEN_HEIGHT-128,SCREEN_WIDTH/2+256,SCREEN_HEIGHT-32,GPE_MAIN_THEME->PopUp_Box_Color,false, noticeAlphaValue);
            gcanvas->render_rectangle( SCREEN_WIDTH/2-256,SCREEN_HEIGHT-128,SCREEN_WIDTH/2+256,SCREEN_HEIGHT-32,GPE_MAIN_THEME->PopUp_Box_Border_Color,true,noticeAlphaValue);
            render_text( SCREEN_WIDTH/2,SCREEN_HEIGHT-128+GENERAL_GPE_GUI_PADDING,tempUserMessageTitle,GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP,noticeAlphaValue);
            render_text( SCREEN_WIDTH/2,SCREEN_HEIGHT-128+GENERAL_GPE_GUI_PADDING*2+GPE_AVERAGE_LINE_HEIGHT,tempUserMessageSubtitle,GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP,noticeAlphaValue);
            render_text( SCREEN_WIDTH/2,SCREEN_HEIGHT-128+GENERAL_GPE_GUI_PADDING*3+GPE_AVERAGE_LINE_HEIGHT*2,tempUserMessageText,GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP,noticeAlphaValue);
        }
        temporayMessageDisplayTimer--;
    }
    */
}

void GPE_Overlay_System::render_tooltip(int xPos, int yPos)
{
    if( (int)toolTipString.size()>0)
    {
        if( xPos < 0)
        {
            xPos = input->mouse_x;
        }
        if( yPos < 0 || yPos > SCREEN_HEIGHT-GENERAL_GPE_GUI_PADDING-toolTipTexture->get_height() )
        {
            yPos = input->mouse_y+32;
        }

        int TEXTBOX_FONT_SIZE_WIDTH = 12;
        int TEXTBOX_FONT_SIZE_HEIGHT = 12;
        if( FONT_TOOLTIP!=NULL)
        {
            FONT_TOOLTIP->get_metrics("A",&TEXTBOX_FONT_SIZE_WIDTH,&TEXTBOX_FONT_SIZE_HEIGHT);
        }

        int xPos2 = 0;
        int yPos2 = yPos+TEXTBOX_FONT_SIZE_HEIGHT+GENERAL_GPE_GUI_PADDING/2;

        int maxLength = (SCREEN_WIDTH - GENERAL_GPE_GUI_PADDING*4)/TEXTBOX_FONT_SIZE_WIDTH;
        int lineCountForTip = 1;
        std::vector< std::string > toolTipLines;
        std::string tempToolTipString = toolTipString;
        if( (int)tempToolTipString.size() >= maxLength)
        {
            xPos = 0;
            xPos2 = SCREEN_WIDTH;
            while ( (int)tempToolTipString.size() > 0 )
            {
                toolTipLines.push_back( get_substring(tempToolTipString,0,maxLength) );
                tempToolTipString = get_substring(tempToolTipString,maxLength);
            }
            yPos2= yPos+TEXTBOX_FONT_SIZE_HEIGHT*( (int)toolTipLines.size() )+GENERAL_GPE_GUI_PADDING/2;
        }
        else
        {
            toolTipLines.push_back(toolTipString);
            if(xPos+TEXTBOX_FONT_SIZE_WIDTH*(int)toolTipString.size()+GENERAL_GPE_GUI_PADDING*2 > SCREEN_WIDTH)
            {
                xPos = SCREEN_WIDTH-( TEXTBOX_FONT_SIZE_WIDTH*(int)toolTipString.size() )-GENERAL_GPE_GUI_PADDING*2;
                if( xPos < 0)
                {
                    xPos = 0;
                }
            }
            xPos2 = xPos+ TEXTBOX_FONT_SIZE_WIDTH*(int)toolTipString.size()+GENERAL_GPE_GUI_PADDING*2;
        }

        gcanvas->render_rectangle( xPos-GENERAL_GPE_GUI_PADDING/2,yPos-GENERAL_GPE_GUI_PADDING/2,xPos2,yPos2,GPE_MAIN_THEME->PopUp_Box_Color,false);
        for( int i = 0; i < (int)toolTipLines.size(); i++)
        {
            gfs->render_text( xPos+GENERAL_GPE_GUI_PADDING,yPos+TEXTBOX_FONT_SIZE_HEIGHT*i, toolTipLines[i],GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_TOOLTIP,FA_LEFT,FA_TOP);
        }

        gcanvas->render_rectangle( xPos-GENERAL_GPE_GUI_PADDING/2,yPos-GENERAL_GPE_GUI_PADDING/2,xPos2,yPos2,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
    }
    tipTipJustUpdated = false;
}
