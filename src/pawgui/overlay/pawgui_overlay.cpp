/*
pawgui_overlay.cpp
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
#include "pawgui_overlay.h"

namespace pawgui
{
    overlay_system * main_overlay_system = nullptr;

    overlay_system::overlay_system()
    {
        tempUserMessageTitle = "";
        tempUserMessageSubtitle = "";
        tempUserMessageText = "";
        temporayMessageDisplayTimer = 0;
        temporayMessageDisplayStartTimer = 0;
        tempTopLeftMessage = false;
        previousScreenshot = nullptr;
        toolTipstring = "";
        toolTipTexture = nullptr;
        toolTipX = 0;
        toolTipY = 0;
    }

    overlay_system::~overlay_system()
    {

    }

    bool overlay_system::overhead_used()
    {
        if( tipTipJustUpdated)
        {
            return true;
        }
        return false;
    }

    void overlay_system::take_frozen_screenshot()
    {
        if( previousScreenshot==nullptr)
        {
            previousScreenshot = gpe::rph->get_new_texture();
        }
        if( previousScreenshot!=nullptr && gpe::renderer_main!=nullptr)
        {
            gpe::renderer_main->save_screenshot(  gpe::get_user_settings_folder() +"frozen_screenshot.png");
            previousScreenshot->load_new_texture( gpe::get_user_settings_folder() +"frozen_screenshot.png");
        }
    }

    void overlay_system::update_tooltip(std::string newTip)
    {
        if( newTip!=toolTipstring)
        {
            toolTipstring = newTip;
            if( toolTipTexture==nullptr)
            {
                toolTipTexture = gpe::rph->get_new_texture();
            }
            if( toolTipTexture!=nullptr)
            {
                //toolTipTexture->loadFromRenderedText(gpe::renderer_main,newTip,pawgui::theme_main->popup_box_font_color,gpe::font_default);
            }
            tipTipJustUpdated = true;
        }
    }

    void overlay_system::update_temporary_message(std::string mTitle, std::string mSubTitle, std::string mText, float messDuration, bool topLeftMessage)
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

    void overlay_system::render_frozen_screenshot()
    {
        if( gpe::renderer_main!=nullptr &&  previousScreenshot!=nullptr)
        {
            previousScreenshot->render_tex_resized(0,0, gpe::screen_width, gpe::screen_height);
        }
    }

    void overlay_system::render_temporary_message()
    {
        /*
        if( temporayMessageDisplayTimer > 0 && temporayMessageDisplayStartTimer > 0 )
        {
            int noticeAlphaValue = (temporayMessageDisplayTimer/temporayMessageDisplayStartTimer)*255;
            if( tempTopLeftMessage)
            {
                gpe::gcanvas->render_rectangle( 0,0,256,96,pawgui::theme_main->popup_box_color,false, noticeAlphaValue);
                gpe::gcanvas->render_rectangle( 0,0,256,96,pawgui::theme_main->popup_box_border_color,true,noticeAlphaValue);
                render_text( 0,padding_default,tempUserMessageTitle,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_top,noticeAlphaValue);
                render_text( 0,padding_default*2+default_line_height,tempUserMessageSubtitle,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_top,noticeAlphaValue);
                render_text( 0,padding_default*3+default_line_height*2,tempUserMessageText,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_top,noticeAlphaValue);

            }
            else
            {
                gpe::gcanvas->render_rectangle( gpe::screen_width/2-256,gpe::screen_height-128,gpe::screen_width/2+256,gpe::screen_height-32,pawgui::theme_main->popup_box_color,false, noticeAlphaValue);
                gpe::gcanvas->render_rectangle( gpe::screen_width/2-256,gpe::screen_height-128,gpe::screen_width/2+256,gpe::screen_height-32,pawgui::theme_main->popup_box_border_color,true,noticeAlphaValue);
                render_text( gpe::screen_width/2,gpe::screen_height-128+padding_default,tempUserMessageTitle,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_top,noticeAlphaValue);
                render_text( gpe::screen_width/2,gpe::screen_height-128+padding_default*2+default_line_height,tempUserMessageSubtitle,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_top,noticeAlphaValue);
                render_text( gpe::screen_width/2,gpe::screen_height-128+padding_default*3+default_line_height*2,tempUserMessageText,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_center,gpe::fa_top,noticeAlphaValue);
            }
            temporayMessageDisplayTimer--;
        }
        */
    }

    void overlay_system::render_tooltip(int x_pos, int y_pos)
    {
        if( (int)toolTipstring.size()>0)
        {
            if( x_pos < 0)
            {
                x_pos = gpe::input->mouse_position_x;
            }
            if( y_pos < 0 || y_pos > gpe::screen_height-padding_default-toolTipTexture->get_height() )
            {
                y_pos = gpe::input->mouse_position_y+32;
            }

            int TEXTBOX_FONT_SIZE_WIDTH = 12;
            int TEXTBOX_FONT_SIZE_HEIGHT = 12;
            if( FONT_TOOLTIP!=nullptr)
            {
                FONT_TOOLTIP->get_metrics("A",&TEXTBOX_FONT_SIZE_WIDTH,&TEXTBOX_FONT_SIZE_HEIGHT);
            }

            int x_pos2 = 0;
            int y_pos2 = y_pos+TEXTBOX_FONT_SIZE_HEIGHT+padding_default/2;

            int maxLength = ( gpe::screen_width - padding_default*4)/TEXTBOX_FONT_SIZE_WIDTH;
            int lineCountForTip = 1;
            std::vector< std::string > toolTipLines;
            std::string tempToolTipstring = toolTipstring;
            if( (int)tempToolTipstring.size() >= maxLength)
            {
                x_pos = 0;
                x_pos2 = gpe::screen_width;
                while ( (int)tempToolTipstring.size() > 0 )
                {
                    toolTipLines.push_back( stg_ex::get_substring(tempToolTipstring,0,maxLength) );
                    tempToolTipstring = stg_ex::get_substring(tempToolTipstring,maxLength);
                }
                y_pos2= y_pos+TEXTBOX_FONT_SIZE_HEIGHT*( (int)toolTipLines.size() )+padding_default/2;
            }
            else
            {
                toolTipLines.push_back(toolTipstring);
                if(x_pos+TEXTBOX_FONT_SIZE_WIDTH*(int)toolTipstring.size()+padding_default*2 > gpe::screen_width)
                {
                    x_pos = gpe::screen_width-( TEXTBOX_FONT_SIZE_WIDTH*(int)toolTipstring.size() )-padding_default*2;
                    if( x_pos < 0)
                    {
                        x_pos = 0;
                    }
                }
                x_pos2 = x_pos+ TEXTBOX_FONT_SIZE_WIDTH*(int)toolTipstring.size()+padding_default*2;
            }

            gpe::gcanvas->render_rectangle( x_pos-padding_default/2,y_pos-padding_default/2,x_pos2,y_pos2,pawgui::theme_main->popup_box_color,false);
            for( int i = 0; i < (int)toolTipLines.size(); i++)
            {
                gpe::gfs->render_text( x_pos+padding_default,y_pos+TEXTBOX_FONT_SIZE_HEIGHT*i, toolTipLines[i],pawgui::theme_main->popup_box_font_color,FONT_TOOLTIP,gpe::fa_left,gpe::fa_top);
            }

            gpe::gcanvas->render_rectangle( x_pos-padding_default/2,y_pos-padding_default/2,x_pos2,y_pos2,pawgui::theme_main->popup_box_border_color,true);
        }
        tipTipJustUpdated = false;
    }
}
