/*
pawgui_popups.cpp
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

#include "../base/pawgui_base.h"
#include "../buttons/pawgui_buttons.h"
#include "../labels/pawgui_labels.h"
#include "../popups/pawgui_popups.h"
#include "../text_input/pawgui_text_input.h"

namespace pawgui
{
    void display_user_alert(std::string messageTitle, std::string messageContent, int messageType, gpe::shape_rect * cam)
    {
        gpe::game_runtime->end_loop();
        //resource_dragged = nullptr;
        gpe::cursor_main_controller->cursor_change("arrow");
        //editor_gui_main->reset_gui_info();
        main_overlay_system->update_tooltip("");
        main_overlay_system->take_frozen_screenshot( );
        gpe::input->reset_all_input();
        bool exitOperation = false;
        bool windowBeingDragged = false;

        gpe::shape_rect widget_box;

        widget_box.w = gpe::screen_width*3/4 -128 - padding_default*4;
        widget_box.h = gpe::screen_height*3/4 - 64 - padding_default*4;
        widget_box.x = gpe::screen_width - widget_box.w;
        widget_box.y = gpe::screen_height - widget_box.h;
        int maxMessageWidth = 0;
        int maxMessageHeight = 0;
        int defaultFontWidth = 0;
        int defaultFontHeight = 0;
        int iSubMessage = 0;
        if( font_default_prompt!=nullptr)
        {
            font_default_prompt->get_metrics("A",&defaultFontWidth, &defaultFontHeight);
        }

        std::vector < std::string > messageSubTitles;

        widget_button_label * ok_button = new widget_button_label( "Okay","");
        if( defaultFontWidth > 0 && defaultFontHeight > 0)
        {
            maxMessageWidth = (widget_box.w-64) / defaultFontWidth;
            maxMessageHeight = (widget_box.h-64) / (defaultFontHeight+padding_default);
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
                widget_box.h = messageSubTitles.size()*(defaultFontHeight+padding_default)+padding_default*4+64;
                widget_box.y = (gpe::screen_height - widget_box.h)/2;
            }
            else
            {
                widget_box.h = maxMessageHeight*(defaultFontHeight+padding_default)+padding_default*4+64;
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

            widget_box.w = (maxMessageWidth*defaultFontWidth)+128 + padding_default*4;
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
                if( ok_button!=nullptr)
                {
                    ok_button->set_coords(widget_box.x+( widget_box.w-ok_button->get_width() )/2,widget_box.y+widget_box.h-padding_default-ok_button->get_height() );
                    ok_button->process_self( nullptr, nullptr);
                    if( ok_button->is_clicked() && !windowBeingDragged )
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
                    gpe::gcanvas->render_rectangle( 0,0,gpe::screen_width,gpe::screen_height,pawgui::theme_main->program_color, false, 255);
                    main_overlay_system->render_frozen_screenshot( );
                }
                //Update screen
                gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->popup_box_color,false);

                gpe::gcanvas->render_rectangle( widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+32,pawgui::theme_main->popup_box_highlight_color,false);

                gpe::gfs->render_text( widget_box.x+widget_box.w/2,widget_box.y+padding_default,messageTitle,pawgui::theme_main->popup_box_highlight_font_color,gpe::font_default,gpe::fa_center,gpe::fa_top);
                gpe::gfs->render_text( widget_box.x+widget_box.w-padding_default,widget_box.y+padding_default,"×",pawgui::theme_main->popup_box_highlight_font_color,gpe::font_default,gpe::fa_right,gpe::fa_top);

                for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
                {
                    gpe::gfs->render_text( widget_box.x+32,widget_box.y+padding_default+32+iSubMessage*(defaultFontHeight+padding_default),messageSubTitles.at(iSubMessage),pawgui::theme_main->popup_box_font_color,font_default_prompt,gpe::fa_left,gpe::fa_top);
                }
                if( ok_button!=nullptr)
                {
                    ok_button->render_self(  nullptr, nullptr);
                }
                if( gpe::point_within_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&widget_box) )
                {
                    gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->popup_box_highlight_alt_color,true);
                }
                else
                {
                    gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->popup_box_border_color,true);
                }
                gpe::game_runtime->end_loop();
            }
        }
        if( ok_button!=nullptr )
        {
            delete ok_button;
            ok_button = nullptr;
        }
        gpe::game_runtime->start_loop();
    }

    int display_prompt_message(std::string messageTitle, std::string messageContent, bool showCancel_button )
    {
        gpe::game_runtime->end_loop();
        //resource_dragged = nullptr;
        int maxMessageWidth = 0;
        int maxMessageHeight = 0;
        int defaultFontWidth = 12;
        int defaultFontHeight = 12;
        std::vector < std::string > messageSubTitles;
        int iSubMessage = 0;
        gpe::cursor_main_controller->cursor_change("arrow");
        main_overlay_system->take_frozen_screenshot( );
        gpe::input->reset_all_input();
        bool exitOperation = false;
        int returnVal = display_query_cancel;
        widget_button_label * yes_button = new widget_button_label( "Yes[ENTER]","");
        yes_button->set_width(96);
        widget_button_label * noButon = nullptr;
        widget_button_label * cancel_button = nullptr;
        if( showCancel_button)
        {
            cancel_button = new widget_button_label( "Cancel[ESC]","");
            cancel_button->set_width(96);
            noButon = new widget_button_label( "No[N]","");
            noButon->set_width(96);
        }
        else
        {
            noButon = new widget_button_label( "No[ESC]","");
            noButon->set_width(96);
        }

        bool boxIsMoving = false;
        bool boxWasResized = true;
        bool boxbeing_resized  = false;

        if( font_default_prompt!=nullptr)
        {
            font_default_prompt->get_metrics("A",&defaultFontWidth, &defaultFontHeight);
        }

        gpe::shape_rect widget_box;
        widget_box.w = 528;
        widget_box.h = 320;
        widget_box.x = (gpe::screen_width - widget_box.w)/2;
        widget_box.y = 72;
        widget_box.x = (gpe::screen_width - widget_box.w)/2;
        widget_box.y = 72;
        maxMessageWidth = (widget_box.w-32) / defaultFontWidth;
        maxMessageHeight = (widget_box.h-64) / (defaultFontHeight+padding_default);
        int newBarX2Pos = widget_box.x+widget_box.w;
        int newBarY2Pos = widget_box.w+widget_box.h;
        main_overlay_system->render_frozen_screenshot( );

        if( defaultFontWidth > 0 && defaultFontHeight > 0)
        {
            maxMessageWidth = (widget_box.w-32) / defaultFontWidth;
            maxMessageHeight = (widget_box.h-64) / (defaultFontHeight+padding_default);

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
                widget_box.h = messageSubTitles.size()*(defaultFontHeight+padding_default)+padding_default*4+64;
            }
            else
            {
                widget_box.h = maxMessageHeight*(defaultFontHeight+padding_default)+padding_default*4+64;
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
                    else if( boxbeing_resized)
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
                        boxbeing_resized = true;
                    }
                }

                if( gpe::input->check_mouse_released( mb_anybutton  ) )
                {
                    boxIsMoving = false;
                    boxbeing_resized = false;
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
                    maxMessageHeight = (widget_box.h-64) / (defaultFontHeight+padding_default);

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
                        widget_box.h = messageSubTitles.size()*(defaultFontHeight+padding_default)+padding_default*4+64;
                    }
                    else
                    {
                        widget_box.h = maxMessageHeight*(defaultFontHeight+padding_default)+padding_default*4+64;
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

                yes_button->set_coords( padding_default+64,widget_box.h-32);
                noButon->set_coords(yes_button->get_xpos()+yes_button->get_width()+padding_default,yes_button->get_ypos() );
                if( cancel_button!=nullptr)
                {
                    cancel_button->set_coords( noButon->get_xpos()+noButon->get_width()+padding_default,noButon->get_ypos() );
                    cancel_button->process_self(&widget_box);
                }
                yes_button->process_self(&widget_box);
                noButon->process_self(&widget_box) ;


                if( gpe::input->check_kb_released(kb_esc) || gpe::window_controller_main->window_closed )
                {
                    if( cancel_button!=nullptr)
                    {
                        exitOperation = true;
                        returnVal = display_query_cancel;
                    }
                    else
                    {
                        exitOperation = true;
                        returnVal = display_query_no;
                    }
                }
                else if( gpe::input->check_kb_released(kb_n) )
                {
                    exitOperation = true;
                    returnVal = display_query_no;
                }
                else if( noButon->is_clicked() )
                {
                    exitOperation = true;
                    returnVal = display_query_no;
                }
                else if( cancel_button!=nullptr &&  cancel_button->is_clicked() )
                {
                    exitOperation = true;
                    returnVal = display_query_cancel;
                }
                else if( gpe::input->check_kb_released(kb_enter) || yes_button->is_clicked() )
                {
                    exitOperation = true;
                    returnVal = display_query_yes;
                }
                if( !gpe::window_controller_main->is_resized() )
                {
                    gpe::renderer_main->reset_viewpoint();
                    //Update screen
                    //if( gpe::input->window_input_received )
                    {
                        main_overlay_system->render_frozen_screenshot( );
                    }
                    gpe::renderer_main->set_viewpoint( &widget_box);

                    gpe::gcanvas->render_rectangle( 0,0,widget_box.w,widget_box.h,pawgui::theme_main->popup_box_color,false);
                    gpe::gcanvas->render_rectangle( 0,0,widget_box.w,32,pawgui::theme_main->popup_box_highlight_color,false);

                    gpe::gfs->render_text( padding_default*2,padding_default,messageTitle,pawgui::theme_main->popup_box_highlight_font_color,font_default_prompt,gpe::fa_left,gpe::fa_top);
                    //render_text( widget_box.x+padding_default+32,widget_box.y+64,messageContent,pawgui::theme_main->popup_box_font_color,font_default_prompt,gpe::fa_left,gpe::fa_top);
                    for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
                    {
                        gpe::gfs->render_text( 32,padding_default+32+iSubMessage*(defaultFontHeight+padding_default),messageSubTitles.at(iSubMessage),pawgui::theme_main->popup_box_font_color,font_default_prompt,gpe::fa_left,gpe::fa_top);
                    }

                    yes_button->render_self( &widget_box );
                    noButon->render_self( &widget_box );
                    if( cancel_button!=nullptr)
                    {
                        cancel_button->render_self( &widget_box );
                    }
                    gpe::gcanvas->render_rectangle( 1,1,widget_box.w-1,widget_box.h-1,pawgui::theme_main->popup_box_border_color,true);
                    gpe::gcanvas->render_rectangle( 0,0,widget_box.w,widget_box.h,pawgui::theme_main->popup_box_highlight_color,true);
                    //editor_gui_main-render_gui_info(gpe::renderer_main, true);
                }
                gpe::game_runtime->end_loop();
            }
        }

        gpe::renderer_main->reset_viewpoint();
        if( font_default_prompt!=nullptr)
        {
            font_default_prompt->clear_cache();
        }
        if( yes_button!=nullptr)
        {
            delete yes_button;
            yes_button = nullptr;
        }
        if( noButon!=nullptr)
        {
            delete noButon;
            noButon = nullptr;
        }
        if( cancel_button!=nullptr)
        {
            delete cancel_button;
            cancel_button = nullptr;
        }
        gpe::input->reset_all_input();
        messageSubTitles.clear();

        gpe::game_runtime->start_loop();
        return returnVal;
    }

    std::string get_string_from_popup(std::string messageTitle, std::string messageContent, std::string startName)
    {
        gpe::game_runtime->end_loop();
        //resource_dragged = nullptr;
        main_overlay_system->update_tooltip("");
        gpe::cursor_main_controller->cursor_change("arrow");
        main_overlay_system->take_frozen_screenshot( );
        gpe::input->reset_all_input();
        bool exitOperation = false;
        int frame = 0;
        int TEXTBOX_FONT_SIZE_WIDTH = 12;
        int TEXTBOX_FONT_SIZE_HEIGHT = 12;
        if( font_default_prompt!=nullptr)
        {
            font_default_prompt->get_metrics("A",&TEXTBOX_FONT_SIZE_WIDTH,&TEXTBOX_FONT_SIZE_HEIGHT);
        }
        int titleWidth = TEXTBOX_FONT_SIZE_WIDTH * (int)messageTitle.size();
        int messageWidth = TEXTBOX_FONT_SIZE_WIDTH * (int)messageContent.size();
        int biggeststringWidth = std::max(titleWidth, messageWidth);
        std::string returnVal = startName;
        widget_button_label * yes_button = new widget_button_label( "Okay","");
        widget_button_label * cancel_button = new widget_button_label( "Cancel","");
        widget_input_text * newstringBox = new widget_input_text(startName);
        newstringBox->switch_inuse(true);
        int buttonsWidth = yes_button->get_width()+cancel_button->get_width();
        int promptBoxWidth = buttonsWidth;
        if( promptBoxWidth < biggeststringWidth)
        {
            promptBoxWidth =biggeststringWidth;
        }
        promptBoxWidth+=padding_default*3;
        gpe::shape_rect widget_box;
        widget_box.x = (gpe::screen_width-promptBoxWidth)/2-padding_default;
        widget_box.y = gpe::screen_height/2-64-padding_default;
        widget_box.w = promptBoxWidth+128;
        widget_box.h = 192;
        main_overlay_system->render_frozen_screenshot( );
        while(exitOperation==false)
        {
            gpe::cursor_main_controller->cursor_change("arrow");
            gpe::game_runtime->start_loop();

            //editor_gui_main->reset_gui_info();

            widget_box.x = (gpe::screen_width-promptBoxWidth)/2-padding_default;
            widget_box.y = gpe::screen_height/2-64-padding_default;

            newstringBox->set_coords( widget_box.x+padding_default,widget_box.y+64);
            newstringBox->set_width(widget_box.w - 64);

            yes_button->set_coords( widget_box.x+padding_default,newstringBox->get_ypos()+newstringBox->get_height() + padding_default);
            cancel_button->set_coords( yes_button->get_xpos()+yes_button->get_width()+padding_default,yes_button->get_ypos() );

            newstringBox->process_self();
            yes_button->process_self();
            cancel_button->process_self();

            if( gpe::input->check_kb_released(kb_esc) || cancel_button->is_clicked() )
            {
                exitOperation = true;
                returnVal = "";
            }
            else if( gpe::input->check_kb_released(kb_enter) || yes_button->is_clicked() )
            {
                exitOperation = true;
                if( newstringBox!=nullptr)
                {
                    returnVal = newstringBox->get_string();
                }
            }

            //Update screen
            //if( gpe::input->window_input_received )
            {
                main_overlay_system->render_frozen_screenshot( );
            }
            gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->popup_box_color,false);
            gpe::gcanvas->render_rectangle( widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+32,pawgui::theme_main->popup_box_highlight_color,false);

            gpe::gfs->render_text( (gpe::screen_width-promptBoxWidth)/2+padding_default,gpe::screen_height/2-64,messageTitle,pawgui::theme_main->popup_box_highlight_font_color,font_default_prompt,gpe::fa_left,gpe::fa_center );
            gpe::gfs->render_text( (gpe::screen_width-promptBoxWidth)/2+padding_default,gpe::screen_height/2-32,messageContent,pawgui::theme_main->popup_box_font_color,font_default_prompt,gpe::fa_left,gpe::fa_top );
            yes_button->render_self( );
            cancel_button->render_self( );
            newstringBox->render_self( );
            gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->popup_box_border_color,true );
            gpe::game_runtime->end_loop();
        }
        if( yes_button!=nullptr)
        {
            delete yes_button;
            yes_button = nullptr;
        }
        if( cancel_button!=nullptr)
        {
            delete cancel_button;
            cancel_button = nullptr;
        }
        if( newstringBox!=nullptr)
        {
            delete newstringBox;
            newstringBox = nullptr;
        }

        gpe::input->reset_all_input();
        gpe::game_runtime->start_loop();
        return returnVal;
    }
}
