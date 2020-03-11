/*
paw_gui_popups.cpp
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

void GPE_LoaderDiplay::set_bar1( float max_value)
{
    if( max_value> 0 )
    {
        item1Max = max_value;
        item1Value = 0;
    }
    else
    {
        item1Max = item1Value  = 0;
    }
}

void GPE_LoaderDiplay::set_bar2( float max_value)
{
    if( max_value> 0 )
    {
        item2Max = max_value;
        item2Value = 0;
    }
    else
    {
        item1Max = item2Value  = 0;
    }
}

void GPE_LoaderDiplay::render_loader()
{
    //if( gpe::input->has_new_input() )
    {
        //Update screen
        gpe::game_runtime->end_loop( );
        gpe::game_runtime->start_loop();
        gpe::gcanvas->render_rectangle( 32,gpe::screen_height/2 - 48,gpe::screen_width-32,gpe::screen_height/2 + 48,theme_main->popup_box_color,false);
        gpe::gfs->render_text( gpe::screen_width/2,gpe::screen_height/2 - 32,displayMessageTitle,theme_main->popup_box_font_color,font_default,gpe::fa_center,gpe::fa_top);
        gpe::gfs->render_text( gpe::screen_width/2,gpe::screen_height/2,displayMessageSubtitle,theme_main->popup_box_font_color,font_default,gpe::fa_center,gpe::fa_top);
        gpe::gfs->render_text( gpe::screen_width/2,gpe::screen_height/2 + 32,displayMessageString,theme_main->popup_box_font_color,font_default,gpe::fa_center,gpe::fa_top);
        gpe::gcanvas->render_rectangle( 32,gpe::screen_height/2 - 48,gpe::screen_width-32,gpe::screen_height/2 + 48,theme_main->popup_box_border_color,true);

        gpe::game_runtime->end_loop();
        gpe::game_runtime->start_loop();
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
void display_user_alert(std::string messageTitle, std::string messageContent, int messageType, gpe::shape_rect * cam)
{
    gpe::game_runtime->end_loop();
    //RESOURCE_TO_DRAG = NULL;
    gpe::cursor_main_controller->cursor_change("arrow");
    //editor_gui_main->reset_gui_info();
    main_OVERLAY->update_tooltip("");
    main_OVERLAY->take_frozen_screenshot( );
    gpe::input->reset_all_input();
    bool exitOperation = false;
    bool windowBeingDragged = false;

    gpe::shape_rect widget_box;

    widget_box.w = gpe::screen_width*3/4 -128 - GENERAL_GPE_GUI_PADDING*4;
    widget_box.h = gpe::screen_height*3/4 - 64 - GENERAL_GPE_GUI_PADDING*4;
    widget_box.x = gpe::screen_width - widget_box.w;
    widget_box.y = gpe::screen_height - widget_box.h;
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
        maxMessageWidth = (widget_box.w-64) / defaultFontWidth;
        maxMessageHeight = (widget_box.h-64) / (defaultFontHeight+GENERAL_GPE_GUI_PADDING);
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
            stg_ex::wrap_string(messageContent,messageSubTitles,maxMessageWidth,maxMessageHeight);
        }
        else
        {
            messageContent = "...";
            messageSubTitles.push_back(messageContent);
        }

        if( (int) messageSubTitles.size() < maxMessageHeight )
        {
            widget_box.h = messageSubTitles.size()*(defaultFontHeight+GENERAL_GPE_GUI_PADDING)+GENERAL_GPE_GUI_PADDING*4+64;
            widget_box.y = (gpe::screen_height - widget_box.h)/2;
        }
        else
        {
            widget_box.h = maxMessageHeight*(defaultFontHeight+GENERAL_GPE_GUI_PADDING)+GENERAL_GPE_GUI_PADDING*4+64;
            widget_box.y = (gpe::screen_height - widget_box.h)/2;
        }

        maxMessageWidth = (int)messageTitle.size();
        for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
        {
            if( (int)messageSubTitles.at(iSubMessage).size()  > maxMessageWidth )
            {
                maxMessageWidth = (int)messageSubTitles.at(iSubMessage).size();
            }
        }

        widget_box.w = (maxMessageWidth*defaultFontWidth)+128 + GENERAL_GPE_GUI_PADDING*4;
        widget_box.x = (gpe::screen_width - widget_box.w)/2;

        while( exitOperation==false )
        {
            gpe::cursor_main_controller->cursor_change("arrow");
            gpe::game_runtime->start_loop();
            //editor_gui_main->reset_gui_info();

            if( windowBeingDragged )
            {
                gpe::cursor_main_controller->cursor_change("sizeall");
                if( gpe::input->check_mouse_down( mb_left ) )
                {
                    widget_box.x = gpe::input->mouse_position_x-widget_box.w/2;
                    widget_box.y = gpe::input->mouse_position_y;
                }
                else
                {
                    windowBeingDragged = false;
                }

                if( widget_box.x+widget_box.w > gpe::screen_width )
                {
                    widget_box.x = gpe::screen_width - widget_box.w;
                }

                if( widget_box.y+widget_box.h > gpe::screen_height )
                {
                    widget_box.y = gpe::screen_height - widget_box.h;
                }

                if( widget_box.x < 0)
                {
                    widget_box.x = 0;
                }

                if( widget_box.y < 0)
                {
                    widget_box.y = 0;
                }
            }
            else
            {
                if( gpe::input->check_mouse_released( mb_left))
                {
                    if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,widget_box.x+widget_box.w-32,widget_box.y,widget_box.x+widget_box.w,widget_box.y+30) )
                    {
                        exitOperation = true;
                    }
                }
                else if( gpe::input->check_mouse_down( mb_left ) )
                {
                    if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,widget_box.x,widget_box.y,widget_box.x+widget_box.w-32,widget_box.y+32) )
                    {
                        windowBeingDragged = true;
                    }
                }
            }
            if( okButton!=NULL)
            {
                okButton->set_coords(widget_box.x+( widget_box.w-okButton->get_width() )/2,widget_box.y+widget_box.h-GENERAL_GPE_GUI_PADDING-okButton->get_height() );
                okButton->process_self( NULL, NULL);
                if( okButton->is_clicked() && !windowBeingDragged )
                {
                    exitOperation = true;
                }
            }
            if( gpe::input->check_kb_released(kb_esc) || gpe::input->check_kb_released(kb_space) || gpe::input->check_kb_released(kb_enter) || gpe::window_controller_main->is_resized() )
            {
                exitOperation = true;
            }

            gpe::renderer_main->reset_viewpoint();
            //if( WINDOW_WAS_JUST_RESIZED )
            {
                gpe::gcanvas->render_rectangle( 0,0,gpe::screen_width,gpe::screen_height,theme_main->program_color, false, 255);
                main_OVERLAY->render_frozen_screenshot( );
            }
            //Update screen
            gpe::gcanvas->render_rect( &widget_box,theme_main->popup_box_color,false);

            gpe::gcanvas->render_rectangle( widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+32,theme_main->popup_box_highlight_color,false);

            gpe::gfs->render_text( widget_box.x+widget_box.w/2,widget_box.y+GENERAL_GPE_GUI_PADDING,messageTitle,theme_main->popup_box_highlight_font_color,font_default,gpe::fa_center,gpe::fa_top);
            gpe::gfs->render_text( widget_box.x+widget_box.w-GENERAL_GPE_GUI_PADDING,widget_box.y+GENERAL_GPE_GUI_PADDING,"×",theme_main->popup_box_highlight_font_color,font_default,gpe::fa_right,gpe::fa_top);

            for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
            {
                gpe::gfs->render_text( widget_box.x+32,widget_box.y+GENERAL_GPE_GUI_PADDING+32+iSubMessage*(defaultFontHeight+GENERAL_GPE_GUI_PADDING),messageSubTitles.at(iSubMessage),theme_main->popup_box_font_color,FONT_DEFAULT_PROMPT,gpe::fa_left,gpe::fa_top);
            }
            if( okButton!=NULL)
            {
                okButton->render_self(  NULL, NULL);
            }
            if( gpe::point_within_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&widget_box) )
            {
                gpe::gcanvas->render_rect( &widget_box,theme_main->popup_box_highlight_alt_color,true);
            }
            else
            {
                gpe::gcanvas->render_rect( &widget_box,theme_main->popup_box_border_color,true);
            }
            gpe::game_runtime->end_loop();
        }
    }
    if( okButton!=NULL )
    {
        delete okButton;
        okButton = NULL;
    }
    gpe::game_runtime->start_loop();
}

int GPE_Display_Basic_Prompt(std::string messageTitle, std::string messageContent, bool showCancelButton )
{
    gpe::game_runtime->end_loop();
    //RESOURCE_TO_DRAG = NULL;
    int maxMessageWidth = 0;
    int maxMessageHeight = 0;
    int defaultFontWidth = 12;
    int defaultFontHeight = 12;
    std::vector < std::string > messageSubTitles;
    int iSubMessage = 0;
    gpe::cursor_main_controller->cursor_change("arrow");
    main_OVERLAY->take_frozen_screenshot( );
    gpe::input->reset_all_input();
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

    gpe::shape_rect widget_box;
    widget_box.w = 528;
    widget_box.h = 320;
    widget_box.x = (gpe::screen_width - widget_box.w)/2;
    widget_box.y = 72;
    widget_box.x = (gpe::screen_width - widget_box.w)/2;
    widget_box.y = 72;
    maxMessageWidth = (widget_box.w-32) / defaultFontWidth;
    maxMessageHeight = (widget_box.h-64) / (defaultFontHeight+GENERAL_GPE_GUI_PADDING);
    int newBarX2Pos = widget_box.x+widget_box.w;
    int newBarY2Pos = widget_box.w+widget_box.h;
    main_OVERLAY->render_frozen_screenshot( );

    if( defaultFontWidth > 0 && defaultFontHeight > 0)
    {
        maxMessageWidth = (widget_box.w-32) / defaultFontWidth;
        maxMessageHeight = (widget_box.h-64) / (defaultFontHeight+GENERAL_GPE_GUI_PADDING);

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
            stg_ex::wrap_string(messageContent,messageSubTitles,maxMessageWidth,maxMessageHeight);
        }
        else
        {
            messageContent = "...";
            messageSubTitles.push_back(messageContent);
        }

        if( (int) messageSubTitles.size() < maxMessageHeight )
        {
            widget_box.h = messageSubTitles.size()*(defaultFontHeight+GENERAL_GPE_GUI_PADDING)+GENERAL_GPE_GUI_PADDING*4+64;
        }
        else
        {
            widget_box.h = maxMessageHeight*(defaultFontHeight+GENERAL_GPE_GUI_PADDING)+GENERAL_GPE_GUI_PADDING*4+64;
        }

        maxMessageWidth = (int)messageTitle.size();
        for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
        {
            if( (int)messageSubTitles.at(iSubMessage).size()  > maxMessageWidth )
            {
                maxMessageWidth = (int)messageSubTitles.at(iSubMessage).size();
            }
        }

        widget_box.x = (gpe::screen_width-widget_box.w)/2;
        newBarX2Pos = widget_box.x+widget_box.w;
        newBarY2Pos = widget_box.w+widget_box.h;
        int widget_boxMinWidth = 64;
        int widget_boxMinHeight = widget_box.h;

        while(exitOperation==false )
        {
            gpe::cursor_main_controller->cursor_change("arrow");
            //Start the frame timer
            gpe::game_runtime->start_loop();
            //editor_gui_main->reset_gui_info();

            if( gpe::input->check_mouse_down( mb_left ) && !gpe::input->check_mouse_pressed(0) )
            {
                if( boxIsMoving)
                {
                    gpe::cursor_main_controller->cursor_change("sizeall");
                    widget_box.x = gpe::input->mouse_position_x-widget_box.w/2;
                    widget_box.y = gpe::input->mouse_position_y;
                }
                else if( boxBeingResized)
                {
                    gpe::cursor_main_controller->cursor_change("sizenwse");
                    newBarX2Pos = gpe::input->mouse_position_x;
                    if( newBarX2Pos > widget_box.x+widget_boxMinWidth && newBarX2Pos < gpe::screen_width-32 && newBarX2Pos> 0 )
                    {
                        widget_box.w = newBarX2Pos-widget_box.x;
                        boxWasResized = true;
                    }
                    newBarY2Pos = gpe::input->mouse_position_y;
                    if( newBarY2Pos > widget_box.y+widget_boxMinHeight && newBarY2Pos < gpe::screen_height-32 && newBarY2Pos > 0 )
                    {
                        widget_box.h = newBarY2Pos-widget_box.y;
                        boxWasResized = true;
                    }
                }
            }

            if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+32) )
            {
                gpe::cursor_main_controller->cursor_change("sizeall");
                if( gpe::input->check_mouse_pressed(0) )
                {
                    boxIsMoving = true;
                }
            }
            else if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,widget_box.x+widget_box.w-32,widget_box.y+widget_box.h-32,widget_box.x+widget_box.w,widget_box.y+widget_box.h) )
            {
                gpe::cursor_main_controller->cursor_change("sizenwse");
                if( gpe::input->check_mouse_pressed( mb_left ) )
                {
                    boxBeingResized = true;
                }
            }

            if( gpe::input->check_mouse_released( mb_anybutton  ) )
            {
                boxIsMoving = false;
                boxBeingResized = false;
                gpe::cursor_main_controller->cursor_change("arrow");
            }
            if( widget_box.w < widget_boxMinWidth)
            {
                widget_box.w = widget_boxMinWidth;
            }
            if( widget_box.h < widget_boxMinHeight)
            {
                widget_box.h = widget_boxMinHeight;
            }
            if(  widget_box.x+widget_box.w > gpe::screen_width)
            {
                widget_box.x = gpe::screen_width-widget_box.w;
                boxWasResized = true;
            }
            if( widget_box.x < 0 )
            {
                widget_box.x = 0;
                boxWasResized = true;
            }

            if(  widget_box.y+widget_box.h > gpe::screen_height )
            {
                widget_box.y = gpe::screen_height-widget_box.h;
                boxWasResized = true;
            }
            if( widget_box.y < 0 )
            {
                widget_box.y = 0;
                boxWasResized = true;
            }

            if( boxWasResized)
            {
                maxMessageWidth = (widget_box.w-32) / defaultFontWidth;
                maxMessageHeight = (widget_box.h-64) / (defaultFontHeight+GENERAL_GPE_GUI_PADDING);

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
                    stg_ex::wrap_string(messageContent,messageSubTitles,maxMessageWidth,maxMessageHeight);
                }
                else
                {
                    messageContent = "...";
                    messageSubTitles.push_back(messageContent);
                }
                /*if( (int) messageSubTitles.size() < maxMessageHeight )
                {
                    widget_box.h = messageSubTitles.size()*(defaultFontHeight+GENERAL_GPE_GUI_PADDING)+GENERAL_GPE_GUI_PADDING*4+64;
                }
                else
                {
                    widget_box.h = maxMessageHeight*(defaultFontHeight+GENERAL_GPE_GUI_PADDING)+GENERAL_GPE_GUI_PADDING*4+64;
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

            yesButton->set_coords( GENERAL_GPE_GUI_PADDING+64,widget_box.h-32);
            noButon->set_coords(yesButton->get_xpos()+yesButton->get_width()+GENERAL_GPE_GUI_PADDING,yesButton->get_ypos() );
            if( cancelButton!=NULL)
            {
                cancelButton->set_coords( noButon->get_xpos()+noButon->get_width()+GENERAL_GPE_GUI_PADDING,noButon->get_ypos() );
                cancelButton->process_self(&widget_box);
            }
            yesButton->process_self(&widget_box);
            noButon->process_self(&widget_box) ;


            if( gpe::input->check_kb_released(kb_esc) || gpe::window_controller_main->window_closed )
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
            else if( gpe::input->check_kb_released(kb_n) )
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
            else if( gpe::input->check_kb_released(kb_enter) || yesButton->is_clicked() )
            {
                exitOperation = true;
                returnVal = DISPLAY_QUERY_YES;
            }
            if( !gpe::window_controller_main->is_resized() )
            {
                gpe::renderer_main->reset_viewpoint();
                //Update screen
                //if( gpe::input->window_input_received )
                {
                    main_OVERLAY->render_frozen_screenshot( );
                }
                gpe::renderer_main->set_viewpoint( &widget_box);

                gpe::gcanvas->render_rectangle( 0,0,widget_box.w,widget_box.h,theme_main->popup_box_color,false);
                gpe::gcanvas->render_rectangle( 0,0,widget_box.w,32,theme_main->popup_box_highlight_color,false);

                gpe::gfs->render_text( GENERAL_GPE_GUI_PADDING*2,GENERAL_GPE_GUI_PADDING,messageTitle,theme_main->popup_box_highlight_font_color,FONT_DEFAULT_PROMPT,gpe::fa_left,gpe::fa_top);
                //render_text( widget_box.x+GENERAL_GPE_GUI_PADDING+32,widget_box.y+64,messageContent,theme_main->popup_box_font_color,FONT_DEFAULT_PROMPT,gpe::fa_left,gpe::fa_top);
                for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
                {
                    gpe::gfs->render_text( 32,GENERAL_GPE_GUI_PADDING+32+iSubMessage*(defaultFontHeight+GENERAL_GPE_GUI_PADDING),messageSubTitles.at(iSubMessage),theme_main->popup_box_font_color,FONT_DEFAULT_PROMPT,gpe::fa_left,gpe::fa_top);
                }

                yesButton->render_self( &widget_box );
                noButon->render_self( &widget_box );
                if( cancelButton!=NULL)
                {
                    cancelButton->render_self( &widget_box );
                }
                gpe::gcanvas->render_rectangle( 1,1,widget_box.w-1,widget_box.h-1,theme_main->popup_box_border_color,true);
                gpe::gcanvas->render_rectangle( 0,0,widget_box.w,widget_box.h,theme_main->popup_box_highlight_color,true);
                //editor_gui_main-render_gui_info(gpe::renderer_main, true);
            }
            gpe::game_runtime->end_loop();
        }
    }

    gpe::renderer_main->reset_viewpoint();
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
    gpe::input->reset_all_input();
    messageSubTitles.clear();

    gpe::game_runtime->start_loop();
    return returnVal;
}

std::string get_string_from_popup(std::string messageTitle, std::string messageContent, std::string startName)
{
    gpe::game_runtime->end_loop();
    //RESOURCE_TO_DRAG = NULL;
    main_OVERLAY->update_tooltip("");
    gpe::cursor_main_controller->cursor_change("arrow");
    main_OVERLAY->take_frozen_screenshot( );
    gpe::input->reset_all_input();
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
    gpe_text_widget_string * newStringBox = new gpe_text_widget_string(startName);
    newStringBox->switch_inuse(true);
    int buttonsWidth = yesButton->get_width()+cancelButton->get_width();
    int promptBoxWidth = buttonsWidth;
    if( promptBoxWidth < biggestStringWidth)
    {
        promptBoxWidth =biggestStringWidth;
    }
    promptBoxWidth+=GENERAL_GPE_GUI_PADDING*3;
    gpe::shape_rect widget_box;
    widget_box.x = (gpe::screen_width-promptBoxWidth)/2-GENERAL_GPE_GUI_PADDING;
    widget_box.y = gpe::screen_height/2-64-GENERAL_GPE_GUI_PADDING;
    widget_box.w = promptBoxWidth+128;
    widget_box.h = 192;
    main_OVERLAY->render_frozen_screenshot( );
    while(exitOperation==false)
    {
        gpe::cursor_main_controller->cursor_change("arrow");
        gpe::game_runtime->start_loop();

        //editor_gui_main->reset_gui_info();

        widget_box.x = (gpe::screen_width-promptBoxWidth)/2-GENERAL_GPE_GUI_PADDING;
        widget_box.y = gpe::screen_height/2-64-GENERAL_GPE_GUI_PADDING;

        newStringBox->set_coords( widget_box.x+GENERAL_GPE_GUI_PADDING,widget_box.y+64);
        newStringBox->set_width(widget_box.w - 64);

        yesButton->set_coords( widget_box.x+GENERAL_GPE_GUI_PADDING,newStringBox->get_ypos()+newStringBox->get_height() + GENERAL_GPE_GUI_PADDING);
        cancelButton->set_coords( yesButton->get_xpos()+yesButton->get_width()+GENERAL_GPE_GUI_PADDING,yesButton->get_ypos() );

        newStringBox->process_self();
        yesButton->process_self();
        cancelButton->process_self();

        if( gpe::input->check_kb_released(kb_esc) || cancelButton->is_clicked() )
        {
            exitOperation = true;
            returnVal = "";
        }
        else if( gpe::input->check_kb_released(kb_enter) || yesButton->is_clicked() )
        {
            exitOperation = true;
            if( newStringBox!=NULL)
            {
                returnVal = newStringBox->get_string();
            }
        }

        //Update screen
        //if( gpe::input->window_input_received )
        {
            main_OVERLAY->render_frozen_screenshot( );
        }
        gpe::gcanvas->render_rect( &widget_box,theme_main->popup_box_color,false);
        gpe::gcanvas->render_rectangle( widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+32,theme_main->popup_box_highlight_color,false);

        gpe::gfs->render_text( (gpe::screen_width-promptBoxWidth)/2+GENERAL_GPE_GUI_PADDING,gpe::screen_height/2-64,messageTitle,theme_main->popup_box_highlight_font_color,FONT_DEFAULT_PROMPT,gpe::fa_left,gpe::fa_center );
        gpe::gfs->render_text( (gpe::screen_width-promptBoxWidth)/2+GENERAL_GPE_GUI_PADDING,gpe::screen_height/2-32,messageContent,theme_main->popup_box_font_color,FONT_DEFAULT_PROMPT,gpe::fa_left,gpe::fa_top );
        yesButton->render_self( );
        cancelButton->render_self( );
        newStringBox->render_self( );
        gpe::gcanvas->render_rect( &widget_box,theme_main->popup_box_border_color,true );
        gpe::game_runtime->end_loop();
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

    gpe::input->reset_all_input();
    gpe::game_runtime->start_loop();
    return returnVal;
}

