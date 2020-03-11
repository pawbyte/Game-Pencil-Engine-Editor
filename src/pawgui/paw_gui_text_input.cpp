/*
paw_gui_text_input.cpp
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

#include "paw_gui_text_input.h"


gpe_text_widget_string::gpe_text_widget_string( std::string startName,std::string placeHolderText)
{
    resubmissionAllowed = false;
    opName = "";
    lastfloatClickAction = 0;
    guiListTypeName = "inputtext";
    cursorPos = 0;
    prevCursorPos =0;
    cursorHistoryPos = -1;
    showPlaceHolder = true;
    if( (int)startName.size()>512)
    {
        startName = startName.substr(0,512);
    }
    textInputString = startName;
    cursorPos = (int) textInputString.size();
    showLabel = false;
    inputLabel = "";
    if( (int)placeHolderText.size()>512)
    {
        placeHolderText = placeHolderText.substr(0,512);
    }
    placeHolderString = placeHolderText;
    widget_box.x = 0;
    widget_box.y = 0;
    widget_box.w = 192;
    widget_box.h = 24;

    FONT_TEXTINPUT->get_metrics("A",&fontSizeW,&fontSizeH);
    if( fontSizeW<=0)
    {
        fontSizeW = 12;
    }
    maxCharactersInView = widget_box.w/fontSizeW;

    inputFieldPos = 0;
    hasValidInput = true;
    showCursor = false;
    cursorTimer = 0;
    startXPos = 0;
    upArrowDelay = 0;
    downArrowDelay = 0;
    leftArrowDelay = 0;
    rightArrowDelay = 0;
    enterDelay = 0;
    bscDelay = 0;
    delDelay = 0;
    selectionCursorPos = selectionEndCursorPos = 0;
    fieldElementBox.x = widget_box.x;
    fieldElementBox.y = widget_box.y;
    prevwidget_boxW = fieldElementBox.w = widget_box.w;
    prevwidget_boxH = fieldElementBox.h = widget_box.h;
}

gpe_text_widget_string::~gpe_text_widget_string()
{

}

std::string gpe_text_widget_string::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+textInputString;
}

void gpe_text_widget_string::load_data(std::string dataString)
{
    textInputString = dataString;
}

void gpe_text_widget_string::copy_selection()
{
    if( selectionCursorPos >=0 && selectionCursorPos < (int)textInputString.size() && selectionEndCursorPos >=0 && selectionEndCursorPos <= (int)textInputString.size() )
    {
        int minHighlightPos = std::min(selectionCursorPos,selectionEndCursorPos);
        int maxHighlightPos = std::max(selectionCursorPos,selectionEndCursorPos);
        std::string strToCopy = stg_ex::get_substring(textInputString,minHighlightPos,maxHighlightPos-minHighlightPos);
        gpe::input->clipboard_set( strToCopy );
        //main_OVERLAY->update_temporary_message("Selection copied","","",0.25);
    }
}

void gpe_text_widget_string::cut_selection()
{
    copy_selection();
    delete_selection();
}

void gpe_text_widget_string::delete_selection()
{
    if(  (int)textInputString.size() > 0 && selectionCursorPos!=selectionEndCursorPos )
    {
        int minHighlightPos = std::min(selectionCursorPos,selectionEndCursorPos);
        int maxHighlightPos = std::max(selectionCursorPos,selectionEndCursorPos);
        if(minHighlightPos >=0 && minHighlightPos < (int)textInputString.size() && maxHighlightPos >0 && maxHighlightPos <= (int)textInputString.size() )
        {
            std::string leftSideString = stg_ex::get_substring(textInputString,0,minHighlightPos);
            std::string rightSideString = stg_ex::get_substring(textInputString,maxHighlightPos);
            textInputString = leftSideString+rightSideString;
            selectionCursorPos = 0;
            selectionEndCursorPos = 0;
            cursorPos = minHighlightPos;
            if( startXPos >minHighlightPos)
            {
                startXPos = minHighlightPos;
            }
        }
    }
}

void gpe_text_widget_string::get_cursor_from_mouse( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    cursorPos = floor( startXPos+(gpe::input->mouse_position_x-viewedSpace->x-fieldElementBox.x)/fontSizeW);
    if( cursorPos < 0)
    {
        cursorPos = 0;
    }
    if( cursorPos > (int)textInputString.size() )
    {
        cursorPos = (int)textInputString.size();
    }
}

std::string gpe_text_widget_string::get_plain_string()
{
    if( stg_ex::is_alnum(textInputString,true,true) )
    {
        return textInputString;
    }
    else
    {
        return "''";
    }
}

std::string gpe_text_widget_string::get_label()
{
    return inputLabel;
}

std::string gpe_text_widget_string::get_string()
{
    return textInputString;
}

bool gpe_text_widget_string::has_content()
{
    return ( (int)textInputString.size() > 0);
}

bool gpe_text_widget_string::is_valid()
{
    return hasValidInput;
}

void gpe_text_widget_string::move_left(int xToMove )
{
    if( xToMove > 0)
    {
        startXPos-=xToMove;
        if( startXPos < 0 )
        {
            startXPos = 0;
        }
    }
}

void gpe_text_widget_string::move_right(int xToMove )
{
    int currStrSize = (int)textInputString.size();
    if( xToMove > 0 && currStrSize > maxCharactersInView )
    {
        startXPos+=xToMove;
        if( startXPos >= currStrSize-maxCharactersInView )
        {
            startXPos = currStrSize-maxCharactersInView;
        }
        if( startXPos >= currStrSize )
        {
            startXPos = currStrSize-1;
        }
        if( startXPos < 0 )
        {
            startXPos = 0;
        }
    }
}

void gpe_text_widget_string::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    prevCursorPos = cursorPos;
    inputSubmitted = false;
    bool pasteCommandGiven = false;
    viewedSpace = gpe::camera_find(viewedSpace);
    maxCharactersInView = widget_box.w/fontSizeW;
    cam = gpe::camera_find(cam);
    fieldElementBox.x = widget_box.x;
    fieldElementBox.y = widget_box.y;
    fieldElementBox.w = prevwidget_boxW;
    fieldElementBox.h = prevwidget_boxH;
    if( showLabel )
    {
        set_label(inputLabel);
    }
    /*
    Old Protocol for Label Fields
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);

    New Protocol for labeled fields

    */
    //
    if( isEnabled)
    {
        isClicked = false;
        isHovered = false;
        clickedOutside = false;
        viewedSpace = gpe::camera_find(viewedSpace);
        cam = gpe::camera_find(cam);
        if(viewedSpace!=NULL && cam!=NULL)
        {
            if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,viewedSpace->x,viewedSpace->y,viewedSpace->x+viewedSpace->w,viewedSpace->y+viewedSpace->h) )
            {
                if (gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,fieldElementBox.x+viewedSpace->x-cam->x,fieldElementBox.y+viewedSpace->y-cam->y,fieldElementBox.x+fieldElementBox.w+viewedSpace->x-cam->x,fieldElementBox.y+fieldElementBox.h+viewedSpace->y-cam->y) )
                {
                    isHovered = true;
                    if( (int)descriptionText.size()>0 )
                    {
                        main_OVERLAY->update_tooltip(descriptionText);
                    }
                    else
                    {
                        main_OVERLAY->update_tooltip(opName);
                    }
                    if( gpe::input->check_mouse_released( mb_left))
                    {
                        isClicked = true;
                        hasArrowkeyControl = true;
                        hasScrollControl = false;
                    }
                }
                else if( gpe::input->check_mouse_released( mb_anybutton ) )
                {
                    clickedOutside = true;
                    hasScrollControl = false;
                    hasArrowkeyControl = false;
                }
            }
            else if( gpe::input->check_mouse_released( mb_anybutton ) )
            {
                clickedOutside = true;
                hasScrollControl = false;
                hasArrowkeyControl = false;
            }
        }
    }

    //
    if( isClicked)
    {
        isInUse = true;
        inputFieldPos = 0;
        gpe::input->inputted_keys = "";
        get_cursor_from_mouse(viewedSpace,cam);
    }
    if( isHovered)
    {
        gpe::cursor_main_controller->cursor_change("ibeam");
    }
    if(clickedOutside)
    {
        isInUse = false;
        reset_selection();
    }
    if( isHovered)
    {
        if( gpe::input->check_mouse_button_clicked(0) )
        {
            if( lastfloatClickAction ==0)
            {
                get_cursor_from_mouse(viewedSpace,cam);
                selectionCursorPos = cursorPos;
                selectionEndCursorPos = cursorPos;
                int iPrev = 0;
                int jNext = 0;
                if( cursorPos>=0 &&  cursorPos <= (int)textInputString.size() )
                {
                    if( textInputString[cursorPos]==' ')
                    {
                        for(iPrev = cursorPos-1; iPrev >=0; iPrev-- )
                        {
                            if( textInputString[iPrev]==' ')
                            {
                                selectionCursorPos--;
                            }
                            else
                            {
                                break;
                            }
                        }
                        for(jNext = cursorPos; jNext < (int)textInputString.size(); jNext++ )
                        {
                            if( textInputString[jNext]==' ')
                            {
                                selectionEndCursorPos++;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    else if( stg_ex::char_is_alnum( textInputString[cursorPos],false,false) )
                    {
                        for(iPrev = cursorPos-1; iPrev >=0; iPrev-- )
                        {
                            if( stg_ex::char_is_alnum( textInputString[iPrev],false,false) )
                            {
                                selectionCursorPos--;
                            }
                            else
                            {
                                break;
                            }
                        }

                        for(jNext = cursorPos; jNext < (int)textInputString.size(); jNext++ )
                        {
                            if( stg_ex::char_is_alnum( textInputString[jNext],false,false) )
                            {
                                selectionEndCursorPos++;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        //for symbols
                        for(iPrev = cursorPos-1; iPrev >=0; iPrev-- )
                        {
                            if( stg_ex::char_is_alnum( textInputString[iPrev],false,false)==false)
                            {
                                selectionCursorPos--;
                            }
                            else
                            {
                                break;
                            }
                        }
                        for(jNext = cursorPos; jNext < (int)textInputString.size(); jNext++ )
                        {
                            if( stg_ex::char_is_alnum( textInputString[jNext],false,false)==false)
                            {
                                selectionEndCursorPos++;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
                lastfloatClickAction = 1;
            }
            else
            {
                select_all();
                lastfloatClickAction = 0;
            }
            gpe::input->reset_all_input();

        }
        else if( gpe::input->check_mouse_pressed(0)   )
        {
            get_cursor_from_mouse(viewedSpace,cam);
            selectionCursorPos = cursorPos;
            selectionEndCursorPos = cursorPos;
        }
        else if( gpe::input->check_mouse_down( mb_left )  )
        {
            get_cursor_from_mouse(viewedSpace,cam);
            selectionEndCursorPos = cursorPos;
        }
        else if( gpe::input->check_mouse_down( mb_right ))
        {
            GPE_open_context_menu(-1,-1,128);
            main_CONTEXT_MENU->add_menu_option("Undo",0,paw_gui_rsm->texture_add("backButtonIcon", gpe::app_directory_name+"resources/buttons/backward.png"),-1,NULL,true,true);
            main_CONTEXT_MENU->add_menu_option("Cut",1,paw_gui_rsm->texture_add("cutButtonIcon", gpe::app_directory_name+"resources/buttons/cut.png"),-1,NULL,false,true);
            main_CONTEXT_MENU->add_menu_option("Copy",2,paw_gui_rsm->texture_add("copyButtonIcon", gpe::app_directory_name+"resources/buttons/copy.png"),-1,NULL,false,true);
            main_CONTEXT_MENU->add_menu_option("Paste",3,paw_gui_rsm->texture_add("pasteButtonIcon", gpe::app_directory_name+"resources/buttons/paste.png"),-1,NULL,false,true);
            main_CONTEXT_MENU->add_menu_option("Delete",4,paw_gui_rsm->texture_add("removeButtonIcon", gpe::app_directory_name+"resources/buttons/remove.png"),-1,NULL,true,true);
            main_CONTEXT_MENU->add_menu_option("Select All",5,paw_gui_rsm->texture_add("stickyButtonIcon", gpe::app_directory_name+"resources/buttons/sticky-note.png"),-1,NULL,true,true);
            int menuSelection = GPE_Get_Context_Result();
            if( menuSelection==5)
            {
                select_all();
            }
            else
            {
                switch(menuSelection)
                {
                case 1:
                    cut_selection();
                    break;
                case 2:
                    copy_selection();
                    break;
                case 3:
                    pasteCommandGiven = true;
                    break;
                case 4:
                    delete_selection();
                    break;
                default:
                    break;
                }
                reset_selection();
            }
        }
        /*
        else if( RESOURCE_TO_DRAG!=NULL )
        {
            if( gpe::input->check_mouse_released( mb_left))
            {
                gpe::cursor_main_controller->cursor_change(GPE_CURSOR_IBEAM);
                get_cursor_from_mouse(viewedSpace, cam);
                if( cursorPos >=0 && cursorPos <= (int)textInputString.size() )
                {
                    set_string( stg_ex::get_substring(textInputString,0,cursorPos)+RESOURCE_TO_DRAG->get_name()+stg_ex::get_substring(textInputString,cursorPos) );
                    RESOURCE_TO_DRAG = NULL;
                }
            }
        }*/
    }

    //limit_space_to_rect(viewedSpace,&INP_mouse_position_x,&INP_mouse_position_y);
    if( isEnabled &&  isInUse )
    {
        cursorTimer+=gpe::time_keeper->get_delta_ticks();
        if( cursorTimer > main_GUI_SETTINGS->cursorBlinkTime )
        {
            showCursor = !showCursor;
            cursorTimer = 0;
        }
        //special control action happening
        //Process Input Field delay timers
        if( gpe::input->kb_button_down[kb_backspace]  )
        {
            bscDelay+=gpe::time_keeper->get_delta_ticks();
        }
        else
        {
            bscDelay = 0;
        }
        if( gpe::input->kb_button_down[kb_delete] )
        {
            delDelay+=gpe::time_keeper->get_delta_ticks();
        }
        else
        {
            delDelay = 0;
        }
        if( gpe::input->kb_button_down[kb_up] )
        {
            upArrowDelay+=gpe::time_keeper->get_delta_ticks();
        }
        else
        {
            upArrowDelay = 0;
        }
        if( gpe::input->kb_button_down[kb_down] )
        {
            downArrowDelay+=gpe::time_keeper->get_delta_ticks();
        }
        else
        {
            downArrowDelay = 0;
        }
        if( gpe::input->kb_button_down[kb_left]  )
        {
            leftArrowDelay+=gpe::time_keeper->get_delta_ticks();
        }
        else
        {
            leftArrowDelay = 0;
        }
        if( gpe::input->kb_button_down[kb_right] )
        {
            rightArrowDelay+=gpe::time_keeper->get_delta_ticks();
        }
        else
        {
            rightArrowDelay = 0;
        }
        if( gpe::input->kb_button_down[kb_enter] )
        {
            enterDelay+=gpe::time_keeper->get_delta_ticks();
        }
        else
        {
            enterDelay = 0;
        }

        if( gpe::input->kb_button_down[kb_ctrl]  )
        {
            if( gpe::input->kb_button_down[kb_a])
            {
                select_all();
                showCursor = true;
                cursorTimer = 0;
            }
            else if( gpe::input->kb_button_down[kb_c])
            {
                copy_selection();
                showCursor = true;
                cursorTimer = 0;
            }
            else if( gpe::input->kb_button_released[kb_v])
            {
                pasteCommandGiven = true;
                showCursor = true;
                cursorTimer = 0;
            }
            else if( gpe::input->kb_button_down[kb_x])
            {
                cut_selection();
                showCursor = true;
                cursorTimer = 0;
            }
            else if( gpe::input->mouse_scrolling_up )
            {
                move_left(4);
                showCursor = true;
                cursorTimer = 0;
            }
            else if( gpe::input->mouse_scrolling_down )
            {
                move_right(4);
                showCursor = true;
                cursorTimer = 0;
            }
            else if( leftArrowDelay >= main_GUI_SETTINGS->textInputDelayTime )
            {
                leftArrowDelay = -1;
                if( gpe::input->kb_shift_pressed)
                {
                    if( selectionCursorPos==selectionEndCursorPos )
                    {
                        selectionCursorPos = cursorPos;
                    }
                }
                if( cursorPos >=0 && cursorPos <= (int)textInputString.size() )
                {
                    if( cursorPos >=(int)textInputString.size() )
                    {
                        cursorPos = (int)textInputString.size()-1;
                    }
                    if( cursorPos < 0)
                    {
                        cursorPos = 0;
                    }
                    else if( cursorPos < (int)textInputString.size() )
                    {
                        int iNCursorX = cursorPos-1;
                        for( iNCursorX = cursorPos-1; iNCursorX >=0; iNCursorX--)
                        {
                            if( stg_ex::char_is_alnum(textInputString[iNCursorX],false,false)==false )
                            {
                                break;
                            }
                        }
                        if( iNCursorX< 0)
                        {
                            iNCursorX = 0;
                        }
                        cursorPos = iNCursorX;
                    }
                    else
                    {
                        cursorPos = 0;
                    }
                }
                //scroll_to_cursor();
                showCursor = true;
                cursorTimer = 0;
                if( gpe::input->kb_shift_pressed)
                {
                    selectionEndCursorPos = cursorPos;
                }
                leftArrowDelay = -1;
            }
            else if( rightArrowDelay >= main_GUI_SETTINGS->textInputDelayTime )
            {
                rightArrowDelay = -1;
                if( gpe::input->kb_shift_pressed)
                {
                    if( selectionCursorPos==selectionEndCursorPos )
                    {
                        selectionCursorPos = cursorPos;
                    }
                }
                if( cursorPos >=0 && cursorPos < (int)textInputString.size() )
                {
                    if( cursorPos >=(int)textInputString.size() )
                    {
                        cursorPos = (int)textInputString.size()-1;
                    }
                    if( cursorPos < 0)
                    {
                        cursorPos = 0;
                    }
                    else if( cursorPos < (int)textInputString.size()-1 )
                    {
                        int iNCursorX = cursorPos-1;
                        for( iNCursorX = cursorPos+1; iNCursorX < (int)textInputString.size(); iNCursorX++)
                        {
                            if( stg_ex::char_is_alnum(textInputString[iNCursorX],false,false)==false )
                            {
                                break;
                            }
                        }
                        if( iNCursorX< 0)
                        {
                            iNCursorX = 0;
                        }
                        cursorPos = iNCursorX;
                    }
                    else
                    {
                        cursorPos = 0;
                    }
                }
                //scroll_to_cursor();
                showCursor = true;
                cursorTimer = 0;
                if( gpe::input->kb_shift_pressed)
                {
                    selectionEndCursorPos = cursorPos;
                }
                rightArrowDelay = 0;
            }
        }
        else if( gpe::input->mouse_scrolling_up )
        {
            move_left(4);
            showCursor = true;
            cursorTimer = 0;
        }
        else if( gpe::input->mouse_scrolling_down )
        {
            move_right(4);
            showCursor = true;
            cursorTimer = 0;
        }
        else if( gpe::input->kb_button_down[kb_alt])
        {
            //special alt action
            showCursor = true;
            cursorTimer = 0;
        }
        else if( bscDelay >= main_GUI_SETTINGS->textInputDelayTime )
        {
            bscDelay = 0;
            if( selectionCursorPos!=selectionEndCursorPos)
            {
                delete_selection();
            }
            else
            {
                int prevSize = (int)textInputString.size();
                if( prevSize>0  && cursorPos>0)
                {
                    cursorPos-=1;
                    std::string lineToEdit = textInputString+"";
                    textInputString = stg_ex::get_substring(lineToEdit,0,cursorPos );
                    std::string rightSide = stg_ex::get_substring(lineToEdit,cursorPos+1);
                    textInputString+=rightSide;
                    startXPos-=1;
                    if( startXPos < 0)
                    {
                        startXPos=0;
                    }
                }
            }
            showCursor = true;
            cursorTimer = 0;
            bscDelay = 0;
            //gpe::input->reset_all_input();
        }
        else if( delDelay >= main_GUI_SETTINGS->textInputDelayTime   )
        {
            delDelay = 0;
            if( selectionCursorPos!=selectionEndCursorPos)
            {
                delete_selection();
            }
            else
            {
                int prevSize = (int)textInputString.size();
                if( prevSize>0  && cursorPos>0)
                {
                    std::string lineToEdit = textInputString+"";
                    textInputString = stg_ex::get_substring(lineToEdit,0,cursorPos );
                    std::string rightSide = stg_ex::get_substring(lineToEdit,cursorPos+1);
                    textInputString+=rightSide;
                    startXPos-=1;
                    if( startXPos < 0)
                    {
                        startXPos=0;
                    }
                }
                delDelay = 0;
                gpe::input->kb_button_released[kb_delete] = false;
            }
            showCursor = true;
            cursorTimer = 0;
        }
        else if( leftArrowDelay >= main_GUI_SETTINGS->textInputDelayTime  )
        {
            reset_selection();
            if( cursorPos > 0)
            {
                cursorPos-=1;
                if( cursorPos <= startXPos)
                {
                    move_left(1);
                    showCursor = true;
                    cursorTimer = 0;
                }
            }
            showCursor = true;
            cursorTimer = 0;
            leftArrowDelay = -1;
        }
        else if( rightArrowDelay >= main_GUI_SETTINGS->textInputDelayTime  )
        {
            reset_selection(1);
            if( cursorPos < (int)textInputString.size() )
            {
                cursorPos+=1;
                if( cursorPos >= startXPos+maxCharactersInView)
                {
                    move_right(1);
                    showCursor = true;
                    cursorTimer = 0;
                }
            }
            showCursor = true;
            cursorTimer = 0;
            rightArrowDelay = -1;
        }
        else if( upArrowDelay >= main_GUI_SETTINGS->textInputDelayTime  )
        {
            cursorHistoryPos--;
            cursorPos = 0;
            if( cursorHistoryPos >=0 && (int)listOfPastStrings.size() > 0 )
            {
                record_string(textInputString);
                set_string(listOfPastStrings[cursorHistoryPos]);
                select_all();
                showCursor = true;
                cursorTimer = 0;
            }
            showCursor = true;
            cursorTimer = 0;
            upArrowDelay = -1;
        }
        else if( downArrowDelay >= main_GUI_SETTINGS->textInputDelayTime  )
        {
            if( (int)listOfPastStrings.size() >0 && cursorHistoryPos>=-1 && cursorHistoryPos < (int)listOfPastStrings.size() -1 )
            {
                cursorHistoryPos++;
                cursorPos = 0;
                record_string(textInputString);
                set_string(listOfPastStrings[cursorHistoryPos]);
                select_all();
            }
            else
            {
                gpe::error_log->report("Unable to navigate from ["+ stg_ex::int_to_string(cursorHistoryPos)+" with ["+ stg_ex::int_to_string( (int)listOfPastStrings.size() )+"] past strings....");
            }
            showCursor = true;
            cursorTimer = 0;
            downArrowDelay = -1;
        }
        else if( gpe::input->kb_button_pressed[kb_enter] )
        {
            isInUse = resubmissionAllowed;
            inputSubmitted = true;
            showCursor = true;
            cursorTimer = 0;
            enterDelay = 0;
            //gpe::error_log->report("Submitting self data...");
        }
        else
        {
            int moreStringSize = (int)gpe::input->inputted_keys.size();
            if( moreStringSize > 0)
            {
                delete_selection();
                textInputString=stg_ex::get_substring(textInputString,0,cursorPos)+gpe::input->inputted_keys+stg_ex::get_substring(textInputString,cursorPos);
                cursorPos+=(int)gpe::input->inputted_keys.size();
                if( cursorPos>startXPos+maxCharactersInView)
                {
                    startXPos+=moreStringSize;
                }
                showCursor = true;
                cursorTimer = 0;
            }
            gpe::input->inputted_keys = "";
        }

        if( pasteCommandGiven && gpe::input->clipboard_empty() == false )
        {
            delete_selection();
            std::string clipboardStr = gpe::input->clipboard_string();
            clipboardStr = stg_ex::string_replace_all(clipboardStr,"\r\n","\n");
            clipboardStr = stg_ex::string_replace_all(clipboardStr,"\r","\n");
            clipboardStr = stg_ex::string_replace_all(clipboardStr,"\t","    ");
            std::string leftBeforePaste = stg_ex::get_substring(textInputString,0,cursorPos);
            std::string rightBeforePaste = "";
            if( cursorPos < (int)textInputString.size() )
            {
                rightBeforePaste = stg_ex::get_substring(textInputString,cursorPos+1);
            }
            textInputString=leftBeforePaste+clipboardStr+rightBeforePaste;
            cursorPos+=(int)clipboardStr.size();
            move_right( (int)clipboardStr.size() );
            showCursor = true;
            cursorTimer = 0;
        }
    }
    else
    {
        bscDelay = -1;
        delDelay = -1;
        enterDelay = -1;
        upArrowDelay = -1;
        downArrowDelay = -1;
        leftArrowDelay = -1;
        rightArrowDelay = -1;
    }
}

void gpe_text_widget_string::record_string( std::string strToRecord)
{
    if( (int)strToRecord.size() > 0)
    {
        int recordListSize = (int)listOfPastStrings.size();
        if( recordListSize > 0)
        {
            bool duplicateStr = false;
            for( int i = 0; i < recordListSize; i++)
            {
                if( strToRecord ==listOfPastStrings[i])
                {
                    duplicateStr = true;
                }
            }
            if( !duplicateStr )
            {
                listOfPastStrings.insert( listOfPastStrings.begin(),strToRecord);
                /*if( recordListSize > MAX_STORED_TEXT_PREVIOUS_ENTRIES)
                {
                    listOfPastStrings.erase(listOfPastStrings.begin() );
                }*/

            }
        }
        else
        {
            listOfPastStrings.push_back( strToRecord);
        }
    }
}

void gpe_text_widget_string::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( isEnabled && cam!=NULL)
    {
        if(showBorderBox)
        {
            if( hasValidInput )
            {
                gpe::gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,theme_main->input_color,false, 128 );
            }
            else
            {
                gpe::gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,theme_main->input_error_Box_color,false, 128 );
            }
        }

        if( showLabel && (int)inputLabel.size() > 0)
        {
            if( hAlign == gpe::fa_center)
            {
                gpe::gfs->render_text( widget_box.x+widget_box.w/2-cam->x,widget_box.y-2-cam->y,inputLabel,theme_main->main_box_font_color,FONT_TEXTINPUT,gpe::fa_center,gpe::fa_top);
            }
            else
            {
                gpe::gfs->render_text( widget_box.x-cam->x,widget_box.y-2-cam->y,inputLabel,theme_main->main_box_font_color,FONT_TEXTINPUT,gpe::fa_left,gpe::fa_top);
            }
        }

        if(selectionCursorPos!=selectionEndCursorPos )
        {
            int minHighlightPos = std::min(selectionCursorPos, selectionEndCursorPos);
            if( minHighlightPos < startXPos)
            {
                minHighlightPos = startXPos;
            }
            int maxHighlightPos = std::max(selectionCursorPos, selectionEndCursorPos);
            if( maxHighlightPos < startXPos)
            {
                maxHighlightPos = startXPos;
            }
            maxHighlightPos = std::min(startXPos+maxCharactersInView,maxHighlightPos);

            gpe::gcanvas->render_rectangle(
                fieldElementBox.x+(minHighlightPos-startXPos)*fontSizeW - cam->x,
                fieldElementBox.y+1 - cam->y,
                fieldElementBox.x+2+(maxHighlightPos-startXPos)*fontSizeW- cam->x,
                fieldElementBox.y+fieldElementBox.h-1 - cam->y,
                theme_main->input_selected_color,false, 128 );
        }
        int subLength = 0;
        if( (int)textInputString.size() > maxCharactersInView)
        {
            subLength = (int)textInputString.size()-startXPos;
            if( subLength >maxCharactersInView)
            {
                subLength = maxCharactersInView;
            }
        }
        else
        {
            subLength =  (int)textInputString.size();
        }

        if( subLength < 0)
        {
            subLength = 0;
        }

        if( (int)textInputString.size()>0)
        {
            gpe::gfs->render_text( fieldElementBox.x+4-cam->x,fieldElementBox.y+fieldElementBox.h/2- cam->y,stg_ex::get_substring(textInputString,startXPos,subLength),theme_main->input_font_color,FONT_TEXTINPUT,gpe::fa_left,gpe::fa_middle,255);
        }
        else if(showPlaceHolder && (int)placeHolderString.size() > 0 )
        {
            if( (int)placeHolderString.size() > maxCharactersInView)
            {
                subLength = (int)placeHolderString.size()-startXPos;
                if( subLength >maxCharactersInView)
                {
                    subLength = maxCharactersInView;
                }
            }
            else
            {
                subLength =  (int)placeHolderString.size();
            }
            gpe::gfs->render_text( fieldElementBox.x+4-cam->x,fieldElementBox.y+fieldElementBox.h/2 - cam->y,stg_ex::get_substring(placeHolderString,0,subLength),theme_main->input_faded_font_color,FONT_TEXTINPUT,gpe::fa_left,gpe::fa_middle);
        }
        if(showBorderBox)
        {
            if( isInUse )
            {
                gpe::gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,theme_main->input_highlight_outline_color,true);
            }
            else if( isHovered )
            {
                gpe::gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,theme_main->input_highlight_alt_color,true);
            }
            else
            {
                gpe::gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,theme_main->input_outline_color,true);
            }
        }

        if(isInUse && prevCursorPos!=cursorPos && prevCursorPos >=startXPos && prevCursorPos <= startXPos+maxCharactersInView )
        {
            gpe::gcanvas->render_vertical_line_color( fieldElementBox.x+GENERAL_GPE_GUI_PADDING/2+(prevCursorPos-startXPos)*fontSizeW- cam->x,fieldElementBox.y - cam->y,fieldElementBox.y+fieldElementBox.h - cam->y,theme_main->input_color);
        }

        if(isInUse && cursorPos >=startXPos && cursorPos <= startXPos+maxCharactersInView )
        {
            if(showCursor)
            {
                gpe::gcanvas->render_vertical_line_color( fieldElementBox.x+GENERAL_GPE_GUI_PADDING/2+(cursorPos-startXPos)*fontSizeW- cam->x,fieldElementBox.y - cam->y,fieldElementBox.y+fieldElementBox.h - cam->y,theme_main->input_font_color);
            }
            else
            {
                gpe::gcanvas->render_vertical_line_color( fieldElementBox.x+GENERAL_GPE_GUI_PADDING/2+(cursorPos-startXPos)*fontSizeW- cam->x,fieldElementBox.y - cam->y,fieldElementBox.y+fieldElementBox.h - cam->y,theme_main->input_color);
            }
        }
    }
}

void gpe_text_widget_string::reset_selection(int resetDirection)
{
    if( selectionCursorPos!=selectionEndCursorPos )
    {
        if( resetDirection < 0)
        {
            cursorPos = std::min(selectionCursorPos, selectionEndCursorPos);
        }
        else
        {
            cursorPos = std::max(selectionCursorPos, selectionEndCursorPos);
        }
    }
    selectionCursorPos =0;
    selectionEndCursorPos = 0;
}

void gpe_text_widget_string::select_all()
{
    selectionCursorPos = 0;
    selectionEndCursorPos = textInputString.size();
    cursorPos = 0;
    gpe::input->reset_all_input();
}

void gpe_text_widget_string::set_height(int newHeight)
{
    if( showLabel)
    {
        fieldElementBox.h = newHeight;
    }
    else
    {
        widget_box.h = newHeight;
        fieldElementBox.h = newHeight;
    }
    prevwidget_boxH = newHeight;
}

void gpe_text_widget_string::set_label(std::string newLabel)
{
    if( (int)newLabel.size() > 0)
    {
        inputLabel = newLabel;
        showLabel = true;

        widget_box.h=prevwidget_boxH+fontSizeH;
        //widget_box.w = std::max( prevwidget_boxW, ( fontSizeW *(int)inputLabel.size()  ) );
        fieldElementBox.x = widget_box.x;
        fieldElementBox.y = widget_box.y+fontSizeH;
        //fieldElementBox.w = prevwidget_boxW;
        fieldElementBox.h = prevwidget_boxH;
    }
    else
    {
        inputLabel = "";
        showLabel = false;
    }
}

void gpe_text_widget_string::set_name(std::string new_name)
{
    if( (int)new_name.size() > 0)
    {
        /*if( opName == inputLabel )
        {
            set_label(new_name);
        }*/
        opName = new_name;
    }
}

void gpe_text_widget_string::set_number( float numberIn)
{
    std::string newString = stg_ex::float_to_string(numberIn);
    textInputString = newString;
    record_string(textInputString);
    cursorPos = (int) textInputString.size();

}

void gpe_text_widget_string::set_string(std::string newString)
{
    if( (int)textInputString.size() > 0)
    {
        record_string( textInputString );
    }
    if( (int)newString.size() > 0)
    {
        record_string( newString );
    }
    textInputString = newString;
    cursorPos = (int) textInputString.size();
}

void gpe_text_widget_string::set_placeholder(std::string newString)
{
    placeHolderString = newString;
}

void gpe_text_widget_string::set_width(int newWidth)
{
    if( showLabel)
    {
        fieldElementBox.w = newWidth;
    }
    else
    {
        widget_box.w = newWidth;
        fieldElementBox.w = newWidth;
    }
    widget_box.w = newWidth;
    fieldElementBox.w = newWidth;
    prevwidget_boxW = newWidth;
}

void gpe_text_widget_string::set_max_width( int nMW)
{
    maxWidth = nMW;
    if( maxWidth > 0 )
    {
        if( widget_box.w > maxWidth || fieldElementBox.w > maxWidth)
        {
            fieldElementBox.w = maxWidth;
            widget_box.w = maxWidth;
            prevwidget_boxW = maxWidth;
        }
    }
}

void gpe_text_widget_string::set_max_height( int nMH)
{
    maxHeight = nMH;
    if( maxHeight > 0 && widget_box.h > maxHeight)
    {
        widget_box.h = maxHeight;
    }
}

bool gpe_text_widget_string::was_submitted()
{
    return inputSubmitted;
}


gpe_text_widget_number::gpe_text_widget_number( std::string placeHolderText, bool onlyWholeNums, float hMin, float hMax)
{
    resubmissionAllowed = false;
    textInputString = "";
    guiListTypeName = "inputnumber";
    guiListTypeId = 5;
    forceValidNumber = false;
    showLabel = false;
    inputLabel = "";
    if( (int)placeHolderText.size()>512)
    {
        placeHolderText = placeHolderText.substr(0,512);
    }
    placeHolderString = placeHolderText;
    showPlaceHolder = true;
    widget_box.x = 0;
    widget_box.y = 0;
    widget_box.w = 192;
    widget_box.h = 24;

    inputFieldPos = 0;
    onlyWholeNumbers = onlyWholeNums;

    minInputNumber = hMin;
    maxInputNumber = hMax;


    fontSizeH = 12;
    fontSizeW = 12;
    FONT_TEXTINPUT->get_metrics("A",&fontSizeW,&fontSizeH);
    if( fontSizeW<=0)
    {
        fontSizeW = 12;
    }
    maxCharactersInView = widget_box.w/fontSizeW;

    hasValidInput = true;
    heldNumber = 0;
    startXPos = 0;
    cursorPos = 0;
    textInputString = "0";
    set_label( placeHolderText );
}

gpe_text_widget_number::~gpe_text_widget_number()
{

}

int gpe_text_widget_number::get_held_int()
{
    return (int)get_held_number();
}

float gpe_text_widget_number::get_held_number()
{
    heldNumber = stg_ex::string_to_float(textInputString, INT_MIN);
    return heldNumber;
}

std::string gpe_text_widget_number::get_plain_string()
{
    heldNumber = stg_ex::string_to_float(textInputString, INT_MIN);
    return stg_ex::float_to_string(heldNumber);
}

float gpe_text_widget_number::make_valid_number( float fallbackNumber )
{
    heldNumber = stg_ex::string_to_float(textInputString, INT_MIN);
    if( heldNumber== INT_MIN || heldNumber < minInputNumber || heldNumber > maxInputNumber)
    {
        heldNumber = fallbackNumber;
        hasValidInput = true;
        set_string(""+ stg_ex::float_to_string( fallbackNumber ) );
    }
    return heldNumber;
}

void gpe_text_widget_number::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam)
{
    hasValidInput = true;
    gpe_text_widget_string::process_self(viewedSpace,cam);
    if( (int)textInputString.size() > 0)
    {
        if( onlyWholeNumbers)
        {
            heldNumber = stg_ex::string_to_int(textInputString, INT_MIN);
            if( heldNumber== INT_MIN || heldNumber < minInputNumber || heldNumber > maxInputNumber)
            {
                heldNumber = 0;
                if( forceValidNumber)
                {
                    set_string("0");
                }
                hasValidInput = false;
            }
        }
        else
        {
            heldNumber = stg_ex::string_to_float(textInputString, INT_MIN);
            if( heldNumber == INT_MIN || heldNumber < minInputNumber || heldNumber > maxInputNumber )
            {
                heldNumber = 0;
                if( forceValidNumber)
                {
                    set_string("0");
                }
                hasValidInput = false;
            }
        }
    }
    else
    {
        heldNumber = 0;
        if( forceValidNumber)
        {
            set_string("0");
        }
        hasValidInput = false;
    }
}

