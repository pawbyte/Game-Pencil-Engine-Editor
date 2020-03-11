/*
paw_gui_overlay.cpp
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
#include "paw_gui_overlay.h"

GPE_Overlay_System * main_OVERLAY = NULL;

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
        previousScreenshot = gpe::rph->get_new_texture();
    }
    if( previousScreenshot!=NULL && gpe::renderer_main!=NULL)
    {
        gpe::renderer_main->save_screenshot(  gpe::get_user_settings_folder() +"frozen_screenshot.png");
        previousScreenshot->load_new_texture( gpe::renderer_main, gpe::get_user_settings_folder() +"frozen_screenshot.png");
    }
}

void GPE_Overlay_System::update_tooltip(std::string newTip)
{
    if( newTip!=toolTipString)
    {
        toolTipString = newTip;
        if( toolTipTexture==NULL)
        {
            toolTipTexture = gpe::rph->get_new_texture();
        }
        if( toolTipTexture!=NULL)
        {
            //toolTipTexture->loadFromRenderedText(gpe::renderer_main,newTip,theme_main->popup_box_font_color,font_default);
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
        temporayMessageDisplayStartTimer = temporayMessageDisplayTimer = messDuration* gpe::fps_cap;
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
    if( gpe::renderer_main!=NULL &&  previousScreenshot!=NULL)
    {
        previousScreenshot->render_tex_resized(0,0, gpe::screen_width, gpe::screen_height);
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
            gpe::gcanvas->render_rectangle( 0,0,256,96,theme_main->popup_box_color,false, noticeAlphaValue);
            gpe::gcanvas->render_rectangle( 0,0,256,96,theme_main->popup_box_border_color,true,noticeAlphaValue);
            render_text( 0,GENERAL_GPE_GUI_PADDING,tempUserMessageTitle,theme_main->popup_box_font_color,font_default,gpe::fa_left,gpe::fa_top,noticeAlphaValue);
            render_text( 0,GENERAL_GPE_GUI_PADDING*2+GPE_AVERAGE_LINE_HEIGHT,tempUserMessageSubtitle,theme_main->popup_box_font_color,font_default,gpe::fa_left,gpe::fa_top,noticeAlphaValue);
            render_text( 0,GENERAL_GPE_GUI_PADDING*3+GPE_AVERAGE_LINE_HEIGHT*2,tempUserMessageText,theme_main->popup_box_font_color,font_default,gpe::fa_left,gpe::fa_top,noticeAlphaValue);

        }
        else
        {
            gpe::gcanvas->render_rectangle( gpe::screen_width/2-256,gpe::screen_height-128,gpe::screen_width/2+256,gpe::screen_height-32,theme_main->popup_box_color,false, noticeAlphaValue);
            gpe::gcanvas->render_rectangle( gpe::screen_width/2-256,gpe::screen_height-128,gpe::screen_width/2+256,gpe::screen_height-32,theme_main->popup_box_border_color,true,noticeAlphaValue);
            render_text( gpe::screen_width/2,gpe::screen_height-128+GENERAL_GPE_GUI_PADDING,tempUserMessageTitle,theme_main->popup_box_font_color,font_default,gpe::fa_center,gpe::fa_top,noticeAlphaValue);
            render_text( gpe::screen_width/2,gpe::screen_height-128+GENERAL_GPE_GUI_PADDING*2+GPE_AVERAGE_LINE_HEIGHT,tempUserMessageSubtitle,theme_main->popup_box_font_color,font_default,gpe::fa_center,gpe::fa_top,noticeAlphaValue);
            render_text( gpe::screen_width/2,gpe::screen_height-128+GENERAL_GPE_GUI_PADDING*3+GPE_AVERAGE_LINE_HEIGHT*2,tempUserMessageText,theme_main->popup_box_font_color,font_default,gpe::fa_center,gpe::fa_top,noticeAlphaValue);
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
            xPos = gpe::input->mouse_position_x;
        }
        if( yPos < 0 || yPos > gpe::screen_height-GENERAL_GPE_GUI_PADDING-toolTipTexture->get_height() )
        {
            yPos = gpe::input->mouse_position_y+32;
        }

        int TEXTBOX_FONT_SIZE_WIDTH = 12;
        int TEXTBOX_FONT_SIZE_HEIGHT = 12;
        if( FONT_TOOLTIP!=NULL)
        {
            FONT_TOOLTIP->get_metrics("A",&TEXTBOX_FONT_SIZE_WIDTH,&TEXTBOX_FONT_SIZE_HEIGHT);
        }

        int xPos2 = 0;
        int yPos2 = yPos+TEXTBOX_FONT_SIZE_HEIGHT+GENERAL_GPE_GUI_PADDING/2;

        int maxLength = ( gpe::screen_width - GENERAL_GPE_GUI_PADDING*4)/TEXTBOX_FONT_SIZE_WIDTH;
        int lineCountForTip = 1;
        std::vector< std::string > toolTipLines;
        std::string tempToolTipString = toolTipString;
        if( (int)tempToolTipString.size() >= maxLength)
        {
            xPos = 0;
            xPos2 = gpe::screen_width;
            while ( (int)tempToolTipString.size() > 0 )
            {
                toolTipLines.push_back( stg_ex::get_substring(tempToolTipString,0,maxLength) );
                tempToolTipString = stg_ex::get_substring(tempToolTipString,maxLength);
            }
            yPos2= yPos+TEXTBOX_FONT_SIZE_HEIGHT*( (int)toolTipLines.size() )+GENERAL_GPE_GUI_PADDING/2;
        }
        else
        {
            toolTipLines.push_back(toolTipString);
            if(xPos+TEXTBOX_FONT_SIZE_WIDTH*(int)toolTipString.size()+GENERAL_GPE_GUI_PADDING*2 > gpe::screen_width)
            {
                xPos = gpe::screen_width-( TEXTBOX_FONT_SIZE_WIDTH*(int)toolTipString.size() )-GENERAL_GPE_GUI_PADDING*2;
                if( xPos < 0)
                {
                    xPos = 0;
                }
            }
            xPos2 = xPos+ TEXTBOX_FONT_SIZE_WIDTH*(int)toolTipString.size()+GENERAL_GPE_GUI_PADDING*2;
        }

        gpe::gcanvas->render_rectangle( xPos-GENERAL_GPE_GUI_PADDING/2,yPos-GENERAL_GPE_GUI_PADDING/2,xPos2,yPos2,theme_main->popup_box_color,false);
        for( int i = 0; i < (int)toolTipLines.size(); i++)
        {
            gpe::gfs->render_text( xPos+GENERAL_GPE_GUI_PADDING,yPos+TEXTBOX_FONT_SIZE_HEIGHT*i, toolTipLines[i],theme_main->popup_box_font_color,FONT_TOOLTIP,gpe::fa_left,gpe::fa_top);
        }

        gpe::gcanvas->render_rectangle( xPos-GENERAL_GPE_GUI_PADDING/2,yPos-GENERAL_GPE_GUI_PADDING/2,xPos2,yPos2,theme_main->popup_box_border_color,true);
    }
    tipTipJustUpdated = false;
}
