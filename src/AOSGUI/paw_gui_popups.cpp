/*
paw_gui_popups.cpp
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

#include "paw_gui_base.h"
#include "paw_gui_buttons.h"
#include "paw_gui_labels.h"
#include "paw_gui_popups.h"
#include "paw_gui_text_input.h"

GPE_LoaderDiplay * GPE_LOADER = NULL;

GPE_LoaderDiplay::GPE_LoaderDiplay()
{
    item1Max = item1Value = 0;
    item2Max = item2Value = 0;
    displayMessageTitle = "";
    displayMessageSubtitle = "";
    displayMessageString = "";
}

GPE_LoaderDiplay::~GPE_LoaderDiplay()
{

}

void GPE_LoaderDiplay::increment_bar1()
{
    item1Value++;
    if( item1Value >= item1Max )
    {
        item1Value = item1Max;
    }
}
void GPE_LoaderDiplay::increment_bar2()
{
    item2Value++;
    if( item2Value >= item2Max )
    {
        item2Value = item2Max;
    }
}
void GPE_LoaderDiplay::increment_and_update( std::string subTitle, std::string message, int barNumber, bool renderUpdate )
{
    if( barNumber == 2 )
    {
        increment_bar2();
    }
    else
    {
        //defaults to bar 1
        increment_bar1();
    }
    update_submessages( subTitle, message, renderUpdate );
}

void GPE_LoaderDiplay::set_bar1( double maxValue)
{
    if( maxValue> 0 )
    {
        item1Max = maxValue;
        item1Value = 0;
    }
    else
    {
        item1Max = item1Value  = 0;
    }
}

void GPE_LoaderDiplay::set_bar2( double maxValue)
{
    if( maxValue> 0 )
    {
        item2Max = maxValue;
        item2Value = 0;
    }
    else
    {
        item1Max = item2Value  = 0;
    }
}

void GPE_LoaderDiplay::render_loader()
{
    //if( input->input_received() )
    {
        //Update screen
        gpe->end_loop( false,true );
        gpe->start_loop();
        gcanvas->render_rectangle( 32,SCREEN_HEIGHT/2 - 48,SCREEN_WIDTH-32,SCREEN_HEIGHT/2 + 48,GPE_MAIN_THEME->PopUp_Box_Color,false);
        gfs->render_text( SCREEN_WIDTH/2,SCREEN_HEIGHT/2 - 32,displayMessageTitle,GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP);
        gfs->render_text( SCREEN_WIDTH/2,SCREEN_HEIGHT/2,displayMessageSubtitle,GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP);
        gfs->render_text( SCREEN_WIDTH/2,SCREEN_HEIGHT/2 + 32,displayMessageString,GPE_MAIN_THEME->PopUp_Box_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP);
        gcanvas->render_rectangle( 32,SCREEN_HEIGHT/2 - 48,SCREEN_WIDTH-32,SCREEN_HEIGHT/2 + 48,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);

        gpe->end_loop( false,true );
        gpe->start_loop();
    }
}



void GPE_LoaderDiplay::reset()
{
    item1Max = item1Value = 0;
    item2Max =  item2Value = 0;
    displayMessageTitle = "";
    displayMessageSubtitle = "";
    displayMessageString = "";
}

void GPE_LoaderDiplay::update_messages(std::string title, std::string subTitle, std::string message, bool renderUpdate  )
{
    displayMessageTitle = title;
    update_submessages( subTitle, message, renderUpdate );
}

void GPE_LoaderDiplay::update_submessages(  std::string subTitle, std::string message, bool renderUpdate  )
{
    displayMessageSubtitle = subTitle;
    displayMessageString = message;
    if( renderUpdate )
    {
        render_loader();
    }
}
void display_user_alert(std::string messageTitle, std::string messageContent, int messageType,GPE_Rect * cam)
{
    gpe->end_loop();
    //RESOURCE_TO_DRAG = NULL;
    GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
    MAIN_OVERLAY->process_cursor();
    //GPE_MAIN_GUI->reset_gui_info();
    MAIN_OVERLAY->update_tooltip("");
    MAIN_OVERLAY->take_frozen_screenshot( );
    input->reset_all_input();
    bool exitOperation = false;
    bool windowBeingDragged = false;

    GPE_Rect elementBox;

    elementBox.w = SCREEN_WIDTH*3/4 -128 - GENERAL_GPE_PADDING*4;
    elementBox.h = SCREEN_HEIGHT*3/4 - 64 - GENERAL_GPE_PADDING*4;
    elementBox.x = SCREEN_WIDTH - elementBox.w;
    elementBox.y = SCREEN_HEIGHT - elementBox.h;
    int maxMessageWidth = 0;
    int maxMessageHeight = 0;
    int defaultFontWidth = 0;
    int defaultFontHeight = 0;
    int iSubMessage = 0;
    if( FONT_DEFAULT_PROMPT!=NULL)
    {
        FONT_DEFAULT_PROMPT->get_metrics("A",&defaultFontWidth, &defaultFontHeight);
    }

    std::vector < std::string > messageSubTitles;

    GPE_ToolLabelButton * okButton = new GPE_ToolLabelButton( "Okay","");
    if( defaultFontWidth > 0 && defaultFontHeight > 0)
    {
        maxMessageWidth = (elementBox.w-64) / defaultFontWidth;
        maxMessageHeight = (elementBox.h-64) / (defaultFontHeight+GENERAL_GPE_PADDING);
        if( (int)messageTitle.size() > 0)
        {
            if((int)messageTitle.size() > maxMessageWidth )
            {
                messageTitle = messageTitle.substr(0,maxMessageWidth-3)+"...";
            }
        }
        else
        {
            messageTitle = "Alert";
        }

        if( (int)messageContent.size() > 0)
        {
            wrap_string(messageContent,messageSubTitles,maxMessageWidth,maxMessageHeight);
        }
        else
        {
            messageContent = "...";
            messageSubTitles.push_back(messageContent);
        }

        if( (int) messageSubTitles.size() < maxMessageHeight )
        {
            elementBox.h = messageSubTitles.size()*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
            elementBox.y = (SCREEN_HEIGHT - elementBox.h)/2;
        }
        else
        {
            elementBox.h = maxMessageHeight*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
            elementBox.y = (SCREEN_HEIGHT - elementBox.h)/2;
        }

        maxMessageWidth = (int)messageTitle.size();
        for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
        {
            if( (int)messageSubTitles.at(iSubMessage).size()  > maxMessageWidth )
            {
                maxMessageWidth = (int)messageSubTitles.at(iSubMessage).size();
            }
        }

        elementBox.w = (maxMessageWidth*defaultFontWidth)+128 + GENERAL_GPE_PADDING*4;
        elementBox.x = (SCREEN_WIDTH - elementBox.w)/2;

        while( exitOperation==false )
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            gpe->start_loop();
            //GPE_MAIN_GUI->reset_gui_info();

            if( windowBeingDragged )
            {
                GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZEALL);
                if( input->check_mouse_down( mb_left ) )
                {
                    elementBox.x = input->mouse_x-elementBox.w/2;
                    elementBox.y = input->mouse_y;
                }
                else
                {
                    windowBeingDragged = false;
                }

                if( elementBox.x+elementBox.w > SCREEN_WIDTH )
                {
                    elementBox.x = SCREEN_WIDTH - elementBox.w;
                }

                if( elementBox.y+elementBox.h > SCREEN_HEIGHT )
                {
                    elementBox.y = SCREEN_HEIGHT - elementBox.h;
                }

                if( elementBox.x < 0)
                {
                    elementBox.x = 0;
                }

                if( elementBox.y < 0)
                {
                    elementBox.y = 0;
                }
            }
            else
            {
                if( input->check_mouse_released( mb_left))
                {
                    if( point_between(input->mouse_x,input->mouse_y,elementBox.x+elementBox.w-32,elementBox.y,elementBox.x+elementBox.w,elementBox.y+30) )
                    {
                        exitOperation = true;
                    }
                }
                else if( input->check_mouse_down( mb_left ) )
                {
                    if( point_between(input->mouse_x,input->mouse_y,elementBox.x,elementBox.y,elementBox.x+elementBox.w-32,elementBox.y+32) )
                    {
                        windowBeingDragged = true;
                    }
                }
            }
            if( okButton!=NULL)
            {
                okButton->set_coords(elementBox.x+( elementBox.w-okButton->get_width() )/2,elementBox.y+elementBox.h-GENERAL_GPE_PADDING-okButton->get_height() );
                okButton->process_self( NULL, NULL);
                if( okButton->is_clicked() && !windowBeingDragged )
                {
                    exitOperation = true;
                }
            }
            if( input->check_keyboard_released(kb_esc) || input->check_keyboard_released(kb_space) || input->check_keyboard_released(kb_enter) || WINDOW_WAS_JUST_RESIZED)
            {
                exitOperation = true;
            }

            MAIN_RENDERER->reset_viewpoint();
            //if( WINDOW_WAS_JUST_RESIZED )
            {
                gcanvas->render_rectangle( 0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GPE_MAIN_THEME->Program_Color, false, 255);
                MAIN_OVERLAY->render_frozen_screenshot( );
            }
            //Update screen
            gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Color,false);

            gcanvas->render_rectangle( elementBox.x,elementBox.y,elementBox.x+elementBox.w,elementBox.y+32,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,false);

            gfs->render_text( elementBox.x+elementBox.w/2,elementBox.y+GENERAL_GPE_PADDING,messageTitle,GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_TOP);
            gfs->render_text( elementBox.x+elementBox.w-GENERAL_GPE_PADDING,elementBox.y+GENERAL_GPE_PADDING,"X",GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color,GPE_DEFAULT_FONT,FA_RIGHT,FA_TOP);

            for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
            {
                gfs->render_text( elementBox.x+32,elementBox.y+GENERAL_GPE_PADDING+32+iSubMessage*(defaultFontHeight+GENERAL_GPE_PADDING),messageSubTitles.at(iSubMessage),GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
            }
            if( okButton!=NULL)
            {
                okButton->render_self(  NULL, NULL, true);
            }
            if( point_within_rect(input->mouse_x,input->mouse_y,&elementBox) )
            {
                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Highlight_Alt_Color,true);
            }
            else
            {
                gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
            }
            MAIN_OVERLAY->process_cursor();
            gpe->end_loop();
        }
    }
    if( okButton!=NULL )
    {
        delete okButton;
        okButton = NULL;
    }
    gpe->start_loop();
}

int GPE_Display_Basic_Prompt(std::string messageTitle, std::string messageContent, bool showCancelButton )
{
    gpe->end_loop();
    //RESOURCE_TO_DRAG = NULL;
    int maxMessageWidth = 0;
    int maxMessageHeight = 0;
    int defaultFontWidth = 12;
    int defaultFontHeight = 12;
    std::vector < std::string > messageSubTitles;
    int iSubMessage = 0;
    GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
    MAIN_OVERLAY->process_cursor();
    MAIN_OVERLAY->take_frozen_screenshot( );
    input->reset_all_input();
    bool exitOperation = false;
    int returnVal = DISPLAY_QUERY_CANCEL;
    GPE_ToolLabelButton * yesButton = new GPE_ToolLabelButton( "Yes[ENTER]","");
    yesButton->set_width(96);
    GPE_ToolLabelButton * noButon = NULL;
    GPE_ToolLabelButton * cancelButton = NULL;
    if( showCancelButton)
    {
        cancelButton = new GPE_ToolLabelButton( "Cancel[ESC]","");
        cancelButton->set_width(96);
        noButon = new GPE_ToolLabelButton( "No[N]","");
        noButon->set_width(96);
    }
    else
    {
        noButon = new GPE_ToolLabelButton( "No[ESC]","");
        noButon->set_width(96);
    }

    bool boxIsMoving = false;
    bool boxWasResized = true;
    bool boxBeingResized  = false;

    if( FONT_DEFAULT_PROMPT!=NULL)
    {
        FONT_DEFAULT_PROMPT->get_metrics("A",&defaultFontWidth, &defaultFontHeight);
    }

    GPE_Rect elementBox;
    elementBox.w = 528;
    elementBox.h = 320;
    elementBox.x = (SCREEN_WIDTH - elementBox.w)/2;
    elementBox.y = 72;
    elementBox.x = (SCREEN_WIDTH - elementBox.w)/2;
    elementBox.y = 72;
    maxMessageWidth = (elementBox.w-32) / defaultFontWidth;
    maxMessageHeight = (elementBox.h-64) / (defaultFontHeight+GENERAL_GPE_PADDING);
    int newBarX2Pos = elementBox.x+elementBox.w;
    int newBarY2Pos = elementBox.w+elementBox.h;
    MAIN_OVERLAY->render_frozen_screenshot( );

    if( defaultFontWidth > 0 && defaultFontHeight > 0)
    {
        maxMessageWidth = (elementBox.w-32) / defaultFontWidth;
        maxMessageHeight = (elementBox.h-64) / (defaultFontHeight+GENERAL_GPE_PADDING);

        messageSubTitles.clear();
        if( (int)messageTitle.size() > 0)
        {
            if((int)messageTitle.size() > maxMessageWidth )
            {
                messageTitle = messageTitle.substr(0,maxMessageWidth-3)+"...";
            }
        }
        else
        {
            messageTitle = "Alert";
        }

        if( (int)messageContent.size() > 0)
        {
            wrap_string(messageContent,messageSubTitles,maxMessageWidth,maxMessageHeight);
        }
        else
        {
            messageContent = "...";
            messageSubTitles.push_back(messageContent);
        }

        if( (int) messageSubTitles.size() < maxMessageHeight )
        {
            elementBox.h = messageSubTitles.size()*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
        }
        else
        {
            elementBox.h = maxMessageHeight*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
        }

        maxMessageWidth = (int)messageTitle.size();
        for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
        {
            if( (int)messageSubTitles.at(iSubMessage).size()  > maxMessageWidth )
            {
                maxMessageWidth = (int)messageSubTitles.at(iSubMessage).size();
            }
        }

        elementBox.x = (SCREEN_WIDTH-elementBox.w)/2;
        newBarX2Pos = elementBox.x+elementBox.w;
        newBarY2Pos = elementBox.w+elementBox.h;
        int elementBoxMinWidth = 64;
        int elementBoxMinHeight = elementBox.h;

        while(exitOperation==false )
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            //Start the frame timer
            gpe->start_loop();
            //GPE_MAIN_GUI->reset_gui_info();

            if( input->check_mouse_down( mb_left ) && !input->check_mouse_pressed(0) )
            {
                if( boxIsMoving)
                {
                    GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZEALL);
                    elementBox.x = input->mouse_x-elementBox.w/2;
                    elementBox.y = input->mouse_y;
                }
                else if( boxBeingResized)
                {
                    GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZENWSE);
                    newBarX2Pos = input->mouse_x;
                    if( newBarX2Pos > elementBox.x+elementBoxMinWidth && newBarX2Pos < SCREEN_WIDTH-32 && newBarX2Pos> 0 )
                    {
                        elementBox.w = newBarX2Pos-elementBox.x;
                        boxWasResized = true;
                    }
                    newBarY2Pos = input->mouse_y;
                    if( newBarY2Pos > elementBox.y+elementBoxMinHeight && newBarY2Pos < SCREEN_HEIGHT-32 && newBarY2Pos > 0 )
                    {
                        elementBox.h = newBarY2Pos-elementBox.y;
                        boxWasResized = true;
                    }
                }
            }

            if( point_between(input->mouse_x,input->mouse_y,elementBox.x,elementBox.y,elementBox.x+elementBox.w,elementBox.y+32) )
            {
                GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZEALL);
                if( input->check_mouse_pressed(0) )
                {
                    boxIsMoving = true;
                }
            }
            else if( point_between(input->mouse_x,input->mouse_y,elementBox.x+elementBox.w-32,elementBox.y+elementBox.h-32,elementBox.x+elementBox.w,elementBox.y+elementBox.h) )
            {
                GPE_change_cursor(SDL_SYSTEM_CURSOR_SIZENWSE);
                if( input->check_mouse_pressed( mb_left ) )
                {
                    boxBeingResized = true;
                }
            }

            if( input->check_mouse_released( mb_anybutton  ) )
            {
                boxIsMoving = false;
                boxBeingResized = false;
                GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
            }
            if( elementBox.w < elementBoxMinWidth)
            {
                elementBox.w = elementBoxMinWidth;
            }
            if( elementBox.h < elementBoxMinHeight)
            {
                elementBox.h = elementBoxMinHeight;
            }
            if(  elementBox.x+elementBox.w > SCREEN_WIDTH)
            {
                elementBox.x = SCREEN_WIDTH-elementBox.w;
                boxWasResized = true;
            }
            if( elementBox.x < 0 )
            {
                elementBox.x = 0;
                boxWasResized = true;
            }

            if(  elementBox.y+elementBox.h > SCREEN_HEIGHT )
            {
                elementBox.y = SCREEN_HEIGHT-elementBox.h;
                boxWasResized = true;
            }
            if( elementBox.y < 0 )
            {
                elementBox.y = 0;
                boxWasResized = true;
            }

            if( boxWasResized)
            {
                maxMessageWidth = (elementBox.w-32) / defaultFontWidth;
                maxMessageHeight = (elementBox.h-64) / (defaultFontHeight+GENERAL_GPE_PADDING);

                messageSubTitles.clear();
                /*if( (int)messageTitle.size() > 0)
                {
                    if((int)messageTitle.size() > maxMessageWidth )
                    {
                        messageTitle = messageTitle.substr(0,maxMessageWidth-3)+"...";
                    }
                }
                else
                {
                    messageTitle = "Alert";
                }*/

                if( (int)messageContent.size() > 0)
                {
                    wrap_string(messageContent,messageSubTitles,maxMessageWidth,maxMessageHeight);
                }
                else
                {
                    messageContent = "...";
                    messageSubTitles.push_back(messageContent);
                }
                /*if( (int) messageSubTitles.size() < maxMessageHeight )
                {
                    elementBox.h = messageSubTitles.size()*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
                }
                else
                {
                    elementBox.h = maxMessageHeight*(defaultFontHeight+GENERAL_GPE_PADDING)+GENERAL_GPE_PADDING*4+64;
                }*/

                maxMessageWidth = (int)messageTitle.size();
                for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
                {
                    if( (int)messageSubTitles.at(iSubMessage).size()  > maxMessageWidth )
                    {
                        maxMessageWidth = (int)messageSubTitles.at(iSubMessage).size();
                    }
                }
                boxWasResized = false;
            }

            yesButton->set_coords( GENERAL_GPE_PADDING+64,elementBox.h-32);
            noButon->set_coords(yesButton->get_xpos()+yesButton->get_width()+GENERAL_GPE_PADDING,yesButton->get_ypos() );
            if( cancelButton!=NULL)
            {
                cancelButton->set_coords( noButon->get_xpos()+noButon->get_width()+GENERAL_GPE_PADDING,noButon->get_ypos() );
                cancelButton->process_self(&elementBox,&GPE_DEFAULT_CAMERA);
            }
            yesButton->process_self(&elementBox,&GPE_DEFAULT_CAMERA);
            noButon->process_self(&elementBox,&GPE_DEFAULT_CAMERA) ;


            if( input->check_keyboard_released(kb_esc) || MAIN_RENDERER->windowClosed )
            {
                if( cancelButton!=NULL)
                {
                    exitOperation = true;
                    returnVal = DISPLAY_QUERY_CANCEL;
                }
                else
                {
                    exitOperation = true;
                    returnVal = DISPLAY_QUERY_NO;
                }
            }
            else if( input->check_keyboard_released(kb_n) )
            {
                exitOperation = true;
                returnVal = DISPLAY_QUERY_NO;
            }
            else if( noButon->is_clicked() )
            {
                exitOperation = true;
                returnVal = DISPLAY_QUERY_NO;
            }
            else if( cancelButton!=NULL &&  cancelButton->is_clicked() )
            {
                exitOperation = true;
                returnVal = DISPLAY_QUERY_CANCEL;
            }
            else if( input->check_keyboard_released(kb_enter) || yesButton->is_clicked() )
            {
                exitOperation = true;
                returnVal = DISPLAY_QUERY_YES;
            }
            if( !WINDOW_WAS_JUST_RESIZED)
            {
                MAIN_RENDERER->reset_viewpoint();
                //Update screen
                //if( input->windowEventHappendInFrame )
                {
                    MAIN_OVERLAY->render_frozen_screenshot( );
                }
                MAIN_RENDERER->set_viewpoint( &elementBox);

                gcanvas->render_rectangle( 0,0,elementBox.w,elementBox.h,GPE_MAIN_THEME->PopUp_Box_Color,false);
                gcanvas->render_rectangle( 0,0,elementBox.w,32,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,false);

                gfs->render_text( GENERAL_GPE_PADDING*2,GENERAL_GPE_PADDING,messageTitle,GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
                //render_text( elementBox.x+GENERAL_GPE_PADDING+32,elementBox.y+64,messageContent,GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
                for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
                {
                    gfs->render_text( 32,GENERAL_GPE_PADDING+32+iSubMessage*(defaultFontHeight+GENERAL_GPE_PADDING),messageSubTitles.at(iSubMessage),GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
                }

                yesButton->render_self( &elementBox,&GPE_DEFAULT_CAMERA);
                noButon->render_self( &elementBox,&GPE_DEFAULT_CAMERA);
                if( cancelButton!=NULL)
                {
                    cancelButton->render_self( &elementBox,&GPE_DEFAULT_CAMERA);
                }
                gcanvas->render_rectangle( 1,1,elementBox.w-1,elementBox.h-1,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
                gcanvas->render_rectangle( 0,0,elementBox.w,elementBox.h,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,true);
                //GPE_MAIN_GUI-render_gui_info(MAIN_RENDERER, true);
                MAIN_OVERLAY->process_cursor();
            }
            gpe->end_loop();
        }
    }

    MAIN_RENDERER->reset_viewpoint();
    //GPE_MAIN_GUI->render_gui_info(MAIN_RENDERER, true);
    if( FONT_DEFAULT_PROMPT!=NULL)
    {
        FONT_DEFAULT_PROMPT->clear_cache();
    }
    if( yesButton!=NULL)
    {
        delete yesButton;
        yesButton = NULL;
    }
    if( noButon!=NULL)
    {
        delete noButon;
        noButon = NULL;
    }
    if( cancelButton!=NULL)
    {
        delete cancelButton;
        cancelButton = NULL;
    }
    input->reset_all_input();
    messageSubTitles.clear();

    gpe->start_loop();
    return returnVal;
}

std::string get_string_from_popup(std::string messageTitle, std::string messageContent, std::string startName)
{
    gpe->end_loop();
    //RESOURCE_TO_DRAG = NULL;
    MAIN_OVERLAY->update_tooltip("");
    GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
    MAIN_OVERLAY->process_cursor();
    MAIN_OVERLAY->take_frozen_screenshot( );
    input->reset_all_input();
    bool exitOperation = false;
    int frame = 0;
    int TEXTBOX_FONT_SIZE_WIDTH = 12;
    int TEXTBOX_FONT_SIZE_HEIGHT = 12;
    if( FONT_DEFAULT_PROMPT!=NULL)
    {
        FONT_DEFAULT_PROMPT->get_metrics("A",&TEXTBOX_FONT_SIZE_WIDTH,&TEXTBOX_FONT_SIZE_HEIGHT);
    }
    int titleWidth = TEXTBOX_FONT_SIZE_WIDTH * (int)messageTitle.size();
    int messageWidth = TEXTBOX_FONT_SIZE_WIDTH * (int)messageContent.size();
    int biggestStringWidth = std::max(titleWidth, messageWidth);
    std::string returnVal = startName;
    GPE_ToolLabelButton * yesButton = new GPE_ToolLabelButton( "Okay","");
    GPE_ToolLabelButton * cancelButton = new GPE_ToolLabelButton( "Cancel","");
    GPE_TextInputBasic * newStringBox = new GPE_TextInputBasic(startName);
    newStringBox->switch_inuse(true);
    int buttonsWidth = yesButton->get_width()+cancelButton->get_width();
    int promptBoxWidth = buttonsWidth;
    if( promptBoxWidth < biggestStringWidth)
    {
        promptBoxWidth =biggestStringWidth;
    }
    promptBoxWidth+=GENERAL_GPE_PADDING*3;
    GPE_Rect elementBox;
    elementBox.x = (SCREEN_WIDTH-promptBoxWidth)/2-GENERAL_GPE_PADDING;
    elementBox.y = SCREEN_HEIGHT/2-64-GENERAL_GPE_PADDING;
    elementBox.w = promptBoxWidth+128;
    elementBox.h = 192;
    MAIN_OVERLAY->render_frozen_screenshot( );
    while(exitOperation==false)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_ARROW);
        gpe->start_loop();

        //GPE_MAIN_GUI->reset_gui_info();

        elementBox.x = (SCREEN_WIDTH-promptBoxWidth)/2-GENERAL_GPE_PADDING;
        elementBox.y = SCREEN_HEIGHT/2-64-GENERAL_GPE_PADDING;

        newStringBox->set_coords( elementBox.x+GENERAL_GPE_PADDING,elementBox.y+64);
        newStringBox->set_width(elementBox.w - 64);

        yesButton->set_coords( elementBox.x+GENERAL_GPE_PADDING,newStringBox->get_ypos()+newStringBox->get_height() + GENERAL_GPE_PADDING);
        cancelButton->set_coords( yesButton->get_xpos()+yesButton->get_width()+GENERAL_GPE_PADDING,yesButton->get_ypos() );

        newStringBox->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        yesButton->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        cancelButton->process_self(&GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);

        if( input->check_keyboard_released(kb_esc) || cancelButton->is_clicked() )
        {
            exitOperation = true;
            returnVal = "";
        }
        else if( input->check_keyboard_released(kb_enter) || yesButton->is_clicked() )
        {
            exitOperation = true;
            if( newStringBox!=NULL)
            {
                returnVal = newStringBox->get_string();
            }
        }

        //Update screen
        //if( input->windowEventHappendInFrame )
        {
            MAIN_OVERLAY->render_frozen_screenshot( );
        }
        gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Color,false);
        gcanvas->render_rectangle( elementBox.x,elementBox.y,elementBox.x+elementBox.w,elementBox.y+32,GPE_MAIN_THEME->PopUp_Box_Highlight_Color,false);

        gfs->render_text( (SCREEN_WIDTH-promptBoxWidth)/2+GENERAL_GPE_PADDING,SCREEN_HEIGHT/2-64,messageTitle,GPE_MAIN_THEME->PopUp_Box_Highlight_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
        gfs->render_text( (SCREEN_WIDTH-promptBoxWidth)/2+GENERAL_GPE_PADDING,SCREEN_HEIGHT/2-32,messageContent,GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_DEFAULT_PROMPT,FA_LEFT,FA_TOP);
        yesButton->render_self( &GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        cancelButton->render_self( &GPE_DEFAULT_CAMERA,&GPE_DEFAULT_CAMERA);
        newStringBox->render_self( &GPE_DEFAULT_CAMERA, &GPE_DEFAULT_CAMERA);
        gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
        //GPE_MAIN_GUI-render_gui_info(MAIN_RENDERER, true);
        MAIN_OVERLAY->process_cursor();
        //GPE_MAIN_GUI->render_gui_info(MAIN_RENDERER, true);
        gpe->end_loop();
    }
    if( yesButton!=NULL)
    {
        delete yesButton;
        yesButton = NULL;
    }
    if( cancelButton!=NULL)
    {
        delete cancelButton;
        cancelButton = NULL;
    }
    if( newStringBox!=NULL)
    {
        delete newStringBox;
        newStringBox = NULL;
    }

    input->reset_all_input();
    gpe->start_loop();
    return returnVal;
}

