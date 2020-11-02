/*
pawgui_text_editor.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#include "pawgui_context.h"
#include "pawgui_file_popups.h"
#include "pawgui_general_resource_container.h"
#include "pawgui_search_controller.h"
#include "pawgui_statusbar.h"
#include "pawgui_text_editor.h"

namespace pawgui
{
    widget_text_anchor_controller * main_anchor_controller  = NULL;

    log_entry::log_entry(std::string projectName, std::string resource_name, std::string logTxt, std::string funcName, int lineNumb, int charNumb)
    {
        projectLogBelongsTo = projectName;
        resource_nameRelated = resource_name;
        functionName = funcName; //where applicable
        logText = logTxt;
        lineToReach = lineNumb;
        characterToReach = charNumb;
    }


    log_entry::~log_entry()
    {
        projectLogBelongsTo = "";
        resource_nameRelated = "";
        functionName = ""; //where applicable
        logText = "";
    }


    widget_text_anchor::widget_text_anchor(int lineN, int charN, std::string messageIn, std::string alertInfo, int anchorType)
    {
        anchorType = anchorType;
        lineNumber = lineN;
        characterNumber = charN;
        lineMessage = messageIn;
        lineAlert = alertInfo;
        if( main_anchor_controller!=NULL)
        {
            anchorProjectName = main_anchor_controller->searchResultProjectName;
            anchorProjectResourceId = main_anchor_controller->searchResultResourceId;
            anchorProjectResourceName = main_anchor_controller->searchResultResourceName;
        }
        else
        {
            anchorProjectName = "";
            anchorProjectResourceId = 0;
            anchorProjectResourceName = "";
        }
        if( (int)anchorProjectResourceName.size() > 0 )
        {
            widget_name = "["+anchorProjectResourceName+"]["+lineMessage+"]";
        }
        else
        {
            widget_name = "["+lineMessage+"]";
        }
        widget_name+=" Ln "+ stg_ex::int_to_string(lineN+1)+"Col "+ stg_ex::int_to_string(charN+1);
        widget_box.x = 0;
        widget_box.y = 0;
        if( FONT_LABEL_ANCHOR!=NULL)
        {
            int bWid = 0;
            int bHgt = 0;
            FONT_LABEL_ANCHOR->get_metrics(widget_name.c_str(), &bWid, &bHgt);
            widget_box.w = bWid;
            widget_box.h = bHgt+padding_default*2;
        }
    }

    widget_text_anchor::~widget_text_anchor()
    {

    }

    void widget_text_anchor::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        widget_basic::process_self(view_space,cam);

        if( isHovered)
        {
            gpe::cursor_main_controller->cursor_change("hand");
        }
        if( isInUse)
        {
            if( gpe::input->check_kb_down(kb_ctrl) && gpe::input->check_kb_released(kb_c) )
            {
                gpe::input->clipboard_set (widget_name.c_str() );
            }
        }
    }

    void widget_text_anchor::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( (int)widget_name.size() > 0 && view_space!=NULL && cam!=NULL )
        {
            if( isHovered)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_highlight_color,false);
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_border_highlight_color,false);
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_color,false);
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_border_color,true);
            }
            gpe::gfs->render_text( widget_box.x-cam->x+padding_default,widget_box.y-cam->y+padding_default,widget_name,pawgui::theme_main->text_box_font_color,FONT_LABEL_ANCHOR,gpe::fa_left,gpe::fa_middle);
        }
    }

    widget_text_anchor_controller::widget_text_anchor_controller()
    {
        searchResultProjectName = "";
        searchResultResourceName = "";
        searchResultResourceId = -1;
    }

    widget_text_anchor_controller::~widget_text_anchor_controller()
    {
        clear_list( true );
    }

    void widget_text_anchor_controller::clear_list( bool deleteAnchors  )
    {
        if( deleteAnchors )
        {
            widget_text_anchor * tempAnchor = NULL;
            int gcSize =  loggedAnchors.size();
            for( int gcItr = gcSize-1; gcItr >=0; gcItr-- )
            {
                tempAnchor =  loggedAnchors.at( gcItr );
                if( tempAnchor !=NULL )
                {
                    delete tempAnchor;
                    tempAnchor = NULL;

                }
            }
        }
        loggedAnchors.clear();
    }

    widget_text_editor::widget_text_editor(bool saveFirstEdit)
    {
        tempCLineXStartPos = 0;
        tempCLineXEndPos = 0;
        needsNewLine = true;
        isFullWidth = true;
        editorZoomLevel= 1;
        defaultLineHeight = 16; //px
        redrawDelay = 0;
        redrawDelayMax = 2;
        parseForErrorsTimerGoal = 60;
        parseForErrorsTimerPos = 0;
        missingSymbolDetected = false;
        missingSymbolStartXPos = 0;
        missingSymbolStartYPos = 0;
        misingSymbolSuggestedEndYPos = 0;
        misingSymbolSuggestedEndXPos = 0;

        highlightXPos = 0;
        highlightYPos = 0;
        widget_type = "textarea";
        lastfloatClickAction = 0;
        undoableActionOccurred = false;
        currentPositionInHistory = 0;
        hasLineBreak = true;
        showButtonBar = true;
        isReadOnly = false;
        isCodeEditor = true;
        isTextLog= false;
        codeEditorType = 0;
        showXScroll = false;
        showYScroll = false;
        useWordWrap = false;
        displayMode = 0;
        cursorXPos = 0;
        cursorYPos = 0;
        prevCursorXPos = cursorXPos;
        prevCursorYPos = cursorYPos;
        cursorTimer = 0;
        showCursor = false;
        inputFieldPos = 0;
        maxLength = 2048;
        maxLineCount = 32768;
        lineCountBoxWidth = 48;
        lineStartXPos = 0;
        charactersWithinView = 16;
        linesWithinView = 4;
        lineStartYPos = 0;
        textInputString = "";
        lineToEdit = "";

        tabDelay = 0;
        bscDelay = 0;
        delDelay = 0;
        enterDelay = 0;
        upDelay = 0;
        downDelay = 0;
        leftDelay = 0;
        rightDelay = 0;
        dKeyDelay = 0;
        textXScroll = new widget_scrollbar_xaxis();
        textYScroll = new widget_scrollbar_yaxis();
        TEXTBOX_FONT_SIZE_WIDTH = 10;
        TEXTBOX_FONT_SIZE_HEIGHT = 10;
        if( font_textinput!=NULL)
        {
            font_textinput->get_metrics("A",&TEXTBOX_FONT_SIZE_WIDTH,&TEXTBOX_FONT_SIZE_HEIGHT);
        }

        lineCountBox = new gpe::shape_rect();
        lineCountBox->x = 0;
        lineCountBox->y = 0;
        lineCountBox->w = 0;
        lineCountBox->h = 0;

        renderBox = new gpe::shape_rect();
        renderBox->x = 0;
        renderBox->y = 0;
        renderBox->w = 0;
        renderBox->h = 0;

        textSpaceRect = new gpe::shape_rect();
        textSpaceRect->x = 0;
        textSpaceRect->y = 0;
        textSpaceRect->w = 0;
        textSpaceRect->h = 0;

        textEditorButtonBar = NULL;
        create_buttonbar();

        selectionCursorXPos = 0;
        selectionCursorYPos = 0;
        selectionEndCursorXPos = 0;
        selectionEndCursorYPos = 0;


        symbolCursorXPos = -1;
        symbolCursorYPos = -1;
        symbolEndCursorXPos = -1;
        symbolEndCursorYPos = -1;

        commentLineText = new syntax_line();
        datatypeLineText = new syntax_line();
        functionLineText = new syntax_line();
        variableLineText = new syntax_line();
        projectFunctionLineText = new syntax_line();
        projectKeywordLineText = new syntax_line();
        keywordLineText = new syntax_line();
        normalLineText = new syntax_line();
        numberLineText = new syntax_line();
        dQuoteLineText = new syntax_line();
        sQuoteLineText = new syntax_line();
        symbolLineText = new syntax_line();
        clear_all_lines();
        listOfStrings.push_back("");
        widget_box.w = 640;
        widget_box.h = 480;
        if( saveFirstEdit)
        {
            save_edit();
        }
    }

    widget_text_editor::~widget_text_editor()
    {
        if( textSpaceRect!=NULL)
        {
            delete textSpaceRect;
            textSpaceRect = NULL;
        }
        if( textXScroll!=NULL)
        {
            delete textXScroll;
            textXScroll = NULL;
        }

        if( textYScroll!=NULL)
        {
            delete textYScroll;
            textYScroll= NULL;
        }
        if( textEditorButtonBar!=NULL)
        {
            delete textEditorButtonBar;
            textEditorButtonBar = NULL;
        }
        if( commentLineText!=NULL)
        {
            delete commentLineText;
            commentLineText = NULL;
        }
        if( datatypeLineText!=NULL)
        {
            delete datatypeLineText;
            datatypeLineText = NULL;
        }
        if( dQuoteLineText!=NULL)
        {
            delete dQuoteLineText;
            dQuoteLineText = NULL;
        }
        if( functionLineText!=NULL)
        {
            delete functionLineText;
            functionLineText = NULL;
        }
        if( keywordLineText!=NULL)
        {
            delete keywordLineText;
            keywordLineText = NULL;
        }
        if( normalLineText!=NULL)
        {
            delete normalLineText;
            normalLineText = NULL;
        }

        if( numberLineText!=NULL)
        {
            delete numberLineText;
            numberLineText = NULL;
        }

        if( sQuoteLineText!=NULL)
        {
            delete sQuoteLineText;
            sQuoteLineText = NULL;
        }

        if( symbolLineText!=NULL)
        {
            delete symbolLineText;
            symbolLineText = NULL;
        }

        if( projectFunctionLineText!=NULL)
        {
            delete projectFunctionLineText;
            projectFunctionLineText = NULL;
        }

        if( projectKeywordLineText!=NULL)
        {
            delete projectKeywordLineText;
            projectKeywordLineText = NULL;
        }
        if( variableLineText!=NULL)
        {
            delete variableLineText;
            variableLineText = NULL;
        }
        int iD = 0;
        widget_text_editor * tempText = NULL;
        for( iD = (int)savedHistory.size()-1; iD >=0; iD--)
        {
            tempText = savedHistory.at(iD);
            if( tempText!=NULL)
            {
                delete tempText;
                tempText = NULL;
            }
        }

        widget_text_anchor * tempAnchor = NULL;
        for( iD = (int)anchorPositions.size()-1; iD >=0; iD--)
        {
            tempAnchor = anchorPositions.at(iD);
            if( tempAnchor!=NULL)
            {
                delete tempAnchor;
                tempAnchor = NULL;
            }
        }
        savedHistory.clear();
        listOfStrings.clear();
        if( textEditorButtonBar!=NULL)
        {
            delete textEditorButtonBar;
            textEditorButtonBar = NULL;
        }
    }

    void widget_text_editor::activate_self()
    {
        isEnabled = true;
        isInUse = true;
        isClicked = true;
        showCursor = true;
        cursorTimer = 0;
    }

    void widget_text_editor::add_line( std::string newLine, bool scrollToBottom)
    {
        listOfStrings.push_back( stg_ex::trim_right_inplace(newLine) );
        if( scrollToBottom)
        {
            scroll_to_pos( (int)listOfStrings.size()-1, 0 );
        }
    }

    void widget_text_editor::adjust_fortabs()
    {
        /*
            if( cursorXPos > 0 && main_settings!=NULL && main_settings->autoFindMouseTabs )
            {
                if( (int)listOfStrings.size() > cursorYPos )
                {
                    std::string currStrToCheck = listOfStrings[cursorYPos];
                    int currLineSize = (int)currStrToCheck.size();
                    if( currLineSize > cursorXPos )
                    {
                        if( (int)currStrToCheck[cursorXPos]==' ')
                        {
                            int preSpacesCounted = 0;
                            int postSpacesCounted = 0;
                            int allSpacesCounted = 0;
                            int i = 0;

                            for( i = cursorXPos-1; i>= 0; i--)
                            {
                                if( currStrToCheck[i]==' ')
                                {
                                    preSpacesCounted++;
                                }
                                else
                                {
                                    break;
                                }
                            }

                            for( i = cursorXPos+1; i < currLineSize; i++)
                            {
                                if( currStrToCheck[i]==' ')
                                {
                                    postSpacesCounted++;
                                }
                                else
                                {
                                    break;
                                }
                            }

                            allSpacesCounted = preSpacesCounted+postSpacesCounted+1;
                            preSpacesCounted = preSpacesCounted%main_settings->tabSpaceCount;
                            postSpacesCounted = postSpacesCounted%main_settings->tabSpaceCount;
                            if( allSpacesCounted%main_settings->tabSpaceCount==0)
                            {
                                if( preSpacesCounted >postSpacesCounted )
                                {
                                    cursorXPos+=postSpacesCounted+1;
                                }
                                else
                                {
                                    cursorXPos-=preSpacesCounted;
                                }
                                scroll_to_cursor();
                            }
                        }
                    }
                    else
                    {
                        cursorXPos = (int)currStrToCheck.size();
                    }
                }
            }
        */
    }

    bool widget_text_editor::can_redo()
    {
        if( currentPositionInHistory >=0 && currentPositionInHistory <= (int)savedHistory.size()-1)
        {
            return true;
        }
        return false;
    }

    bool widget_text_editor::can_undo()
    {
        if( currentPositionInHistory > 0 && (int)savedHistory.size() > 0)
        {
            return true;
        }
        return false;
    }

    void widget_text_editor::clear_line(int lineNumber)
    {

    }

    void widget_text_editor::clear_all_lines()
    {
        listOfStrings.clear();
        cursorXPos = 0;
        cursorYPos = 0;
        selectionCursorXPos = 0;
        selectionCursorYPos = 0;
        selectionEndCursorXPos = 0;
        selectionEndCursorYPos = 0;
        lineStartYPos = 0;
        linesWithinView = 0;
    }

    void widget_text_editor::clear_text_anchors()
    {
        if( (int)anchorPositions.size() > 0)
        {
            widget_text_anchor * tAnchor = NULL;
            for( int i = (int)anchorPositions.size()-1; i >=0; i--)
            {
                tAnchor = anchorPositions[i];
                delete tAnchor;
                tAnchor = NULL;
            }
            anchorPositions.clear();
        }
    }

    void widget_text_editor::create_buttonbar()
    {
        if( textEditorButtonBar!=NULL)
        {
            delete textEditorButtonBar;
            textEditorButtonBar = NULL;
        }
        textEditorButtonBar = new widget_button_iconbar( 16);
        textEditorButtonBar->set_height(24);
        textEditorButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/save.png","Export Text",TEXTAREA_OPTION_EXPORT,false );
        textEditorButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file.png","Import Text",TEXTAREA_OPTION_IMPORT,true );
        textEditorButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/eraser.png","Clear Text ",TEXTAREA_OPTION_CLEAR );
        textEditorButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/backward.png","Undo Action",TEXTAREA_OPTION_UNDO,false );
        textEditorButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/forward.png","Redo Action",TEXTAREA_OPTION_REDO,true );

        textEditorButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cut.png","Cut",TEXTAREA_OPTION_CUT,false );
        textEditorButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/copy.png","Copy",TEXTAREA_OPTION_COPY,true );
        textEditorButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/paste.png","Paste",TEXTAREA_OPTION_PASTE,true );

        //textEditorButtonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/info.png","Text Info (Not Available Yet)",TEXTAREA_OPTION_TEXTINFO,true );
        textEditorButtonBar->limit_width(true);
    }

    void widget_text_editor::copy_selection()
    {
        if( selectionCursorYPos >=0 && selectionCursorYPos < (int)listOfStrings.size() && selectionEndCursorYPos >=0 && selectionEndCursorYPos < (int)listOfStrings.size() )
        {
            int minHighlightXPos = 0, maxHighlightXPos = 0;
            std::string newClipBoardText = "";
            std::string strToCopy = "";
            if(selectionCursorXPos!=selectionEndCursorXPos || selectionCursorYPos!=selectionEndCursorYPos)
            {
                std::string tempStrToUse = "";
                int minHighlightYPos = std::min(selectionCursorYPos, selectionEndCursorYPos);
                int maxHighlightYPos = std::max(selectionCursorYPos, selectionEndCursorYPos);

                if( selectionCursorYPos > selectionEndCursorYPos)
                {
                    minHighlightXPos = selectionEndCursorXPos;
                    maxHighlightXPos = selectionCursorXPos;
                }
                else
                {
                    maxHighlightXPos = selectionEndCursorXPos;
                    minHighlightXPos = selectionCursorXPos;
                }
                if(minHighlightYPos!=maxHighlightYPos)
                {
                    tempStrToUse = listOfStrings.at(minHighlightYPos);
                    strToCopy = stg_ex::get_substring(tempStrToUse,minHighlightXPos,(int)std::string::npos);
                    if( maxHighlightYPos-minHighlightYPos > 1)
                    {
                        for( int i= minHighlightYPos+1; i <=maxHighlightYPos-1; i++ )
                        {
                            tempStrToUse = listOfStrings[i];
                            strToCopy += "\n"+tempStrToUse;
                        }
                    }
                    tempStrToUse = listOfStrings.at(maxHighlightYPos);
                    strToCopy += "\n"+stg_ex::get_substring(tempStrToUse,0,maxHighlightXPos);
                }
                else
                {
                    strToCopy = listOfStrings.at(selectionCursorYPos);
                    minHighlightXPos = std::min(selectionCursorXPos, selectionEndCursorXPos);
                    maxHighlightXPos = std::max(selectionCursorXPos, selectionEndCursorXPos);
                    strToCopy = stg_ex::get_substring(strToCopy,minHighlightXPos,maxHighlightXPos-minHighlightXPos);
                }
                gpe::input->clipboard_set(strToCopy.c_str() );
                //main_overlay_system->update_temporary_message("Selection copied","","",0.25);
            }
        }
    }

    bool widget_text_editor::compile_into_code(std::ofstream * fileTarget, int leftTabAmount, bool useNewLines, bool useLastNewLine )
    {
        if( fileTarget!=NULL && leftTabAmount >=0)
        {
            if( fileTarget->is_open() )
            {
                std::string nestedTabsStr = generate_tabs( leftTabAmount  );
                if( !parse_code_javascript() )
                {
                    return false;
                }
                int strSize = (int)listOfStrings.size();

                for( int i = 0; i < strSize-1; i++)
                {
                    if( (int)listOfStrings[i].size() > 0 )
                    {
                        if( useNewLines)
                        {
                            *fileTarget << nestedTabsStr+listOfStrings[i] << "\n";
                        }
                        else
                        {
                            *fileTarget <<nestedTabsStr+listOfStrings[i] << " ";
                        }
                    }
                }
                if( (int)listOfStrings[strSize-1].size() > 0 )
                {
                    if( useNewLines)
                    {
                        if( useLastNewLine)
                        {
                            *fileTarget << nestedTabsStr+listOfStrings[strSize-1] << "\n";
                        }
                        else
                        {
                            *fileTarget << nestedTabsStr+listOfStrings[strSize-1];
                        }
                    }
                    else
                    {
                        *fileTarget <<nestedTabsStr+listOfStrings[strSize-1] << " ";
                    }
                }
                return true;
            }
        }
        return false;
    }

    int widget_text_editor::compare_with(widget_text_editor * other)
    {
        if( other!=NULL)
        {
            if( (int)listOfStrings.size() == (int)other->listOfStrings.size()  && (int)listOfStrings.size() > 0 )
            {
                std::string myString = "";
                std::string otherString = "";
                for( int i = (int)listOfStrings.size()-1; i>=0; i--)
                {
                    myString = listOfStrings[i];
                    otherString = other->listOfStrings[i];
                    if( (int)myString.size()==(int)otherString.size() )
                    {
                        if( myString.compare(otherString)!=0 )
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                return 1;
            }
            return 0;
        }
        return -1;
    }

    bool widget_text_editor::copy_all()
    {
        if( has_content() )
        {
            std::string superLongClipboardStr = "";
            for( int i = 0; i < (int)listOfStrings.size(); i++)
            {
                superLongClipboardStr+=listOfStrings[i]+"\n";
            }
            if( gpe::input->clipboard_set( superLongClipboardStr.c_str() )!=0 )
            {
                return false;
            }
            return true;
        }
        return false;
    }

    void widget_text_editor::copy_source(widget_text_editor * other, bool overwriteSelf)
    {
        if( other!=NULL)
        {
            if( overwriteSelf)
            {
                clear_all_lines();
            }
            cursorXPos = other->cursorXPos;
            cursorYPos = other->cursorYPos;
            lineStartXPos = other->lineStartXPos;
            lineStartYPos = other->lineStartYPos;
            selectionCursorXPos = other->selectionCursorXPos;
            selectionCursorYPos = other->selectionCursorYPos;
            selectionEndCursorXPos= other->selectionEndCursorXPos;
            selectionEndCursorYPos = other->selectionEndCursorYPos;

            for( int i = 0; i < (int)other->listOfStrings.size(); i++)
            {
                listOfStrings.push_back( other->listOfStrings[i] );
            }
        }
    }

    void widget_text_editor::cut_selection()
    {
        if( !isReadOnly)
        {
            copy_selection();
            delete_selection();
        }
    }

    void widget_text_editor::delete_content()
    {

    }

    void widget_text_editor::delete_selection()
    {
        if( !isReadOnly)
        {
            if( selectionCursorYPos >=0 && selectionCursorYPos < (int)listOfStrings.size() && selectionEndCursorYPos >=0 && selectionEndCursorYPos < (int)listOfStrings.size() )
            {
                if( selectionCursorXPos!=selectionEndCursorXPos || selectionCursorYPos!=selectionEndCursorYPos)
                {
                    int minHighlightXPos = 0, maxHighlightXPos = 0;
                    std::string newClipBoardText = "";
                    std::string strToCopy = "";
                    std::string tempStrToUse = "";
                    if(selectionCursorXPos!=selectionEndCursorXPos || selectionCursorYPos!=selectionEndCursorYPos)
                    {
                        int minHighlightYPos = std::min(selectionCursorYPos, selectionEndCursorYPos);
                        int maxHighlightYPos = std::max(selectionCursorYPos, selectionEndCursorYPos);

                        if( selectionCursorYPos > selectionEndCursorYPos)
                        {
                            minHighlightXPos = selectionEndCursorXPos;
                            maxHighlightXPos = selectionCursorXPos;
                        }
                        else
                        {
                            maxHighlightXPos = selectionEndCursorXPos;
                            minHighlightXPos = selectionCursorXPos;
                        }

                        if(minHighlightYPos!=maxHighlightYPos)
                        {
                            tempStrToUse = listOfStrings.at(minHighlightYPos);
                            if( minHighlightXPos >= 0)
                            {
                                strToCopy = stg_ex::get_substring(tempStrToUse,0,minHighlightXPos);
                            }
                            tempStrToUse = listOfStrings.at(maxHighlightYPos);
                            if( maxHighlightXPos <= (int)tempStrToUse.size() )
                            {
                                strToCopy += stg_ex::get_substring(tempStrToUse,maxHighlightXPos);
                            }
                            for( int i=maxHighlightYPos; i >= minHighlightYPos; i-- )
                            {
                                listOfStrings.erase(listOfStrings.begin()+i);
                            }
                            if( (int)strToCopy.size() > 0)
                            {
                                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = minHighlightXPos;
                                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = minHighlightYPos;
                                listOfStrings.insert(listOfStrings.begin()+minHighlightYPos,strToCopy);
                            }
                            else if( (int)listOfStrings.size() > minHighlightYPos)
                            {
                                listOfStrings.insert(listOfStrings.begin()+minHighlightYPos,"");
                                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = minHighlightYPos;
                            }
                            else
                            {
                                listOfStrings.push_back("");
                                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = (int)listOfStrings.size()-1;
                            }
                        }
                        else
                        {
                            minHighlightXPos = std::min(selectionCursorXPos, selectionEndCursorXPos);
                            maxHighlightXPos = std::max(selectionCursorXPos, selectionEndCursorXPos);
                            tempStrToUse = listOfStrings.at(selectionCursorYPos);
                            strToCopy = stg_ex::get_substring(tempStrToUse,0,minHighlightXPos);
                            strToCopy += stg_ex::get_substring(tempStrToUse,maxHighlightXPos);
                            listOfStrings.erase(listOfStrings.begin()+minHighlightYPos);
                            if( strToCopy.size() > 0)
                            {
                                listOfStrings.insert(listOfStrings.begin()+minHighlightYPos,strToCopy);
                                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = minHighlightXPos;
                                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = minHighlightYPos;
                            }
                            else if( (int)listOfStrings.size() > minHighlightYPos)
                            {
                                listOfStrings.insert(listOfStrings.begin()+minHighlightYPos,"");
                                tempStrToUse = listOfStrings.at(minHighlightYPos);
                                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = minHighlightYPos;
                            }
                            else
                            {
                                listOfStrings.push_back("");
                                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = (int)listOfStrings.size()-1;
                            }
                        }
                        save_edit();
                    }
                }

                if( listOfStrings.size() <=0)
                {
                    listOfStrings.push_back("");
                    cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                    cursorYPos = selectionCursorYPos = selectionEndCursorYPos = 0;
                    save_edit();
                }
                if( cursorXPos < 0)
                {
                    cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                    cursorYPos = selectionCursorYPos = selectionEndCursorYPos = (int)listOfStrings.size()-1;
                }
                lineToEdit = listOfStrings[cursorYPos];

            }
            else
            {
                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = 0;
                lineToEdit = listOfStrings[cursorYPos];
            }
        }
    }

    void widget_text_editor::duplicate_text()
    {
        if( !isReadOnly)
        {
            std::string prevClipboard = gpe::input->clipboard_string();

            if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
            {
                //adds selection to below...
                if( selectionCursorXPos!=selectionEndCursorXPos || selectionCursorYPos!=selectionEndCursorYPos)
                {
                    copy_selection();
                    if( selectionCursorYPos > selectionEndCursorYPos)
                    {
                        cursorXPos = selectionCursorXPos;
                        cursorYPos = selectionCursorYPos;
                    }
                    else
                    {
                        if( selectionCursorXPos > selectionEndCursorXPos)
                        {
                            cursorXPos = selectionCursorXPos;
                            cursorYPos = selectionEndCursorYPos;
                        }
                        else
                        {
                            cursorXPos = selectionEndCursorXPos;
                            cursorYPos = selectionEndCursorYPos;
                        }
                    }
                    int prevCX = cursorXPos;
                    int prevCY = cursorYPos;
                    paste_clipboard();
                    cursorXPos = prevCX;
                    cursorYPos = prevCY;
                    gpe::input->clipboard_set(prevClipboard.c_str() );
                }
                else
                {
                    std::string cLineToCopy = listOfStrings[cursorYPos];
                    listOfStrings.insert(listOfStrings.begin()+cursorYPos+1,cLineToCopy);
                    save_edit();
                }
            }
        }
    }

    void widget_text_editor::export_text(std::string new_file_name)
    {
        if( !new_file_name.empty() )
        {
            std::ofstream myfile (new_file_name.c_str() );
            if (myfile.is_open())
            {
                int strSize = (int)listOfStrings.size();
                for( int i = 0; i < strSize; i++)
                {
                    if( i !=strSize-1)
                    {
                        myfile << listOfStrings[i] << "\n";
                    }
                    else
                    {
                        myfile << listOfStrings[i];
                    }
                }
                myfile.close();
            }
            else
            {
                gpe::error_log->report("Unable to save text " + new_file_name);
            }
        }
    }

    void widget_text_editor::export_text_anchors(std::string new_file_name)
    {
        if( !new_file_name.empty() )
        {
            std::ofstream myfile (new_file_name.c_str() );
            if (myfile.is_open())
            {
                int strSize = (int)anchorPositions.size();
                widget_text_anchor * tAnchor = NULL;
                for( int i = 0; i < strSize; i++)
                {
                    tAnchor = anchorPositions[i];
                    if( tAnchor!=NULL)
                    {
                        myfile <<  tAnchor->lineNumber << "," << tAnchor->characterNumber;
                        if( i !=strSize-1)
                        {
                            myfile << "\n";
                        }
                    }
                }
                myfile.close();
            }
            else
            {
                gpe::error_log->report("Unable to save text anchors " + new_file_name);
            }
        }
    }

    int widget_text_editor::find_all_strings(std::string strTarget,bool matchCase, bool addAnchor, std::string areaTitle)
    {
        int stringsFoundInSearch = 0;
        if( strTarget.size() > 0)
        {
            //int tempXCursor = cursorXPos;
            //int tempYCursor = cursorYPos;
            int tempLineXStart = lineStartXPos;
            int tempLineYStart = lineStartYPos;
            widget_text_anchor * nTextAnchor = NULL;
            clear_text_anchors();
            cursorXPos = 0;
            cursorYPos = 0;
            clear_text_anchors();
            while( find_string(strTarget,true,matchCase,false) ==true)
            {
                //add to list
                stringsFoundInSearch++;
                if( addAnchor && main_anchor_controller!=NULL )
                {
                    nTextAnchor = new widget_text_anchor(selectionCursorYPos,selectionCursorXPos,areaTitle,"",GPE_ANCHOR_REGULAR);
                    main_anchor_controller->loggedAnchors.push_back(nTextAnchor);
                }
                else
                {
                    nTextAnchor = new widget_text_anchor(selectionCursorYPos,selectionCursorXPos,"Found ["+strTarget+"]","",GPE_ANCHOR_REGULAR);
                    anchorPositions.push_back(nTextAnchor);
                }
                cursorXPos+=(int)strTarget.size();
            }
            selectionCursorXPos = selectionEndCursorXPos = 0;
            selectionCursorYPos = selectionEndCursorYPos = 0;
            scroll_to_pos(tempLineYStart,tempLineXStart);
        }
        return stringsFoundInSearch;
    }

    void widget_text_editor::find_mouse_cursor(int *mXCursor, int *mYCursor, gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( view_space!=NULL && cam!=NULL && (int)listOfStrings.size() > 0)
        {
            if( gpe::point_within_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y, textSpaceRect)  )
            {
                *mYCursor = (float)(gpe::input->mouse_position_y - textSpaceRect->y )/( defaultLineHeight * editorZoomLevel); //gets the widget_box.y pos essentially
                *mYCursor+=lineStartYPos;
                if( *mYCursor < lineStartYPos)
                {
                    *mYCursor = lineStartYPos;
                }
                if( *mYCursor >= (int)listOfStrings.size() )
                {
                    *mYCursor = (int)listOfStrings.size() - 1;
                }

                if( *mYCursor < 0 )
                {
                    *mYCursor = 0;
                }
                if( (int)listOfStrings.size()==0)
                {
                    listOfStrings.push_back("");
                }
                *mXCursor = (float)(gpe::input->mouse_position_x - textSpaceRect->x )/( TEXTBOX_FONT_SIZE_WIDTH * editorZoomLevel);
                mXCursor+=  (int)lineStartXPos;
                std::string newString = listOfStrings.at(*mYCursor);
                int maxCursorSpace = (int)newString.size();
                if( *mXCursor > maxCursorSpace )
                {
                    *mXCursor = maxCursorSpace;
                }
                if( *mXCursor < 0 )
                {
                    *mXCursor = 0;
                }
            }
        }
    }

    bool widget_text_editor::find_connected_symbols()
    {
        symbolCursorXPos = -1;
        symbolCursorYPos = -1;
        symbolEndCursorXPos = -1;
        symbolEndCursorYPos = -1;
        if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
        {
            lineToEdit =listOfStrings[cursorYPos];
            int seekedCursorYPos = cursorYPos;
            if( (int)lineToEdit.size() > 0)
            {
                int fCursorXPos = cursorXPos;
                std::string beginSymbol;
                std::string endSymbol;
                std::string charStr = "";
                bool foundBookedSymbol = false;
                bool searchForEndSymbol = false;

                int subStrSearchBeginPos = cursorXPos;
                int subStrSearchEndPos = cursorXPos;
                int foundOtherMatches = 0;
                int foundSeekedMatches = 0;
                if( cursorXPos >=0 && cursorXPos <(int)lineToEdit.size() )
                {
                    fCursorXPos = cursorXPos;
                    subStrSearchBeginPos = fCursorXPos;
                    subStrSearchEndPos = fCursorXPos;
                }
                charStr = stg_ex::get_substring(lineToEdit,fCursorXPos,1);
                if( charStr =="(")
                {
                    beginSymbol = "(";
                    endSymbol = ")";
                    symbolCursorXPos = fCursorXPos;
                    symbolCursorYPos = cursorYPos;
                    foundBookedSymbol = true;
                    searchForEndSymbol = true;
                }
                else if( charStr =="{")
                {
                    beginSymbol = "{";
                    endSymbol = "}";
                    symbolCursorXPos = fCursorXPos;
                    symbolCursorYPos = cursorYPos;
                    foundBookedSymbol = true;
                    searchForEndSymbol = true;
                }
                else if( charStr =="[")
                {
                    beginSymbol = "[";
                    endSymbol = "]";
                    symbolCursorXPos = fCursorXPos;
                    symbolCursorYPos = cursorYPos;
                    foundBookedSymbol = true;
                    searchForEndSymbol = true;
                }
                else if( charStr ==")")
                {
                    beginSymbol = "(";
                    endSymbol = ")";
                    symbolEndCursorXPos = fCursorXPos;
                    symbolEndCursorYPos = cursorYPos;
                    foundBookedSymbol = true;
                }
                else if( charStr =="}")
                {
                    beginSymbol = "{";
                    endSymbol = "}";
                    symbolEndCursorXPos = fCursorXPos;
                    symbolEndCursorYPos = cursorYPos;
                    foundBookedSymbol = true;
                }
                else if( charStr =="]")
                {
                    beginSymbol = "[";
                    endSymbol = "]";
                    symbolEndCursorXPos = fCursorXPos;
                    symbolEndCursorYPos = cursorYPos;
                    foundBookedSymbol = true;
                }

                if( foundBookedSymbol==false && fCursorXPos-1>=0 && fCursorXPos-1 < (int)lineToEdit.size() )
                {
                    charStr = stg_ex::get_substring(lineToEdit,fCursorXPos-1,1);
                    if( charStr =="(")
                    {
                        beginSymbol = "(";
                        endSymbol = ")";
                        symbolCursorXPos = fCursorXPos-1;
                        symbolCursorYPos = cursorYPos;
                        foundBookedSymbol = true;
                        searchForEndSymbol = true;
                        fCursorXPos-=1;
                    }
                    else if( charStr =="{")
                    {
                        beginSymbol = "{";
                        endSymbol = "}";
                        symbolCursorXPos = fCursorXPos-1;
                        symbolCursorYPos = cursorYPos;
                        foundBookedSymbol = true;
                        searchForEndSymbol = true;
                        fCursorXPos-=1;
                    }
                    else if( charStr =="[")
                    {
                        beginSymbol = "[";
                        endSymbol = "]";
                        symbolCursorXPos = fCursorXPos-1;
                        symbolCursorYPos = cursorYPos;
                        foundBookedSymbol = true;
                        searchForEndSymbol = true;
                        fCursorXPos-=1;
                    }
                    else if( charStr ==")")
                    {
                        beginSymbol = "(";
                        endSymbol = ")";
                        symbolEndCursorXPos = fCursorXPos-1;
                        symbolEndCursorYPos = cursorYPos;
                        foundBookedSymbol = true;
                        fCursorXPos-=1;
                    }
                    else if( charStr =="}")
                    {
                        beginSymbol = "{";
                        endSymbol = "}";
                        symbolEndCursorXPos = fCursorXPos-1;
                        symbolEndCursorYPos = cursorYPos;
                        foundBookedSymbol = true;
                        fCursorXPos-=1;
                    }
                    else if( charStr =="]")
                    {
                        beginSymbol = "[";
                        endSymbol = "]";
                        symbolEndCursorXPos = fCursorXPos-1;
                        symbolEndCursorYPos = cursorYPos;
                        foundBookedSymbol = true;
                        fCursorXPos-=1;
                    }
                }

                if( fCursorXPos >= (int)lineToEdit.size() )
                {
                    fCursorXPos = (int)lineToEdit.size()-1;
                }

                if( foundBookedSymbol )
                {
                    std::string searchedStr = listOfStrings[seekedCursorYPos];
                    std::string tempStr;
                    int prevFoundXPos = 0;
                    if( searchForEndSymbol)
                    {
                        foundOtherMatches = 0;
                        foundSeekedMatches = 0;
                        prevFoundXPos = symbolCursorXPos+1;
                        while( seekedCursorYPos < (int)listOfStrings.size() )
                        {
                            searchedStr = listOfStrings[seekedCursorYPos];

                            if( prevFoundXPos!=(int)std::string::npos)
                            {
                                subStrSearchBeginPos = searchedStr.find(beginSymbol,prevFoundXPos);
                                subStrSearchEndPos = searchedStr.find(endSymbol,prevFoundXPos);

                                if( subStrSearchBeginPos!=(int)std::string::npos && subStrSearchEndPos!=(int)std::string::npos)
                                {
                                    if( subStrSearchBeginPos > subStrSearchEndPos)
                                    {
                                        foundSeekedMatches++;
                                        prevFoundXPos = subStrSearchEndPos+1;
                                    }
                                    else
                                    {
                                        foundOtherMatches++;
                                        prevFoundXPos = subStrSearchBeginPos+1;
                                    }
                                }
                                else if(subStrSearchEndPos!=(int)std::string::npos)
                                {
                                    foundSeekedMatches++;
                                    prevFoundXPos = subStrSearchEndPos+1;
                                }
                                else if(subStrSearchBeginPos!=(int)std::string::npos)
                                {
                                    foundOtherMatches++;
                                    prevFoundXPos = subStrSearchBeginPos+1;
                                }
                                else
                                {
                                    seekedCursorYPos++;
                                    prevFoundXPos = subStrSearchBeginPos = 0;
                                    subStrSearchEndPos = 0;
                                }

                                if( foundSeekedMatches > foundOtherMatches )
                                {
                                    symbolEndCursorXPos = subStrSearchEndPos;
                                    symbolEndCursorYPos = seekedCursorYPos;
                                    return true;
                                }
                            }

                            if( subStrSearchBeginPos >=(int)searchedStr.size() )
                            {
                                subStrSearchBeginPos = (int)std::string::npos ;
                            }
                            if( subStrSearchEndPos >=(int)searchedStr.size() )
                            {
                                subStrSearchEndPos = (int)std::string::npos ;
                            }
                            if( subStrSearchBeginPos==(int)std::string::npos && subStrSearchEndPos==(int)std::string::npos)
                            {
                                seekedCursorYPos++;
                                prevFoundXPos = subStrSearchBeginPos = subStrSearchEndPos = 0;
                            }
                        }
                    }
                    else
                    {
                        //reverse iterate
                        foundOtherMatches = 0;
                        foundSeekedMatches = 0;
                        subStrSearchBeginPos = prevFoundXPos;
                        subStrSearchEndPos = prevFoundXPos;
                        seekedCursorYPos = cursorYPos;

                        if( symbolEndCursorXPos > 0)
                        {
                            prevFoundXPos = symbolEndCursorXPos-1;
                        }
                        else if( seekedCursorYPos > 0 && seekedCursorYPos <(int)listOfStrings.size() )
                        {
                            seekedCursorYPos -=1;
                            searchedStr = listOfStrings[seekedCursorYPos];
                            prevFoundXPos = (int)searchedStr.size()-1;
                        }
                        else
                        {
                            return false;
                        }

                        int cSearchCursorXPos = 0;

                        while( seekedCursorYPos >= 0 )
                        {
                            searchedStr = listOfStrings[seekedCursorYPos];

                            for( cSearchCursorXPos = prevFoundXPos; cSearchCursorXPos>=0; cSearchCursorXPos--)
                            {
                                charStr = stg_ex::get_substring(searchedStr,cSearchCursorXPos,1);
                                if( charStr ==beginSymbol)
                                {
                                    foundSeekedMatches++;
                                }
                                else if( charStr ==endSymbol)
                                {
                                    foundOtherMatches++;
                                }
                                if( foundSeekedMatches > foundOtherMatches )
                                {
                                    symbolCursorXPos = cSearchCursorXPos;
                                    symbolCursorYPos = seekedCursorYPos;
                                    return true;
                                }
                            }
                            seekedCursorYPos--;
                            if( seekedCursorYPos>=0 && seekedCursorYPos < (int)listOfStrings.size() )
                            {
                                searchedStr = listOfStrings[seekedCursorYPos];
                                prevFoundXPos = (int)searchedStr.size()-1;
                            }
                        }
                    }
                }
            }
        }
        symbolCursorXPos = -1;
        symbolCursorYPos = -1;
        symbolEndCursorXPos = -1;
        symbolEndCursorYPos = -1;
        return false;
    }

    bool widget_text_editor::find_string(std::string strTarget, bool searchDown, bool matchCase, bool startAtFront)
    {
        if( (int)strTarget.size() > 0 && has_content() )
        {
            int searchCursorXPos = 0;
            int searchCursorYPos = 0;
            std::size_t foundSubXPos;
            int foundSubYPos = 0;
            std::string strToSearch = "";
            if( searchDown)
            {
                //search down
                if( startAtFront==false)
                {
                    searchCursorXPos = cursorXPos;
                    searchCursorYPos = cursorYPos;
                }
                if( searchCursorYPos <0 || searchCursorYPos >= (int)listOfStrings.size() )
                {
                    searchCursorYPos = 0;
                }
                strToSearch = listOfStrings.at(searchCursorYPos);
                foundSubXPos= strToSearch.find(strTarget, searchCursorXPos);
                if( (int)foundSubXPos!=(int)std::string::npos)
                {
                    foundSubYPos = searchCursorYPos;
                    selectionCursorXPos = foundSubXPos;
                    cursorXPos = selectionEndCursorXPos = foundSubXPos+(int)strTarget.size();
                    cursorYPos = selectionCursorYPos = foundSubYPos;
                    selectionEndCursorYPos = foundSubYPos;
                    scroll_to_selection();
                    return true;
                }
                else
                {
                    for( int i = searchCursorYPos+1; i < (int)listOfStrings.size(); i++)
                    {
                        strToSearch = listOfStrings[i];
                        foundSubXPos= strToSearch.find(strTarget, 0);
                        if( (int)foundSubXPos!=(int)std::string::npos)
                        {
                            foundSubYPos = i;
                            selectionCursorXPos = foundSubXPos;
                            cursorXPos = selectionEndCursorXPos = foundSubXPos+(int)strTarget.size();
                            cursorYPos = selectionCursorYPos = foundSubYPos;
                            selectionEndCursorYPos = foundSubYPos;
                            scroll_to_selection();
                            return true;
                        }
                    }
                }
            }
            else
            {
                //search up
                if( startAtFront==false)
                {
                    searchCursorYPos = listOfStrings.size()-1;
                    searchCursorXPos = (listOfStrings.at(searchCursorYPos).size() );
                }
                else
                {
                    searchCursorXPos = cursorXPos;
                    searchCursorYPos = cursorYPos;
                }
            }
        }
        return false;
    }

    void widget_text_editor::find_documentation_description(int tCursorX, int tCursorY)
    {
        if( main_syntax_highlighter!=NULL )
        {
            main_syntax_highlighter->documentationIsBeingShown = false;
            main_syntax_highlighter->iSuggestedStartPos = 0;
            main_syntax_highlighter->iSuggestionPos = 0;
        }
        int  i = 0;

        if( tCursorY < 0)
        {
            tCursorY = cursorYPos;
        }

        if( tCursorX < 0)
        {
            tCursorX = cursorXPos;
        }

        if( tCursorY >=(int)listOfStrings.size() )
        {
            tCursorY = (int)listOfStrings.size() -1;
        }


        if( !main_syntax_highlighter->codeBeingSuggested && tCursorY >=0 && tCursorY < (int)listOfStrings.size() && isCodeEditor)
        {
            std::string currentStringInView = "";
            tempCLineXStartPos = 0;
            tempCLineXEndPos = tCursorX;
            //int tempCLineYPos = tCursorY;
            std::string cLineToParse = listOfStrings.at(tCursorY);
            syntax_compiler_term * tempTerm = NULL;
            highlightXPos = tCursorX;
            highlightYPos = tCursorY;
            if( tCursorX >=0 && (int)tCursorX <=(int)cLineToParse.size() && (int)cLineToParse.size() > 0)
            {
                if( tCursorX==(int)cLineToParse.size() )
                {
                    tempCLineXEndPos = tCursorX-1;
                }
                if( tempCLineXEndPos < 0)
                {
                    tempCLineXEndPos = 0;
                }
                //goes back one if character isn't alpha numeric
                if( !stg_ex::char_is_alnum(cLineToParse[tempCLineXEndPos],false,true) )
                {
                    if( (int)cLineToParse.size() > tempCLineXEndPos-1)
                    {
                        if( stg_ex::char_is_alnum(cLineToParse[tempCLineXEndPos-1],false,true) )
                        {
                            tempCLineXEndPos--;
                        }
                    }
                }
                //makes sure our subtext is alphanumeric without spaces
                if( stg_ex::char_is_alnum(cLineToParse[tempCLineXEndPos],false,true) )
                {
                    int formerEndLinePos = tempCLineXEndPos;
                    for( i = formerEndLinePos-1; i< (int)cLineToParse.size(); i++)
                    {
                        if( stg_ex::char_is_alnum(cLineToParse[i],false,true) ==false )
                        {
                            tempCLineXEndPos = i-1;
                            break;
                        }
                        else
                        {
                            tempCLineXEndPos = i;
                        }
                    }

                    for( i = tempCLineXEndPos-1; i >=0; i--)
                    {
                        if( stg_ex::char_is_alnum(cLineToParse[i],false,true) ==false )
                        {
                            tempCLineXStartPos = i+1;
                            break;
                        }
                        else
                        {
                            tempCLineXStartPos = i;
                        }
                    }
                    if( tempCLineXStartPos > lineStartXPos)
                    {
                        highlightXPos = tempCLineXStartPos;
                    }
                    //makes sure the length is long enough to check for
                    if(tempCLineXEndPos - tempCLineXStartPos >= 2)
                    {
                        currentStringInView = stg_ex::get_substring(cLineToParse, tempCLineXStartPos, tempCLineXEndPos-tempCLineXStartPos+1);

                        //Avoids rechecking if term is already being highlighted
                        if( main_syntax_highlighter->highlightedTerm!=NULL)
                        {
                            if( main_syntax_highlighter->highlightedTerm->termString == currentStringInView)
                            {
                                main_syntax_highlighter->documentationIsBeingShown = true;
                                return;
                            }
                            else
                            {
                                main_syntax_highlighter->highlightedTerm = NULL;
                            }
                        }

                        if( codeEditorType==0)
                        {
                            for( i = main_syntax_highlighter->activeProjectKeywords.size()-1; i>=0; i--)
                            {
                                tempTerm = main_syntax_highlighter->activeProjectKeywords[i];
                                if( tempTerm!=NULL)
                                {
                                    if( tempTerm->termString==currentStringInView)
                                    {
                                        main_syntax_highlighter->highlightedTerm = tempTerm;
                                        main_syntax_highlighter->documentationIsBeingShown = true;
                                        return;
                                    }
                                }
                            }

                            for( i = main_syntax_highlighter->activeProjectKeywords.size()-1; i>=0; i--)
                            {
                                tempTerm = main_syntax_highlighter->activeProjectKeywords[i];
                                if( tempTerm!=NULL)
                                {
                                    if( tempTerm->termString == currentStringInView)
                                    {
                                        main_syntax_highlighter->highlightedTerm = tempTerm;
                                        main_syntax_highlighter->documentationIsBeingShown = true;
                                        return;
                                    }
                                }
                            }
                            tempTerm = main_syntax_highlighter->find_matching_function( currentStringInView);
                            if( tempTerm!=NULL)
                            {
                                main_syntax_highlighter->highlightedTerm = tempTerm;
                                main_syntax_highlighter->documentationIsBeingShown = true;
                                return;
                            }

                            tempTerm = main_syntax_highlighter->find_matching_variable( currentStringInView);
                            if( tempTerm!=NULL)
                            {
                                main_syntax_highlighter->highlightedTerm = tempTerm;
                                main_syntax_highlighter->documentationIsBeingShown = true;
                                return;
                            }
                        }
                    }
                    else
                    {
                        main_syntax_highlighter->highlightedTerm = NULL;
                    }
                }
                else
                {
                    main_syntax_highlighter->highlightedTerm = NULL;
                }
            }
            else
            {
                main_syntax_highlighter->highlightedTerm = NULL;
            }
        }
        else
        {
            main_syntax_highlighter->highlightedTerm = NULL;
        }
    }

    void widget_text_editor::find_suggested_text()
    {
        //Avoids function if the main highlighter class is "somehow NULL"
        if( main_syntax_highlighter==NULL)
        {
            return;
        }

        main_syntax_highlighter->clear_suggestions();
        int  i = 0;
        if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
        {
            std::string currentStringInView = "";
            tempCLineXStartPos = 0;
            tempCLineXEndPos = cursorXPos;
            //int tempCLineYPos = cursorYPos;
            std::string cLineToParse = listOfStrings[cursorYPos];
            syntax_compiler_term * tempTerm = NULL;
            if( cursorXPos >=0 && (int)cursorXPos <=(int)cLineToParse.size() && (int)cLineToParse.size() > 0)
            {
                if( cursorXPos==(int)cLineToParse.size() )
                {
                    tempCLineXEndPos = cursorXPos-1;
                }
                if( tempCLineXEndPos < 0)
                {
                    tempCLineXEndPos = 0;
                }
                //goes back one if character isn't alpha numeric
                if( !stg_ex::char_is_alnum(cLineToParse[tempCLineXEndPos],false,true) )
                {
                    if( (int)cLineToParse.size() > tempCLineXEndPos-1)
                    {
                        if( stg_ex::char_is_alnum(cLineToParse[tempCLineXEndPos-1],false,true) )
                        {
                            tempCLineXEndPos--;
                        }
                    }
                }

                //makes sure our subtext is alphanumeric without spaces
                if( stg_ex::char_is_alnum(cLineToParse[tempCLineXEndPos],false,true) )
                {
                    for( i = tempCLineXEndPos-1; i >=0; i--)
                    {
                        if( stg_ex::char_is_alnum(cLineToParse[i],false,true) ==false )
                        {
                            tempCLineXStartPos = i+1;
                            break;
                        }
                    }

                    //makes sure the length is long enough to check for
                    if(tempCLineXEndPos - tempCLineXStartPos >= 2)
                    {
                        currentStringInView = cLineToParse.substr( tempCLineXStartPos, tempCLineXEndPos-tempCLineXStartPos+1);
                        //css suggestions
                        if( codeEditorType==1)
                        {

                        }
                        else if( codeEditorType==0)
                        {
                            //Project Level Functions and Keywords
                            for( i = main_syntax_highlighter->activeProjectFunctions.size()-1; i>=0; i--)
                            {
                                tempTerm = main_syntax_highlighter->activeProjectFunctions[i];
                                if( tempTerm!=NULL)
                                {
                                    if( stg_ex::string_starts(tempTerm->termString,currentStringInView) )
                                    {
                                        main_syntax_highlighter->suggestedCompilerTerms.push_back(tempTerm);
                                    }
                                }
                            }

                            for( i = main_syntax_highlighter->activeProjectKeywords.size()-1; i>=0; i--)
                            {
                                tempTerm = main_syntax_highlighter->activeProjectKeywords[i];
                                if( tempTerm!=NULL)
                                {
                                    if( stg_ex::string_starts(tempTerm->termString,currentStringInView) )
                                    {
                                        main_syntax_highlighter->suggestedCompilerTerms.push_back(tempTerm);
                                    }
                                }
                            }

                            //Language suggestions
                            if( main_syntax_highlighter!=NULL && main_syntax_highlighter->defaultLanguage!=NULL)
                            {
                                for( i = main_syntax_highlighter->defaultLanguage->languageConstants.size()-1; i>=0; i--)
                                {
                                    tempTerm = main_syntax_highlighter->defaultLanguage->languageConstants[i];
                                    if( tempTerm!=NULL)
                                    {
                                        if( stg_ex::string_starts(tempTerm->termString,currentStringInView) )
                                        {
                                            main_syntax_highlighter->suggestedCompilerTerms.push_back(tempTerm);
                                        }
                                    }
                                }

                                for( i = main_syntax_highlighter->defaultLanguage->languageFunctions.size()-1; i>=0; i--)
                                {
                                    tempTerm = main_syntax_highlighter->defaultLanguage->languageFunctions[i];
                                    if( tempTerm!=NULL)
                                    {
                                        if( stg_ex::string_starts(tempTerm->termString,currentStringInView) )
                                        {
                                            main_syntax_highlighter->suggestedCompilerTerms.push_back(tempTerm);
                                        }
                                    }
                                }

                                for( i = main_syntax_highlighter->defaultLanguage->languageKeywords.size()-1; i>=0; i--)
                                {
                                    tempTerm = main_syntax_highlighter->defaultLanguage->languageKeywords[i];
                                    if( tempTerm!=NULL)
                                    {
                                        if( stg_ex::string_starts(tempTerm->termString,currentStringInView) )
                                        {
                                            main_syntax_highlighter->suggestedCompilerTerms.push_back(tempTerm);
                                        }
                                    }
                                }

                                for( i = main_syntax_highlighter->defaultLanguage->languageVariables.size()-1; i>=0; i--)
                                {
                                    tempTerm = main_syntax_highlighter->defaultLanguage->languageVariables[i];
                                    if( tempTerm!=NULL)
                                    {
                                        if( stg_ex::string_starts(tempTerm->termString,currentStringInView) )
                                        {
                                            main_syntax_highlighter->suggestedCompilerTerms.push_back(tempTerm);
                                        }
                                    }
                                }
                            }
                        }
                        int tTerwindow_width = 0;
                        main_syntax_highlighter->maxSuggestedTextWidth = 0;
                        //soon add optional sorting [alphabetically] of these terms here....
                        for(  i = 0; i < (int)main_syntax_highlighter->suggestedCompilerTerms.size(); i++)
                        {
                            tempTerm = main_syntax_highlighter->suggestedCompilerTerms[i];
                            if( tempTerm!=NULL)
                            {
                                if( tempTerm->termType==cterm_function)
                                {
                                    tTerwindow_width = (int)tempTerm->termString.size()+(int)tempTerm->get_parameters().size()+(int)tempTerm->termFunctionReturnType.size()+3; // functionName(parameters)
                                }
                                else
                                {
                                    tTerwindow_width = (int)tempTerm ->termString.size();
                                }
                                //User Global
                                if( (int)tempTerm->termScope.size() > 0 && tempTerm->termScope!="User Global" )
                                {
                                    tTerwindow_width+=tempTerm->termScope.size()+1;
                                }

                                if( tTerwindow_width > main_syntax_highlighter->maxSuggestedTextWidth)
                                {
                                    main_syntax_highlighter->maxSuggestedTextWidth = tTerwindow_width;
                                }
                            }
                        }
                        TEXTBOX_FONT_SIZE_WIDTH = 12;
                        TEXTBOX_FONT_SIZE_HEIGHT = 12;

                        if( font_textinput!=NULL)
                        {
                            font_textinput->get_metrics("A",&TEXTBOX_FONT_SIZE_WIDTH,&TEXTBOX_FONT_SIZE_HEIGHT);
                        }
                        main_syntax_highlighter-> maxSuggestedTextWidth= 64+main_syntax_highlighter->maxSuggestedTextWidth*TEXTBOX_FONT_SIZE_WIDTH;
                    }
                }
            }
        }
        if( (int)main_syntax_highlighter->suggestedCompilerTerms.size() > 0)
        {
            main_syntax_highlighter->codeBeingSuggested = true;
        }
    }

    int widget_text_editor::get_xcursor()
    {
        return cursorXPos;
    }

    int widget_text_editor::get_ycursor()
    {
        return cursorYPos;
    }

    std::string widget_text_editor::get_string()
    {
        std::string returnString = "";
        for(int i=0; i < (int)listOfStrings.size(); i+=1)
        {
            returnString+=listOfStrings[i]+"\n";
        }
        return returnString;
    }

    std::string widget_text_editor::get_string_spaces()
    {
        std::string returnString = "";
        int listSize = (int)listOfStrings.size();
        for(int i=0; i < listSize; i+=1)
        {
            if( i ==listSize-1)
            {
                returnString+=listOfStrings[i]+" ";
            }
            else
            {
                returnString+=listOfStrings[i];
            }
        }
        return returnString;
    }

    int widget_text_editor::get_line_count()
    {
        return (int)listOfStrings.size();
    }

    std::string widget_text_editor::get_line_string(int lineNumberIn)
    {
        if( lineNumberIn >=0 && lineNumberIn < (int)listOfStrings.size() )
        {
            return listOfStrings.at(lineNumberIn);
        }
        return "";
    }

    std::string widget_text_editor::get_short_hightlighted()
    {
        if( selectionCursorXPos!=selectionEndCursorXPos || selectionCursorYPos!=selectionEndCursorYPos )
        {
            int yLineToCopy = std::min( selectionCursorYPos, selectionEndCursorYPos);
            if( (int)listOfStrings.size() > yLineToCopy)
            {
                int tempYlineLength = (int)listOfStrings[yLineToCopy].size();
                int minXPosToCopy = 0, maxXPosToCopy = 0;
                if(selectionCursorYPos==selectionEndCursorYPos)
                {
                    minXPosToCopy = std::min( selectionCursorXPos, selectionEndCursorXPos);
                    maxXPosToCopy = std::max( selectionCursorXPos, selectionEndCursorXPos);
                }
                else
                {
                    if( selectionCursorYPos > selectionEndCursorYPos)
                    {
                        minXPosToCopy = selectionEndCursorXPos;
                    }
                    else
                    {
                        minXPosToCopy = selectionCursorXPos;
                    }
                    maxXPosToCopy = tempYlineLength;
                }
                if( tempYlineLength > minXPosToCopy && tempYlineLength >= maxXPosToCopy )
                {
                    return listOfStrings[yLineToCopy].substr(minXPosToCopy,maxXPosToCopy-minXPosToCopy);
                }
            }
        }
        return "";
    }

    std::vector <std::string> widget_text_editor::get_all_strings()
    {
        return listOfStrings;
    }

    int widget_text_editor::get_most_anchors_characters()
    {
        int maxCharsUsed = 0;
        widget_text_anchor * tAnchor;
        for( int i = 0; i < (int)anchorPositions.size(); i++)
        {
            tAnchor = anchorPositions[i];
            if( tAnchor!=NULL)
            {
                if( (int)tAnchor->lineMessage.size() > maxCharsUsed )
                {
                    maxCharsUsed = (int)tAnchor->lineMessage.size();
                }
            }
        }
        return maxCharsUsed;
    }

    int widget_text_editor::get_most_characters_used()
    {
        int maxCharsUsed = 0;
        for( int i = 0; i < (int)listOfStrings.size(); i++)
        {
            if( (int)listOfStrings[i].size() > maxCharsUsed)
            {
                maxCharsUsed = (int)listOfStrings[i].size();
            }
        }
        return maxCharsUsed;
    }

    void widget_text_editor::handle_scrolling( gpe::shape_rect * view_space, gpe::shape_rect * cam  )
    {
        view_space = gpe::camera_find( view_space );
        cam = gpe::camera_find( cam );

        charactersWithinView = renderBox->w/( ( float)TEXTBOX_FONT_SIZE_WIDTH);
        linesWithinView = renderBox->h/( (float) defaultLineHeight*editorZoomLevel  );

        showXScroll = false;
        showYScroll = false;

        int listSize = (int)listOfStrings.size();
        if( linesWithinView <  listSize )
        {
            showYScroll = true;
        }

        int mostChractersUsed  = get_most_characters_used();
        if( charactersWithinView <  mostChractersUsed )
        {
            showXScroll = true;
        }
        else
        {
            lineStartXPos = 0;
            textXScroll->reset_scroller();
        }

        //Handles X-Axis
        if( textXScroll!=NULL)
        {
            //int prevYPos = lineStartYPos;
            textXScroll->set_coords( renderBox->x,renderBox->y+renderBox->h - textXScroll->get_height()  );
            textXScroll->set_width( renderBox->w-lineCountBoxWidth );

            textXScroll->fullRect.x = 0;
            textXScroll->fullRect.y = 0;
            textXScroll->fullRect.w = (float)mostChractersUsed;
            textXScroll->fullRect.h = renderBox->h;

            textXScroll->contextRect.x = (float)lineStartXPos;
            textXScroll->contextRect.w = (float)charactersWithinView;

            textXScroll->process_self(view_space,cam );
            if( textXScroll->has_moved() || textXScroll->is_scrolling() )
            {
                lineStartXPos = mostChractersUsed * ( (float)textXScroll->scrollXPos/(float)textXScroll->get_width() );
            }
        }
        else
        {
            lineStartXPos = 0;
            textXScroll->reset_scroller();
        }

        //Handles Y-Axis
        if( textYScroll!=NULL)
        {
            if( showYScroll )
            {
                //int prevYPos = lineStartYPos;

                textYScroll->set_coords( renderBox->x+renderBox->w - textYScroll->get_width(), renderBox->y  );
                textYScroll->set_height( renderBox->h );

                textYScroll->fullRect.x = 0;
                textYScroll->fullRect.y = 0;
                textYScroll->fullRect.w = renderBox->w;
                textYScroll->fullRect.h = (float)listSize;

                textYScroll->contextRect.y = (float)lineStartYPos;
                textYScroll->contextRect.h = (float)linesWithinView;

                textYScroll->process_self(view_space,cam );
                if( textYScroll->has_moved() )
                {
                    lineStartYPos =  round ( textYScroll->contextRect.y );
                    //    lineStartYPos =  ceil( ( (float)listOfStrings.size() ) * ( (float)textYScroll->scrollYPos/(float)textYScroll->widget_box.h ) );
                }
            }
            else
            {
                lineStartYPos = 0;
                textYScroll->reset_scroller();
            }
        }

        if( lineStartXPos < 0)
        {
            lineStartXPos = 0;
        }
        if( lineStartYPos < 0)
        {
            lineStartYPos = 0;
        }
    }

    bool widget_text_editor::has_content()
    {
        if((int)listOfStrings.size() > 1)
        {
            return true;
        }
        else if((int)listOfStrings.size()==1 )
        {
            std::string firstLine = listOfStrings[0];
            if( (int)firstLine.size() > 0)
            {
                return true;
            }
        }
        return false;
    }

    bool widget_text_editor::has_selection()
    {
        if( selectionCursorXPos!=selectionEndCursorXPos || selectionCursorYPos!=selectionEndCursorYPos )
        {
            return true;
        }
        return false;
    }

    bool widget_text_editor::import_text(std::string new_file_name)
    {
        if( !new_file_name.empty() )
        {
            std::ifstream newTxtFile( new_file_name.c_str() );
            //If the level file could be loaded
            if( !newTxtFile.fail() )
            {
                //makes sure the file is open
                if (newTxtFile.is_open())
                {
                    std::string currLine = "";
                    clear_all_lines();
                    while ( newTxtFile.good() )
                    {
                        getline (newTxtFile,currLine); //gets the next line of the file
                        currLine = stg_ex::trim_right_inplace(currLine);
                        currLine = stg_ex::string_replace_all(currLine,"\t","   ");
                        if(!currLine.empty() )
                        {
                            listOfStrings.push_back(currLine);
                        }
                        else
                        {
                            listOfStrings.push_back("");
                        }
                    }
                    newTxtFile.close();
                    save_edit();
                    return true;
                }
            }
        }
        gpe::error_log->report("Unable to import text from ["+new_file_name+"] file." );
        return false;
    }

    void widget_text_editor::init_save_history()
    {
        widget_text_editor * tempText;
        for( int iD = (int)savedHistory.size()-1; iD >=0; iD--)
        {
            tempText = savedHistory.at(iD);
            if( tempText!=NULL)
            {
                delete tempText;
                tempText = NULL;
            }
        }
        savedHistory.clear();
        currentPositionInHistory = 0;
        save_edit();
    }

    bool widget_text_editor::is_read_only()
    {
        return isReadOnly;
    }

    void widget_text_editor::log_editable_action()
    {
        if( undoableActionOccurred==false)
        {
            time(&lastUndoableActionTime);
            undoableActionOccurred = true;
        }
    }

    void widget_text_editor::move_down(int yToMove )
    {
        if( yToMove > 0)
        {
            lineStartYPos+=yToMove;
            if( lineStartYPos > (int)listOfStrings.size()-linesWithinView )
            {
                lineStartYPos = (int)listOfStrings.size()-linesWithinView;
            }
            find_documentation_description();
        }
    }

    void widget_text_editor::move_up(int yToMove )
    {
        if( yToMove > 0)
        {
            lineStartYPos-=yToMove;
            if( lineStartYPos < 0)
            {
                lineStartYPos = 0;
            }
            find_documentation_description();
        }
    }

    void widget_text_editor::move_right(int xToMove )
    {
        if( xToMove > 0)
        {
            lineStartXPos+=xToMove;
            if( lineStartXPos >= get_most_characters_used()-charactersWithinView )
            {
                lineStartXPos = get_most_characters_used()-charactersWithinView;
            }
            find_documentation_description();
        }
    }

    void widget_text_editor::move_left(int xToMove )
    {
        if( xToMove > 0)
        {
            lineStartXPos-=xToMove;
            if( lineStartXPos < 0 )
            {
                lineStartXPos = 0;
            }
            find_documentation_description();
        }
    }

    void widget_text_editor::paste_clipboard()
    {
        if(!isReadOnly)
        {
            std::string clipboardStr = gpe::input->clipboard_string();
            if( clipboardStr.size() > 0)
            {
                clipboardStr = stg_ex::string_replace_all(clipboardStr,"\r\n","\n");
                clipboardStr = stg_ex::string_replace_all(clipboardStr,"\r","\n");
                clipboardStr = stg_ex::string_replace_all(clipboardStr,"\t","    ");
                if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
                {
                    lineToEdit = listOfStrings[cursorYPos];
                    if( cursorXPos >(int)lineToEdit.size() )
                    {
                        cursorXPos = (int)lineToEdit.size() - 1;
                    }
                    if( cursorXPos < 0)
                    {
                        cursorXPos = 0;
                    }
                    int newLinePos=clipboardStr.find_first_of("\n");
                    if(newLinePos!=(int)std::string::npos)
                    {
                        std::string cbNLString = stg_ex::split_first_string(clipboardStr,'\n');
                        if( (int)lineToEdit.size() >= cursorXPos)
                        {
                            textInputString = stg_ex::get_substring(lineToEdit,0,cursorXPos);
                        }
                        else
                        {
                            textInputString= "";
                        }
                        textInputString+=cbNLString;
                        std::string remainerOfStr="";
                        if( (int)lineToEdit.size() > cursorXPos )
                        {
                            remainerOfStr = stg_ex::get_substring(lineToEdit,cursorXPos);
                        }
                        listOfStrings.erase(listOfStrings.begin()+cursorYPos);
                        listOfStrings.insert(listOfStrings.begin()+cursorYPos,textInputString);
                        cursorYPos+=1;
                        newLinePos=clipboardStr.find_first_of('\n');
                        while (newLinePos!=(int)std::string::npos)
                        {
                            cbNLString = stg_ex::split_first_string(clipboardStr,'\n');
                            listOfStrings.insert(listOfStrings.begin()+cursorYPos,cbNLString);
                            newLinePos=clipboardStr.find_first_of("\n");
                            cursorYPos+=1;
                        }
                        textInputString = clipboardStr;
                        textInputString+=remainerOfStr;
                        listOfStrings.insert(listOfStrings.begin()+cursorYPos,textInputString);
                        cursorXPos=(int)clipboardStr.size();
                    }
                    else
                    {
                        if( (int)lineToEdit.size() >= cursorXPos)
                        {
                            textInputString = stg_ex::get_substring(lineToEdit,0,cursorXPos);
                        }
                        else
                        {
                            textInputString = "";
                        }
                        textInputString+=clipboardStr;
                        if( (int)lineToEdit.size() >= cursorXPos)
                        {
                            textInputString+=stg_ex::get_substring(lineToEdit,cursorXPos);
                        }
                        cursorXPos+=(int)clipboardStr.size();
                        listOfStrings.erase(listOfStrings.begin()+cursorYPos);
                        listOfStrings.insert(listOfStrings.begin()+cursorYPos,textInputString);
                    }
                    save_edit();
                }
            }
        }
    }

    bool widget_text_editor::parse_code_javascript()
    {
        //resets symbol information
        missingSymbolDetected = false;


        if( has_content() )
        {
            foundSyntaxErrors.clear();
            missingSymbolStartXPos = 0;
            missingSymbolStartYPos = 0;
            misingSymbolSuggestedEndYPos = 0;
            misingSymbolSuggestedEndXPos = 0;

            int i = 0;
            std::string currStringToRender = "";
            std::string currentLineInView = "";

            int tempSynStringSize = 0;
            int currPosToParse = 0, lineEnd = 0;
            bool isInBlockCommentMode = false;
            bool isInfloatQuoteMode = false;
            bool isInSingleQuoteMode = false;
            int maxLine = (int)listOfStrings.size();
            int endBlockCommentPos = 0;
            int endDQuoteCommentPos = 0;
            int endSQuoteCommentPos = 0;
            bool commentFoundInSymbols = false;
            int openBracesCount = 0;
            int openBracketsCount = 0;
            int openParenthesisCount = 0;
            for( i=0; i < maxLine; i++)
            {
                currStringToRender = listOfStrings[i];
                currPosToParse = 0;
                openParenthesisCount = 0;
                lineEnd = (int)currStringToRender.size();

                endBlockCommentPos=(int)std::string::npos;
                endDQuoteCommentPos=(int)std::string::npos;
                endSQuoteCommentPos=(int)std::string::npos;
                while (currPosToParse < lineEnd)
                {
                    if( isInBlockCommentMode)
                    {
                        endBlockCommentPos = currStringToRender.find("*/");
                        if( endBlockCommentPos==(int)std::string::npos)
                        {
                            currPosToParse = lineEnd;
                        }
                        else
                        {
                            currPosToParse = endBlockCommentPos+2;
                            isInBlockCommentMode = false;
                        }
                    }
                    else if( isInfloatQuoteMode)
                    {
                        endDQuoteCommentPos = currStringToRender.find('"',currPosToParse);
                        if( endDQuoteCommentPos==(int)std::string::npos)
                        {
                            currPosToParse = lineEnd;
                        }
                        else
                        {
                            currPosToParse = endDQuoteCommentPos+1;
                            isInfloatQuoteMode = false;
                        }
                    }
                    else if( isInSingleQuoteMode)
                    {
                        endSQuoteCommentPos = currStringToRender.find("'",currPosToParse);
                        if( endSQuoteCommentPos==(int)std::string::npos)
                        {
                            currPosToParse = lineEnd;
                        }
                        else
                        {
                            currPosToParse = endSQuoteCommentPos+1;
                            isInSingleQuoteMode = false;
                        }
                    }
                    else
                    {
                        while (currPosToParse < lineEnd && currStringToRender[currPosToParse]==' ')
                        {
                            currPosToParse++;
                        }
                        if (currPosToParse < lineEnd)
                        {
                            if (currPosToParse+1<lineEnd && currStringToRender[currPosToParse] == '/' && currPosToParse + 1 < lineEnd && currStringToRender[currPosToParse + 1] == '/' )
                            {
                                currPosToParse = lineEnd;
                            }
                            else if (currPosToParse+1<lineEnd && currStringToRender[currPosToParse] == '/' && currStringToRender[currPosToParse + 1] == '*' )
                            {
                                isInBlockCommentMode = true;
                                currPosToParse+=2;
                            }
                            else if(currStringToRender[currPosToParse] == '"')
                            {
                                isInfloatQuoteMode = true;
                                currPosToParse++;
                            }
                            else if(currStringToRender[currPosToParse] == '\'')
                            {
                                isInSingleQuoteMode = true;
                                currPosToParse++;
                            }
                            else if(currStringToRender[currPosToParse] == '(')
                            {
                                openParenthesisCount++;
                                currPosToParse++;
                            }
                            else if(currStringToRender[currPosToParse] == '[')
                            {
                                openBracesCount++;
                                currPosToParse++;
                            }
                            else if(currStringToRender[currPosToParse] == '{')
                            {
                                openBracketsCount++;
                                currPosToParse++;
                            }
                            else if(currStringToRender[currPosToParse] == ')')
                            {
                                if( isInfloatQuoteMode==false && isInSingleQuoteMode==false && isInBlockCommentMode==false)
                                {
                                    if( openParenthesisCount > 0)
                                    {
                                        openParenthesisCount--;
                                    }
                                    else
                                    {
                                        missingSymbolDetected= true;
                                        foundSyntaxErrors.push_back( "Invalid ')' found on line "+ stg_ex::int_to_string(i+1)+ " @ "+ stg_ex::int_to_string(currPosToParse)+" ParentCount ["+ stg_ex::int_to_string(openParenthesisCount)+"].");
                                    }
                                }
                                currPosToParse++;
                            }
                            else if(currStringToRender[currPosToParse] == ']')
                            {
                                if( openBracesCount > 0)
                                {
                                    openBracesCount--;
                                }
                                else
                                {
                                    missingSymbolDetected= true;
                                    foundSyntaxErrors.push_back("Invalid ']' found on line "+ stg_ex::int_to_string(i+1)+" @ "+ stg_ex::int_to_string(currPosToParse)+" BracesCount ["+ stg_ex::int_to_string(openBracesCount)+"].");
                                }
                                currPosToParse++;
                            }
                            else if(currStringToRender[currPosToParse] == '}')
                            {
                                if( openBracketsCount > 0)
                                {
                                    openBracketsCount--;
                                }
                                else
                                {
                                    missingSymbolDetected= true;
                                    foundSyntaxErrors.push_back("Invalid '}' found on line "+ stg_ex::int_to_string(i+1)+" @ "+ stg_ex::int_to_string(currPosToParse)+" BracketsCount ["+ stg_ex::int_to_string(openBracketsCount)+"].");
                                }
                                currPosToParse++;
                            }
                            else if (isdigit(currStringToRender[currPosToParse]) )
                            {
                                currPosToParse++;
                                while( (currPosToParse < lineEnd && isdigit( currStringToRender[currPosToParse] )  ) || currStringToRender[currPosToParse] ==' ')
                                {
                                    currPosToParse++;
                                }
                            }
                            /*
                            else if ( main_syntax_highlighter->charIsSymbol(currStringToRender[currPosToParse]) )
                            {
                                commentFoundInSymbols = false;
                                while( ( commentFoundInSymbols==false && currPosToParse < lineEnd && main_syntax_highlighter->charIsSymbol(currStringToRender[currPosToParse] ) )|| currStringToRender[currPosToParse] ==' ')
                                {
                                    if( lineEnd > currPosToParse+1)
                                    {
                                        if( currStringToRender[currPosToParse] == '/' && (currStringToRender[currPosToParse + 1] == '/' || currStringToRender[currPosToParse + 1] == '*' ) )
                                        {
                                            commentFoundInSymbols = true;
                                        }
                                        else if (currStringToRender[currPosToParse] == '*' && currStringToRender[currPosToParse + 1] == '/' )
                                        {
                                            commentFoundInSymbols = true;
                                        }
                                        else
                                        {
                                            currPosToParse++;
                                        }
                                    }
                                    else
                                    {
                                        currPosToParse++;
                                    }
                                }
                                currPosToParse++;
                            }
                            */
                            else if ( stg_ex::char_is_alpha(currStringToRender[currPosToParse],true,true) )
                            {
                                //color = pawgui::theme_main->text_box_font_color;
                                currPosToParse++;
                                while (currPosToParse < lineEnd && stg_ex::char_is_alnum(currStringToRender[currPosToParse],true,true) )
                                {
                                    currPosToParse++;
                                }
                            }
                            else
                            {
                                //color = pawgui::theme_main->text_box_font_color;
                                //anything else is just regular text as well...
                                currPosToParse++;
                            }
                        }
                    }
                }
            }

            if( isInBlockCommentMode)
            {
                missingSymbolDetected = true;
                foundSyntaxErrors.push_back( "Missing Symbol: [*/] - End Block Comment" );
            }
            else if( isInfloatQuoteMode)
            {
                missingSymbolDetected = true;
                foundSyntaxErrors.push_back("\"/ - End Quote" );
            }
            else if( isInSingleQuoteMode)
            {
                missingSymbolDetected = true;
                foundSyntaxErrors.push_back( "' - End Single Quote" );
            }
        }
        return !missingSymbolDetected;
    }

    void widget_text_editor::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        hasScrollControl = false;
        prevCursorXPos = cursorXPos;
        prevCursorYPos = cursorYPos;
        setup_editor(view_space,cam);
        widget_basic::process_self(view_space,cam);
        handle_scrolling(view_space,cam);

        if( clickedOutside)
        {
            hasArrowkeyControl = false;
            hasScrollControl = false;
            gpe::input->inputted_keys = "";
        }
        else if( isClicked)
        {
            hasArrowkeyControl = true;
            hasScrollControl = true;
            isInUse = true;
            gpe::input->inputted_keys = "";
        }

        if( isInUse)
        {
            hasArrowkeyControl = true;
            hasScrollControl = true;
        }

        if( (int)listOfStrings.size()==0)
        {
            listOfStrings.push_back("");
            reset_self();
        }

        bool pasteCommandGiven = false;
        bool mouseHoveringInTextArea = false;
        bool buttonBarClicked = false;

        if( !isReadOnly && textEditorButtonBar!=NULL && showButtonBar )
        {
            textEditorButtonBar->set_coords(widget_box.x,widget_box.y);
            //sets the buttonbar to the width of the text editor( minus width of yScroll width[16 ).
            //textEditorButtonBar->set_width(widget_box.w);
            textEditorButtonBar->set_height(24);
            textEditorButtonBar->process_self(view_space,cam);
            buttonBarClicked = textEditorButtonBar->is_clicked();
            if( textEditorButtonBar->selectedOption>= 0 && textEditorButtonBar->selectedOption < TEXTAREA_OPTION_MAX_OPTIONS)
            {
                if( textEditorButtonBar->selectedOption==TEXTAREA_OPTION_IMPORT)
                {
                    std::string importTextFileName = get_filename_open_from_popup("Import Text","",main_settings->fileOpenTextFileDir );
                    if( (int)importTextFileName.size() > 0)
                    {
                        //if( display_prompt_message("Warning!","Clearing this text area is irreversible. Are you sure you want to continue this operation?")==display_query_yes )
                        {
                            import_text( importTextFileName);
                        }
                    }
                }
                else if(textEditorButtonBar->selectedOption ==TEXTAREA_OPTION_REDO)
                {
                    if( can_redo() )
                    {
                        redo_edit();
                    }
                }
                else if(textEditorButtonBar->selectedOption ==TEXTAREA_OPTION_UNDO)
                {
                    if( can_undo() )
                    {
                        undo_edit();
                    }
                }
                else if( textEditorButtonBar->selectedOption==TEXTAREA_OPTION_EXPORT)
                {
                    std::string exportTextFileName = get_filename_save_from_popup("Export Text","",main_settings->fileOpenFunctionDir);
                    if( sff_ex::file_exists(exportTextFileName) )
                    {
                        if( display_prompt_message("Warning!","File Exists already, do you wish to overwrite it?)")==display_query_yes )
                        {
                            export_text( exportTextFileName);
                        }
                    }
                    else
                    {
                        export_text( exportTextFileName);
                    }
                }
                else if( textEditorButtonBar->selectedOption==TEXTAREA_OPTION_CLEAR)
                {
                    if( display_prompt_message("Warning!","Are you sure you want to continue this operation?")==display_query_yes )
                    {
                        clear_all_lines();
                        listOfStrings.push_back("");
                        save_edit();
                    }
                }
                else if( textEditorButtonBar->selectedOption==TEXTAREA_OPTION_COPY)
                {
                    copy_selection();
                }
                else if( textEditorButtonBar->selectedOption==TEXTAREA_OPTION_CUT)
                {
                    cut_selection();
                }
                else if( textEditorButtonBar->selectedOption==TEXTAREA_OPTION_PASTE)
                {
                    pasteCommandGiven = true;
                }
            }
        }
        else if( textEditorButtonBar!=NULL)
        {
            textEditorButtonBar->set_coords(widget_box.x,widget_box.y);

            textEditorButtonBar->set_height(0);
        }
        if( !buttonBarClicked && (int)listOfStrings.size() >0 )
        {
            textInputString = "";
            cursorTimer+=gpe::time_keeper->get_delta_ticks();

            if( cursorTimer > main_settings->cursorBlinkTime )
            {
                showCursor = !showCursor;
                cursorTimer = 0;
            }

            if( isClicked )
            {
                isInUse = true;
                //inputFieldPos = 0;
                gpe::input->inputted_keys = "";
            }
            else if( clickedOutside )
            {
                isInUse = false;
            }


            if( isHovered && (showXScroll || showYScroll ) )
            {
                hasScrollControl = true;
            }

            if( isEnabled &&  isInUse && cam!=NULL && textEditorButtonBar!=NULL)
            {
                if( gpe::point_within_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y, textSpaceRect)  )
                {
                    mouseHoveringInTextArea = true;
                    gpe::cursor_main_controller->cursor_change("ibeam");
                }

                if( mouseHoveringInTextArea && textXScroll->is_scrolling()==false && textYScroll->is_scrolling()==false )
                {
                    if( gpe::input->check_mouse_down( mb_left ) )
                    {
                        update_cursor_to_mouse(view_space, cam);
                        if( lineStartXPos < 0)
                        {
                            lineStartXPos = 0;
                        }
                        if( lineStartYPos < 0)
                        {
                            lineStartYPos = 0;
                        }
                    }
                    //Handles the Mouse movements & buttons
                    if( gpe::input->check_mouse_button_clicked(0) && resource_dragged==NULL )
                    {
                        update_cursor_to_mouse(view_space, cam);
                        if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
                        {
                            lineToEdit = listOfStrings[cursorYPos];

                            selectionCursorXPos = selectionEndCursorXPos = cursorXPos;
                            selectionCursorYPos = selectionEndCursorYPos = cursorYPos;
                            if( (int)lineToEdit.size() == 0 )
                            {
                                if( cursorYPos+1 < (int)listOfStrings.size() )
                                {
                                    cursorXPos = 0;
                                    cursorYPos++;
                                    selectionEndCursorYPos = cursorYPos;
                                    selectionCursorXPos = 0;
                                    selectionEndCursorXPos = 0;
                                }
                            }
                            else
                            {
                                int iPrev = cursorXPos;
                                int jNext = cursorXPos;
                                if( lastfloatClickAction ==0)
                                {
                                    if( cursorXPos>=0 &&  cursorXPos <= (int)lineToEdit.size() )
                                    {
                                        if( lineToEdit[cursorXPos]==' ')
                                        {
                                            for(iPrev = cursorXPos-1; iPrev >=0; iPrev-- )
                                            {
                                                if( lineToEdit[iPrev]==' ')
                                                {
                                                    selectionCursorXPos--;
                                                }
                                                else
                                                {
                                                    break;
                                                }
                                            }
                                            for(jNext = cursorXPos; jNext < (int)lineToEdit.size(); jNext++ )
                                            {
                                                if( lineToEdit[jNext]==' ')
                                                {
                                                    selectionEndCursorXPos++;
                                                }
                                                else
                                                {
                                                    break;
                                                }
                                            }
                                        }
                                        else if( stg_ex::char_is_alnum( lineToEdit[cursorXPos],false,true) )
                                        {
                                            for(iPrev = cursorXPos-1; iPrev >=0; iPrev-- )
                                            {
                                                if( stg_ex::char_is_alnum( lineToEdit[iPrev],false,true) )
                                                {
                                                    selectionCursorXPos--;
                                                }
                                                else
                                                {
                                                    break;
                                                }
                                            }

                                            for(jNext = cursorXPos; jNext < (int)lineToEdit.size(); jNext++ )
                                            {
                                                if( stg_ex::char_is_alnum( lineToEdit[jNext],false,true) )
                                                {
                                                    selectionEndCursorXPos++;
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
                                            for(iPrev = cursorXPos-1; iPrev >=0; iPrev-- )
                                            {
                                                if( stg_ex::char_is_alnum( lineToEdit[iPrev],true,true)==false)
                                                {
                                                    selectionCursorXPos--;
                                                }
                                                else
                                                {
                                                    break;
                                                }
                                            }
                                            for(jNext = cursorXPos; jNext < (int)lineToEdit.size(); jNext++ )
                                            {
                                                if( stg_ex::char_is_alnum( lineToEdit[jNext],true,true)==false)
                                                {
                                                    selectionEndCursorXPos++;
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
                                else if( (int)lineToEdit.size() > 0 )
                                {
                                    selectionCursorXPos = 0;
                                    selectionEndCursorXPos = (int)lineToEdit.size();
                                    cursorXPos = 0;
                                    lastfloatClickAction = 0;
                                }
                                else if( cursorYPos+1 < (int)listOfStrings.size() )
                                {
                                    cursorXPos = 0;
                                    cursorYPos++;
                                    selectionEndCursorYPos = cursorYPos;
                                    selectionCursorXPos = 0;
                                    selectionEndCursorXPos = 0;
                                }
                                gpe::input->reset_all_input();
                                find_documentation_description();
                            }
                        }
                    }
                    else if( gpe::input->check_mouse_pressed(0) )
                    {
                        //if( lastfloatClickAction==0)
                        {
                            update_cursor_to_mouse(view_space, cam);
                            if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
                            {
                                selectionCursorXPos = selectionEndCursorXPos = cursorXPos;
                                selectionCursorYPos = selectionEndCursorYPos = cursorYPos;
                            }
                        }
                        find_documentation_description();
                    }
                    else if( gpe::input->check_mouse_down( mb_left ) )
                    {
                        //if( lastfloatClickAction==0)
                        {
                            update_cursor_to_mouse(view_space, cam);
                            if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
                            {
                                selectionEndCursorXPos = cursorXPos;
                                selectionEndCursorYPos = cursorYPos;
                            }
                        }
                        find_documentation_description();
                    }
                    else if( gpe::input->check_mouse_down( mb_right ))
                    {
                        context_menu_open(-1,-1,128);
                        main_context_menu->add_menu_option("Undo",0,rsm_gui->texture_add("backButtonIcon", gpe::app_directory_name+"resources/buttons/backward.png"),-1,NULL,true,!isReadOnly && can_undo());
                        main_context_menu->add_menu_option("Redo",1,rsm_gui->texture_add("forwardButtonIcon", gpe::app_directory_name+"resources/buttons/forward.png"),-1,NULL,true,!isReadOnly && can_redo() );
                        main_context_menu->add_menu_option("Cut",2,rsm_gui->texture_add("cutButtonIcon", gpe::app_directory_name+"resources/buttons/cut.png"),-1,NULL,false,!isReadOnly);
                        main_context_menu->add_menu_option("Copy",3,rsm_gui->texture_add("copyButtonIcon", gpe::app_directory_name+"resources/buttons/copy.png"),-1,NULL,false,true);
                        main_context_menu->add_menu_option("Paste",4,rsm_gui->texture_add("pasteButtonIcon", gpe::app_directory_name+"resources/buttons/paste.png"),-1,NULL,false,!isReadOnly);
                        main_context_menu->add_menu_option("Delete",5,rsm_gui->texture_add("removeButtonIcon", gpe::app_directory_name+"resources/buttons/remove.png"),-1,NULL,true,!isReadOnly);
                        main_context_menu->add_menu_option("Select All",6,rsm_gui->texture_add("stickyButtonIcon", gpe::app_directory_name+"resources/buttons/sticky-note.png"),-1,NULL,true,true);
                        int menuSelection = context_menu_process();

                        if( menuSelection==0)
                        {
                            if( can_undo() && !isReadOnly )
                            {
                                undo_edit();
                            }
                        }
                        else if( menuSelection==1 && !isReadOnly )
                        {
                            if( can_redo() )
                            {
                                redo_edit();
                            }
                        }
                        if( menuSelection==6)
                        {
                            select_all();
                        }
                        else
                        {
                            switch(menuSelection)
                            {
                            case 2:
                                cut_selection();
                                break;
                            case 3:
                                copy_selection();
                                break;
                            case 4:
                                pasteCommandGiven = true;
                                break;
                            case 5:
                                delete_selection();
                                break;
                            default:
                                break;
                            }
                            reset_selection();
                        }
                        lastfloatClickAction = 0;
                        main_syntax_highlighter->highlightedTerm = NULL;
                    }
                    else if( gpe::input->mouse_movement_received && resource_dragged==NULL )
                    {
                        //Highlights documenation under mouse if found.
                        int tMouseX = 0, tMouseY = 0;
                        find_mouse_cursor(&tMouseX,&tMouseY, view_space, cam);
                        if( tMouseX!=highlightXPos || tMouseY!=highlightYPos)
                        {
                            find_documentation_description(tMouseX,tMouseY);
                        }
                    }
                    else if( resource_dragged!=NULL && !isReadOnly )
                    {
                        if( gpe::input->check_mouse_released( mb_left))
                        {
                            if( gpe::point_within_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y, textSpaceRect)  )
                            {
                                mouseHoveringInTextArea = true;
                                gpe::cursor_main_controller->cursor_change("ibeam");

                                update_cursor_to_mouse(view_space, cam);
                                if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
                                {
                                    if( cursorXPos >=0 && cursorXPos <= (int)listOfStrings[cursorYPos].size() )
                                    {

                                        listOfStrings[cursorYPos] = stg_ex::get_substring(listOfStrings[cursorYPos],0,cursorXPos)+resource_dragged->get_name()+stg_ex::get_substring(listOfStrings[cursorYPos],cursorXPos);
                                        resource_dragged = NULL;
                                    }
                                }
                            }
                            else
                            {
                                gpe::error_log->report("Unable to drag resource into textArea...");
                            }
                        }
                    }
                }
                else if( gpe::input->check_mouse_down( mb_left ) && textXScroll->is_scrolling()==false && textYScroll->is_scrolling()==false )
                {
                    //if( lastfloatClickAction==0)
                    if( gpe::input->mouse_position_x < textSpaceRect->x )
                    {
                        if( cursorXPos > 0)
                        {
                            cursorXPos-=1;
                        }
                        move_left(1);
                    }
                    else if( gpe::input->mouse_position_x > textSpaceRect->x+ textSpaceRect->w )
                    {
                        lineToEdit = listOfStrings[cursorYPos];
                        if( cursorXPos > (int)lineToEdit.size()-1 )
                        {
                            cursorXPos+=1;
                        }
                        move_right(1);
                    }

                    if( gpe::input->mouse_position_y > textSpaceRect->y && gpe::input->mouse_position_y < textSpaceRect->y+4 )
                    {
                        if( cursorYPos > 0)
                        {
                            cursorYPos-=1;
                        }
                        move_up(1);
                    }
                    else if( gpe::input->mouse_position_y > textSpaceRect->y+textSpaceRect->h-4 )
                    {
                        if( cursorYPos < (int)listOfStrings.size()-1 )
                        {
                            cursorYPos+=1;
                        }
                        move_down(1);
                    }

                    update_cursor_to_mouse(view_space, cam);
                    if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
                    {
                        selectionEndCursorXPos = cursorXPos;
                        selectionEndCursorYPos = cursorYPos;
                    }
                    showCursor = false;
                    cursorTimer = 30;
                    find_documentation_description();
                }
            }

            //special control action happening
            if( isEnabled &&   main_search_controller->using_search()==false && isInUse  )
            {
                //used to delay events from happening superfast
                if( gpe::input->kb_button_down[kb_backspace] )
                {
                    bscDelay +=gpe::time_keeper->get_delta_ticks();
                }
                else
                {
                    bscDelay = 0;
                }
                if( gpe::input->kb_button_down[kb_delete] )
                {
                    delDelay += gpe::time_keeper->get_delta_ticks();
                }
                else
                {
                    delDelay = 0;
                }
                if( gpe::input->kb_button_down[kb_tab] )
                {
                    tabDelay += gpe::time_keeper->get_delta_ticks();
                }
                if(gpe::input->kb_button_down[kb_enter] )
                {
                    enterDelay+=gpe::time_keeper->get_delta_ticks();
                }
                else
                {
                    enterDelay = 0;
                }
                if( gpe::input->kb_button_down[kb_left] )
                {
                    leftDelay+=gpe::time_keeper->get_delta_ticks();
                }
                else
                {
                    leftDelay = 0;
                }

                if( gpe::input->kb_button_down[kb_right]  )
                {
                    rightDelay+=gpe::time_keeper->get_delta_ticks();
                }
                else
                {
                    rightDelay = 0;
                }

                if(gpe::input->kb_button_down[kb_up]  )
                {
                    upDelay+=gpe::time_keeper->get_delta_ticks();
                }
                else
                {
                    upDelay = 0;
                }
                if(gpe::input->kb_button_down[kb_down] )
                {
                    downDelay+=gpe::time_keeper->get_delta_ticks();
                }
                else
                {
                    downDelay = 0;
                }

                if(gpe::input->kb_button_down[kb_d] )
                {
                    dKeyDelay+=gpe::time_keeper->get_delta_ticks();
                }
                else
                {
                    dKeyDelay = 0;
                }

                if( gpe::input->kb_button_down[kb_ctrl]  )
                {
                    main_syntax_highlighter->highlightedTerm = NULL;
                    if( gpe::input->kb_button_released[kb_a])
                    {
                        select_all();
                    }
                    else if( gpe::input->kb_button_released[kb_c])
                    {
                        copy_selection();
                    }
                    else if( gpe::input->kb_button_released[kb_f] ||  gpe::input->kb_button_released[kb_h] )
                    {
                        main_search_controller->findTextStringBox->set_string( get_short_hightlighted() );
                    }
                    else if( dKeyDelay >= (main_settings->textAreaDelayTime)  || ( !gpe::input->kb_button_pressed[kb_d] && gpe::input->kb_button_released[kb_d] ) )
                    {
                        if( !isReadOnly )
                        {
                            duplicate_text();
                            dKeyDelay = 0;
                        }
                    }
                    else if( gpe::input->kb_button_released[kb_v] && !isReadOnly)
                    {
                        pasteCommandGiven = true;
                    }
                    else if( gpe::input->kb_button_released[kb_x] && !isReadOnly)
                    {
                        cut_selection();
                        scroll_to_cursor();
                    }
                    else if( gpe::input->kb_button_released[kb_y] && !isReadOnly)
                    {
                        if( can_redo() )
                        {
                            redo_edit();
                            gpe::input->reset_all_input();
                            process_self( view_space,cam );
                        }
                    }
                    else if( gpe::input->kb_button_released[kb_z] && !isReadOnly)
                    {
                        if( can_undo() )
                        {
                            undo_edit();
                            gpe::input->reset_all_input();
                            process_self( view_space,cam );
                        }
                    }
                    else if( upDelay > (main_settings->textAreaDelayTime)  || ( !gpe::input->kb_button_pressed[kb_up] && gpe::input->kb_button_released[kb_up] ) )
                    {
                        lineStartYPos--;
                        upDelay = 0;
                        showCursor = true;
                        cursorTimer = 0;
                    }
                    else if( downDelay > (main_settings->textAreaDelayTime)  || ( !gpe::input->kb_button_pressed[kb_down] && gpe::input->kb_button_released[kb_down] ) )
                    {
                        lineStartYPos++;
                        downDelay = 0;
                        showCursor = true;
                        cursorTimer = 0;
                    }
                    else if( leftDelay > (main_settings->textAreaDelayTime)  || ( !gpe::input->kb_button_pressed[kb_left] && gpe::input->kb_button_released[kb_left] ) )
                    {
                        if( gpe::input->kb_shift_pressed)
                        {
                            if( selectionCursorXPos==selectionEndCursorXPos && selectionCursorYPos==selectionEndCursorYPos )
                            {
                                selectionCursorXPos = cursorXPos;
                                selectionCursorYPos = cursorYPos;
                            }
                        }
                        if( cursorXPos <=0)
                        {
                            if( cursorYPos > 0)
                            {
                                cursorYPos--;
                                cursorXPos = (int)listOfStrings[cursorYPos].size()-1;
                                if( cursorXPos < 0)
                                {
                                    cursorXPos = 0;
                                }
                            }
                        }
                        else if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
                        {
                            std::string currentLineToScroll = listOfStrings[cursorYPos];
                            if( cursorXPos >=(int)currentLineToScroll.size() )
                            {
                                cursorXPos = (int)currentLineToScroll.size()-1;
                            }
                            if( cursorXPos < 0)
                            {
                                if( cursorYPos >0 && cursorYPos < (int)listOfStrings.size() )
                                {
                                    cursorYPos--;
                                    cursorXPos = listOfStrings[cursorYPos].size()-1;
                                }
                            }
                            else if( cursorXPos < (int)currentLineToScroll.size() )
                            {
                                int iNCursorX = cursorXPos-1;
                                for( iNCursorX = cursorXPos-1; iNCursorX >=0; iNCursorX--)
                                {
                                    if( stg_ex::char_is_alnum(currentLineToScroll[iNCursorX],false,true)==false )
                                    {
                                        break;
                                    }
                                }
                                if( iNCursorX< 0)
                                {
                                    iNCursorX = 0;
                                }
                                cursorXPos = iNCursorX;
                            }
                            else
                            {
                                cursorXPos = 0;
                            }
                        }
                        scroll_to_cursor();
                        showCursor = true;
                        cursorTimer = 0;
                        if( gpe::input->kb_shift_pressed)
                        {
                            selectionEndCursorXPos = cursorXPos;
                            selectionEndCursorYPos = cursorYPos;
                        }
                        leftDelay = 0;
                    }
                    else if( rightDelay > (main_settings->textAreaDelayTime)  || ( !gpe::input->kb_button_pressed[kb_right] && gpe::input->kb_button_released[kb_right] ) )
                    {
                        if( gpe::input->kb_shift_pressed)
                        {
                            if( selectionCursorXPos==selectionEndCursorXPos && selectionCursorYPos==selectionEndCursorYPos )
                            {
                                selectionCursorXPos = cursorXPos;
                                selectionCursorYPos = cursorYPos;
                            }
                        }
                        if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
                        {
                            std::string currentLineToScroll = listOfStrings[cursorYPos];
                            if( cursorXPos >=(int)currentLineToScroll.size() )
                            {
                                if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size()-1 )
                                {
                                    cursorYPos++;
                                    cursorXPos = 0;
                                }
                            }
                            else if( cursorXPos >= 0 && cursorXPos < (int)currentLineToScroll.size() )
                            {
                                int iNCursorX = cursorXPos+1;
                                for( iNCursorX = cursorXPos+1; iNCursorX <(int)currentLineToScroll.size(); iNCursorX++)
                                {
                                    if( stg_ex::char_is_alnum(currentLineToScroll[iNCursorX],false,true)==false )
                                    {
                                        break;
                                    }
                                }
                                if( iNCursorX >=(int)currentLineToScroll.size() )
                                {
                                    iNCursorX >(int)currentLineToScroll.size()-1;
                                }
                                if( iNCursorX < 0)
                                {
                                    iNCursorX = 0;
                                }
                                cursorXPos = iNCursorX;
                            }
                        }
                        scroll_to_cursor();
                        showCursor = true;
                        cursorTimer = 0;
                        if( gpe::input->kb_shift_pressed)
                        {
                            selectionEndCursorXPos = cursorXPos;
                            selectionEndCursorYPos = cursorYPos;
                        }
                        rightDelay = 0;
                    }
                    main_syntax_highlighter->clear_all();
                }
                else
                {
                    dKeyDelay = 0;
                    if( gpe::input->mouse_scrolling_up )
                    {
                        //move_up( linesWithinView/4);
                        main_syntax_highlighter->highlightedTerm = NULL;
                    }
                    else if( mouseHoveringInTextArea && gpe::input->mouse_scrolling_down )
                    {
                        //move_down( linesWithinView/4);
                        main_syntax_highlighter->highlightedTerm = NULL;
                    }
                    else if(  (enterDelay >= (main_settings->textAreaDelayTime)  )  && !isReadOnly )
                    {
                        if( main_syntax_highlighter->codeBeingSuggested )
                        {
                            if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
                            {
                                std::string prevStr = listOfStrings[cursorYPos];
                                if( main_syntax_highlighter->iSuggestionPos >= 0 && main_syntax_highlighter->iSuggestionPos < (int)main_syntax_highlighter->suggestedCompilerTerms.size() )
                                {
                                    syntax_compiler_term * tempTerm = main_syntax_highlighter->suggestedCompilerTerms.at(main_syntax_highlighter->iSuggestionPos);
                                    if( tempTerm!=NULL)
                                    {
                                        if( tempTerm->termType==cterm_function)
                                        {
                                            prevStr = prevStr.substr(0,tempCLineXStartPos)+tempTerm->termString+"()"+prevStr.substr(tempCLineXEndPos+1);
                                            cursorXPos = tempCLineXStartPos + (int)tempTerm->termString.size()+1;
                                        }
                                        else
                                        {
                                            prevStr = prevStr.substr(0,tempCLineXStartPos)+tempTerm->termString+prevStr.substr(tempCLineXEndPos+1);
                                            cursorXPos = tempCLineXStartPos + (int)tempTerm->termString.size();
                                        }
                                        listOfStrings[cursorYPos] = prevStr;
                                    }
                                }
                            }
                            main_syntax_highlighter->clear_all();
                            enterDelay = 0;
                            gpe::input->reset_all_input();
                        }
                        else
                        {
                            log_editable_action();
                            delete_selection();
                            lineToEdit = listOfStrings[cursorYPos];
                            //go to next line
                            std::string nextString = "";
                            if( (int)lineToEdit.size()> 0)
                            {
                                nextString = stg_ex::get_substring(lineToEdit,cursorXPos);
                                lineToEdit = stg_ex::get_substring(lineToEdit,0, cursorXPos);
                            }
                            else
                            {
                                lineToEdit = "";
                                nextString = "";
                            }
                            cursorXPos = 0;
                            if( (int)listOfStrings.size()>0 )
                            {
                                listOfStrings.erase(listOfStrings.begin()+cursorYPos );
                                listOfStrings.insert(listOfStrings.begin()+cursorYPos,lineToEdit);
                                int numbOfSpaces =  stg_ex::get_leading_space_count(lineToEdit);
                                cursorXPos = 0;
                                if( numbOfSpaces > 0)
                                {
                                    for(int i= 0; i < numbOfSpaces; i++)
                                    {
                                        nextString=" "+nextString;
                                    }
                                    cursorXPos = numbOfSpaces;
                                }
                                if( (int)lineToEdit.size()>0)
                                {
                                    std::string lastChar = stg_ex::get_substring(lineToEdit, (int)lineToEdit.size()-1 );
                                    if( lastChar.compare("{" )==0)
                                    {
                                        nextString=generate_tabs(1)+nextString;
                                        cursorXPos+=get_tab_space_count();
                                    }
                                }
                                listOfStrings.insert(listOfStrings.begin()+cursorYPos+1,nextString);
                                gpe::input->reset_all_input();
                            }
                            else
                            {
                                listOfStrings.push_back(lineToEdit);
                                listOfStrings.push_back(nextString);
                            }
                            if( cursorXPos < 0)
                            {
                                cursorXPos = 0;
                            }
                            cursorYPos+=1;
                            move_down();
                            showCursor = true;
                            cursorTimer = 0;
                            scroll_to_cursor();
                            main_syntax_highlighter->clear_all();
                            enterDelay = 0;
                        }
                        main_syntax_highlighter->highlightedTerm = NULL;
                        enterDelay = 0;
                    }
                    else if( leftDelay >= (main_settings->textAreaDelayTime)  )
                    {
                        if( gpe::input->kb_shift_pressed )
                        {
                            if( selectionCursorXPos==selectionEndCursorXPos && selectionCursorYPos==selectionEndCursorYPos )
                            {
                                selectionCursorXPos = cursorXPos;
                                selectionCursorYPos = cursorYPos;
                            }
                        }
                        else
                        {
                            reset_selection(-1);
                        }
                        if( cursorXPos > 0)
                        {
                            int tabCharCount = get_tab_space_count();
                            if( (int)lineToEdit.size() >= cursorXPos-tabCharCount )
                            {
                                bool hasLeadingTabs = true;
                                for( int iCharPos = std::min( cursorXPos-1, (int)lineToEdit.size()-1 ); iCharPos >=0 && iCharPos >=cursorXPos-tabCharCount; iCharPos-- )
                                {
                                    if( lineToEdit[iCharPos]!=' ')
                                    {
                                        hasLeadingTabs = false;
                                    }
                                }
                                if( hasLeadingTabs)
                                {
                                    cursorXPos-=tabCharCount;
                                }
                                else
                                {
                                    cursorXPos-=1;
                                }
                            }
                            else
                            {
                                cursorXPos-=1;
                            }
                            //adjust_fortabs();
                        }
                        else if( cursorYPos > 0 )
                        {
                            cursorYPos-=1;
                            move_up();
                            std::string prevString = listOfStrings[cursorYPos];
                            cursorXPos = prevString.size();
                            if( cursorXPos < 0)
                            {
                                cursorXPos = 0;
                            }
                            adjust_fortabs();
                        }
                        if( gpe::input->kb_shift_pressed)
                        {
                            selectionEndCursorXPos = cursorXPos;
                            selectionEndCursorYPos = cursorYPos;
                        }
                        leftDelay = 0;
                        scroll_to_cursor();
                        find_documentation_description();
                    }
                    else if( rightDelay >= (main_settings->textAreaDelayTime)  )
                    {
                        lineToEdit = listOfStrings[cursorYPos];
                        if( gpe::input->kb_shift_pressed)
                        {
                            if( selectionCursorXPos==selectionEndCursorXPos && selectionCursorYPos==selectionEndCursorYPos )
                            {
                                selectionCursorXPos = cursorXPos;
                                selectionCursorYPos = cursorYPos;
                            }
                        }
                        else
                        {
                            reset_selection(1);
                        }
                        //special shift action
                        cursorXPos+=1;
                        if( cursorXPos > (int)lineToEdit.size() )
                        {
                            //go to next line if available
                            if(cursorYPos < (int) listOfStrings.size()-1 )
                            {
                                cursorXPos = 0;
                                cursorYPos+=1;
                                move_down();
                            }
                            else
                            {
                                cursorXPos = (int)lineToEdit.size();
                            }
                        }
                        else
                        {
                            //adjust_fortabs();
                        }
                        if( gpe::input->kb_shift_pressed)
                        {
                            selectionEndCursorXPos = cursorXPos;
                            selectionEndCursorYPos = cursorYPos;
                        }
                        rightDelay = 0;
                        showCursor = true;
                        cursorTimer = 0;
                        scroll_to_cursor();
                        find_documentation_description();
                    }
                    else if( upDelay >= (main_settings->textAreaDelayTime)  )
                    {
                        main_syntax_highlighter->highlightedTerm = NULL;
                        if( main_syntax_highlighter->codeBeingSuggested )
                        {
                            if( main_syntax_highlighter->iSuggestionPos > 0)
                            {
                                main_syntax_highlighter->iSuggestionPos--;
                                if( main_syntax_highlighter->iSuggestionPos < main_syntax_highlighter->iSuggestedStartPos )
                                {
                                    main_syntax_highlighter->iSuggestedStartPos = main_syntax_highlighter->iSuggestionPos;
                                }
                            }
                            upDelay = -1;
                        }
                        else
                        {
                            if( gpe::input->kb_shift_pressed)
                            {
                                if( selectionCursorXPos==selectionEndCursorXPos && selectionCursorYPos==selectionEndCursorYPos )
                                {
                                    selectionCursorXPos = cursorXPos;
                                    selectionCursorYPos = cursorYPos;
                                }
                            }
                            else
                            {
                                reset_selection(-1);
                            }
                            //special shift action
                            if( cursorYPos>0)
                            {
                                cursorYPos-=1;
                                if( lineStartYPos==cursorYPos+1)
                                {
                                    move_up();
                                }
                                else if( cursorYPos < lineStartYPos || cursorYPos > lineStartYPos+linesWithinView)
                                {
                                    lineStartYPos = cursorYPos;
                                }
                                std::string prevLine = listOfStrings[cursorYPos];
                                if( cursorXPos >= (int)prevLine.size() )
                                {
                                    cursorXPos = (int)prevLine.size()-1;
                                    if( cursorXPos<0)
                                    {
                                        cursorXPos = 0;
                                    }
                                }
                            }
                            if( gpe::input->kb_shift_pressed)
                            {
                                selectionEndCursorXPos = cursorXPos;
                                selectionEndCursorYPos = cursorYPos;
                            }
                            upDelay = 0;
                            showCursor = true;
                            cursorTimer = 0;
                            adjust_fortabs();
                            scroll_to_cursor();
                            find_documentation_description();
                        }
                    }
                    else if( downDelay >= (main_settings->textAreaDelayTime)  )
                    {
                        main_syntax_highlighter->highlightedTerm  = NULL;
                        if( main_syntax_highlighter->codeBeingSuggested )
                        {
                            if( main_syntax_highlighter->iSuggestionPos < (int)main_syntax_highlighter->suggestedCompilerTerms.size()-1 )
                            {
                                main_syntax_highlighter->iSuggestionPos++;
                                if( main_syntax_highlighter->iSuggestionPos >= main_syntax_highlighter->iSuggestedStartPos+main_syntax_highlighter->suggestedTextMaxInViewCount)
                                {
                                    main_syntax_highlighter->iSuggestedStartPos = main_syntax_highlighter->iSuggestionPos;
                                    if( main_syntax_highlighter->iSuggestedStartPos+main_syntax_highlighter->suggestedTextMaxInViewCount >= (int)main_syntax_highlighter->suggestedCompilerTerms.size() )
                                    {
                                        main_syntax_highlighter->iSuggestedStartPos = (int)main_syntax_highlighter->suggestedCompilerTerms.size() -  main_syntax_highlighter->suggestedTextMaxInViewCount;
                                    }
                                    if( main_syntax_highlighter->iSuggestedStartPos < 0)
                                    {
                                        main_syntax_highlighter->iSuggestedStartPos = 0;
                                    }
                                }
                            }
                            downDelay = -1;
                        }
                        else
                        {
                            if( gpe::input->kb_shift_pressed)
                            {
                                if( selectionCursorXPos==selectionEndCursorXPos && selectionCursorYPos==selectionEndCursorYPos )
                                {
                                    selectionCursorXPos = cursorXPos;
                                    selectionCursorYPos = cursorYPos;
                                }
                            }
                            else
                            {
                                reset_selection(1);
                            }
                            //special shift action
                            if( cursorYPos < (int)listOfStrings.size()-1 )
                            {
                                cursorYPos+=1;
                                if( lineStartYPos>=cursorYPos+linesWithinView-1)
                                {
                                    move_down();
                                }
                                else if( cursorYPos < lineStartYPos || cursorYPos > lineStartYPos+linesWithinView)
                                {
                                    lineStartYPos = cursorYPos;
                                }
                                std::string nextLine = listOfStrings[cursorYPos];
                                if( cursorXPos >= (int)nextLine.size() )
                                {
                                    cursorXPos = (int)nextLine.size()-1;
                                    if( cursorXPos<0)
                                    {
                                        cursorXPos = 0;
                                    }
                                }
                            }
                            if( gpe::input->kb_shift_pressed)
                            {
                                selectionEndCursorXPos = cursorXPos;
                                selectionEndCursorYPos = cursorYPos;
                            }
                            downDelay = 0;
                            showCursor = true;
                            cursorTimer = 0;
                            adjust_fortabs();
                            scroll_to_cursor();
                            find_documentation_description();
                        }
                    }
                    else if( bscDelay >= (main_settings->textAreaDelayTime)  )
                    {
                        bscDelay = 0;
                        if( !isReadOnly)
                        {
                            bscDelay = 0;
                            if( selectionCursorXPos!=selectionEndCursorXPos || selectionCursorYPos!=selectionEndCursorYPos)
                            {
                                delete_selection();
                            }
                            else if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
                            {
                                lineToEdit = listOfStrings[cursorYPos];
                                int prevSize = (int)lineToEdit.size();
                                if( prevSize>0)
                                {
                                    if( cursorXPos > prevSize )
                                    {
                                        cursorXPos = prevSize;
                                    }
                                }
                                else
                                {
                                    cursorXPos = 0;
                                }
                                if( cursorXPos>0 && cursorXPos <= (int)lineToEdit.size() )
                                {
                                    textInputString = stg_ex::get_substring(lineToEdit,0,cursorXPos);
                                    std::string rightSide = stg_ex::get_substring(lineToEdit,cursorXPos);
                                    if( cursorXPos >= (int)lineToEdit.size() )
                                    {
                                        textInputString = stg_ex::get_substring(lineToEdit,0);
                                        rightSide = "";
                                    }
                                    int trailingSpaces = stg_ex::get_trailing_space_count(textInputString);
                                    int tabsToCheckCount = get_tab_space_count();
                                    if( trailingSpaces >= tabsToCheckCount )
                                    {
                                        cursorXPos -= tabsToCheckCount;
                                        textInputString = stg_ex::get_substring(textInputString,0,cursorXPos);
                                    }
                                    else
                                    {
                                        cursorXPos -=1;
                                        textInputString = stg_ex::get_substring(textInputString,0,cursorXPos);
                                    }

                                    textInputString+=rightSide;
                                    listOfStrings.erase(listOfStrings.begin()+cursorYPos);
                                    if( cursorYPos>=0)
                                    {
                                        listOfStrings.insert(listOfStrings.begin()+cursorYPos,textInputString);
                                    }
                                    else
                                    {
                                        listOfStrings.push_back(textInputString);
                                    }
                                    log_editable_action();
                                }
                                else if( cursorYPos>=1)
                                {
                                    log_editable_action();
                                    textInputString = lineToEdit;
                                    std::string prevString = listOfStrings.at(cursorYPos-1);
                                    cursorXPos = prevString.size();
                                    listOfStrings.erase(listOfStrings.begin()+cursorYPos);
                                    cursorYPos -=1;
                                    if( cursorYPos < lineStartYPos && cursorYPos>=0)
                                    {
                                        lineStartYPos-=1;
                                    }
                                    textInputString = listOfStrings[cursorYPos]+lineToEdit;
                                    listOfStrings.erase(listOfStrings.begin()+cursorYPos);
                                    listOfStrings.insert(listOfStrings.begin()+cursorYPos,textInputString);
                                    log_editable_action();
                                }
                            }
                            bscDelay = 0;
                            showCursor = true;
                            cursorTimer = 0;
                            scroll_to_cursor();
                        }
                        main_syntax_highlighter->clear_all();
                    }
                    else if( delDelay > (main_settings->textAreaDelayTime)  )
                    {
                        delDelay = 0;
                        if(!isReadOnly)
                        {
                            log_editable_action();
                            if( selectionCursorXPos!=selectionEndCursorXPos || selectionCursorYPos!=selectionEndCursorYPos)
                            {
                                delete_selection();
                            }
                            else if( cursorYPos >=0 && cursorYPos < (int)listOfStrings.size() )
                            {
                                lineToEdit = listOfStrings[cursorYPos];
                                int prevSize = (int)lineToEdit.size();
                                if( prevSize>0)
                                {
                                    if( cursorXPos > prevSize )
                                    {
                                        cursorXPos = prevSize;
                                    }
                                }
                                else
                                {
                                    cursorXPos = 0;
                                }
                                if( cursorXPos>=0 && cursorXPos < (int)lineToEdit.size() )
                                {
                                    textInputString = stg_ex::get_substring(lineToEdit,0,cursorXPos);
                                    std::string rightSide = stg_ex::get_substring(lineToEdit,cursorXPos+1);
                                    if( cursorXPos >= (int)lineToEdit.size() )
                                    {
                                        textInputString = stg_ex::get_substring(lineToEdit,0);
                                        rightSide = "";
                                    }

                                    textInputString+=rightSide;
                                    listOfStrings.erase(listOfStrings.begin()+cursorYPos);
                                    if( cursorYPos>=0)
                                    {
                                        listOfStrings.insert(listOfStrings.begin()+cursorYPos,textInputString);
                                    }
                                    else
                                    {
                                        listOfStrings.push_back(textInputString);
                                    }
                                    log_editable_action();
                                }
                                else if( cursorYPos>=0 && cursorXPos>=(int)lineToEdit.size() && cursorYPos < (int)listOfStrings.size()-1 )
                                {
                                    log_editable_action();
                                    textInputString = lineToEdit;
                                    std::string prevString = listOfStrings.at(cursorYPos+1);
                                    cursorXPos = lineToEdit.size();
                                    textInputString = lineToEdit+prevString;
                                    listOfStrings.erase(listOfStrings.begin()+cursorYPos+1);
                                    listOfStrings.erase(listOfStrings.begin()+cursorYPos);
                                    listOfStrings.insert(listOfStrings.begin()+cursorYPos,textInputString);
                                    log_editable_action();
                                }
                            }
                            delDelay = 0;
                            showCursor = true;
                            cursorTimer = 0;
                            scroll_to_cursor();
                        }
                        main_syntax_highlighter->clear_all();

                    }
                    else if( tabDelay > (main_settings->textAreaDelayTime) )
                    {
                        tabDelay = 0;
                        if( !isReadOnly)
                        {
                            int tabStartYPos = 0;
                            int tabEndYPos = 0;
                            if( selectionCursorYPos>selectionEndCursorYPos)
                            {
                                tabEndYPos = selectionCursorYPos;
                                tabStartYPos = selectionEndCursorYPos;
                            }
                            else
                            {
                                tabStartYPos = selectionCursorYPos;
                                tabEndYPos = selectionEndCursorYPos;
                            }

                            int tabCharCount = get_tab_space_count();
                            if( gpe::input->kb_shift_pressed)
                            {
                                std::string strToUnTab = "";
                                if( selectionCursorXPos!=selectionEndCursorXPos || selectionCursorYPos!=selectionEndCursorYPos )
                                {
                                    for( int iTabPos = tabStartYPos; iTabPos <= tabEndYPos; iTabPos++)
                                    {
                                        strToUnTab = listOfStrings[iTabPos];
                                        if( has_early_tab(strToUnTab) )
                                        {
                                            strToUnTab = untab_string(strToUnTab);
                                            listOfStrings[iTabPos] = strToUnTab;
                                            if( iTabPos ==selectionCursorYPos && selectionCursorYPos==selectionEndCursorYPos )
                                            {
                                                if( selectionCursorXPos>selectionEndCursorXPos)
                                                {
                                                    selectionCursorXPos-=tabCharCount;
                                                }
                                                else
                                                {
                                                    selectionEndCursorXPos-=tabCharCount;
                                                }
                                            }
                                            else if(iTabPos ==selectionEndCursorYPos  )
                                            {
                                                if( selectionCursorYPos>selectionEndCursorYPos)
                                                {
                                                    selectionCursorXPos-=tabCharCount;
                                                }
                                                else
                                                {
                                                    selectionEndCursorXPos-=tabCharCount;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                std::string tabAddition = generate_tabs(1);
                                if( selectionCursorXPos!=selectionEndCursorXPos || selectionCursorYPos!=selectionEndCursorYPos )
                                {
                                    for( int iTabPos = tabStartYPos; iTabPos <= tabEndYPos; iTabPos++)
                                    {
                                        listOfStrings[iTabPos] = tabAddition+listOfStrings[iTabPos];
                                    }
                                    if( selectionCursorYPos==selectionEndCursorYPos)
                                    {
                                        if( selectionCursorXPos>selectionEndCursorXPos)
                                        {
                                            selectionCursorXPos+=tabCharCount;
                                        }
                                        else
                                        {
                                            selectionEndCursorXPos+=tabCharCount;
                                        }
                                    }
                                    else
                                    {
                                        if( selectionCursorYPos>selectionEndCursorYPos)
                                        {
                                            selectionCursorXPos+=tabCharCount;
                                        }
                                        else
                                        {
                                            selectionEndCursorXPos+=tabCharCount;
                                        }
                                    }
                                }
                                else
                                {
                                    delete_selection();
                                    textInputString = stg_ex::get_substring(lineToEdit,0,cursorXPos);
                                    textInputString+=tabAddition;
                                    textInputString+=stg_ex::get_substring(lineToEdit,cursorXPos);
                                    cursorXPos+=tabCharCount;
                                    gpe::input->inputted_keys = "";
                                    listOfStrings.erase(listOfStrings.begin()+cursorYPos);
                                    listOfStrings.insert(listOfStrings.begin()+cursorYPos,textInputString);
                                    scroll_to_cursor();
                                }

                                scroll_to_pos(lineStartYPos, cursorXPos+tabCharCount );
                            }
                            save_edit();
                        }
                        showCursor = true;
                        cursorTimer = 0;
                        main_syntax_highlighter->clear_all();
                        tabDelay= 0;
                    }
                    else if( (int)gpe::input->inputted_keys.size()>0 && !isReadOnly )
                    {
                        main_syntax_highlighter->highlightedTerm = NULL;
                        //Type input into textarea
                        delete_selection();
                        log_editable_action();
                        textInputString = stg_ex::get_substring(lineToEdit,0,cursorXPos);
                        std::string preInputString = stg_ex::string_replace_all(textInputString," ","");
                        textInputString+=gpe::input->inputted_keys;
                        listOfStrings.erase(listOfStrings.begin()+cursorYPos);
                        std::string restofLine = stg_ex::get_substring(lineToEdit,cursorXPos);
                        if( gpe::input->inputted_keys=="{" && (int)restofLine.size()==0 && (int)preInputString.size()==0 )
                        {
                            int numbOfSpaces =  stg_ex::get_leading_space_count(lineToEdit);
                            cursorXPos = 0;
                            std::string nextString = "";
                            if( numbOfSpaces > 0)
                            {
                                for(int i= 0; i < numbOfSpaces; i++)
                                {
                                    nextString=" "+nextString;
                                }
                                cursorXPos = numbOfSpaces;
                            }
                            std::string bracketString = nextString+"}";
                            nextString="    "+nextString;
                            cursorXPos+=4;

                            textInputString+=restofLine;
                            listOfStrings.insert(listOfStrings.begin()+cursorYPos,textInputString);
                            listOfStrings.insert(listOfStrings.begin()+cursorYPos+1,nextString);
                            listOfStrings.insert(listOfStrings.begin()+cursorYPos+2,bracketString);
                            cursorYPos+=1;
                            scroll_to_pos(cursorYPos+1,cursorXPos);
                            gpe::input->inputted_keys = "";
                            save_edit();
                        }
                        else
                        {
                            log_editable_action();
                            cursorXPos+=(int)gpe::input->inputted_keys.size();
                            if( gpe::input->inputted_keys=="[" )
                            {
                                textInputString+="]";
                            }
                            else if( gpe::input->inputted_keys=="{" )
                            {
                                textInputString+="}";
                            }
                            else if( gpe::input->inputted_keys=="(" )
                            {
                                textInputString+=")";
                            }
                            textInputString+=restofLine;
                            listOfStrings.insert(listOfStrings.begin()+cursorYPos,textInputString);

                            gpe::input->inputted_keys = "";
                            if( isCodeEditor)
                                find_suggested_text();
                            scroll_to_cursor();
                        }
                        gpe::input->inputted_keys = "";
                    }
                    else if( gpe::input->check_kb_released(kb_esc) )
                    {
                        main_search_controller->close_finder();
                        main_syntax_highlighter->clear_all();
                    }
                }
            }

            parseForErrorsTimerPos++;
            if( parseForErrorsTimerPos >=parseForErrorsTimerGoal)
            {
                if( isCodeEditor && codeEditorType==0)
                {
                    //parse_code_javascript(view_space,cam);
                }
                parseForErrorsTimerPos = 0;
            }
        }
        else
        {
            hasScrollControl = false;
            hasArrowkeyControl = false;
        }

        bool scrollingUp = false;
        bool scrollingDown = false;
        if( isHovered)
        {
            if( gpe::input->mouse_scrolling_up )
            {
                if( gpe::input->kb_button_down[kb_ctrl] )
                {
                    move_left(charactersWithinView/8);
                    main_syntax_highlighter->highlightedTerm  = NULL;
                }
                else
                {
                    move_up( 3 );
                    main_syntax_highlighter->highlightedTerm = NULL;
                }
            }
            else if( gpe::input->mouse_scrolling_down )
            {
                if( gpe::input->kb_button_down[kb_ctrl])
                {
                    move_right(charactersWithinView/8);
                    main_syntax_highlighter->highlightedTerm = NULL;
                }
                else
                {
                    move_down( 3 );
                    main_syntax_highlighter->highlightedTerm = NULL;
                }
            }
            hasScrollControl= true;
        }

        if( lineStartXPos < 0)
        {
            lineStartXPos = 0;
        }
        if( lineStartYPos < 0)
        {
            lineStartYPos = 0;
        }

        main_settings->textAreaFindBox.x = widget_box.x+lineCountBoxWidth;
        //if( showXScroll && textXScroll!=NULL)
        {
            //main_settings->textAreaFindBox.y = widget_box.y+textEditorButtonBar->get_height()+renderBox->h+textXScroll->widget_box.h;
        }
        //else
        {
            main_settings->textAreaFindBox.y = widget_box.y+textEditorButtonBar->get_height()+renderBox->h;
        }
        main_settings->textAreaFindBox.w = widget_box.w-lineCountBoxWidth;

        int findAllResult = 0;
        switch( main_search_controller->textSearchMode )
        {
        //find
        case search_mode::find_text:
            if( main_search_controller->findTextStringBox->has_content() )
            {
                if( main_search_controller->findTextStringBox->was_submitted() || main_search_controller->findButton->is_clicked() )
                {
                    if( find_string(main_search_controller->findTextStringBox->get_string(),main_search_controller->findMatchCase->is_clicked(),false)==false )
                    {
                        cursorXPos = 0;
                        cursorYPos = 0;
                        lineStartXPos = 0;
                        lineStartYPos = 0;
                        if( find_string(main_search_controller->findTextStringBox->get_string(),true,main_search_controller->findMatchCase->is_clicked(),false)==false)
                        {
                            main_overlay_system->update_temporary_message("Searched for","["+main_search_controller->findTextStringBox->get_string()+"]","Unable to Find String");
                        }
                    }
                    main_search_controller->showFindAllResults = false;
                }
                /*
                else if( main_search_controller->findAllButton->is_clicked() )
                {
                    findAllResult = find_all_strings(main_search_controller->findTextStringBox->get_string(),main_search_controller->findMatchCase->is_clicked() );
                    displayMessageTitle = "Substring Search";
                    displayMessageSubtitle = main_search_controller->findTextStringBox->get_string();

                    if( findAllResult > 0)
                    {
                        main_loader_display->update_messages( "Replacing" , main_search_controller->findTextStringBox->get_string(), "Found "+ stg_ex::int_to_string(findAllResult)+" results" );
                        main_search_controller->showFindAllResults = true;
                    }
                    else
                    {
                        main_loader_display->update_messages( "Replacing" , main_search_controller->findTextStringBox->get_string(), "No matches found");
                        main_search_controller->showFindAllResults = false;
                    }
                    main_overlay_system->update_temporary_message(displayMessageTitle,displayMessageSubtitle,displayMessageString,5);
                }
                */
            }
            break;

        //goto line
        case search_mode::goto_line:
            //main_search_controller->goToLineStringBox->set_string("1");

            if( (int)listOfStrings.size() > 0)
            {
                main_search_controller->goToLineStringBox->descriptionText = "Go To Line: 1 - "+ stg_ex::int_to_string( get_line_count() ) ;
            }
            else
            {
                main_search_controller->goToLineStringBox->descriptionText = "Go To Line: ";
            }

            if( ( main_search_controller->goToLineStringBox->was_submitted() || main_search_controller->goToButton->is_clicked() ) &&  main_search_controller->goToLineStringBox->is_valid() )
            {
                scroll_to_pos(main_search_controller->goToLineStringBox->get_held_number()-1,0);
            }
            break;

        //find/replace
        case search_mode::replace_text:
            if( !isReadOnly)
            {
                if( main_search_controller->findTextStringBox->has_content() )
                {
                    if( main_search_controller->findTextStringBox->was_submitted() || main_search_controller->findButton->is_clicked() )
                    {
                        if( find_string(main_search_controller->findTextStringBox->get_string(),true,main_search_controller->findMatchCase->is_clicked(),false)==false )
                        {
                            cursorXPos = 0;
                            cursorYPos = 0;
                            lineStartXPos = 0;
                            lineStartYPos = 0;
                            if( find_string(main_search_controller->findTextStringBox->get_string(),true,main_search_controller->findMatchCase->is_clicked(),false)==false)
                            {
                                main_overlay_system->update_temporary_message("Searched for","["+main_search_controller->findTextStringBox->get_string()+"]","Unable to Find String");
                            }
                        }
                        main_search_controller->showFindAllResults = false;
                    }
                    /*
                    else if( main_search_controller->findAllButton->is_clicked() )
                    {
                        findAllResult = find_all_strings(main_search_controller->findTextStringBox->get_string(),main_search_controller->findMatchCase->is_clicked() );
                        displayMessageTitle = "Substring Search";
                        displayMessageSubtitle = main_search_controller->findTextStringBox->get_string();


                        if( findAllResult > 0)
                        {
                            main_loader_display->update_messages( "Replacing" , needle, stg_ex::int_to_string(replaceCount)+" times");
                            main_search_controller->showFindAllResults = true;
                        }
                        else
                        {
                            main_loader_display->update_messages( "Replacing" , needle, "No matches found");

                            main_search_controller->showFindAllResults = false;
                        }
                        main_overlay_system->update_temporary_message(displayMessageTitle,displayMessageSubtitle,displayMessageString,15);
                    }
                    */
                    else if( main_search_controller->replaceTextStringBox->has_content() )
                    {
                        if( main_search_controller->replaceTextStringBox->was_submitted() || main_search_controller->replaceButton->is_clicked() )
                        {
                            main_search_controller->showFindAllResults = false;
                        }
                        else if( main_search_controller->replaceAllButton->is_clicked() )
                        {
                            if( display_prompt_message("Warning!","All copies of the text will be replaced. Are you sure you want to continue this operation?")==display_query_yes )
                            {
                                findAllResult = find_all_strings(main_search_controller->findTextStringBox->get_string(),true);
                                if( main_loader_display != NULL )
                                {
                                    main_loader_display->update_messages( "Replacing Substring", main_search_controller->findTextStringBox->get_string(),"With ["+main_search_controller->replaceTextStringBox->get_string()+"]"  );
                                }

                                if( findAllResult > 0)
                                {
                                    int replaceCount = replace_all_found( main_search_controller->findTextStringBox->get_string(), main_search_controller->replaceTextStringBox->get_string() ) ;
                                    if( main_loader_display != NULL )
                                    {
                                        main_loader_display->update_messages( "Replaced Substring", main_search_controller->findTextStringBox->get_string(), stg_ex::int_to_string( replaceCount )+" times" );
                                    }
                                    main_search_controller->showFindAllResults = true;
                                }
                                else
                                {
                                    main_loader_display->update_messages( "Replaced Substring", main_search_controller->findTextStringBox->get_string(), "No matches found" );
                                    main_search_controller->showFindAllResults = false;
                                }
                            }
                        }
                    }
                }
            }
            break;

        default:

        break;
        }

        if( !isReadOnly && pasteCommandGiven && gpe::input->clipboard_empty() == false )
        {
            delete_selection();
            paste_clipboard();
            main_syntax_highlighter->clear_all();
            gpe::input->reset_all_input();
            process_self( view_space,cam );
        }

        if( undoableActionOccurred && !isReadOnly )
        {
            time_t currentTimeNow;
            time(&currentTimeNow);
            if( difftime(currentTimeNow,lastUndoableActionTime ) >=4 )
            {
                save_edit();
            }
        }
        set_highlight_pos( view_space, cam );
    }

    void widget_text_editor::redo_edit()
    {
        if( can_redo() )
        {
            if( currentPositionInHistory >=0 && currentPositionInHistory+1 < (int)savedHistory.size() )
            {
                widget_text_editor * tempPlace = savedHistory.at(currentPositionInHistory+1);
                if( tempPlace!=NULL)
                {
                    copy_source(tempPlace,true);
                    currentPositionInHistory++;
                    showCursor = true;
                    cursorTimer = 0;
                }
            }
        }
    }

    void widget_text_editor::render_code( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( view_space!=NULL && cam!=NULL && textEditorButtonBar!=NULL && has_content() )
        {
            int mostCharactersOfText = get_most_characters_used();
            if( mostCharactersOfText > charactersWithinView && showYScroll )
            {
                mostCharactersOfText-=2;
            }
            int i = 0, j = 0;
            int parsedTokensCount = 0;
            std::string currStringToRender = "";
            std::string currentLineInView = "";
            //Processes the sythax to re-render each one
            std::string foundGPEDataType = "";
            std::string foundGPEFunction = "";
            std::string foundGPEVariable = "";
            std::string foundGPEKeyword = "";
            std::string foundGPEProjectFunction = "";
            std::string foundGPEProjectKeyword = "";
            std::string foundSyntaxString = "";
            int tempSynStringSize = 0;
            int currPosToParse = 0, lineEnd = 0;
            gpe::color *color = NULL;
            int textRenderXPos = 0, textRenderYPos = 0;
            bool isInBlockCommentMode = false;
            bool isInfloatQuoteMode = false;
            bool isInSingleQuoteMode = false;
            int minLineToRender = std::max( (int)lineStartYPos-50,0);
            int intLineStart = (int)lineStartYPos;
            int maxLineToRender = std::min( (int)lineStartYPos+(int)linesWithinView,(int)listOfStrings.size()-1);
            int endBlockCommentPos = 0;
            int endDQuoteCommentPos = 0;
            int endSQuoteCommentPos = 0;
            bool commentFoundInSymbols = false;
            parsed_text * mLineComment = new parsed_text(-1, -1);
            parsed_text * dqLineComment = new parsed_text(-1, -1);
            parsed_text * sqLineComment = new parsed_text(-1, -1);
            parsed_text * tempParseTextToAdd = new parsed_text(-1, -1);

            //Finds the previous mode of  the editor up to 20 lines to the current lineStartYPos
            for( i=minLineToRender; i < intLineStart && i < (int)listOfStrings.size(); i++)
            {
                currStringToRender = listOfStrings[i];
                currPosToParse = 0;
                lineEnd = (int)currStringToRender.size();
                while (currPosToParse < lineEnd)
                {
                    if( isInBlockCommentMode==false && isInfloatQuoteMode==false && isInSingleQuoteMode==false)
                    {
                        if (currPosToParse < lineEnd)
                        {
                            if (currPosToParse+1<lineEnd && currStringToRender[currPosToParse] == '/' && currPosToParse + 1 < lineEnd && currStringToRender[currPosToParse + 1] == '/' )
                            {
                                currPosToParse = lineEnd;
                            }
                            else if (currPosToParse+1<lineEnd && currStringToRender[currPosToParse] == '/' && currStringToRender[currPosToParse + 1] == '*' )
                            {
                                isInBlockCommentMode = true;
                                currPosToParse+=2;
                            }
                            else if(currStringToRender[currPosToParse] == '"')
                            {
                                isInfloatQuoteMode = true;
                                currPosToParse++;
                            }
                            else if(currStringToRender[currPosToParse] == '\'')
                            {
                                isInSingleQuoteMode = true;
                                currPosToParse++;
                            }
                            else
                            {
                                currPosToParse++;
                            }
                        }
                    }

                    if( isInBlockCommentMode)
                    {
                        endBlockCommentPos = currStringToRender.find("*/");
                        if( endBlockCommentPos==(int)std::string::npos)
                        {
                            currPosToParse = lineEnd;
                        }
                        else
                        {
                            currPosToParse = endBlockCommentPos+2;
                            isInBlockCommentMode = false;
                        }
                    }
                    else if( isInfloatQuoteMode)
                    {
                        endDQuoteCommentPos = currStringToRender.find('"',currPosToParse);
                        if( endDQuoteCommentPos==(int)std::string::npos)
                        {
                            currPosToParse = lineEnd;
                        }
                        else
                        {
                            currPosToParse = endDQuoteCommentPos+1;
                            isInfloatQuoteMode = false;
                        }
                    }
                    else if( isInSingleQuoteMode)
                    {
                        endSQuoteCommentPos = currStringToRender.find("'",currPosToParse);
                        if( endSQuoteCommentPos==(int)std::string::npos)
                        {
                            currPosToParse = lineEnd;
                        }
                        else
                        {
                            currPosToParse = endSQuoteCommentPos+1;
                            isInSingleQuoteMode = false;
                        }
                    }
                }
            }

            for( i=intLineStart; i <= maxLineToRender; i++)
            {
                //resets highlight boxes and such
                if( commentLineText!=NULL)
                {
                    commentLineText->reset_self();
                }
                if( datatypeLineText!=NULL)
                {
                    datatypeLineText->reset_self();
                }
                if( dQuoteLineText!=NULL)
                {
                    dQuoteLineText->reset_self();
                }
                if( functionLineText!=NULL)
                {
                    functionLineText->reset_self();
                }
                if( keywordLineText!=NULL)
                {
                    keywordLineText->reset_self();
                }
                if( variableLineText!=NULL)
                {
                    variableLineText->reset_self();
                }
                if( normalLineText!=NULL)
                {
                    normalLineText->reset_self();
                }

                if( numberLineText!=NULL)
                {
                    numberLineText->reset_self();
                }

                if( sQuoteLineText!=NULL)
                {
                    sQuoteLineText->reset_self();
                }

                if( symbolLineText!=NULL)
                {
                    symbolLineText->reset_self();
                }

                if( projectFunctionLineText!=NULL)
                {
                    projectFunctionLineText->reset_self();
                }

                if( projectKeywordLineText!=NULL)
                {
                    projectKeywordLineText->reset_self();
                }

                currStringToRender = listOfStrings[i];
                currPosToParse = 0;

                lineEnd = (int)currStringToRender.size();

                endBlockCommentPos=(int)std::string::npos;
                endDQuoteCommentPos=(int)std::string::npos;
                endSQuoteCommentPos=(int)std::string::npos;

                foundGPEKeyword = "";
                foundGPEVariable = "";
                foundGPEDataType = "";
                foundGPEProjectFunction = "";
                foundGPEProjectKeyword = "";
                mLineComment->textStart = 0;
                mLineComment->textEnd = -1;
                tempParseTextToAdd->textStart = 0;
                tempParseTextToAdd->textEnd = -1;

                //Add C/C++ macro processor here..[1.14 begin]
                //Add C/C++ macro processor here..[1.14 end]

                while (currPosToParse < lineEnd)
                {
                    while (currPosToParse < lineEnd && currStringToRender[currPosToParse]==' ')
                    {
                        currPosToParse++;
                    }
                    if( isInBlockCommentMode==false && isInfloatQuoteMode==false && isInSingleQuoteMode==false)
                    {
                        /*
                        while (currPosToParse < lineEnd && currStringToRender[currPosToParse]==' ')
                        {
                            currPosToParse++;
                        }*/
                        if (currPosToParse < lineEnd)
                        {
                            if (currPosToParse+1<lineEnd && currStringToRender[currPosToParse] == '/' && currPosToParse + 1 < lineEnd && currStringToRender[currPosToParse + 1] == '/' )
                            {
                                commentLineText->foundParses.push_back( new parsed_text(currPosToParse, lineEnd));
                                currPosToParse = lineEnd;
                            }
                            else if (currPosToParse+1<lineEnd && currStringToRender[currPosToParse] == '/' && currStringToRender[currPosToParse + 1] == '*' )
                            {
                                isInBlockCommentMode = true;
                                mLineComment->textStart = currPosToParse;
                                currPosToParse+=2;
                            }
                            else if( currStringToRender[currPosToParse] == '"' )
                            {
                                isInfloatQuoteMode = true;
                                dqLineComment->textStart = currPosToParse;
                                currPosToParse++;
                            }
                            else if( currStringToRender[currPosToParse] == '\'' )
                            {
                                isInSingleQuoteMode = true;
                                sqLineComment->textStart = currPosToParse;
                                currPosToParse++;
                            }
                            else if ( isdigit(currStringToRender[currPosToParse]) )
                            {
                                tempParseTextToAdd->textStart = currPosToParse;
                                currPosToParse++;
                                while( (currPosToParse < lineEnd && isdigit( currStringToRender[currPosToParse] )  ) || currStringToRender[currPosToParse] ==' ')
                                {
                                    currPosToParse++;
                                }
                                tempParseTextToAdd->textEnd = currPosToParse;
                                numberLineText->foundParses.push_back(tempParseTextToAdd);
                                tempParseTextToAdd = new parsed_text(0, -1);
                            }
                            else if( main_syntax_highlighter->charIsSymbol(currStringToRender[currPosToParse] ) )
                            {
                                tempParseTextToAdd->textStart = currPosToParse;
                                currPosToParse++;
                                commentFoundInSymbols = false;
                                while( ( commentFoundInSymbols==false && currPosToParse < lineEnd && main_syntax_highlighter->charIsSymbol(currStringToRender[currPosToParse] ) )|| currStringToRender[currPosToParse] ==' ')
                                {
                                    if( lineEnd > currPosToParse+1)
                                    {
                                        if( currStringToRender[currPosToParse] == '/' && (currStringToRender[currPosToParse + 1] == '/' || currStringToRender[currPosToParse + 1] == '*' ) )
                                        {
                                            commentFoundInSymbols = true;
                                        }
                                        else if (currStringToRender[currPosToParse] == '*' && currStringToRender[currPosToParse + 1] == '/' )
                                        {
                                            commentFoundInSymbols = true;
                                        }
                                        else
                                        {
                                            currPosToParse++;
                                        }
                                    }
                                    else
                                    {
                                        currPosToParse++;
                                    }
                                }
                                tempParseTextToAdd->textEnd = currPosToParse;
                                symbolLineText->foundParses.push_back(tempParseTextToAdd);
                                tempParseTextToAdd = new parsed_text(0, -1);
                            }
                            else
                            {
                                if( main_syntax_highlighter!=NULL)
                                {
                                    foundGPEProjectFunction = main_syntax_highlighter->find_project_function(currStringToRender,currPosToParse);
                                    foundGPEProjectKeyword = main_syntax_highlighter->find_project_keyword(currStringToRender,currPosToParse);
                                }
                                if( (foundGPEProjectFunction )!="")
                                {
                                    tempParseTextToAdd->textStart = currPosToParse;
                                    while (currPosToParse < lineEnd && foundGPEProjectFunction !="")
                                    {
                                        if(foundGPEProjectFunction!="" )
                                        {
                                            currPosToParse+=(int)foundGPEProjectFunction.size();
                                        }
                                        else
                                        {
                                            currPosToParse++;
                                        }
                                        foundGPEProjectFunction= main_syntax_highlighter->find_project_function(currStringToRender,currPosToParse);
                                    }
                                    tempParseTextToAdd->textEnd = currPosToParse;
                                    projectFunctionLineText->foundParses.push_back(tempParseTextToAdd);
                                    tempParseTextToAdd = new parsed_text(-1, -1);
                                }
                                else if( (foundGPEProjectKeyword )!="")
                                {
                                    tempParseTextToAdd->textStart = currPosToParse;
                                    while (currPosToParse < lineEnd && foundGPEProjectKeyword !="")
                                    {
                                        if(foundGPEProjectKeyword!="" )
                                        {
                                            currPosToParse+=(int)foundGPEProjectKeyword.size();
                                        }
                                        else
                                        {
                                            currPosToParse++;
                                        }
                                        foundGPEProjectKeyword= main_syntax_highlighter->find_project_keyword(currStringToRender,currPosToParse);
                                    }
                                    tempParseTextToAdd->textEnd = currPosToParse;
                                    projectKeywordLineText->foundParses.push_back(tempParseTextToAdd);
                                    tempParseTextToAdd = new parsed_text(-1, -1);
                                }
                                else
                                {
                                    foundGPEDataType= main_syntax_highlighter->find_gpe_datatype(currStringToRender,currPosToParse);
                                    if( (foundGPEDataType )!="")
                                    {
                                        tempParseTextToAdd->textStart = currPosToParse;
                                        while (currPosToParse < lineEnd && foundGPEDataType !="")
                                        {
                                            if(foundGPEDataType!="" )
                                            {
                                                currPosToParse+=(int)foundGPEDataType.size();
                                            }
                                            else
                                            {
                                                currPosToParse++;
                                            }
                                            foundGPEDataType= main_syntax_highlighter->find_gpe_datatype(currStringToRender,currPosToParse);
                                        }
                                        tempParseTextToAdd->textEnd = currPosToParse;
                                        datatypeLineText->foundParses.push_back(tempParseTextToAdd);
                                        tempParseTextToAdd = new parsed_text(-1, -1);
                                    }
                                    else
                                    {
                                        foundGPEFunction =main_syntax_highlighter->find_gpe_function(currStringToRender,currPosToParse);
                                        if( (foundGPEFunction )!="")
                                        {
                                            tempParseTextToAdd->textStart = currPosToParse;
                                            while (currPosToParse < lineEnd && foundGPEFunction !="")
                                            {
                                                if(foundGPEFunction!="" )
                                                {
                                                    currPosToParse+=(int)foundGPEFunction.size();
                                                }
                                                else
                                                {
                                                    currPosToParse++;
                                                }
                                                foundGPEFunction= main_syntax_highlighter->find_gpe_function(currStringToRender,currPosToParse);
                                            }
                                            tempParseTextToAdd->textEnd = currPosToParse;
                                            functionLineText->foundParses.push_back(tempParseTextToAdd);
                                            tempParseTextToAdd = new parsed_text(-1, -1);
                                        }
                                        else
                                        {
                                            foundGPEVariable = main_syntax_highlighter->find_gpe_variable(currStringToRender,currPosToParse);
                                            if( (foundGPEVariable )!="")
                                            {
                                                tempParseTextToAdd->textStart = currPosToParse;
                                                while (currPosToParse < lineEnd && foundGPEVariable !="")
                                                {
                                                    if(foundGPEVariable!="" )
                                                    {
                                                        currPosToParse+=(int)foundGPEVariable.size();
                                                    }
                                                    else
                                                    {
                                                        currPosToParse++;
                                                    }
                                                    foundGPEVariable = main_syntax_highlighter->find_gpe_variable(currStringToRender,currPosToParse);
                                                }
                                                tempParseTextToAdd->textEnd = currPosToParse;
                                                variableLineText->foundParses.push_back(tempParseTextToAdd);
                                                tempParseTextToAdd = new parsed_text(-1, -1);
                                            }
                                            else
                                            {
                                                foundGPEKeyword = main_syntax_highlighter->find_gpe_keyword(currStringToRender,currPosToParse);
                                                if( (foundGPEKeyword )!="")
                                                {
                                                    tempParseTextToAdd->textStart = currPosToParse;
                                                    while (currPosToParse < lineEnd && foundGPEKeyword !="")
                                                    {
                                                        if(foundGPEKeyword!="" )
                                                        {
                                                            currPosToParse+=(int)foundGPEKeyword.size();
                                                        }
                                                        else
                                                        {
                                                            currPosToParse++;
                                                        }
                                                        foundGPEKeyword = main_syntax_highlighter->find_gpe_keyword(currStringToRender,currPosToParse);
                                                    }
                                                    tempParseTextToAdd->textEnd = currPosToParse;
                                                    keywordLineText->foundParses.push_back(tempParseTextToAdd);
                                                    tempParseTextToAdd = new parsed_text(-1, -1);
                                                }
                                                else if ( stg_ex::char_is_alpha(currStringToRender[currPosToParse],false,true) )
                                                {
                                                    //color = pawgui::theme_main->text_box_font_color;
                                                    tempParseTextToAdd->textStart = currPosToParse;
                                                    currPosToParse++;
                                                    while (currPosToParse < lineEnd && stg_ex::char_is_alnum(currStringToRender[currPosToParse],false,true) )
                                                    {
                                                        currPosToParse++;
                                                    }
                                                    tempParseTextToAdd->textEnd = currPosToParse;
                                                    normalLineText->foundParses.push_back(tempParseTextToAdd);
                                                    tempParseTextToAdd = new parsed_text(-1, -1);
                                                }
                                                else
                                                {
                                                    if(currStringToRender[currPosToParse]!=' ')
                                                    {
                                                        //color = pawgui::theme_main->text_box_font_color;
                                                        //anything else is just regular text as well...
                                                        tempParseTextToAdd->textStart = currPosToParse;
                                                        tempParseTextToAdd->textEnd = currPosToParse+1;
                                                        normalLineText->foundParses.push_back(tempParseTextToAdd);
                                                    }
                                                    tempParseTextToAdd = new parsed_text(-1, -1);
                                                    currPosToParse++;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if( isInBlockCommentMode)
                    {
                        endBlockCommentPos = currStringToRender.find("*/");
                        if( endBlockCommentPos==(int)std::string::npos)
                        {
                            currPosToParse = lineEnd;
                        }
                        else
                        {
                            currPosToParse = endBlockCommentPos+2;
                            isInBlockCommentMode = false;
                        }
                        mLineComment->textEnd = currPosToParse;
                        commentLineText->foundParses.push_back(mLineComment);
                        mLineComment = new parsed_text(0, -1);
                    }
                    else if( isInfloatQuoteMode)
                    {
                        endDQuoteCommentPos = currStringToRender.find('"',currPosToParse);
                        if( endDQuoteCommentPos==(int)std::string::npos)
                        {
                            currPosToParse = lineEnd;
                        }
                        else
                        {
                            currPosToParse = endDQuoteCommentPos+1;
                            isInfloatQuoteMode = false;
                        }
                        dqLineComment->textEnd = currPosToParse;
                        dQuoteLineText->foundParses.push_back(dqLineComment);
                        dqLineComment = new parsed_text(0, -1);
                    }
                    else if( isInSingleQuoteMode)
                    {
                        endSQuoteCommentPos = currStringToRender.find("'",currPosToParse);
                        if( endSQuoteCommentPos==(int)std::string::npos)
                        {
                            currPosToParse = lineEnd;
                        }
                        else
                        {
                            currPosToParse = endSQuoteCommentPos+1;
                            isInSingleQuoteMode = false;
                        }
                        sqLineComment->textEnd = currPosToParse;
                        sQuoteLineText->foundParses.push_back(sqLineComment);
                        sqLineComment = new parsed_text(0, -1);
                    }
                }

                if ( i >=lineStartYPos  )
                {
                    textRenderXPos = renderBox->x+2;
                    textRenderYPos = renderBox->y+(i-lineStartYPos)*(editorZoomLevel*defaultLineHeight);

                    color = pawgui::theme_main->text_box_font_color;
                    normalLineText->render_tokens( font_textinput,currStringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    color = pawgui::theme_main->text_box_font_number_color;
                    numberLineText->render_tokens( font_textinput_number,currStringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color,true );

                    color = pawgui::theme_main->text_box_font_symbols_color;
                    symbolLineText->render_tokens( font_textinput_symbol,currStringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    //
                    if( colorblind_mode_on )
                    {
                        color = pawgui::theme_main->text_box_font_function_alt_color;
                    }
                    else
                    {
                        color = pawgui::theme_main->text_box_font_function_color;
                    }
                    functionLineText->render_tokens( font_textinput_function,currStringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    //
                    if( colorblind_mode_on)
                    {
                        color = pawgui::theme_main->text_box_font_keyword_alt_color;
                    }
                    else
                    {
                        color = pawgui::theme_main->text_box_font_keyword_color;
                    }
                    keywordLineText->render_tokens( font_textinput_keyword,currStringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    if( colorblind_mode_on)
                    {
                        color = pawgui::theme_main->text_box_project_keyword_alt_color;
                    }
                    else
                    {
                        color = pawgui::theme_main->text_box_project_keyword_color;
                    }
                    projectKeywordLineText->render_tokens( font_textinput,currStringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    if( colorblind_mode_on)
                    {
                        color = pawgui::theme_main->text_box_project_function_alt_color;
                    }
                    else
                    {
                        color = pawgui::theme_main->text_box_project_function_color;
                    }
                    projectFunctionLineText->render_tokens( font_textinput,currStringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );



                    if( colorblind_mode_on)
                    {
                        color = pawgui::theme_main->text_box_font_variable_alt_color;
                    }
                    else
                    {
                        color = pawgui::theme_main->text_box_font_variable_color;
                    }
                    variableLineText->render_tokens( font_textinput,currStringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    color = pawgui::theme_main->text_box_font_datatype_color;
                    datatypeLineText->render_tokens( font_textinput,currStringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    color = pawgui::theme_main->text_box_font_datatype_color;
                    datatypeLineText->render_tokens( font_textinput,currStringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    color = pawgui::theme_main->text_box_font_squote_color;
                    sQuoteLineText->render_tokens( font_textinput_quote,currStringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    color = pawgui::theme_main->text_box_font_dquote_color;
                    dQuoteLineText->render_tokens( font_textinput_quote,currStringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    color = pawgui::theme_main->text_box_font_comment_color;
                    commentLineText->render_tokens( font_textinput_comment,currStringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );
                }
            }

            if( mLineComment!=NULL)
            {
                delete mLineComment;
                mLineComment = NULL;
            }
            if( dqLineComment!=NULL)
            {
                delete dqLineComment;
                dqLineComment = NULL;
            }
            if( sqLineComment!=NULL)
            {
                delete sqLineComment;
                sqLineComment = NULL;
            }
            if( tempParseTextToAdd!=NULL)
            {
                delete tempParseTextToAdd;
                tempParseTextToAdd = NULL;
            }
        }
    }

    void widget_text_editor::render_linebox( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( view_space!=NULL && cam!=NULL )
        {
            //draws the line count box
            if( lineCountBoxWidth>0 && main_settings->showTextEditorLineCount )
            {
                gpe::gcanvas->render_rect(lineCountBox,pawgui::theme_main->text_box_outer_color,false);
                int lineSize = (editorZoomLevel*defaultLineHeight);
                int lineYPos = lineCountBox->y;
                for(  int i=lineStartYPos; i <= lineStartYPos+linesWithinView && i < (int)listOfStrings.size(); i++)
                {
                    if( i == cursorYPos)
                    {
                        gpe::gcanvas->render_rectangle( lineCountBox->x,lineYPos,lineCountBox->x + lineCountBox->w, lineYPos+lineSize,pawgui::theme_main->text_box_highlight_color,false);
                    }
                    gpe::gfs->render_text( lineCountBox->x+lineCountBox->w/2, lineYPos,stg_ex::int_to_string(i+1),pawgui::theme_main->text_box_font_color,font_textinput,gpe::fa_center,gpe::fa_top,255);
                    lineYPos+=lineSize;
                }
                gpe::gcanvas->render_rect( lineCountBox, pawgui::theme_main->text_box_outline_color,true);
            }
        }
    }

    void widget_text_editor::render_plain( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( view_space!=NULL && cam!=NULL && has_content() )
        {
            gpe::color * textColor = pawgui::theme_main->text_box_font_color;
            int foundSpecialLogPos = 0;
            int subCopyStartPos = 0;
            std::string stringToRender = "";
            for( int iLine=lineStartYPos; iLine <= lineStartYPos+linesWithinView && iLine < (int)listOfStrings.size(); iLine++)
            {
                stringToRender = listOfStrings[ iLine ];
                if( (int)stringToRender.size() > lineStartXPos )
                {
                    subCopyStartPos = 0;

                    textColor = pawgui::theme_main->text_box_font_color;
                    if( isTextLog)
                    {
                        textColor = pawgui::theme_main->text_box_font_color;
                        foundSpecialLogPos = stringToRender.find("Error:");
                        if( foundSpecialLogPos!=(int)std::string::npos)
                        {
                            textColor = pawgui::theme_main->text_box_font_color;
                            subCopyStartPos =7;
                        }
                        else
                        {
                            foundSpecialLogPos = stringToRender.find("Warning:");
                            if( foundSpecialLogPos!=(int)std::string::npos)
                            {
                                textColor = pawgui::theme_main->text_box_font_keyword_color;
                                subCopyStartPos =8;
                            }
                            else
                            {
                                foundSpecialLogPos = stringToRender.find("Comment:");
                                if( foundSpecialLogPos!=(int)std::string::npos)
                                {
                                    textColor = pawgui::theme_main->text_box_font_comment_color;
                                    subCopyStartPos =8;
                                }
                            }
                        }
                    }
                    stringToRender = stg_ex::get_substring(stringToRender,lineStartXPos,charactersWithinView );
                    //gpe::error_log->report("Rendering ["+stringToRender+"]..." );
                    gpe::gfs->render_text( renderBox->x+2,renderBox->y+4+(iLine-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                     stringToRender,textColor,font_textinput,gpe::fa_left,gpe::fa_top,255 );
                }
            }
        }
    }

    void widget_text_editor::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        setup_editor(view_space,cam);
        if( view_space!=NULL &&  cam!=NULL  )
        {
            int subCopyStartPos = 0;
            if( (int)listOfStrings.size()==0)
            {
                listOfStrings.push_back("");
                reset_self();
            }

            int mostCharactersOfText = get_most_characters_used();
            if( mostCharactersOfText > charactersWithinView && showYScroll )
            {
                mostCharactersOfText-=2;
            }


            int i = 0;

            std::string currentLineInView = "";
            redrawDelay++;
            //if( redrawDelay >= redrawDelayMax)
            {
                redrawDelay = 0;
            }

            gpe::gcanvas->render_rect( renderBox,pawgui::theme_main->text_box_color,false);
            if( has_content() )
            {
                //Calculates and highlights the symbols
                find_connected_symbols();
                if( symbolCursorXPos >= lineStartXPos && symbolCursorYPos >=lineStartYPos )
                {
                    gpe::gcanvas->render_rectangle(
                        renderBox->x+2+( std::min(mostCharactersOfText,symbolCursorXPos)-lineStartXPos)*TEXTBOX_FONT_SIZE_WIDTH,
                        renderBox->y+(symbolCursorYPos-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                        renderBox->x+2+( std::min(mostCharactersOfText,symbolCursorXPos)-lineStartXPos+1)*TEXTBOX_FONT_SIZE_WIDTH,
                        renderBox->y+(symbolCursorYPos-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight),
                        pawgui::theme_main->text_box_font_comment_color,false,64);
                }
                if( symbolEndCursorXPos >= lineStartXPos && symbolEndCursorYPos >=lineStartYPos )
                {
                    gpe::gcanvas->render_rectangle(
                        renderBox->x+2+( std::min(mostCharactersOfText,symbolEndCursorXPos)-lineStartXPos)*TEXTBOX_FONT_SIZE_WIDTH,
                        renderBox->y+(symbolEndCursorYPos-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                        renderBox->x+2+( std::min(mostCharactersOfText,symbolEndCursorXPos)-lineStartXPos+1)*TEXTBOX_FONT_SIZE_WIDTH,
                        renderBox->y+(symbolEndCursorYPos-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight),
                        pawgui::theme_main->text_box_font_comment_color,false,64);
                }

                //Renders the text highlights
                if(selectionCursorXPos!=selectionEndCursorXPos || selectionCursorYPos!=selectionEndCursorYPos)
                {
                    int minHighlightXPos = 0, maxHighlightXPos = 0;
                    int minHighlightYPos = std::min(selectionCursorYPos, selectionEndCursorYPos);
                    int maxHighlightYPos = std::max(selectionCursorYPos, selectionEndCursorYPos);

                    if( selectionCursorYPos > selectionEndCursorYPos)
                    {
                        minHighlightXPos = selectionEndCursorXPos;
                        maxHighlightXPos = selectionCursorXPos;
                    }
                    else
                    {
                        maxHighlightXPos = selectionEndCursorXPos;
                        minHighlightXPos = selectionCursorXPos;
                    }
                    int calculatedMax = minHighlightXPos-lineStartXPos;

                    if(minHighlightYPos !=maxHighlightYPos)
                    {
                        if( (int)listOfStrings.size() > minHighlightYPos && minHighlightYPos>=lineStartYPos && minHighlightYPos < lineStartYPos+linesWithinView )
                        {
                            if( (int)listOfStrings.at(minHighlightYPos).size() > lineStartXPos && ( minHighlightXPos <= lineStartXPos+charactersWithinView) )
                            {
                                gpe::gcanvas->render_rectangle(
                                    renderBox->x+2+std::max(0, calculatedMax)*TEXTBOX_FONT_SIZE_WIDTH,
                                    renderBox->y+(minHighlightYPos-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                    renderBox->x+2+( std::min(mostCharactersOfText,(int)listOfStrings.at(minHighlightYPos).size() )-lineStartXPos+1)*TEXTBOX_FONT_SIZE_WIDTH,
                                    renderBox->y+(minHighlightYPos-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight),
                                    pawgui::theme_main->text_box_highlight_color,false);
                            }
                            else
                            {
                                gpe::gcanvas->render_rectangle(
                                    renderBox->x+2,
                                    renderBox->y+(i-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                    renderBox->x+2+TEXTBOX_FONT_SIZE_WIDTH,
                                    renderBox->y+(i-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight),pawgui::theme_main->text_box_highlight_color,false);

                            }
                        }

                        if( maxHighlightYPos-minHighlightYPos > 1)
                        {
                            for( i = minHighlightYPos+1; i <=maxHighlightYPos-1 && i < (int)listOfStrings.size() && i <= lineStartYPos+linesWithinView; i++ )
                            {
                                if( i >=lineStartYPos  && minHighlightYPos < lineStartYPos+linesWithinView)
                                {
                                    if( (int)listOfStrings[i].size() > lineStartXPos )
                                    {
                                        gpe::gcanvas->render_rectangle(
                                            renderBox->x+2,
                                            renderBox->y+(i-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                            renderBox->x+2+( std::min( mostCharactersOfText,(int)listOfStrings[i].size() )-lineStartXPos )*TEXTBOX_FONT_SIZE_WIDTH,
                                            renderBox->y+(i-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight),pawgui::theme_main->text_box_highlight_color,false);
                                    }
                                    else
                                    {
                                        gpe::gcanvas->render_rectangle(
                                            renderBox->x+2,
                                            renderBox->y+(i-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                            renderBox->x+2+TEXTBOX_FONT_SIZE_WIDTH,
                                            renderBox->y+(i-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight),pawgui::theme_main->text_box_highlight_color,false);
                                    }
                                }
                            }
                        }

                        if( (int)listOfStrings.size() > maxHighlightYPos && maxHighlightYPos>=lineStartYPos  && maxHighlightYPos < lineStartYPos+linesWithinView )
                        {
                            std::string lastStrToHighlight = listOfStrings.at(maxHighlightYPos);
                            if( (int)lastStrToHighlight.size() > lineStartXPos && maxHighlightXPos >= lineStartXPos )
                            {
                                gpe::gcanvas->render_rectangle(
                                    renderBox->x+2,
                                    renderBox->y+(maxHighlightYPos-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                    renderBox->x+2+( std::min(mostCharactersOfText,maxHighlightXPos)-lineStartXPos)*TEXTBOX_FONT_SIZE_WIDTH,
                                    renderBox->y+(maxHighlightYPos-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight),
                                    pawgui::theme_main->text_box_highlight_color,false);
                            }
                            else
                            {
                                gpe::gcanvas->render_rectangle(
                                    renderBox->x+2,
                                    renderBox->y+(i-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                    renderBox->x+2+TEXTBOX_FONT_SIZE_WIDTH,
                                    renderBox->y+(i-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight),pawgui::theme_main->text_box_highlight_color,false);
                            }
                        }
                    }
                    else if( maxHighlightXPos >= lineStartXPos && maxHighlightYPos>=lineStartYPos && maxHighlightYPos < lineStartYPos+linesWithinView)
                    {
                        minHighlightXPos = std::min(mostCharactersOfText,minHighlightXPos);
                        maxHighlightXPos = std::min(mostCharactersOfText,maxHighlightXPos);

                        if( minHighlightXPos==maxHighlightXPos)
                        {
                            maxHighlightXPos++;
                        }
                        gpe::gcanvas->render_rectangle(
                            renderBox->x+2+( minHighlightXPos-lineStartXPos)*TEXTBOX_FONT_SIZE_WIDTH,
                            renderBox->y+(maxHighlightYPos-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                            renderBox->x+2+( maxHighlightXPos-lineStartXPos)*TEXTBOX_FONT_SIZE_WIDTH,
                            renderBox->y+(maxHighlightYPos-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight),
                            pawgui::theme_main->text_box_highlight_color,false);
                    }
                }
                //Renders the texts
                std::string currStringToRender = "";
                if( main_settings->showTextEditorSyntaxHightlight && isCodeEditor)
                {
                    //Codes
                    render_code( view_space,cam);
                }
                else
                {
                    //Codes
                    render_plain( view_space,cam);
                }



                //Renders the scrollbars
                if( showXScroll && textXScroll!=NULL)
                {
                    textXScroll->render_self( view_space,cam);
                }
                if( showYScroll && textYScroll!=NULL)
                {
                    textYScroll->render_self( view_space,cam);
                }

                //attempt to draw the cursor
                if( cursorYPos < (int) listOfStrings.size() )
                {
                    currentLineInView = listOfStrings[cursorYPos];
                }
            }



            gpe::gcanvas->render_rect( renderBox,pawgui::theme_main->text_box_outline_color,true);
            if( !isReadOnly && textEditorButtonBar!=NULL && showButtonBar )
            {
                textEditorButtonBar->render_self( view_space,cam);
            }

            //Renders the linebox seen on the left
            render_linebox( view_space, cam );
            if( isInUse && (prevCursorXPos >=lineStartXPos && prevCursorXPos <= lineStartXPos+charactersWithinView) &&  ( prevCursorYPos >=lineStartYPos && prevCursorYPos <= lineStartYPos+linesWithinView ) )
            {
                if( prevCursorXPos!=cursorXPos || prevCursorYPos!=cursorYPos )
                {
                    gpe::gcanvas->render_vertical_line_color( renderBox->x+2+(prevCursorXPos-lineStartXPos)*TEXTBOX_FONT_SIZE_WIDTH,
                                                     renderBox->y+(prevCursorYPos-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                                     renderBox->y+(prevCursorYPos-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight),
                                                     pawgui::theme_main->text_box_color);
                }
            }

            //renders the cursor blinking effect
            if( isInUse && (cursorXPos >=lineStartXPos && cursorXPos <= lineStartXPos+charactersWithinView) &&  ( cursorYPos >=lineStartYPos && cursorYPos <= lineStartYPos+linesWithinView ) )
            {
                if( showCursor)
                {
                    gpe::gcanvas->render_vertical_line_color( renderBox->x+2+(cursorXPos-lineStartXPos)*TEXTBOX_FONT_SIZE_WIDTH,
                                                     renderBox->y+(cursorYPos-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                                     renderBox->y+(cursorYPos-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight),
                                                     pawgui::theme_main->text_box_font_color);
                }
                else
                {
                    gpe::gcanvas->render_vertical_line_color( renderBox->x+2+(cursorXPos-lineStartXPos)*TEXTBOX_FONT_SIZE_WIDTH,
                                                     renderBox->y+(cursorYPos-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                                     renderBox->y+(cursorYPos-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight),
                                                     pawgui::theme_main->text_box_color);
                }
            }

            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->text_box_highlight_color,true);
                if( !isReadOnly)
                {
                    std::string editorFeedbackLine = "Line: "+ stg_ex::int_to_string(cursorYPos+1)+", Column: "+ stg_ex::int_to_string(cursorXPos+1);
                    editorFeedbackLine+=", MaxLines: "+ stg_ex::float_to_string(linesWithinView)+", MaxCharacters:"+ stg_ex::float_to_string( charactersWithinView)+")";
                    if( main_statusbar!=NULL)
                    {
                        main_statusbar->set_codestring(editorFeedbackLine);
                    }
                }
            }

            /*
            if( isInUse )
            {
                if( main_syntax_highlighter->codeBeingSuggested && (int)main_syntax_highlighter->suggestedCompilerTerms.size() > 0 )
                {
                    gpe::renderer_main->reset_viewpoint();
                    int iRendSuggestion = 0;
                    syntax_compiler_term * cTerm = NULL;
                    std::string fullPhraseToRender = "";
                    int suggestionRenderYPos = widget_box.y+view_space->y;
                    if( cursorYPos >= lineStartYPos)
                    {
                        if( cursorYPos+suggestedTextMaxInViewCount <= lineStartYPos+linesWithinView && suggestedTextMaxInViewCount < linesWithinView )
                        {
                            suggestionRenderYPos +=(cursorYPos-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight);
                            suggestionRenderYPos+=textEditorButtonBar->get_height();
                        }
                        else if( suggestedTextMaxInViewCount >= (int)suggestedCompilerTerms.size() )
                        {
                            suggestionRenderYPos += (cursorYPos-lineStartYPos-suggestedTextMaxInViewCount)*(editorZoomLevel*defaultLineHeight);
                            suggestionRenderYPos+=textEditorButtonBar->get_height();
                        }
                        else
                        {
                            suggestionRenderYPos+=(cursorYPos-lineStartYPos-(int)suggestedCompilerTerms.size() )*(editorZoomLevel*defaultLineHeight);
                            suggestionRenderYPos+=textEditorButtonBar->get_height();
                        }

                        int iSuggestedEntry = 0;
                        for( int iSuggestedEntry = iSuggestedStartPos;iSuggestedEntry < (int)suggestedCompilerTerms.size() && iSuggestedEntry <  iSuggestedStartPos+suggestedTextMaxInViewCount;iSuggestedEntry++ )
                        {
                            cTerm = suggestedCompilerTerms[iSuggestedEntry];
                            if( cTerm!=NULL)
                            {
                                if( cTerm->termType==cterm_function)
                                {
                                    if( (int)cTerm->termScope.size() > 0 && cTerm->termScope!="User Global" )
                                    {
                                        fullPhraseToRender = cTerm->termFunctionReturnType+" "+cTerm->termScope+"."+cTerm->termString+"("+cTerm->get_parameters()+")";
                                    }
                                    else
                                    {
                                        fullPhraseToRender = cTerm->termFunctionReturnType+" "+cTerm->termString+"("+cTerm->get_parameters()+")";
                                    }
                                }
                                else if( (int)cTerm->termScope.size() > 0 && cTerm->termScope!="User Global" )
                                {
                                    fullPhraseToRender = cTerm->termScope+"."+cTerm->termString;
                                }
                                else
                                {
                                    fullPhraseToRender = cTerm->termString;
                                }
                                if( iSuggestedEntry==iSuggestionPos)
                                {
                                    gpe::gcanvas->render_rectangle(  widget_box.x+view_space->x, suggestionRenderYPos+(editorZoomLevel*defaultLineHeight)*iRendSuggestion,widget_box.x+view_space->x+maxSuggestedTextWidth, suggestionRenderYPos+(editorZoomLevel*defaultLineHeight)*(iRendSuggestion+1),pawgui::theme_main->popup_box_highlight_color,false);
                                    render_only_text( widget_box.x+view_space->x+32, suggestionRenderYPos+(editorZoomLevel*defaultLineHeight)*iRendSuggestion,fullPhraseToRender,pawgui::theme_main->popup_box_highlight_font_color,font_textinput,gpe::fa_left,gpe::fa_top,255 );
                                }
                                else
                                {
                                    gpe::gcanvas->render_rectangle(  widget_box.x+view_space->x, suggestionRenderYPos+(editorZoomLevel*defaultLineHeight)*iRendSuggestion,widget_box.x+view_space->x+maxSuggestedTextWidth, suggestionRenderYPos+(editorZoomLevel*defaultLineHeight)*(iRendSuggestion+1),pawgui::theme_main->popup_box_color,false);
                                    render_only_text( widget_box.x+view_space->x+32, suggestionRenderYPos+(editorZoomLevel*defaultLineHeight)*iRendSuggestion,fullPhraseToRender,pawgui::theme_main->popup_box_font_color,font_textinput,gpe::fa_left,gpe::fa_top,255 );
                                }
                                iRendSuggestion++;
                            }
                        }
                        gpe::gcanvas->render_rectangle(  widget_box.x+view_space->x, suggestionRenderYPos,widget_box.x+view_space->x+maxSuggestedTextWidth, suggestionRenderYPos+default_line_height*(iRendSuggestion+1),pawgui::theme_main->popup_box_border_color,true);

                    }
                    else
                    {
                        codeBeingSuggested = false;
                        suggestedCompilerTerms.clear();
                    }
                    gpe::renderer_main->set_viewpoint( view_space );
                }
            }
            */
        }
    }


    int widget_text_editor::replace_all_found(std::string strToReplace, std::string newString)
    {
        widget_text_anchor * cAnchor = NULL;
        std::string tempLine = "";
        std::string changedLine = "";
        int copiesDeleted = 0;
        export_text_anchors( gpe::app_directory_name+"replace_test.txt");
        for( int iReplace = (int)anchorPositions.size()-1; iReplace >=0; iReplace--)
        {
            cAnchor = anchorPositions.at( iReplace);
            if( cAnchor!=NULL)
            {
                if( (int)listOfStrings.size() > cAnchor->lineNumber )
                {
                    tempLine = listOfStrings.at( cAnchor->lineNumber);
                    if( (int)tempLine.size() >= cAnchor->characterNumber+(int)strToReplace.size() )
                    {
                        changedLine = tempLine.substr(0,cAnchor->characterNumber)+newString+tempLine.substr(cAnchor->characterNumber+(int)strToReplace.size() );
                        listOfStrings[cAnchor->lineNumber] = changedLine;
                        copiesDeleted++;
                    }
                }
            }
        }
        showCursor = true;
        cursorTimer = 0;
        return copiesDeleted;
    }

    void widget_text_editor::reset_selection(int resetDirection)
    {
        if( resetDirection < 0)
        {
            if(selectionCursorXPos!=selectionEndCursorXPos  || selectionCursorYPos!=selectionEndCursorYPos )
            {
                if( selectionCursorYPos==selectionEndCursorYPos)
                {
                    cursorXPos = std::min(selectionCursorXPos, selectionEndCursorXPos);
                    cursorYPos = selectionCursorYPos;
                }
                else if( selectionCursorYPos > selectionEndCursorYPos)
                {
                    cursorXPos = selectionEndCursorXPos;
                    cursorYPos = selectionEndCursorYPos;
                }
                else
                {
                    cursorXPos = selectionCursorXPos;
                    cursorYPos = selectionCursorYPos;
                }
            }

        }
        if(  resetDirection > 0)
        {
            if(selectionCursorXPos!=selectionEndCursorXPos  || selectionCursorYPos!=selectionEndCursorYPos )
            {
                if( selectionCursorYPos==selectionEndCursorYPos)
                {
                    cursorXPos = std::max(selectionCursorXPos, selectionEndCursorXPos);
                    cursorYPos = selectionCursorYPos;
                }
                else if( selectionCursorYPos > selectionEndCursorYPos)
                {
                    cursorXPos = selectionCursorXPos;
                    cursorYPos = selectionCursorYPos;
                }
                else
                {
                    cursorXPos = selectionEndCursorXPos;
                    cursorYPos = selectionEndCursorYPos;
                }
            }
        }
        selectionCursorXPos = 0;
        selectionCursorYPos = 0;
        selectionEndCursorXPos = 0;
        selectionEndCursorYPos = 0;
        if( listOfStrings.size() <=0)
        {
            listOfStrings.push_back("");
            cursorXPos = 0;
            cursorYPos = 0;
        }
        if( cursorYPos >= (int)listOfStrings.size() )
        {

            cursorYPos= (int)listOfStrings.size() - 1;
            lineToEdit = listOfStrings[cursorYPos];
            cursorXPos = (int)lineToEdit.size();
        }
        else if( cursorXPos > (int)listOfStrings[cursorYPos].size() )
        {
            cursorXPos = (int)listOfStrings[cursorYPos].size();
        }
        showCursor = true;
        cursorTimer = 0;
    }

    void widget_text_editor::reset_self()
    {
        cursorXPos = 0;
        cursorYPos = 0;
        selectionCursorXPos = 0;
        selectionCursorYPos = 0;
        selectionEndCursorXPos = 0;
        selectionEndCursorYPos = 0;
        bscDelay = 0;
        delDelay = 0;
        enterDelay = 0;
        upDelay = 0;
        downDelay = 0;
        leftDelay = 0;
        rightDelay = 0;
        tabDelay=0;
        showCursor = true;
        cursorTimer = 0;
    }

    void widget_text_editor::revise_edit(int editId)
    {
        if( editId >=0 && editId < (int)savedHistory.size() )
        {
            widget_text_editor * tempPlace = savedHistory.at(editId);
            if( tempPlace!=NULL)
            {
                save_edit();
                copy_source(tempPlace,true);
            }
        }
    }

    void widget_text_editor::save_edit()
    {
        bool currentContentChanged = true;
        bool prevContentChanged = true;
        bool nextContentChanged = true;
        widget_text_editor * fTextArea = NULL;
        if( currentPositionInHistory > 0 && (int)savedHistory.size() > currentPositionInHistory-1 )
        {
            fTextArea = savedHistory.at(currentPositionInHistory-1);
            if( fTextArea!=NULL)
            {
                if( compare_with(fTextArea)==1)
                {
                    prevContentChanged = false;
                }
            }
        }

        if( currentPositionInHistory >= 0 && (int)savedHistory.size() > currentPositionInHistory )
        {
            fTextArea = savedHistory.at(currentPositionInHistory);
            if( fTextArea!=NULL)
            {
                if( compare_with(fTextArea)==1)
                {
                    currentContentChanged = false;
                }
            }
        }

        if( currentPositionInHistory >= 0 && (int)savedHistory.size() > currentPositionInHistory+1 )
        {
            fTextArea = savedHistory.at(currentPositionInHistory+1);
            if( fTextArea!=NULL)
            {
                if( compare_with(fTextArea)==1)
                {
                    nextContentChanged = false;
                }
            }
        }

        if( currentContentChanged && prevContentChanged && nextContentChanged)
        {
            //int prevSavePos = currentPositionInHistory;
            if( (int)savedHistory.size() >= 128 )
            {
                if( currentPositionInHistory < 32)
                {
                    //remove from right
                    savedHistory.erase(savedHistory.end());
                }
                else if( currentPositionInHistory > 96)
                {
                    //remove from left
                    savedHistory.erase(savedHistory.begin());
                    currentPositionInHistory--;
                }
                else
                {
                    if( currentPositionInHistory>=64)
                    {
                        savedHistory.erase(savedHistory.begin());
                        currentPositionInHistory--;
                    }
                    else
                    {
                        savedHistory.erase(savedHistory.end());
                    }
                }
            }

            widget_text_editor * savedText = new widget_text_editor(false);
            savedText->copy_source(this,true);
            if( currentPositionInHistory >=0 && currentPositionInHistory < (int)savedHistory.size() )
            {
                savedHistory.insert(savedHistory.begin()+currentPositionInHistory+1,savedText);
                currentPositionInHistory++;
            }
            else
            {
                savedHistory.push_back(savedText);
                currentPositionInHistory = savedHistory.size()-1;
            }
        }
        undoableActionOccurred = false;
    }

    void widget_text_editor::select_all()
    {
        cursorXPos = 0;
        cursorYPos = 0;
        selectionCursorXPos = 0;
        selectionCursorYPos = 0;
        selectionEndCursorXPos = 0;
        selectionEndCursorYPos = 0;
        int listSize = (int)listOfStrings.size() ;
        if( listSize > 0)
        {
            std::string tempStr = listOfStrings.back();
            selectionEndCursorYPos = listSize - 1;
            if( (int)tempStr.size() > 0)
            {
                selectionEndCursorXPos = tempStr.size();
            }
        }
        showCursor = true;
        cursorTimer = 0;
    }

    void widget_text_editor::scroll_to_pos(int lineNumberIn, int characterNumberIn)
    {
        if( showYScroll )
        {
            showYScroll = true;
            lineStartYPos = lineNumberIn;
            if( lineStartYPos+linesWithinView >= (int)listOfStrings.size() )
            {
                lineStartYPos =  (int)listOfStrings.size() - linesWithinView;
            }
            if( lineStartYPos < 0)
            {
                lineStartYPos = 0;
            }

        }
        if( showXScroll )
        {
            if( characterNumberIn > lineStartXPos+charactersWithinView)
            {
                lineStartXPos = characterNumberIn-charactersWithinView;
            }
            else if( characterNumberIn < lineStartXPos )
            {
                lineStartXPos = characterNumberIn;
            }

            if( lineStartXPos+charactersWithinView > get_most_characters_used() )
            {
                lineStartXPos =  get_most_characters_used() - charactersWithinView;
            }
            if( lineStartXPos < 0)
            {
                lineStartXPos = 0;
            }
        }
        else
        {
            lineStartXPos = 0;
        }
        showCursor = true;
        cursorTimer = 0;
    }

    void widget_text_editor::scroll_to_cursor()
    {
        if( cursorYPos >= lineStartYPos && cursorYPos <= lineStartYPos+linesWithinView && cursorYPos < (int)listOfStrings.size() )
        {
            if( cursorXPos < lineStartXPos || cursorXPos > lineStartXPos+charactersWithinView)
            {
                scroll_to_pos(lineStartYPos,cursorXPos);
            }
            else
            {
                scroll_to_pos(lineStartYPos, lineStartXPos);
            }
        }
        else
        {
            if( cursorXPos < lineStartXPos ||  cursorXPos > lineStartXPos+charactersWithinView)
            {
                scroll_to_pos(cursorYPos,cursorXPos);
            }
            else
            {
                scroll_to_pos(cursorYPos, lineStartXPos);
            }
        }
    }

    void widget_text_editor::scroll_to_selection()
    {
        scroll_to_pos( selectionCursorYPos, selectionCursorXPos);
    }

    void widget_text_editor::set_read_only()
    {
        isReadOnly = true;
        textEditorButtonBar->set_height(0);
    }

    void widget_text_editor::set_writeable()
    {
        isReadOnly = true;
        textEditorButtonBar->set_height( 24 );
    }

    void widget_text_editor::set_string(std::string newString)
    {

    }

    void widget_text_editor::set_placeholder(std::string newString)
    {

    }

    void widget_text_editor::set_xcursor(int newPos)
    {
        cursorXPos = newPos;
        std::string cLine = listOfStrings[cursorYPos];
        if( cursorXPos < 0 || cursorXPos < (int)cLine.size() )
        {
            cursorXPos = 0;
        }
        showCursor = true;
        cursorTimer = 0;
    }

    void widget_text_editor::set_ycursor(int newPos)
    {
        if( newPos >=0 && newPos < (int)listOfStrings.size() )
        {
            cursorYPos = newPos;
            std::string cLine = listOfStrings.at(newPos);
            if( cursorXPos < 0 || cursorXPos < (int)cLine.size() )
            {
                cursorXPos = 0;
            }
        }
        else
        {
            cursorYPos = 0;
            cursorXPos = 0;
        }
        showCursor = true;
        cursorTimer = 0;
    }

    void widget_text_editor::set_highlight_pos( gpe::shape_rect * view_space, gpe::shape_rect * cam  )
    {
        view_space = gpe::camera_find( view_space );
        cam = gpe::camera_find( cam );
        if( view_space!=NULL && cam!=NULL && isCodeEditor && main_syntax_highlighter!=NULL && !isReadOnly && isInUse )
        {
            if( main_syntax_highlighter->codeBeingSuggested && (int)main_syntax_highlighter->suggestedCompilerTerms.size() > 0 )
            {
                int suggestionRenderYPos = renderBox->y +view_space->y;
                if( cursorYPos >= lineStartYPos)
                {
                    if( cursorYPos+main_syntax_highlighter->suggestedTextMaxInViewCount <= lineStartYPos+linesWithinView && main_syntax_highlighter->suggestedTextMaxInViewCount < linesWithinView )
                    {
                        suggestionRenderYPos +=(cursorYPos-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight);
                        suggestionRenderYPos+=textEditorButtonBar->get_height();
                    }
                    else if( main_syntax_highlighter->suggestedTextMaxInViewCount >= (int)main_syntax_highlighter->suggestedCompilerTerms.size() )
                    {
                        suggestionRenderYPos += (cursorYPos-lineStartYPos-main_syntax_highlighter->suggestedTextMaxInViewCount)*(editorZoomLevel*defaultLineHeight);
                        suggestionRenderYPos+=textEditorButtonBar->get_height();
                    }
                    else
                    {
                        suggestionRenderYPos+=(cursorYPos-lineStartYPos-(int)main_syntax_highlighter->suggestedCompilerTerms.size() )*(editorZoomLevel*defaultLineHeight);
                        suggestionRenderYPos+=textEditorButtonBar->get_height();
                    }
                }
                main_syntax_highlighter->highlightedTermXPos = renderBox->x+view_space->x;
                main_syntax_highlighter->highlightedTermYPos = suggestionRenderYPos;
            }
            else if( main_syntax_highlighter->highlightedTerm!=NULL && highlightYPos >=lineStartYPos && highlightYPos <= lineStartYPos+linesWithinView+3 )
            {
                std::string fullTermDescription = "";
                //highlightedTerm = highlightedTerm;

                main_syntax_highlighter->highlightedTermXPos = renderBox->x+view_space->x+lineCountBoxWidth+( (highlightXPos -lineStartXPos)*TEXTBOX_FONT_SIZE_WIDTH);
                int suggestionRenderYPos = 0;

                if( highlightYPos > lineStartYPos+1)
                {
                    if( highlightYPos+3 <= lineStartYPos+linesWithinView)
                    {
                        suggestionRenderYPos = (highlightYPos-lineStartYPos+1)*default_line_height;
                    }
                    else
                    {
                        suggestionRenderYPos = (highlightYPos-lineStartYPos-4 )*default_line_height;
                    }
                }
                else
                {
                    suggestionRenderYPos=(highlightYPos-lineStartYPos+3)*default_line_height;
                }
                main_syntax_highlighter->highlightedTermYPos = renderBox->y+view_space->y+suggestionRenderYPos;
            }
        }
    }

    void widget_text_editor::setup_editor( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find( view_space );
        cam = gpe::camera_find( cam );
        if( main_settings->showTextEditorLineCount && isReadOnly==false )
        {
            lineCountBoxWidth = main_settings->defaultLineCountWidth;
            lineCountBox->x = widget_box.x - cam->x;
            lineCountBox->y = widget_box.y - cam->y;
            lineCountBox->w = main_settings->defaultLineCountWidth;
            lineCountBox->h = widget_box.h;

        }
        else
        {
            lineCountBoxWidth = 0;
            lineCountBox->x = 0;
            lineCountBox->y = 0;
            lineCountBox->w = 0;
            lineCountBox->h = 0;
        }

        if( textEditorButtonBar!=NULL)
        {
            textEditorButtonBar->set_coords(widget_box.x,widget_box.y);
            //sets the buttonbar to the width of the text editor( minus width of yScroll width[16 ).
            textEditorButtonBar->set_width(widget_box.w);
            if( !isReadOnly)
            {
                textEditorButtonBar->set_height(24);
                textEditorButtonBar->enable_self();
            }
            else
            {
                textEditorButtonBar->set_height(0);
                textEditorButtonBar->disable_self();
            }

            renderBox->x = widget_box.x+lineCountBoxWidth -cam->x;
            renderBox->y = widget_box.y+ textEditorButtonBar->get_height() -cam->y;
            lineCountBox->y = widget_box.y + textEditorButtonBar->get_height() - cam->y;
            lineCountBox->h = renderBox->h = widget_box.h - textEditorButtonBar->get_height();
        }
        else
        {
            renderBox->x = widget_box.x+lineCountBoxWidth -cam->x;
            renderBox->y = widget_box.y -cam->y;
            renderBox->h = widget_box.h;
        }

        renderBox->w = widget_box.w-lineCountBoxWidth;


        textSpaceRect->x = view_space->x + renderBox->x;
        textSpaceRect->y = view_space->y + renderBox->y;
        textSpaceRect->w = renderBox->w;
        textSpaceRect->h = renderBox->h;
    }

    void widget_text_editor::undo_edit()
    {
        if( can_undo() )
        {
            int editId = currentPositionInHistory;
            /*
            if( undoableActionOccurred)
            {
                save_edit();
            }
            if( currentPositionInHistory > editId)
            {
                currentPositionInHistory--;
            }
            */
            widget_text_editor * tempPlace = NULL;
            if( currentPositionInHistory-1 >=0 && currentPositionInHistory-1 < (int)savedHistory.size() )
            {
                tempPlace = savedHistory.at(currentPositionInHistory-1);
                if( tempPlace!=NULL)
                {
                    copy_source(tempPlace,true);
                    currentPositionInHistory--;
                }
            }
            else if( editId==(int)savedHistory.size() && (int)savedHistory.size()>0)
            {
                save_edit();
            }
            showCursor = true;
            cursorTimer = 0;
        }
    }

    void widget_text_editor::update_cursor_to_mouse( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        find_mouse_cursor(&cursorXPos, &cursorYPos, view_space, cam);
        lineToEdit = listOfStrings[cursorYPos];
        showCursor = true;
        cursorTimer = 0;
        adjust_fortabs();
    }

    bool widget_text_editor::write_data_into_file(std::ofstream * fileTarget, int leftTabAmount, bool useNewLines, bool useLastNewLine )
    {
        if( fileTarget!=NULL && leftTabAmount >=0)
        {
            std::string nestedTabsStr = generate_tabs( leftTabAmount  );
            if( fileTarget->is_open() )
            {
                int strSize = (int)listOfStrings.size();

                for( int i = 0; i < strSize-1; i++)
                {
                    if( useNewLines)
                    {
                        *fileTarget << nestedTabsStr+listOfStrings[i] << "\n";
                    }
                    else
                    {
                        *fileTarget <<nestedTabsStr+listOfStrings[i] << " ";
                    }
                }
                if( useNewLines)
                {
                    if( useLastNewLine)
                    {
                        *fileTarget << nestedTabsStr+listOfStrings[strSize-1] << "\n";
                    }
                    else
                    {
                        *fileTarget << nestedTabsStr+listOfStrings[strSize-1];
                    }
                }
                else
                {
                    *fileTarget <<nestedTabsStr+listOfStrings[strSize-1] << " ";
                }
                return true;
            }
        }
        return false;
    }

    bool widget_text_editor::write_short_data_into_file(std::ofstream * fileTarget, int leftTabAmount, bool useNewLines, bool useLastNewLine )
    {
        if( fileTarget!=NULL && leftTabAmount >=0)
        {
            std::string nestedTabsStr = generate_tabs( leftTabAmount  );
            if( fileTarget->is_open() )
            {
                int strSize = (int)listOfStrings.size();
                std::string currentLineToExport = "";
                for( int i = 0; i < strSize-1; i++)
                {
                    currentLineToExport= listOfStrings[i];
                    if( (int)currentLineToExport.size() > 0)
                    {
                        if( useNewLines)
                        {
                            *fileTarget << nestedTabsStr+currentLineToExport << "\n";
                        }
                        else
                        {
                            *fileTarget <<nestedTabsStr+currentLineToExport << " ";
                        }
                    }
                }
                if( useNewLines)
                {
                    if( useLastNewLine)
                    {
                        *fileTarget << nestedTabsStr+listOfStrings[strSize-1] << "\n";
                    }
                    else
                    {
                        *fileTarget << nestedTabsStr+listOfStrings[strSize-1];
                    }
                }
                else
                {
                    *fileTarget <<nestedTabsStr+listOfStrings[strSize-1] << " ";
                }
                return true;
            }
        }
        return false;
    }

    widget_text_editor_wrapped::widget_text_editor_wrapped()
    {
        widget_type = "wrappedtextarea";
        set_read_only();
        isCodeEditor = false;
        widget_box.w = 640;
        widget_box.h = 480;
        paragraphText = "";
    }

    widget_text_editor_wrapped::~widget_text_editor_wrapped()
    {

    }

    void widget_text_editor_wrapped::set_string( std::string strIn)
    {
        if( paragraphText!=strIn)
        {
            paragraphText = strIn;
            update_paragraph();
        }
    }

    void widget_text_editor_wrapped::set_width(int newWidth)
    {
        if( widget_box.w!=newWidth)
        {
            widget_box.w = newWidth;
            update_paragraph();
        }
    }

    void widget_text_editor_wrapped::set_height( int newHeight)
    {
        if( widget_box.h!=newHeight)
        {
            widget_box.h = newHeight;
            update_paragraph();
        }
    }

    void widget_text_editor_wrapped::set_max_width( int nMW)
    {
        if( widget_box.w!=nMW )
        {
            widget_box.w = nMW;
            update_paragraph();
        }
    }

    void widget_text_editor_wrapped::set_max_height( int nMH)
    {
        if( widget_box.h!=nMH )
        {
            widget_box.h = nMH;
            update_paragraph();
        }
    }

    void widget_text_editor_wrapped::update_paragraph(int foundMaxWidth )
    {
        if( widget_box.w > 0 )
        {
            reset_self();
            clear_all_lines();
            if( (int)paragraphText.size() > 0)
            {
                int maxMessageWidth = 0;
                //int maxMessageHeight = 0;
                int defaultFontWidth = 0;
                int defaultFontHeight = 0;
                int iSubMessage = 0;
                if( font_textinput!=NULL)
                {
                    font_textinput->get_metrics("A",&defaultFontWidth, &defaultFontHeight);
                    font_textinput->clear_cache();
                }

                std::vector < std::string > messageSubTitles;

                if( defaultFontWidth > 0 && defaultFontHeight > 0)
                {
                    maxMessageWidth = ( widget_box.w -padding_default)/ defaultFontWidth;

                    if( (int)paragraphText.size() > 0)
                    {
                        stg_ex::wrap_string(paragraphText,messageSubTitles,maxMessageWidth,-1);
                    }

                    maxMessageWidth = 0;
                    if( messageSubTitles.size() <= 0)
                    {
                        add_line("");
                    }
                    else
                    {
                        for( iSubMessage = 0; iSubMessage < (int)messageSubTitles.size(); iSubMessage++)
                        {
                            add_line( messageSubTitles.at(iSubMessage) );
                        }
                    }
                }
            }
            else
            {
                add_line("");
            }
        }
    }
}
