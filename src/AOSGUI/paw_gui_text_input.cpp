/*
paw_gui_text_input.cpp
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

#include "paw_gui_text_input.h"


GPE_TextInputBasic::GPE_TextInputBasic( std::string startName,std::string placeHolderText)
{
    resubmissionAllowed = false;
    opName = "";
    lastDoubleClickAction = 0;
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
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 192;
    elementBox.h = 24;

    FONT_TEXTINPUT->get_metrics("A",&fontSizeW,&fontSizeH);
    if( fontSizeW<=0)
    {
        fontSizeW = 12;
    }
    maxCharactersInView = elementBox.w/fontSizeW;

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
    fieldElementBox.x = elementBox.x;
    fieldElementBox.y = elementBox.y;
    prevelementBoxW = fieldElementBox.w = elementBox.w;
    prevelementBoxH = fieldElementBox.h = elementBox.h;
}

GPE_TextInputBasic::~GPE_TextInputBasic()
{

}

std::string GPE_TextInputBasic::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+textInputString;
}

void GPE_TextInputBasic::load_data(std::string dataString)
{
    textInputString = dataString;
}

void GPE_TextInputBasic::copy_selection()
{
    if( selectionCursorPos >=0 && selectionCursorPos < (int)textInputString.size() && selectionEndCursorPos >=0 && selectionEndCursorPos <= (int)textInputString.size() )
    {
        int minHighlightPos = std::min(selectionCursorPos,selectionEndCursorPos);
        int maxHighlightPos = std::max(selectionCursorPos,selectionEndCursorPos);
        std::string strToCopy = get_substring(textInputString,minHighlightPos,maxHighlightPos-minHighlightPos);
        SDL_SetClipboardText(strToCopy.c_str() );
        //MAIN_OVERLAY->update_temporary_message("Selection copied","","",0.25);
    }
}

void GPE_TextInputBasic::cut_selection()
{
    copy_selection();
    delete_selection();
}

void GPE_TextInputBasic::delete_selection()
{
    if(  (int)textInputString.size() > 0 && selectionCursorPos!=selectionEndCursorPos )
    {
        int minHighlightPos = std::min(selectionCursorPos,selectionEndCursorPos);
        int maxHighlightPos = std::max(selectionCursorPos,selectionEndCursorPos);
        if(minHighlightPos >=0 && minHighlightPos < (int)textInputString.size() && maxHighlightPos >0 && maxHighlightPos <= (int)textInputString.size() )
        {
            std::string leftSideString = get_substring(textInputString,0,minHighlightPos);
            std::string rightSideString = get_substring(textInputString,maxHighlightPos);
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

void GPE_TextInputBasic::get_cursor_from_mouse( GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    cursorPos = floor( startXPos+(input->mouse_x-viewedSpace->x-fieldElementBox.x)/fontSizeW);
    if( cursorPos < 0)
    {
        cursorPos = 0;
    }
    if( cursorPos > (int)textInputString.size() )
    {
        cursorPos = (int)textInputString.size();
    }
}

std::string GPE_TextInputBasic::get_plain_string()
{
    if( is_alnum(textInputString,true,true) )
    {
        return textInputString;
    }
    else
    {
        return "''";
    }
}

std::string GPE_TextInputBasic::get_label()
{
    return inputLabel;
}

std::string GPE_TextInputBasic::get_string()
{
    return textInputString;
}

bool GPE_TextInputBasic::has_content()
{
    return ( (int)textInputString.size() > 0);
}

bool GPE_TextInputBasic::is_valid()
{
    return hasValidInput;
}

void GPE_TextInputBasic::move_left(int xToMove )
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

void GPE_TextInputBasic::move_right(int xToMove )
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

void GPE_TextInputBasic::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    prevCursorPos = cursorPos;
    inputSubmitted = false;
    bool pasteCommandGiven = false;
    viewedSpace = GPE_find_camera(viewedSpace);
    maxCharactersInView = elementBox.w/fontSizeW;
    cam = GPE_find_camera(cam);
    fieldElementBox.x = elementBox.x;
    fieldElementBox.y = elementBox.y;
    fieldElementBox.w = prevelementBoxW;
    fieldElementBox.h = prevelementBoxH;
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
        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);
        if(viewedSpace!=NULL && cam!=NULL)
        {
            if( point_between(input->mouse_x,input->mouse_y,viewedSpace->x,viewedSpace->y,viewedSpace->x+viewedSpace->w,viewedSpace->y+viewedSpace->h) )
            {
                if (point_between(input->mouse_x,input->mouse_y,fieldElementBox.x+viewedSpace->x-cam->x,fieldElementBox.y+viewedSpace->y-cam->y,fieldElementBox.x+fieldElementBox.w+viewedSpace->x-cam->x,fieldElementBox.y+fieldElementBox.h+viewedSpace->y-cam->y) )
                {
                    isHovered = true;
                    if( (int)descriptionText.size()>0 )
                    {
                        MAIN_OVERLAY->update_tooltip(descriptionText);
                    }
                    else
                    {
                        MAIN_OVERLAY->update_tooltip(opName);
                    }
                    if( input->check_mouse_released( mb_left))
                    {
                        isClicked = true;
                        hasArrowkeyControl = true;
                        hasScrollControl = false;
                    }
                }
                else if( input->check_mouse_released( mb_anybutton ) )
                {
                    clickedOutside = true;
                    hasScrollControl = false;
                    hasArrowkeyControl = false;
                }
            }
            else if( input->check_mouse_released( mb_anybutton ) )
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
        input->inkeys = "";
        get_cursor_from_mouse(viewedSpace,cam);
    }
    if( isHovered)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_IBEAM);
    }
    if(clickedOutside)
    {
        isInUse = false;
        reset_selection();
    }
    if( isHovered)
    {
        if( input->check_mouse_doubleclicked(0) )
        {
            if( lastDoubleClickAction ==0)
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
                    else if( char_is_alnum( textInputString[cursorPos],false,false) )
                    {
                        for(iPrev = cursorPos-1; iPrev >=0; iPrev-- )
                        {
                            if( char_is_alnum( textInputString[iPrev],false,false) )
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
                            if( char_is_alnum( textInputString[jNext],false,false) )
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
                            if( char_is_alnum( textInputString[iPrev],false,false)==false)
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
                            if( char_is_alnum( textInputString[jNext],false,false)==false)
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
                lastDoubleClickAction = 1;
            }
            else
            {
                select_all();
                lastDoubleClickAction = 0;
            }
            input->reset_all_input();

        }
        else if( input->check_mouse_pressed(0)   )
        {
            get_cursor_from_mouse(viewedSpace,cam);
            selectionCursorPos = cursorPos;
            selectionEndCursorPos = cursorPos;
        }
        else if( input->check_mouse_down( mb_left )  )
        {
            get_cursor_from_mouse(viewedSpace,cam);
            selectionEndCursorPos = cursorPos;
        }
        else if( input->check_mouse_down( mb_right ))
        {
            GPE_open_context_menu(-1,-1,128);
            MAIN_CONTEXT_MENU->add_menu_option("Undo",0,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/buttons/backward.png"),-1,NULL,true,true);
            MAIN_CONTEXT_MENU->add_menu_option("Cut",1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/buttons/cut.png"),-1,NULL,false,true);
            MAIN_CONTEXT_MENU->add_menu_option("Copy",2,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/buttons/copy.png"),-1,NULL,false,true);
            MAIN_CONTEXT_MENU->add_menu_option("Paste",3,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/buttons/paste.png"),-1,NULL,false,true);
            MAIN_CONTEXT_MENU->add_menu_option("Delete",4,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/buttons/remove.png"),-1,NULL,true,true);
            MAIN_CONTEXT_MENU->add_menu_option("Select All",5,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/buttons/sticky-note.png"),-1,NULL,true,true);
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
            if( input->check_mouse_released( mb_left))
            {
                GPE_change_cursor(SDL_SYSTEM_CURSOR_IBEAM);
                get_cursor_from_mouse(viewedSpace, cam);
                if( cursorPos >=0 && cursorPos <= (int)textInputString.size() )
                {
                    set_string( get_substring(textInputString,0,cursorPos)+RESOURCE_TO_DRAG->get_name()+get_substring(textInputString,cursorPos) );
                    RESOURCE_TO_DRAG = NULL;
                }
            }
        }*/
    }

    //limit_space_to_rect(viewedSpace,&INP_MOUSE_X,&INP_MOUSE_Y);
    if( isEnabled &&  isInUse )
    {
        cursorTimer+=gpe->get_delta_time();
        if( cursorTimer > MAIN_GUI_SETTINGS->cursorBlinkTime )
        {
            showCursor = !showCursor;
            cursorTimer = 0;
        }
        //special control action happening
        //Process Input Field delay timers
        if( input->down[kb_backspace]  && !input->pressed[kb_backspace]  && !input->released[kb_backspace] )
        {
            bscDelay+=gpe->get_delta_time();
        }
        else
        {
            bscDelay = -1;
        }
        if( input->down[kb_delete] && !input->pressed[kb_delete]   && !input->released[kb_delete] )
        {
            delDelay+=gpe->get_delta_time();
        }
        else
        {
            delDelay = -1;
        }
        if( input->down[kb_up] && !input->pressed[kb_up]   && !input->released[kb_up] )
        {
            upArrowDelay+=gpe->get_delta_time();
        }
        else
        {
            upArrowDelay = -1;
        }
        if( input->down[kb_down] && !input->pressed[kb_down]   && !input->released[kb_down] )
        {
            downArrowDelay+=gpe->get_delta_time();
        }
        else
        {
            downArrowDelay = -1;
        }
        if( input->down[kb_left] && !input->pressed[kb_left]   && !input->released[kb_left] )
        {
            leftArrowDelay+=gpe->get_delta_time();
        }
        else
        {
            leftArrowDelay = -1;
        }
        if( input->down[kb_right] && !input->pressed[kb_right]   && !input->released[kb_right] )
        {
            rightArrowDelay+=gpe->get_delta_time();
        }
        else
        {
            rightArrowDelay = -1;
        }
        if( input->down[kb_enter] && !input->pressed[kb_enter]   && !input->released[kb_enter] )
        {
            enterDelay+=gpe->get_delta_time();
        }
        else
        {
            enterDelay = -1;
        }

        if( input->down[kb_ctrl]  )
        {
            if( input->down[kb_a])
            {
                select_all();
                showCursor = true;
                cursorTimer = 0;
            }
            else if( input->down[kb_c])
            {
                copy_selection();
                showCursor = true;
                cursorTimer = 0;
            }
            else if( input->released[kb_v])
            {
                pasteCommandGiven = true;
                showCursor = true;
                cursorTimer = 0;
            }
            else if( input->down[kb_x])
            {
                cut_selection();
                showCursor = true;
                cursorTimer = 0;
            }
            else if( input->mouseScrollingUp > 0)
            {
                move_left(4);
                showCursor = true;
                cursorTimer = 0;
            }
            else if( input->mouseScrollingDown > 0)
            {
                move_right(4);
                showCursor = true;
                cursorTimer = 0;
            }
            else if( leftArrowDelay > MAIN_GUI_SETTINGS->textInputDelayTime )
            {
                leftArrowDelay = -1;
                if( input->shiftKeyIsPressed)
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
                            if( char_is_alnum(textInputString[iNCursorX],false,false)==false )
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
                if( input->shiftKeyIsPressed)
                {
                    selectionEndCursorPos = cursorPos;
                }
                leftArrowDelay = -1;
            }
            else if( rightArrowDelay > MAIN_GUI_SETTINGS->textInputDelayTime )
            {
                rightArrowDelay = -1;
                if( input->shiftKeyIsPressed)
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
                            if( char_is_alnum(textInputString[iNCursorX],false,false)==false )
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
                if( input->shiftKeyIsPressed)
                {
                    selectionEndCursorPos = cursorPos;
                }
                rightArrowDelay = 0;
            }
        }
        else if( input->mouseScrollingUp > 0)
        {
            move_left(4);
            showCursor = true;
            cursorTimer = 0;
        }
        else if( input->mouseScrollingDown > 0)
        {
            move_right(4);
            showCursor = true;
            cursorTimer = 0;
        }
        else if( input->down[kb_alt])
        {
            //special alt action
            showCursor = true;
            cursorTimer = 0;
        }
        else if( bscDelay > MAIN_GUI_SETTINGS->textInputDelayTime )
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
                    textInputString = get_substring(lineToEdit,0,cursorPos );
                    std::string rightSide = get_substring(lineToEdit,cursorPos+1);
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
            //input->reset_all_input();
        }
        else if( delDelay > MAIN_GUI_SETTINGS->textInputDelayTime  )
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
                    textInputString = get_substring(lineToEdit,0,cursorPos );
                    std::string rightSide = get_substring(lineToEdit,cursorPos+1);
                    textInputString+=rightSide;
                    startXPos-=1;
                    if( startXPos < 0)
                    {
                        startXPos=0;
                    }
                }
                delDelay = 0;
                input->released[kb_delete] = false;
            }
            showCursor = true;
            cursorTimer = 0;
        }
        else if( leftArrowDelay > MAIN_GUI_SETTINGS->textInputDelayTime  )
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
        else if( rightArrowDelay > MAIN_GUI_SETTINGS->textInputDelayTime  )
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
        else if( upArrowDelay > MAIN_GUI_SETTINGS->textInputDelayTime  )
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
        else if( downArrowDelay > MAIN_GUI_SETTINGS->textInputDelayTime )
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
                GPE_Report("Unable to navigate from ["+int_to_string(cursorHistoryPos)+" with ["+int_to_string( (int)listOfPastStrings.size() )+"] past strings....");
            }
            showCursor = true;
            cursorTimer = 0;
            downArrowDelay = -1;
        }
        else if( input->pressed[kb_enter] )
        {
            isInUse = resubmissionAllowed;
            inputSubmitted = true;
            showCursor = true;
            cursorTimer = 0;
            enterDelay = 0;
            //GPE_Report("Submitting self data...");
        }
        else
        {
            int moreStringSize = (int)input->inkeys.size();
            if( moreStringSize > 0)
            {
                delete_selection();
                textInputString=get_substring(textInputString,0,cursorPos)+input->inkeys+get_substring(textInputString,cursorPos);
                cursorPos+=(int)input->inkeys.size();
                if( cursorPos>startXPos+maxCharactersInView)
                {
                    startXPos+=moreStringSize;
                }
                showCursor = true;
                cursorTimer = 0;
            }
            input->inkeys = "";
        }

        if( pasteCommandGiven && SDL_HasClipboardText()==SDL_TRUE )
        {
            delete_selection();
            std::string clipboardStr = SDL_GetClipboardText();
            clipboardStr = string_replace_all(clipboardStr,"\r\n","\n");
            clipboardStr = string_replace_all(clipboardStr,"\r","\n");
            clipboardStr = string_replace_all(clipboardStr,"\t","    ");
            std::string leftBeforePaste = get_substring(textInputString,0,cursorPos);
            std::string rightBeforePaste = "";
            if( cursorPos < (int)textInputString.size() )
            {
                rightBeforePaste = get_substring(textInputString,cursorPos+1);
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

void GPE_TextInputBasic::record_string( std::string strToRecord)
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

void GPE_TextInputBasic::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( isEnabled && cam!=NULL)
    {
        if( forceRedraw)
        {
            if(showBorderBox)
            {
                if( hasValidInput )
                {
                    gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Color,false, 128 );
                }
                else
                {
                    gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Error_Box_Color,false, 128 );
                }
            }

            if( showLabel && (int)inputLabel.size() > 0)
            {
                if( hAlign == FA_CENTER)
                {
                    gfs->render_text( elementBox.x+elementBox.w/2-cam->x,elementBox.y-2-cam->y,inputLabel,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_TEXTINPUT,FA_CENTER,FA_TOP);
                }
                else
                {
                    gfs->render_text( elementBox.x-cam->x,elementBox.y-2-cam->y,inputLabel,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_TOP);
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

                gcanvas->render_rectangle(
                    fieldElementBox.x+(minHighlightPos-startXPos)*fontSizeW - cam->x,
                    fieldElementBox.y+1 - cam->y,
                    fieldElementBox.x+2+(maxHighlightPos-startXPos)*fontSizeW- cam->x,
                    fieldElementBox.y+fieldElementBox.h-1 - cam->y,
                    GPE_MAIN_THEME->Input_Selected_Color,false, 128 );
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
                gfs->render_text( fieldElementBox.x+4-cam->x,fieldElementBox.y+fieldElementBox.h/2- cam->y,get_substring(textInputString,startXPos,subLength),GPE_MAIN_THEME->Input_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_MIDDLE,255);
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
                gfs->render_text( fieldElementBox.x+4-cam->x,fieldElementBox.y+fieldElementBox.h/2 - cam->y,get_substring(placeHolderString,0,subLength),GPE_MAIN_THEME->Input_Faded_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_MIDDLE);
            }
            if(showBorderBox)
            {
                if( isInUse )
                {
                    gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Highlight_Outline_Color,true);
                }
                else if( isHovered )
                {
                    gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Highlight_Alt_Color,true);
                }
                else
                {
                    gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Outline_Color,true);
                }
            }
        }

        if(isInUse && prevCursorPos!=cursorPos && prevCursorPos >=startXPos && prevCursorPos <= startXPos+maxCharactersInView )
        {
            gcanvas->render_vertical_line_color( fieldElementBox.x+GENERAL_GPE_PADDING/2+(prevCursorPos-startXPos)*fontSizeW- cam->x,fieldElementBox.y - cam->y,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Color);
        }

        if(isInUse && cursorPos >=startXPos && cursorPos <= startXPos+maxCharactersInView )
        {
            if(showCursor)
            {
                gcanvas->render_vertical_line_color( fieldElementBox.x+GENERAL_GPE_PADDING/2+(cursorPos-startXPos)*fontSizeW- cam->x,fieldElementBox.y - cam->y,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Font_Color);
            }
            else
            {
                gcanvas->render_vertical_line_color( fieldElementBox.x+GENERAL_GPE_PADDING/2+(cursorPos-startXPos)*fontSizeW- cam->x,fieldElementBox.y - cam->y,fieldElementBox.y+fieldElementBox.h - cam->y,GPE_MAIN_THEME->Input_Color);
            }
        }
    }
}

void GPE_TextInputBasic::reset_selection(int resetDirection)
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

void GPE_TextInputBasic::select_all()
{
    selectionCursorPos = 0;
    selectionEndCursorPos = textInputString.size();
    cursorPos = 0;
    input->reset_all_input();
}

void GPE_TextInputBasic::set_height(int newHeight)
{
    if( showLabel)
    {
        fieldElementBox.h = newHeight;
    }
    else
    {
        elementBox.h = newHeight;
        fieldElementBox.h = newHeight;
    }
    prevelementBoxH = newHeight;
}

void GPE_TextInputBasic::set_label(std::string newLabel)
{
    if( (int)newLabel.size() > 0)
    {
        inputLabel = newLabel;
        showLabel = true;

        elementBox.h=prevelementBoxH+fontSizeH;
        //elementBox.w = std::max( prevelementBoxW, ( fontSizeW *(int)inputLabel.size()  ) );
        fieldElementBox.x = elementBox.x;
        fieldElementBox.y = elementBox.y+fontSizeH;
        //fieldElementBox.w = prevelementBoxW;
        fieldElementBox.h = prevelementBoxH;
    }
    else
    {
        inputLabel = "";
        showLabel = false;
    }
}

void GPE_TextInputBasic::set_name(std::string newName)
{
    if( (int)newName.size() > 0)
    {
        /*if( opName == inputLabel )
        {
            set_label(newName);
        }*/
        opName = newName;
    }
}

void GPE_TextInputBasic::set_number( double numberIn)
{
    std::string newString = double_to_string(numberIn);
    textInputString = newString;
    record_string(textInputString);
    cursorPos = (int) textInputString.size();

}

void GPE_TextInputBasic::set_string(std::string newString)
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

void GPE_TextInputBasic::set_placeholder(std::string newString)
{
    placeHolderString = newString;
}

void GPE_TextInputBasic::set_width(int newWidth)
{
    if( showLabel)
    {
        fieldElementBox.w = newWidth;
    }
    else
    {
        elementBox.w = newWidth;
        fieldElementBox.w = newWidth;
    }
    elementBox.w = newWidth;
    fieldElementBox.w = newWidth;
    prevelementBoxW = newWidth;
}

void GPE_TextInputBasic::set_max_width( int nMW)
{
    maxWidth = nMW;
    if( maxWidth > 0 )
    {
        if( elementBox.w > maxWidth || fieldElementBox.w > maxWidth)
        {
            fieldElementBox.w = maxWidth;
            elementBox.w = maxWidth;
            prevelementBoxW = maxWidth;
        }
    }
}

void GPE_TextInputBasic::set_max_height( int nMH)
{
    maxHeight = nMH;
    if( maxHeight > 0 && elementBox.h > maxHeight)
    {
        elementBox.h = maxHeight;
    }
}

bool GPE_TextInputBasic::was_submitted()
{
    return inputSubmitted;
}


GPE_TextInputNumber::GPE_TextInputNumber( std::string placeHolderText, bool onlyWholeNums, double hMin, double hMax)
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
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 192;
    elementBox.h = 24;

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
    maxCharactersInView = elementBox.w/fontSizeW;

    hasValidInput = true;
    heldNumber = 0;
    startXPos = 0;
    cursorPos = 0;
    textInputString = "0";
    set_label( placeHolderText );
}

GPE_TextInputNumber::~GPE_TextInputNumber()
{

}

int GPE_TextInputNumber::get_held_int()
{
    return (int)get_held_number();
}

double GPE_TextInputNumber::get_held_number()
{
    heldNumber = string_to_double(textInputString, INT_MIN);
    return heldNumber;
}

std::string GPE_TextInputNumber::get_plain_string()
{
    heldNumber = string_to_double(textInputString, INT_MIN);
    return double_to_string(heldNumber);
}

double GPE_TextInputNumber::make_valid_number( double fallbackNumber )
{
    heldNumber = string_to_double(textInputString, INT_MIN);
    if( heldNumber== INT_MIN || heldNumber < minInputNumber || heldNumber > maxInputNumber)
    {
        heldNumber = fallbackNumber;
        hasValidInput = true;
        set_string(""+double_to_string( fallbackNumber ) );
    }
    return heldNumber;
}

void GPE_TextInputNumber::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    hasValidInput = true;
    GPE_TextInputBasic::process_self(viewedSpace,cam);
    if( (int)textInputString.size() > 0)
    {
        if( onlyWholeNumbers)
        {
            heldNumber = string_to_int(textInputString, INT_MIN);
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
            heldNumber = string_to_double(textInputString, INT_MIN);
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

