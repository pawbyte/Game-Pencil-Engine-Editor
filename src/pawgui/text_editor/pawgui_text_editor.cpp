/*
pawgui_text_editor.cpp
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

#include "../context/pawgui_context.h"
#include "../file_popups/pawgui_file_popups.h"
#include "../general_resource/pawgui_general_resource_container.h"
#include "../search_controller/pawgui_search_controller.h"
#include "../statusbar/pawgui_statusbar.h"
#include "../text_editor/pawgui_text_editor.h"

namespace pawgui
{
    widget_text_anchor_controller * main_anchor_controller  = nullptr;

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
        if( main_anchor_controller!=nullptr)
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
        if( FONT_LABEL_ANCHOR!=nullptr)
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
        if( (int)widget_name.size() > 0 && view_space!=nullptr && cam!=nullptr )
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
            widget_text_anchor * tempAnchor = nullptr;
            int gcSize =  loggedAnchors.size();
            for( int gcItr = gcSize-1; gcItr >=0; gcItr-- )
            {
                tempAnchor =  loggedAnchors.at( gcItr );
                if( tempAnchor !=nullptr )
                {
                    delete tempAnchor;
                    tempAnchor = nullptr;

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
        show_buttonBar = true;
        isReadOnly = false;
        isCodeEditor = false;
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
        textInputstring = "";
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
        if( font_textinput!=nullptr)
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

        textEditor_buttonBar = nullptr;
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
        listOfstrings.push_back("");
        widget_box.w = 640;
        widget_box.h = 480;
        if( saveFirstEdit)
        {
            save_edit();
        }
    }

    widget_text_editor::~widget_text_editor()
    {
        if( textSpaceRect!=nullptr)
        {
            delete textSpaceRect;
            textSpaceRect = nullptr;
        }
        if( textXScroll!=nullptr)
        {
            delete textXScroll;
            textXScroll = nullptr;
        }

        if( textYScroll!=nullptr)
        {
            delete textYScroll;
            textYScroll= nullptr;
        }
        if( textEditor_buttonBar!=nullptr)
        {
            delete textEditor_buttonBar;
            textEditor_buttonBar = nullptr;
        }
        if( commentLineText!=nullptr)
        {
            delete commentLineText;
            commentLineText = nullptr;
        }
        if( datatypeLineText!=nullptr)
        {
            delete datatypeLineText;
            datatypeLineText = nullptr;
        }
        if( dQuoteLineText!=nullptr)
        {
            delete dQuoteLineText;
            dQuoteLineText = nullptr;
        }
        if( functionLineText!=nullptr)
        {
            delete functionLineText;
            functionLineText = nullptr;
        }
        if( keywordLineText!=nullptr)
        {
            delete keywordLineText;
            keywordLineText = nullptr;
        }
        if( normalLineText!=nullptr)
        {
            delete normalLineText;
            normalLineText = nullptr;
        }

        if( numberLineText!=nullptr)
        {
            delete numberLineText;
            numberLineText = nullptr;
        }

        if( sQuoteLineText!=nullptr)
        {
            delete sQuoteLineText;
            sQuoteLineText = nullptr;
        }

        if( symbolLineText!=nullptr)
        {
            delete symbolLineText;
            symbolLineText = nullptr;
        }

        if( projectFunctionLineText!=nullptr)
        {
            delete projectFunctionLineText;
            projectFunctionLineText = nullptr;
        }

        if( projectKeywordLineText!=nullptr)
        {
            delete projectKeywordLineText;
            projectKeywordLineText = nullptr;
        }
        if( variableLineText!=nullptr)
        {
            delete variableLineText;
            variableLineText = nullptr;
        }
        int iD = 0;
        widget_text_editor * tempText = nullptr;
        for( iD = (int)savedHistory.size()-1; iD >=0; iD--)
        {
            tempText = savedHistory.at(iD);
            if( tempText!=nullptr)
            {
                delete tempText;
                tempText = nullptr;
            }
        }

        widget_text_anchor * tempAnchor = nullptr;
        for( iD = (int)anchorPositions.size()-1; iD >=0; iD--)
        {
            tempAnchor = anchorPositions.at(iD);
            if( tempAnchor!=nullptr)
            {
                delete tempAnchor;
                tempAnchor = nullptr;
            }
        }
        savedHistory.clear();
        listOfstrings.clear();
        if( textEditor_buttonBar!=nullptr)
        {
            delete textEditor_buttonBar;
            textEditor_buttonBar = nullptr;
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
        listOfstrings.push_back( stg_ex::trim_right_inplace(newLine) );
        if( scrollToBottom)
        {
            scroll_to_pos( (int)listOfstrings.size()-1, 0 );
        }
    }

    void widget_text_editor::adjust_fortabs()
    {
        /*
            if( cursorXPos > 0 && main_settings!=nullptr && main_settings->autoFindMouseTabs )
            {
                if( (int)listOfstrings.size() > cursorYPos )
                {
                    std::string currStrToCheck = listOfstrings[cursorYPos];
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
        listOfstrings.clear();
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
            widget_text_anchor * tAnchor = nullptr;
            for( int i = (int)anchorPositions.size()-1; i >=0; i--)
            {
                tAnchor = anchorPositions[i];
                delete tAnchor;
                tAnchor = nullptr;
            }
            anchorPositions.clear();
        }
    }

    void widget_text_editor::create_buttonbar()
    {
        if( textEditor_buttonBar!=nullptr)
        {
            delete textEditor_buttonBar;
            textEditor_buttonBar = nullptr;
        }
        textEditor_buttonBar = new widget_button_iconbar( 16);
        textEditor_buttonBar->set_height(32);
        textEditor_buttonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/save.png","Export Text",TEXTAREA_OPTION_EXPORT,false );
        textEditor_buttonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file.png","Import Text",TEXTAREA_OPTION_IMPORT,true );
        textEditor_buttonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/eraser.png","Clear Text ",TEXTAREA_OPTION_CLEAR );
        textEditor_buttonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/backward.png","Undo Action",TEXTAREA_OPTION_UNDO,false );
        textEditor_buttonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/forward.png","Redo Action",TEXTAREA_OPTION_REDO,true );

        textEditor_buttonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cut.png","Cut",TEXTAREA_OPTION_CUT,false );
        textEditor_buttonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/copy.png","Copy",TEXTAREA_OPTION_COPY,true );
        textEditor_buttonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/paste.png","Paste",TEXTAREA_OPTION_PASTE,true );

        //textEditor_buttonBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/info.png","Text Info (Not Available Yet)",TEXTAREA_OPTION_TEXTINFO,true );
        textEditor_buttonBar->limit_width(true);
    }

    void widget_text_editor::copy_selection()
    {
        if( selectionCursorYPos >=0 && selectionCursorYPos < (int)listOfstrings.size() && selectionEndCursorYPos >=0 && selectionEndCursorYPos < (int)listOfstrings.size() )
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
                    tempStrToUse = listOfstrings.at(minHighlightYPos);
                    strToCopy = stg_ex::get_substring(tempStrToUse,minHighlightXPos,(int)std::string::npos);
                    if( maxHighlightYPos-minHighlightYPos > 1)
                    {
                        for( int i= minHighlightYPos+1; i <=maxHighlightYPos-1; i++ )
                        {
                            tempStrToUse = listOfstrings[i];
                            strToCopy += "\n"+tempStrToUse;
                        }
                    }
                    tempStrToUse = listOfstrings.at(maxHighlightYPos);
                    strToCopy += "\n"+stg_ex::get_substring(tempStrToUse,0,maxHighlightXPos);
                }
                else
                {
                    strToCopy = listOfstrings.at(selectionCursorYPos);
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
        if( fileTarget!=nullptr && leftTabAmount >=0)
        {
            if( fileTarget->is_open() )
            {
                std::string nestedTabsStr = generate_tabs( leftTabAmount  );
                if( !parse_code_javascript() )
                {
                    return false;
                }
                int strSize = (int)listOfstrings.size();

                for( int i = 0; i < strSize-1; i++)
                {
                    if( (int)listOfstrings[i].size() > 0 )
                    {
                        if( useNewLines)
                        {
                            *fileTarget << nestedTabsStr+listOfstrings[i] << "\n";
                        }
                        else
                        {
                            *fileTarget <<nestedTabsStr+listOfstrings[i] << " ";
                        }
                    }
                }
                if( (int)listOfstrings[strSize-1].size() > 0 )
                {
                    if( useNewLines)
                    {
                        if( useLastNewLine)
                        {
                            *fileTarget << nestedTabsStr+listOfstrings[strSize-1] << "\n";
                        }
                        else
                        {
                            *fileTarget << nestedTabsStr+listOfstrings[strSize-1];
                        }
                    }
                    else
                    {
                        *fileTarget <<nestedTabsStr+listOfstrings[strSize-1] << " ";
                    }
                }
                return true;
            }
        }
        return false;
    }

    int widget_text_editor::compare_with(widget_text_editor * other)
    {
        if( other!=nullptr)
        {
            if( (int)listOfstrings.size() == (int)other->listOfstrings.size()  && (int)listOfstrings.size() > 0 )
            {
                std::string mystring = "";
                std::string otherstring = "";
                for( int i = (int)listOfstrings.size()-1; i>=0; i--)
                {
                    mystring = listOfstrings[i];
                    otherstring = other->listOfstrings[i];
                    if( (int)mystring.size()==(int)otherstring.size() )
                    {
                        if( mystring.compare(otherstring)!=0 )
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
            for( int i = 0; i < (int)listOfstrings.size(); i++)
            {
                superLongClipboardStr+=listOfstrings[i]+"\n";
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
        if( other!=nullptr)
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

            for( int i = 0; i < (int)other->listOfstrings.size(); i++)
            {
                listOfstrings.push_back( other->listOfstrings[i] );
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
            if( selectionCursorYPos >=0 && selectionCursorYPos < (int)listOfstrings.size() && selectionEndCursorYPos >=0 && selectionEndCursorYPos < (int)listOfstrings.size() )
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
                            tempStrToUse = listOfstrings.at(minHighlightYPos);
                            if( minHighlightXPos >= 0)
                            {
                                strToCopy = stg_ex::get_substring(tempStrToUse,0,minHighlightXPos);
                            }
                            tempStrToUse = listOfstrings.at(maxHighlightYPos);
                            if( maxHighlightXPos <= (int)tempStrToUse.size() )
                            {
                                strToCopy += stg_ex::get_substring(tempStrToUse,maxHighlightXPos);
                            }
                            for( int i=maxHighlightYPos; i >= minHighlightYPos; i-- )
                            {
                                listOfstrings.erase(listOfstrings.begin()+i);
                            }
                            if( (int)strToCopy.size() > 0)
                            {
                                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = minHighlightXPos;
                                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = minHighlightYPos;
                                listOfstrings.insert(listOfstrings.begin()+minHighlightYPos,strToCopy);
                            }
                            else if( (int)listOfstrings.size() > minHighlightYPos)
                            {
                                listOfstrings.insert(listOfstrings.begin()+minHighlightYPos,"");
                                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = minHighlightYPos;
                            }
                            else
                            {
                                listOfstrings.push_back("");
                                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = (int)listOfstrings.size()-1;
                            }
                        }
                        else
                        {
                            minHighlightXPos = std::min(selectionCursorXPos, selectionEndCursorXPos);
                            maxHighlightXPos = std::max(selectionCursorXPos, selectionEndCursorXPos);
                            tempStrToUse = listOfstrings.at(selectionCursorYPos);
                            strToCopy = stg_ex::get_substring(tempStrToUse,0,minHighlightXPos);
                            strToCopy += stg_ex::get_substring(tempStrToUse,maxHighlightXPos);
                            listOfstrings.erase(listOfstrings.begin()+minHighlightYPos);
                            if( strToCopy.size() > 0)
                            {
                                listOfstrings.insert(listOfstrings.begin()+minHighlightYPos,strToCopy);
                                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = minHighlightXPos;
                                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = minHighlightYPos;
                            }
                            else if( (int)listOfstrings.size() > minHighlightYPos)
                            {
                                listOfstrings.insert(listOfstrings.begin()+minHighlightYPos,"");
                                tempStrToUse = listOfstrings.at(minHighlightYPos);
                                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = minHighlightYPos;
                            }
                            else
                            {
                                listOfstrings.push_back("");
                                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = (int)listOfstrings.size()-1;
                            }
                        }
                        save_edit();
                    }
                }

                if( listOfstrings.size() <=0)
                {
                    listOfstrings.push_back("");
                    cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                    cursorYPos = selectionCursorYPos = selectionEndCursorYPos = 0;
                    save_edit();
                }
                if( cursorXPos < 0)
                {
                    cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                    cursorYPos = selectionCursorYPos = selectionEndCursorYPos = (int)listOfstrings.size()-1;
                }
                lineToEdit = listOfstrings[cursorYPos];

            }
            else
            {
                cursorXPos = selectionCursorXPos = selectionEndCursorXPos = 0;
                cursorYPos = selectionCursorYPos = selectionEndCursorYPos = 0;
                lineToEdit = listOfstrings[cursorYPos];
            }
        }
    }

    void widget_text_editor::duplicate_text()
    {
        if( !isReadOnly)
        {
            std::string prevClipboard = gpe::input->clipboard_string();

            if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
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
                    std::string cLineToCopy = listOfstrings[cursorYPos];
                    listOfstrings.insert(listOfstrings.begin()+cursorYPos+1,cLineToCopy);
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
                int strSize = (int)listOfstrings.size();
                for( int i = 0; i < strSize; i++)
                {
                    if( i !=strSize-1)
                    {
                        myfile << listOfstrings[i] << "\n";
                    }
                    else
                    {
                        myfile << listOfstrings[i];
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
                widget_text_anchor * tAnchor = nullptr;
                for( int i = 0; i < strSize; i++)
                {
                    tAnchor = anchorPositions[i];
                    if( tAnchor!=nullptr)
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
            widget_text_anchor * nTextAnchor = nullptr;
            clear_text_anchors();
            cursorXPos = 0;
            cursorYPos = 0;
            clear_text_anchors();
            while( find_string(strTarget,true,matchCase,false) ==true)
            {
                //add to list
                stringsFoundInSearch++;
                if( addAnchor && main_anchor_controller!=nullptr )
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
        if( view_space!=nullptr && cam!=nullptr && (int)listOfstrings.size() > 0)
        {
            if( gpe::point_within_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y, textSpaceRect)  )
            {
                *mYCursor = (float)(gpe::input->mouse_position_y - textSpaceRect->y )/( defaultLineHeight * editorZoomLevel); //gets the widget_box.y pos essentially
                *mYCursor+=lineStartYPos;
                if( *mYCursor < lineStartYPos)
                {
                    *mYCursor = lineStartYPos;
                }
                if( *mYCursor >= (int)listOfstrings.size() )
                {
                    *mYCursor = (int)listOfstrings.size() - 1;
                }

                if( *mYCursor < 0 )
                {
                    *mYCursor = 0;
                }
                if( (int)listOfstrings.size()==0)
                {
                    listOfstrings.push_back("");
                }
                *mXCursor = (float)(gpe::input->mouse_position_x - textSpaceRect->x )/( TEXTBOX_FONT_SIZE_WIDTH * editorZoomLevel);
                mXCursor+=  (int)lineStartXPos;
                std::string newstring = listOfstrings.at(*mYCursor);
                int maxCursorSpace = (int)newstring.size();
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
        if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
        {
            lineToEdit =listOfstrings[cursorYPos];
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
                    std::string searchedStr = listOfstrings[seekedCursorYPos];
                    std::string tempStr;
                    int prevFoundXPos = 0;
                    if( searchForEndSymbol)
                    {
                        foundOtherMatches = 0;
                        foundSeekedMatches = 0;
                        prevFoundXPos = symbolCursorXPos+1;
                        while( seekedCursorYPos < (int)listOfstrings.size() )
                        {
                            searchedStr = listOfstrings[seekedCursorYPos];

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
                        else if( seekedCursorYPos > 0 && seekedCursorYPos <(int)listOfstrings.size() )
                        {
                            seekedCursorYPos -=1;
                            searchedStr = listOfstrings[seekedCursorYPos];
                            prevFoundXPos = (int)searchedStr.size()-1;
                        }
                        else
                        {
                            return false;
                        }

                        int cSearchCursorXPos = 0;

                        while( seekedCursorYPos >= 0 )
                        {
                            searchedStr = listOfstrings[seekedCursorYPos];

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
                            if( seekedCursorYPos>=0 && seekedCursorYPos < (int)listOfstrings.size() )
                            {
                                searchedStr = listOfstrings[seekedCursorYPos];
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
                if( searchCursorYPos <0 || searchCursorYPos >= (int)listOfstrings.size() )
                {
                    searchCursorYPos = 0;
                }
                strToSearch = listOfstrings.at(searchCursorYPos);
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
                    for( int i = searchCursorYPos+1; i < (int)listOfstrings.size(); i++)
                    {
                        strToSearch = listOfstrings[i];
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
                    searchCursorYPos = listOfstrings.size()-1;
                    searchCursorXPos = (listOfstrings.at(searchCursorYPos).size() );
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
        if( main_syntax_highlighter!=nullptr )
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

        if( tCursorY >=(int)listOfstrings.size() )
        {
            tCursorY = (int)listOfstrings.size() -1;
        }


        if( !main_syntax_highlighter->codeBeingSuggested && tCursorY >=0 && tCursorY < (int)listOfstrings.size() && isCodeEditor)
        {
            std::string currentstringInView = "";
            tempCLineXStartPos = 0;
            tempCLineXEndPos = tCursorX;
            //int tempCLineYPos = tCursorY;
            std::string cLineToParse = listOfstrings.at(tCursorY);
            syntax_compiler_term * tempTerm = nullptr;
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
                        currentstringInView = stg_ex::get_substring(cLineToParse, tempCLineXStartPos, tempCLineXEndPos-tempCLineXStartPos+1);

                        //Avoids rechecking if term is already being highlighted
                        if( main_syntax_highlighter->highlightedTerm!=nullptr)
                        {
                            if( main_syntax_highlighter->highlightedTerm->termstring == currentstringInView)
                            {
                                main_syntax_highlighter->documentationIsBeingShown = true;
                                return;
                            }
                            else
                            {
                                main_syntax_highlighter->highlightedTerm = nullptr;
                            }
                        }

                        if( codeEditorType==0)
                        {
                            for( i = main_syntax_highlighter->activeProjectKeywords.size()-1; i>=0; i--)
                            {
                                tempTerm = main_syntax_highlighter->activeProjectKeywords[i];
                                if( tempTerm!=nullptr)
                                {
                                    if( tempTerm->termstring==currentstringInView)
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
                                if( tempTerm!=nullptr)
                                {
                                    if( tempTerm->termstring == currentstringInView)
                                    {
                                        main_syntax_highlighter->highlightedTerm = tempTerm;
                                        main_syntax_highlighter->documentationIsBeingShown = true;
                                        return;
                                    }
                                }
                            }
                            tempTerm = main_syntax_highlighter->find_matching_function( currentstringInView);
                            if( tempTerm!=nullptr)
                            {
                                main_syntax_highlighter->highlightedTerm = tempTerm;
                                main_syntax_highlighter->documentationIsBeingShown = true;
                                return;
                            }

                            tempTerm = main_syntax_highlighter->find_matching_variable( currentstringInView);
                            if( tempTerm!=nullptr)
                            {
                                main_syntax_highlighter->highlightedTerm = tempTerm;
                                main_syntax_highlighter->documentationIsBeingShown = true;
                                return;
                            }
                        }
                    }
                    else
                    {
                        main_syntax_highlighter->highlightedTerm = nullptr;
                    }
                }
                else
                {
                    main_syntax_highlighter->highlightedTerm = nullptr;
                }
            }
            else
            {
                main_syntax_highlighter->highlightedTerm = nullptr;
            }
        }
        else
        {
            main_syntax_highlighter->highlightedTerm = nullptr;
        }
    }

    void widget_text_editor::find_suggested_text()
    {
        //Avoids function if the main highlighter class is "somehow nullptr"
        if( main_syntax_highlighter==nullptr)
        {
            return;
        }

        main_syntax_highlighter->clear_suggestions();
        int  i = 0;
        if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
        {
            std::string currentstringInView = "";
            tempCLineXStartPos = 0;
            tempCLineXEndPos = cursorXPos;
            //int tempCLineYPos = cursorYPos;
            std::string cLineToParse = listOfstrings[cursorYPos];
            syntax_compiler_term * tempTerm = nullptr;
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
                        currentstringInView = cLineToParse.substr( tempCLineXStartPos, tempCLineXEndPos-tempCLineXStartPos+1);
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
                                if( tempTerm!=nullptr)
                                {
                                    if( stg_ex::string_starts(tempTerm->termstring,currentstringInView) )
                                    {
                                        main_syntax_highlighter->suggestedCompilerTerms.push_back(tempTerm);
                                    }
                                }
                            }

                            for( i = main_syntax_highlighter->activeProjectKeywords.size()-1; i>=0; i--)
                            {
                                tempTerm = main_syntax_highlighter->activeProjectKeywords[i];
                                if( tempTerm!=nullptr)
                                {
                                    if( stg_ex::string_starts(tempTerm->termstring,currentstringInView) )
                                    {
                                        main_syntax_highlighter->suggestedCompilerTerms.push_back(tempTerm);
                                    }
                                }
                            }

                            //Language suggestions
                            if( main_syntax_highlighter!=nullptr && main_syntax_highlighter->defaultLanguage!=nullptr)
                            {
                                for( i = main_syntax_highlighter->defaultLanguage->languageConstants.size()-1; i>=0; i--)
                                {
                                    tempTerm = main_syntax_highlighter->defaultLanguage->languageConstants[i];
                                    if( tempTerm!=nullptr)
                                    {
                                        if( stg_ex::string_starts(tempTerm->termstring,currentstringInView) )
                                        {
                                            main_syntax_highlighter->suggestedCompilerTerms.push_back(tempTerm);
                                        }
                                    }
                                }

                                for( i = main_syntax_highlighter->defaultLanguage->languageFunctions.size()-1; i>=0; i--)
                                {
                                    tempTerm = main_syntax_highlighter->defaultLanguage->languageFunctions[i];
                                    if( tempTerm!=nullptr)
                                    {
                                        if( stg_ex::string_starts(tempTerm->termstring,currentstringInView) )
                                        {
                                            main_syntax_highlighter->suggestedCompilerTerms.push_back(tempTerm);
                                        }
                                    }
                                }

                                for( i = main_syntax_highlighter->defaultLanguage->languageKeywords.size()-1; i>=0; i--)
                                {
                                    tempTerm = main_syntax_highlighter->defaultLanguage->languageKeywords[i];
                                    if( tempTerm!=nullptr)
                                    {
                                        if( stg_ex::string_starts(tempTerm->termstring,currentstringInView) )
                                        {
                                            main_syntax_highlighter->suggestedCompilerTerms.push_back(tempTerm);
                                        }
                                    }
                                }

                                for( i = main_syntax_highlighter->defaultLanguage->languageVariables.size()-1; i>=0; i--)
                                {
                                    tempTerm = main_syntax_highlighter->defaultLanguage->languageVariables[i];
                                    if( tempTerm!=nullptr)
                                    {
                                        if( stg_ex::string_starts(tempTerm->termstring,currentstringInView) )
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
                            if( tempTerm!=nullptr)
                            {
                                if( tempTerm->termType==cterm_function)
                                {
                                    tTerwindow_width = (int)tempTerm->termstring.size()+(int)tempTerm->get_parameters().size()+(int)tempTerm->termFunctionReturnType.size()+3; // functionName(parameters)
                                }
                                else
                                {
                                    tTerwindow_width = (int)tempTerm ->termstring.size();
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

                        if( font_textinput!=nullptr)
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
        std::string returnstring = "";
        for(int i=0; i < (int)listOfstrings.size(); i+=1)
        {
            returnstring+=listOfstrings[i]+"\n";
        }
        return returnstring;
    }

    std::string widget_text_editor::get_string_spaces()
    {
        std::string returnstring = "";
        int listSize = (int)listOfstrings.size();
        for(int i=0; i < listSize; i+=1)
        {
            if( i ==listSize-1)
            {
                returnstring+=listOfstrings[i]+" ";
            }
            else
            {
                returnstring+=listOfstrings[i];
            }
        }
        return returnstring;
    }

    int widget_text_editor::get_line_count()
    {
        return (int)listOfstrings.size();
    }

    std::string widget_text_editor::get_line_string(int lineNumberIn)
    {
        if( lineNumberIn >=0 && lineNumberIn < (int)listOfstrings.size() )
        {
            return listOfstrings.at(lineNumberIn);
        }
        return "";
    }

    std::string widget_text_editor::get_short_hightlighted()
    {
        if( selectionCursorXPos!=selectionEndCursorXPos || selectionCursorYPos!=selectionEndCursorYPos )
        {
            int yLineToCopy = std::min( selectionCursorYPos, selectionEndCursorYPos);
            if( (int)listOfstrings.size() > yLineToCopy)
            {
                int tempYlineLength = (int)listOfstrings[yLineToCopy].size();
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
                    return listOfstrings[yLineToCopy].substr(minXPosToCopy,maxXPosToCopy-minXPosToCopy);
                }
            }
        }
        return "";
    }

    std::vector <std::string> widget_text_editor::get_all_strings()
    {
        return listOfstrings;
    }

    int widget_text_editor::get_most_anchors_characters()
    {
        int maxCharsUsed = 0;
        widget_text_anchor * tAnchor;
        for( int i = 0; i < (int)anchorPositions.size(); i++)
        {
            tAnchor = anchorPositions[i];
            if( tAnchor!=nullptr)
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
        for( int i = 0; i < (int)listOfstrings.size(); i++)
        {
            if( (int)listOfstrings[i].size() > maxCharsUsed)
            {
                maxCharsUsed = (int)listOfstrings[i].size();
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

        int listSize = (int)listOfstrings.size();
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
        if( textXScroll!=nullptr)
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
        if( textYScroll!=nullptr)
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
                    //    lineStartYPos =  ceil( ( (float)listOfstrings.size() ) * ( (float)textYScroll->scrollYPos/(float)textYScroll->widget_box.h ) );
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
        if((int)listOfstrings.size() > 1)
        {
            return true;
        }
        else if((int)listOfstrings.size()==1 )
        {
            std::string firstLine = listOfstrings[0];
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
            textfile_location = new_file_name;
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
                            listOfstrings.push_back(currLine);
                        }
                        else
                        {
                            listOfstrings.push_back("");
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
            if( tempText!=nullptr)
            {
                delete tempText;
                tempText = nullptr;
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
            if( lineStartYPos > (int)listOfstrings.size()-linesWithinView )
            {
                lineStartYPos = (int)listOfstrings.size()-linesWithinView;
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

    void widget_text_editor::open_text_file()
    {
        gpe::external_open_program( textfile_location );
    }

    void widget_text_editor::paste_clipboard()
    {
        if(!isReadOnly)
        {
            std::string clipboardStr = gpe::input->clipboard_string();
            if( (int)clipboardStr.size() > 0)
            {
                clipboardStr = stg_ex::string_replace_all(clipboardStr,"\r\n","\n");
                clipboardStr = stg_ex::string_replace_all(clipboardStr,"\r","\n");
                clipboardStr = stg_ex::string_replace_all(clipboardStr,"\t","    ");
                if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
                {
                    lineToEdit = listOfstrings[cursorYPos];
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
                        std::string cbNLstring = stg_ex::split_first_string(clipboardStr,'\n');
                        if( (int)lineToEdit.size() >= cursorXPos)
                        {
                            textInputstring = stg_ex::get_substring(lineToEdit,0,cursorXPos);
                        }
                        else
                        {
                            textInputstring= "";
                        }
                        textInputstring+=cbNLstring;
                        std::string remainerOfStr="";
                        if( (int)lineToEdit.size() > cursorXPos )
                        {
                            remainerOfStr = stg_ex::get_substring(lineToEdit,cursorXPos);
                        }
                        listOfstrings.erase(listOfstrings.begin()+cursorYPos);
                        listOfstrings.insert(listOfstrings.begin()+cursorYPos,textInputstring);
                        cursorYPos+=1;
                        newLinePos=clipboardStr.find_first_of('\n');
                        while (newLinePos!=(int)std::string::npos)
                        {
                            cbNLstring = stg_ex::split_first_string(clipboardStr,'\n');
                            listOfstrings.insert(listOfstrings.begin()+cursorYPos,cbNLstring);
                            newLinePos=clipboardStr.find_first_of("\n");
                            cursorYPos+=1;
                        }
                        textInputstring = clipboardStr;
                        textInputstring+=remainerOfStr;
                        listOfstrings.insert(listOfstrings.begin()+cursorYPos,textInputstring);
                        cursorXPos=(int)clipboardStr.size();
                    }
                    else
                    {
                        if( (int)lineToEdit.size() >= cursorXPos)
                        {
                            textInputstring = stg_ex::get_substring(lineToEdit,0,cursorXPos);
                        }
                        else
                        {
                            textInputstring = "";
                        }
                        textInputstring+=clipboardStr;
                        if( (int)lineToEdit.size() >= cursorXPos)
                        {
                            textInputstring+=stg_ex::get_substring(lineToEdit,cursorXPos);
                        }
                        cursorXPos+=(int)clipboardStr.size();
                        listOfstrings.erase(listOfstrings.begin()+cursorYPos);
                        listOfstrings.insert(listOfstrings.begin()+cursorYPos,textInputstring);
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
            std::string currstringToRender = "";
            std::string currentLineInView = "";

            int tempSynstringSize = 0;
            int currPosToParse = 0, lineEnd = 0;
            bool isInBlockCommentMode = false;
            bool isInfloatQuoteMode = false;
            bool isInSingleQuoteMode = false;
            int maxLine = (int)listOfstrings.size();
            int endBlockCommentPos = 0;
            int endDQuoteCommentPos = 0;
            int endSQuoteCommentPos = 0;
            bool commentFoundInSymbols = false;
            int openBracesCount = 0;
            int openBracketsCount = 0;
            int openParenthesisCount = 0;
            for( i=0; i < maxLine; i++)
            {
                currstringToRender = listOfstrings[i];
                currPosToParse = 0;
                openParenthesisCount = 0;
                lineEnd = (int)currstringToRender.size();

                endBlockCommentPos=(int)std::string::npos;
                endDQuoteCommentPos=(int)std::string::npos;
                endSQuoteCommentPos=(int)std::string::npos;
                while (currPosToParse < lineEnd)
                {
                    if( isInBlockCommentMode)
                    {
                        endBlockCommentPos = currstringToRender.find("*/");
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
                        endDQuoteCommentPos = currstringToRender.find('"',currPosToParse);
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
                        endSQuoteCommentPos = currstringToRender.find("'",currPosToParse);
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
                        while (currPosToParse < lineEnd && currstringToRender[currPosToParse]==' ')
                        {
                            currPosToParse++;
                        }
                        if (currPosToParse < lineEnd)
                        {
                            if (currPosToParse+1<lineEnd && currstringToRender[currPosToParse] == '/' && currPosToParse + 1 < lineEnd && currstringToRender[currPosToParse + 1] == '/' )
                            {
                                currPosToParse = lineEnd;
                            }
                            else if (currPosToParse+1<lineEnd && currstringToRender[currPosToParse] == '/' && currstringToRender[currPosToParse + 1] == '*' )
                            {
                                isInBlockCommentMode = true;
                                currPosToParse+=2;
                            }
                            else if(currstringToRender[currPosToParse] == '"')
                            {
                                isInfloatQuoteMode = true;
                                currPosToParse++;
                            }
                            else if(currstringToRender[currPosToParse] == '\'')
                            {
                                isInSingleQuoteMode = true;
                                currPosToParse++;
                            }
                            else if(currstringToRender[currPosToParse] == '(')
                            {
                                openParenthesisCount++;
                                currPosToParse++;
                            }
                            else if(currstringToRender[currPosToParse] == '[')
                            {
                                openBracesCount++;
                                currPosToParse++;
                            }
                            else if(currstringToRender[currPosToParse] == '{')
                            {
                                openBracketsCount++;
                                currPosToParse++;
                            }
                            else if(currstringToRender[currPosToParse] == ')')
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
                            else if(currstringToRender[currPosToParse] == ']')
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
                            else if(currstringToRender[currPosToParse] == '}')
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
                            else if (isdigit(currstringToRender[currPosToParse]) )
                            {
                                currPosToParse++;
                                while( (currPosToParse < lineEnd && isdigit( currstringToRender[currPosToParse] )  ) || currstringToRender[currPosToParse] ==' ')
                                {
                                    currPosToParse++;
                                }
                            }
                            /*
                            else if ( main_syntax_highlighter->charIsSymbol(currstringToRender[currPosToParse]) )
                            {
                                commentFoundInSymbols = false;
                                while( ( commentFoundInSymbols==false && currPosToParse < lineEnd && main_syntax_highlighter->charIsSymbol(currstringToRender[currPosToParse] ) )|| currstringToRender[currPosToParse] ==' ')
                                {
                                    if( lineEnd > currPosToParse+1)
                                    {
                                        if( currstringToRender[currPosToParse] == '/' && (currstringToRender[currPosToParse + 1] == '/' || currstringToRender[currPosToParse + 1] == '*' ) )
                                        {
                                            commentFoundInSymbols = true;
                                        }
                                        else if (currstringToRender[currPosToParse] == '*' && currstringToRender[currPosToParse + 1] == '/' )
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
                            else if ( stg_ex::char_is_alpha(currstringToRender[currPosToParse],true,true) )
                            {
                                //color = pawgui::theme_main->text_box_font_color;
                                currPosToParse++;
                                while (currPosToParse < lineEnd && stg_ex::char_is_alnum(currstringToRender[currPosToParse],true,true) )
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

        if( (int)listOfstrings.size()==0)
        {
            listOfstrings.push_back("");
            reset_self();
        }

        bool pasteCommandGiven = false;
        bool mouseHoveringInTextArea = false;
        bool buttonBarClicked = false;

        if( !isReadOnly && textEditor_buttonBar!=nullptr && show_buttonBar )
        {
            textEditor_buttonBar->set_coords(widget_box.x,widget_box.y);
            //sets the buttonbar to the width of the text editor( minus width of yScroll width[16 ).
            //textEditor_buttonBar->set_width(widget_box.w);
            textEditor_buttonBar->set_height( 32 );
            textEditor_buttonBar->process_self(view_space,cam);
            buttonBarClicked = textEditor_buttonBar->is_clicked();
            if( textEditor_buttonBar->selectedOption>= 0 && textEditor_buttonBar->selectedOption < TEXTAREA_OPTION_MAX_OPTIONS)
            {
                if( textEditor_buttonBar->selectedOption==TEXTAREA_OPTION_IMPORT)
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
                else if(textEditor_buttonBar->selectedOption ==TEXTAREA_OPTION_REDO)
                {
                    if( can_redo() )
                    {
                        redo_edit();
                    }
                }
                else if(textEditor_buttonBar->selectedOption ==TEXTAREA_OPTION_UNDO)
                {
                    if( can_undo() )
                    {
                        undo_edit();
                    }
                }
                else if( textEditor_buttonBar->selectedOption==TEXTAREA_OPTION_EXPORT)
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
                else if( textEditor_buttonBar->selectedOption==TEXTAREA_OPTION_CLEAR)
                {
                    if( display_prompt_message("Warning!","Are you sure you want to continue this operation?")==display_query_yes )
                    {
                        clear_all_lines();
                        listOfstrings.push_back("");
                        save_edit();
                    }
                }
                else if( textEditor_buttonBar->selectedOption==TEXTAREA_OPTION_COPY)
                {
                    copy_selection();
                }
                else if( textEditor_buttonBar->selectedOption==TEXTAREA_OPTION_CUT)
                {
                    cut_selection();
                }
                else if( textEditor_buttonBar->selectedOption==TEXTAREA_OPTION_PASTE)
                {
                    pasteCommandGiven = true;
                }
            }
        }
        else if( textEditor_buttonBar!=nullptr)
        {
            textEditor_buttonBar->set_coords(widget_box.x,widget_box.y);

            textEditor_buttonBar->set_height(0);
        }
        if( !buttonBarClicked && (int)listOfstrings.size() >0 )
        {
            textInputstring = "";
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

            if( isEnabled &&  isInUse && cam!=nullptr && textEditor_buttonBar!=nullptr)
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
                    if( gpe::input->check_mouse_double_clicked( mb_left) && resource_dragged==nullptr )
                    {
                        update_cursor_to_mouse(view_space, cam);
                        if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
                        {
                            lineToEdit = listOfstrings[cursorYPos];

                            selectionCursorXPos = selectionEndCursorXPos = cursorXPos;
                            selectionCursorYPos = selectionEndCursorYPos = cursorYPos;
                            if( (int)lineToEdit.size() == 0 )
                            {
                                if( cursorYPos+1 < (int)listOfstrings.size() )
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
                                else if( cursorYPos+1 < (int)listOfstrings.size() )
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
                            if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
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
                            if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
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
                        if( !isReadOnly )
                        {
                            main_context_menu->add_menu_option("Undo",0,rsm_gui->texture_add("back_buttonIcon", gpe::app_directory_name+"resources/buttons/backward.png"),-1,nullptr,true,!isReadOnly && can_undo());
                            main_context_menu->add_menu_option("Redo",1,rsm_gui->texture_add("forward_buttonIcon", gpe::app_directory_name+"resources/buttons/forward.png"),-1,nullptr,true,!isReadOnly && can_redo() );
                            main_context_menu->add_menu_option("Cut",2,rsm_gui->texture_add("cut_buttonIcon", gpe::app_directory_name+"resources/buttons/cut.png"),-1,nullptr,false,!isReadOnly);
                        }
                        main_context_menu->add_menu_option("Copy",3,rsm_gui->texture_add("copy_buttonIcon", gpe::app_directory_name+"resources/buttons/copy.png"),-1,nullptr,false,true);
                        if( !isReadOnly )
                        {
                            main_context_menu->add_menu_option("Paste",4,rsm_gui->texture_add("paste_buttonIcon", gpe::app_directory_name+"resources/buttons/paste.png"),-1,nullptr,false,!isReadOnly);
                            main_context_menu->add_menu_option("Delete",5,rsm_gui->texture_add("remove_buttonIcon", gpe::app_directory_name+"resources/buttons/remove.png"),-1,nullptr,true,!isReadOnly);
                        }
                        main_context_menu->add_menu_option("Select All",6,rsm_gui->texture_add("sticky_buttonIcon", gpe::app_directory_name+"resources/buttons/sticky-note.png"),-1,nullptr,true,true);

                        if( (int)textfile_location.size() > 0 )
                        {
                            main_context_menu->add_menu_option("Open File",7,rsm_gui->texture_add("sticky_buttonIcon", gpe::app_directory_name+"resources/buttons/file-open.png"),-1,nullptr,true,true);
                            main_context_menu->add_menu_option("Refresh Text",8,rsm_gui->texture_add("sticky_buttonIcon", gpe::app_directory_name+"resources/buttons/file-open.png"),-1,nullptr,true,true);
                        }
                        int menuSelection = context_menu_process();

                        if( menuSelection==0)
                        {
                            if( can_undo() && !isReadOnly )
                            {
                                undo_edit();
                            }
                        }
                        else if( menuSelection==1 )
                        {
                            if( can_redo() && !isReadOnly )
                            {
                                redo_edit();
                            }
                        }
                        else if( menuSelection==6)
                        {
                            select_all();
                        }
                        else if( menuSelection==7)
                        {
                            open_text_file();
                        }
                        else if( menuSelection==8)
                        {
                            refresh_text_file();
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
                        main_syntax_highlighter->highlightedTerm = nullptr;
                    }
                    else if( gpe::input->mouse_movement_received && resource_dragged==nullptr )
                    {
                        //Highlights documenation under mouse if found.
                        int tMouseX = 0, tMouseY = 0;
                        find_mouse_cursor(&tMouseX,&tMouseY, view_space, cam);
                        if( tMouseX!=highlightXPos || tMouseY!=highlightYPos)
                        {
                            find_documentation_description(tMouseX,tMouseY);
                        }
                    }
                    else if( resource_dragged!=nullptr && !isReadOnly )
                    {
                        if( gpe::input->check_mouse_released( mb_left))
                        {
                            if( gpe::point_within_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y, textSpaceRect)  )
                            {
                                mouseHoveringInTextArea = true;
                                gpe::cursor_main_controller->cursor_change("ibeam");

                                update_cursor_to_mouse(view_space, cam);
                                if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
                                {
                                    if( cursorXPos >=0 && cursorXPos <= (int)listOfstrings[cursorYPos].size() )
                                    {

                                        listOfstrings[cursorYPos] = stg_ex::get_substring(listOfstrings[cursorYPos],0,cursorXPos)+resource_dragged->get_name()+stg_ex::get_substring(listOfstrings[cursorYPos],cursorXPos);
                                        resource_dragged = nullptr;
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
                        lineToEdit = listOfstrings[cursorYPos];
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
                        if( cursorYPos < (int)listOfstrings.size()-1 )
                        {
                            cursorYPos+=1;
                        }
                        move_down(1);
                    }

                    update_cursor_to_mouse(view_space, cam);
                    if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
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
                    bscDelay += gpe::time_keeper->get_delta_performance();
                }
                else
                {
                    bscDelay = 0;
                }
                if( gpe::input->kb_button_down[kb_delete] )
                {
                    delDelay += gpe::time_keeper->get_delta_performance();
                }
                else
                {
                    delDelay = 0;
                }
                if( gpe::input->kb_button_down[kb_tab] )
                {
                    tabDelay += gpe::time_keeper->get_delta_performance();
                }
                if(gpe::input->kb_button_down[kb_enter] )
                {
                    enterDelay+=gpe::time_keeper->get_delta_performance();
                }
                else
                {
                    enterDelay = 0;
                }
                if( gpe::input->kb_button_down[kb_left] )
                {
                    leftDelay+=gpe::time_keeper->get_delta_performance();
                }
                else
                {
                    leftDelay = 0;
                }

                if( gpe::input->kb_button_down[kb_right]  )
                {
                    rightDelay+=gpe::time_keeper->get_delta_performance();
                }
                else
                {
                    rightDelay = 0;
                }

                if(gpe::input->kb_button_down[kb_up]  )
                {
                    upDelay+=gpe::time_keeper->get_delta_performance();
                }
                else
                {
                    upDelay = 0;
                }
                if(gpe::input->kb_button_down[kb_down] )
                {
                    downDelay+=gpe::time_keeper->get_delta_performance();
                }
                else
                {
                    downDelay = 0;
                }

                if(gpe::input->kb_button_down[kb_d] )
                {
                    dKeyDelay+=gpe::time_keeper->get_delta_performance();
                }
                else
                {
                    dKeyDelay = 0;
                }

                if( gpe::input->kb_button_down[kb_ctrl]  )
                {
                    main_syntax_highlighter->highlightedTerm = nullptr;
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
                        main_search_controller->findTextstringBox->set_string( get_short_hightlighted() );
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
                                cursorXPos = (int)listOfstrings[cursorYPos].size()-1;
                                if( cursorXPos < 0)
                                {
                                    cursorXPos = 0;
                                }
                            }
                        }
                        else if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
                        {
                            std::string currentLineToScroll = listOfstrings[cursorYPos];
                            if( cursorXPos >=(int)currentLineToScroll.size() )
                            {
                                cursorXPos = (int)currentLineToScroll.size()-1;
                            }
                            if( cursorXPos < 0)
                            {
                                if( cursorYPos >0 && cursorYPos < (int)listOfstrings.size() )
                                {
                                    cursorYPos--;
                                    cursorXPos = listOfstrings[cursorYPos].size()-1;
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
                        if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
                        {
                            std::string currentLineToScroll = listOfstrings[cursorYPos];
                            if( cursorXPos >=(int)currentLineToScroll.size() )
                            {
                                if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size()-1 )
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
                        main_syntax_highlighter->highlightedTerm = nullptr;
                    }
                    else if( mouseHoveringInTextArea && gpe::input->mouse_scrolling_down )
                    {
                        //move_down( linesWithinView/4);
                        main_syntax_highlighter->highlightedTerm = nullptr;
                    }
                    else if(  (enterDelay >= (main_settings->textAreaDelayTime)  )  && !isReadOnly )
                    {
                        if( main_syntax_highlighter->codeBeingSuggested )
                        {
                            if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
                            {
                                std::string prevStr = listOfstrings[cursorYPos];
                                if( main_syntax_highlighter->iSuggestionPos >= 0 && main_syntax_highlighter->iSuggestionPos < (int)main_syntax_highlighter->suggestedCompilerTerms.size() )
                                {
                                    syntax_compiler_term * tempTerm = main_syntax_highlighter->suggestedCompilerTerms.at(main_syntax_highlighter->iSuggestionPos);
                                    if( tempTerm!=nullptr)
                                    {
                                        if( tempTerm->termType==cterm_function)
                                        {
                                            prevStr = prevStr.substr(0,tempCLineXStartPos)+tempTerm->termstring+"()"+prevStr.substr(tempCLineXEndPos+1);
                                            cursorXPos = tempCLineXStartPos + (int)tempTerm->termstring.size()+1;
                                        }
                                        else
                                        {
                                            prevStr = prevStr.substr(0,tempCLineXStartPos)+tempTerm->termstring+prevStr.substr(tempCLineXEndPos+1);
                                            cursorXPos = tempCLineXStartPos + (int)tempTerm->termstring.size();
                                        }
                                        listOfstrings[cursorYPos] = prevStr;
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
                            lineToEdit = listOfstrings[cursorYPos];
                            //go to next line
                            std::string nextstring = "";
                            if( (int)lineToEdit.size()> 0)
                            {
                                nextstring = stg_ex::get_substring(lineToEdit,cursorXPos);
                                lineToEdit = stg_ex::get_substring(lineToEdit,0, cursorXPos);
                            }
                            else
                            {
                                lineToEdit = "";
                                nextstring = "";
                            }
                            cursorXPos = 0;
                            if( (int)listOfstrings.size()>0 )
                            {
                                listOfstrings.erase(listOfstrings.begin()+cursorYPos );
                                listOfstrings.insert(listOfstrings.begin()+cursorYPos,lineToEdit);
                                int numbOfSpaces =  stg_ex::get_leading_space_count(lineToEdit);
                                cursorXPos = 0;
                                if( numbOfSpaces > 0)
                                {
                                    for(int i= 0; i < numbOfSpaces; i++)
                                    {
                                        nextstring=" "+nextstring;
                                    }
                                    cursorXPos = numbOfSpaces;
                                }
                                if( (int)lineToEdit.size()>0)
                                {
                                    std::string lastChar = stg_ex::get_substring(lineToEdit, (int)lineToEdit.size()-1 );
                                    if( lastChar.compare("{" )==0)
                                    {
                                        nextstring=generate_tabs(1)+nextstring;
                                        cursorXPos+=get_tab_space_count();
                                    }
                                }
                                listOfstrings.insert(listOfstrings.begin()+cursorYPos+1,nextstring);
                                gpe::input->reset_all_input();
                            }
                            else
                            {
                                listOfstrings.push_back(lineToEdit);
                                listOfstrings.push_back(nextstring);
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
                        main_syntax_highlighter->highlightedTerm = nullptr;
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
                            std::string prevstring = listOfstrings[cursorYPos];
                            cursorXPos = prevstring.size();
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
                        lineToEdit = listOfstrings[cursorYPos];
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
                            if(cursorYPos < (int) listOfstrings.size()-1 )
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
                        main_syntax_highlighter->highlightedTerm = nullptr;
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
                                std::string prevLine = listOfstrings[cursorYPos];
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
                        main_syntax_highlighter->highlightedTerm  = nullptr;
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
                            if( cursorYPos < (int)listOfstrings.size()-1 )
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
                                std::string nextLine = listOfstrings[cursorYPos];
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
                            else if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
                            {
                                lineToEdit = listOfstrings[cursorYPos];
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
                                    textInputstring = stg_ex::get_substring(lineToEdit,0,cursorXPos);
                                    std::string rightSide = stg_ex::get_substring(lineToEdit,cursorXPos);
                                    if( cursorXPos >= (int)lineToEdit.size() )
                                    {
                                        textInputstring = stg_ex::get_substring(lineToEdit,0);
                                        rightSide = "";
                                    }
                                    int trailingSpaces = stg_ex::get_trailing_space_count(textInputstring);
                                    int tabsToCheckCount = get_tab_space_count();
                                    if( trailingSpaces >= tabsToCheckCount )
                                    {
                                        cursorXPos -= tabsToCheckCount;
                                        textInputstring = stg_ex::get_substring(textInputstring,0,cursorXPos);
                                    }
                                    else
                                    {
                                        cursorXPos -=1;
                                        textInputstring = stg_ex::get_substring(textInputstring,0,cursorXPos);
                                    }

                                    textInputstring+=rightSide;
                                    listOfstrings.erase(listOfstrings.begin()+cursorYPos);
                                    if( cursorYPos>=0)
                                    {
                                        listOfstrings.insert(listOfstrings.begin()+cursorYPos,textInputstring);
                                    }
                                    else
                                    {
                                        listOfstrings.push_back(textInputstring);
                                    }
                                    log_editable_action();
                                }
                                else if( cursorYPos>=1)
                                {
                                    log_editable_action();
                                    textInputstring = lineToEdit;
                                    std::string prevstring = listOfstrings.at(cursorYPos-1);
                                    cursorXPos = prevstring.size();
                                    listOfstrings.erase(listOfstrings.begin()+cursorYPos);
                                    cursorYPos -=1;
                                    if( cursorYPos < lineStartYPos && cursorYPos>=0)
                                    {
                                        lineStartYPos-=1;
                                    }
                                    textInputstring = listOfstrings[cursorYPos]+lineToEdit;
                                    listOfstrings.erase(listOfstrings.begin()+cursorYPos);
                                    listOfstrings.insert(listOfstrings.begin()+cursorYPos,textInputstring);
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
                            else if( cursorYPos >=0 && cursorYPos < (int)listOfstrings.size() )
                            {
                                lineToEdit = listOfstrings[cursorYPos];
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
                                    textInputstring = stg_ex::get_substring(lineToEdit,0,cursorXPos);
                                    std::string rightSide = stg_ex::get_substring(lineToEdit,cursorXPos+1);
                                    if( cursorXPos >= (int)lineToEdit.size() )
                                    {
                                        textInputstring = stg_ex::get_substring(lineToEdit,0);
                                        rightSide = "";
                                    }

                                    textInputstring+=rightSide;
                                    listOfstrings.erase(listOfstrings.begin()+cursorYPos);
                                    if( cursorYPos>=0)
                                    {
                                        listOfstrings.insert(listOfstrings.begin()+cursorYPos,textInputstring);
                                    }
                                    else
                                    {
                                        listOfstrings.push_back(textInputstring);
                                    }
                                    log_editable_action();
                                }
                                else if( cursorYPos>=0 && cursorXPos>=(int)lineToEdit.size() && cursorYPos < (int)listOfstrings.size()-1 )
                                {
                                    log_editable_action();
                                    textInputstring = lineToEdit;
                                    std::string prevstring = listOfstrings.at(cursorYPos+1);
                                    cursorXPos = lineToEdit.size();
                                    textInputstring = lineToEdit+prevstring;
                                    listOfstrings.erase(listOfstrings.begin()+cursorYPos+1);
                                    listOfstrings.erase(listOfstrings.begin()+cursorYPos);
                                    listOfstrings.insert(listOfstrings.begin()+cursorYPos,textInputstring);
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
                                        strToUnTab = listOfstrings[iTabPos];
                                        if( has_early_tab(strToUnTab) )
                                        {
                                            strToUnTab = untab_string(strToUnTab);
                                            listOfstrings[iTabPos] = strToUnTab;
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
                                        listOfstrings[iTabPos] = tabAddition+listOfstrings[iTabPos];
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
                                    textInputstring = stg_ex::get_substring(lineToEdit,0,cursorXPos);
                                    textInputstring+=tabAddition;
                                    textInputstring+=stg_ex::get_substring(lineToEdit,cursorXPos);
                                    cursorXPos+=tabCharCount;
                                    gpe::input->inputted_keys = "";
                                    listOfstrings.erase(listOfstrings.begin()+cursorYPos);
                                    listOfstrings.insert(listOfstrings.begin()+cursorYPos,textInputstring);
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
                        main_syntax_highlighter->highlightedTerm = nullptr;
                        //Type input into textarea
                        delete_selection();
                        log_editable_action();
                        textInputstring = stg_ex::get_substring(lineToEdit,0,cursorXPos);
                        std::string preInputstring = stg_ex::string_replace_all(textInputstring," ","");
                        textInputstring+=gpe::input->inputted_keys;
                        listOfstrings.erase(listOfstrings.begin()+cursorYPos);
                        std::string restofLine = stg_ex::get_substring(lineToEdit,cursorXPos);
                        if( gpe::input->inputted_keys=="{" && (int)restofLine.size()==0 && (int)preInputstring.size()==0 )
                        {
                            int numbOfSpaces =  stg_ex::get_leading_space_count(lineToEdit);
                            cursorXPos = 0;
                            std::string nextstring = "";
                            if( numbOfSpaces > 0)
                            {
                                for(int i= 0; i < numbOfSpaces; i++)
                                {
                                    nextstring=" "+nextstring;
                                }
                                cursorXPos = numbOfSpaces;
                            }
                            std::string bracketstring = nextstring+"}";
                            nextstring="    "+nextstring;
                            cursorXPos+=4;

                            textInputstring+=restofLine;
                            listOfstrings.insert(listOfstrings.begin()+cursorYPos,textInputstring);
                            listOfstrings.insert(listOfstrings.begin()+cursorYPos+1,nextstring);
                            listOfstrings.insert(listOfstrings.begin()+cursorYPos+2,bracketstring);
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
                                textInputstring+="]";
                            }
                            else if( gpe::input->inputted_keys=="{" )
                            {
                                textInputstring+="}";
                            }
                            else if( gpe::input->inputted_keys=="(" )
                            {
                                textInputstring+=")";
                            }
                            textInputstring+=restofLine;
                            listOfstrings.insert(listOfstrings.begin()+cursorYPos,textInputstring);

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
                    main_syntax_highlighter->highlightedTerm  = nullptr;
                }
                else
                {
                    move_up( 3 );
                    main_syntax_highlighter->highlightedTerm = nullptr;
                }
            }
            else if( gpe::input->mouse_scrolling_down )
            {
                if( gpe::input->kb_button_down[kb_ctrl])
                {
                    move_right(charactersWithinView/8);
                    main_syntax_highlighter->highlightedTerm = nullptr;
                }
                else
                {
                    move_down( 3 );
                    main_syntax_highlighter->highlightedTerm = nullptr;
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
        //if( showXScroll && textXScroll!=nullptr)
        {
            //main_settings->textAreaFindBox.y = widget_box.y+textEditor_buttonBar->get_height()+renderBox->h+textXScroll->widget_box.h;
        }
        //else
        {
            main_settings->textAreaFindBox.y = widget_box.y+textEditor_buttonBar->get_height()+renderBox->h;
        }
        main_settings->textAreaFindBox.w = widget_box.w-lineCountBoxWidth;

        int findAllResult = 0;
        switch( main_search_controller->textSearchMode )
        {
        //find
        case search_mode::find_text:
            if( main_search_controller->findTextstringBox->has_content() )
            {
                if( main_search_controller->findTextstringBox->was_submitted() || main_search_controller->find_button->is_clicked() )
                {
                    if( find_string(main_search_controller->findTextstringBox->get_string(),main_search_controller->findMatchCase->is_clicked(),false)==false )
                    {
                        cursorXPos = 0;
                        cursorYPos = 0;
                        lineStartXPos = 0;
                        lineStartYPos = 0;
                        if( find_string(main_search_controller->findTextstringBox->get_string(),true,main_search_controller->findMatchCase->is_clicked(),false)==false)
                        {
                            main_overlay_system->update_temporary_message("Searched for","["+main_search_controller->findTextstringBox->get_string()+"]","Unable to Find string");
                        }
                    }
                    main_search_controller->showFindAllResults = false;
                }
                /*
                else if( main_search_controller->findAll_button->is_clicked() )
                {
                    findAllResult = find_all_strings(main_search_controller->findTextstringBox->get_string(),main_search_controller->findMatchCase->is_clicked() );
                    displayMessageTitle = "Substring Search";
                    displayMessageSubtitle = main_search_controller->findTextstringBox->get_string();

                    if( findAllResult > 0)
                    {
                        main_search_controller->showFindAllResults = true;
                    }
                    else
                    {
                        main_search_controller->showFindAllResults = false;
                    }
                    main_overlay_system->update_temporary_message(displayMessageTitle,displayMessageSubtitle,displayMessagestring,5);
                }
                */
            }
            break;

        //goto line
        case search_mode::goto_line:
            //main_search_controller->goToLinestringBox->set_string("1");

            if( (int)listOfstrings.size() > 0)
            {
                main_search_controller->goToLinestringBox->descriptionText = "Go To Line: 1 - "+ stg_ex::int_to_string( get_line_count() ) ;
            }
            else
            {
                main_search_controller->goToLinestringBox->descriptionText = "Go To Line: ";
            }

            if( ( main_search_controller->goToLinestringBox->was_submitted() || main_search_controller->goTo_button->is_clicked() ) &&  main_search_controller->goToLinestringBox->is_valid() )
            {
                scroll_to_pos(main_search_controller->goToLinestringBox->get_held_number()-1,0);
            }
            break;

        //find/replace
        case search_mode::replace_text:
            if( !isReadOnly)
            {
                if( main_search_controller->findTextstringBox->has_content() )
                {
                    if( main_search_controller->findTextstringBox->was_submitted() || main_search_controller->find_button->is_clicked() )
                    {
                        if( find_string(main_search_controller->findTextstringBox->get_string(),true,main_search_controller->findMatchCase->is_clicked(),false)==false )
                        {
                            cursorXPos = 0;
                            cursorYPos = 0;
                            lineStartXPos = 0;
                            lineStartYPos = 0;
                            if( find_string(main_search_controller->findTextstringBox->get_string(),true,main_search_controller->findMatchCase->is_clicked(),false)==false)
                            {
                                main_overlay_system->update_temporary_message("Searched for","["+main_search_controller->findTextstringBox->get_string()+"]","Unable to Find string");
                            }
                        }
                        main_search_controller->showFindAllResults = false;
                    }
                    /*
                    else if( main_search_controller->findAll_button->is_clicked() )
                    {
                        findAllResult = find_all_strings(main_search_controller->findTextstringBox->get_string(),main_search_controller->findMatchCase->is_clicked() );
                        displayMessageTitle = "Substring Search";
                        displayMessageSubtitle = main_search_controller->findTextstringBox->get_string();


                        if( findAllResult > 0)
                        {
                            main_search_controller->showFindAllResults = true;
                        }
                        else
                        {

                            main_search_controller->showFindAllResults = false;
                        }
                        main_overlay_system->update_temporary_message(displayMessageTitle,displayMessageSubtitle,displayMessagestring,15);
                    }
                    */
                    else if( main_search_controller->replaceTextstringBox->has_content() )
                    {
                        if( main_search_controller->replaceTextstringBox->was_submitted() || main_search_controller->replace_button->is_clicked() )
                        {
                            main_search_controller->showFindAllResults = false;
                        }
                        else if( main_search_controller->replaceAll_button->is_clicked() )
                        {
                            if( display_prompt_message("Warning!","All copies of the text will be replaced. Are you sure you want to continue this operation?")==display_query_yes )
                            {
                                findAllResult = find_all_strings(main_search_controller->findTextstringBox->get_string(),true);

                                if( findAllResult > 0)
                                {
                                    int replaceCount = replace_all_found( main_search_controller->findTextstringBox->get_string(), main_search_controller->replaceTextstringBox->get_string() ) ;
                                    main_search_controller->showFindAllResults = true;
                                }
                                else
                                {
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
            //process_self( view_space,cam );
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
                if( tempPlace!=nullptr)
                {
                    copy_source(tempPlace,true);
                    currentPositionInHistory++;
                    showCursor = true;
                    cursorTimer = 0;
                }
            }
        }
    }

    void widget_text_editor::refresh_text_file()
    {
        import_text( textfile_location );
    }

    void widget_text_editor::render_code( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( view_space!=nullptr && cam!=nullptr && textEditor_buttonBar!=nullptr && has_content() )
        {
            int mostCharactersOfText = get_most_characters_used();
            if( mostCharactersOfText > charactersWithinView && showYScroll )
            {
                mostCharactersOfText-=2;
            }
            int i = 0, j = 0;
            int parsedTokensCount = 0;
            std::string currstringToRender = "";
            std::string currentLineInView = "";
            //Processes the sythax to re-render each one
            std::string foundGPEDataType = "";
            std::string foundGPEFunction = "";
            std::string foundGPEVariable = "";
            std::string foundGPEKeyword = "";
            std::string foundGPEProjectFunction = "";
            std::string foundGPEProjectKeyword = "";
            std::string foundSyntaxstring = "";
            int tempSynstringSize = 0;
            int currPosToParse = 0, lineEnd = 0;
            gpe::color *color = nullptr;
            int textRenderXPos = 0, textRenderYPos = 0;
            bool isInBlockCommentMode = false;
            bool isInfloatQuoteMode = false;
            bool isInSingleQuoteMode = false;
            int minLineToRender = std::max( (int)lineStartYPos-50,0);
            int intLineStart = (int)lineStartYPos;
            int maxLineToRender = std::min( (int)lineStartYPos+(int)linesWithinView,(int)listOfstrings.size()-1);
            int endBlockCommentPos = 0;
            int endDQuoteCommentPos = 0;
            int endSQuoteCommentPos = 0;
            bool commentFoundInSymbols = false;
            parsed_text * mLineComment = new parsed_text(-1, -1);
            parsed_text * dqLineComment = new parsed_text(-1, -1);
            parsed_text * sqLineComment = new parsed_text(-1, -1);
            parsed_text * tempParseTextToAdd = new parsed_text(-1, -1);

            //Finds the previous mode of  the editor up to 20 lines to the current lineStartYPos
            for( i=minLineToRender; i < intLineStart && i < (int)listOfstrings.size(); i++)
            {
                currstringToRender = listOfstrings[i];
                currPosToParse = 0;
                lineEnd = (int)currstringToRender.size();
                while (currPosToParse < lineEnd)
                {
                    if( isInBlockCommentMode==false && isInfloatQuoteMode==false && isInSingleQuoteMode==false)
                    {
                        if (currPosToParse < lineEnd)
                        {
                            if (currPosToParse+1<lineEnd && currstringToRender[currPosToParse] == '/' && currPosToParse + 1 < lineEnd && currstringToRender[currPosToParse + 1] == '/' )
                            {
                                currPosToParse = lineEnd;
                            }
                            else if (currPosToParse+1<lineEnd && currstringToRender[currPosToParse] == '/' && currstringToRender[currPosToParse + 1] == '*' )
                            {
                                isInBlockCommentMode = true;
                                currPosToParse+=2;
                            }
                            else if(currstringToRender[currPosToParse] == '"')
                            {
                                isInfloatQuoteMode = true;
                                currPosToParse++;
                            }
                            else if(currstringToRender[currPosToParse] == '\'')
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
                        endBlockCommentPos = currstringToRender.find("*/");
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
                        endDQuoteCommentPos = currstringToRender.find('"',currPosToParse);
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
                        endSQuoteCommentPos = currstringToRender.find("'",currPosToParse);
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
                if( commentLineText!=nullptr)
                {
                    commentLineText->reset_self();
                }
                if( datatypeLineText!=nullptr)
                {
                    datatypeLineText->reset_self();
                }
                if( dQuoteLineText!=nullptr)
                {
                    dQuoteLineText->reset_self();
                }
                if( functionLineText!=nullptr)
                {
                    functionLineText->reset_self();
                }
                if( keywordLineText!=nullptr)
                {
                    keywordLineText->reset_self();
                }
                if( variableLineText!=nullptr)
                {
                    variableLineText->reset_self();
                }
                if( normalLineText!=nullptr)
                {
                    normalLineText->reset_self();
                }

                if( numberLineText!=nullptr)
                {
                    numberLineText->reset_self();
                }

                if( sQuoteLineText!=nullptr)
                {
                    sQuoteLineText->reset_self();
                }

                if( symbolLineText!=nullptr)
                {
                    symbolLineText->reset_self();
                }

                if( projectFunctionLineText!=nullptr)
                {
                    projectFunctionLineText->reset_self();
                }

                if( projectKeywordLineText!=nullptr)
                {
                    projectKeywordLineText->reset_self();
                }

                currstringToRender = listOfstrings[i];
                currPosToParse = 0;

                lineEnd = (int)currstringToRender.size();

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
                    while (currPosToParse < lineEnd && currstringToRender[currPosToParse]==' ')
                    {
                        currPosToParse++;
                    }
                    if( isInBlockCommentMode==false && isInfloatQuoteMode==false && isInSingleQuoteMode==false)
                    {
                        /*
                        while (currPosToParse < lineEnd && currstringToRender[currPosToParse]==' ')
                        {
                            currPosToParse++;
                        }*/
                        if (currPosToParse < lineEnd)
                        {
                            if (currPosToParse+1<lineEnd && currstringToRender[currPosToParse] == '/' && currPosToParse + 1 < lineEnd && currstringToRender[currPosToParse + 1] == '/' )
                            {
                                commentLineText->foundParses.push_back( new parsed_text(currPosToParse, lineEnd));
                                currPosToParse = lineEnd;
                            }
                            else if (currPosToParse+1<lineEnd && currstringToRender[currPosToParse] == '/' && currstringToRender[currPosToParse + 1] == '*' )
                            {
                                isInBlockCommentMode = true;
                                mLineComment->textStart = currPosToParse;
                                currPosToParse+=2;
                            }
                            else if( currstringToRender[currPosToParse] == '"' )
                            {
                                isInfloatQuoteMode = true;
                                dqLineComment->textStart = currPosToParse;
                                currPosToParse++;
                            }
                            else if( currstringToRender[currPosToParse] == '\'' )
                            {
                                isInSingleQuoteMode = true;
                                sqLineComment->textStart = currPosToParse;
                                currPosToParse++;
                            }
                            else if ( isdigit(currstringToRender[currPosToParse]) )
                            {
                                tempParseTextToAdd->textStart = currPosToParse;
                                currPosToParse++;
                                while( (currPosToParse < lineEnd && isdigit( currstringToRender[currPosToParse] )  ) || currstringToRender[currPosToParse] ==' ')
                                {
                                    currPosToParse++;
                                }
                                tempParseTextToAdd->textEnd = currPosToParse;
                                numberLineText->foundParses.push_back(tempParseTextToAdd);
                                tempParseTextToAdd = new parsed_text(0, -1);
                            }
                            else if( main_syntax_highlighter->charIsSymbol(currstringToRender[currPosToParse] ) )
                            {
                                tempParseTextToAdd->textStart = currPosToParse;
                                currPosToParse++;
                                commentFoundInSymbols = false;
                                while( ( commentFoundInSymbols==false && currPosToParse < lineEnd && main_syntax_highlighter->charIsSymbol(currstringToRender[currPosToParse] ) )|| currstringToRender[currPosToParse] ==' ')
                                {
                                    if( lineEnd > currPosToParse+1)
                                    {
                                        if( currstringToRender[currPosToParse] == '/' && (currstringToRender[currPosToParse + 1] == '/' || currstringToRender[currPosToParse + 1] == '*' ) )
                                        {
                                            commentFoundInSymbols = true;
                                        }
                                        else if (currstringToRender[currPosToParse] == '*' && currstringToRender[currPosToParse + 1] == '/' )
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
                                if( main_syntax_highlighter!=nullptr)
                                {
                                    foundGPEProjectFunction = main_syntax_highlighter->find_project_function(currstringToRender,currPosToParse);
                                    foundGPEProjectKeyword = main_syntax_highlighter->find_project_keyword(currstringToRender,currPosToParse);
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
                                        foundGPEProjectFunction= main_syntax_highlighter->find_project_function(currstringToRender,currPosToParse);
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
                                        foundGPEProjectKeyword= main_syntax_highlighter->find_project_keyword(currstringToRender,currPosToParse);
                                    }
                                    tempParseTextToAdd->textEnd = currPosToParse;
                                    projectKeywordLineText->foundParses.push_back(tempParseTextToAdd);
                                    tempParseTextToAdd = new parsed_text(-1, -1);
                                }
                                else
                                {
                                    foundGPEDataType= main_syntax_highlighter->find_gpe_datatype(currstringToRender,currPosToParse);
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
                                            foundGPEDataType= main_syntax_highlighter->find_gpe_datatype(currstringToRender,currPosToParse);
                                        }
                                        tempParseTextToAdd->textEnd = currPosToParse;
                                        datatypeLineText->foundParses.push_back(tempParseTextToAdd);
                                        tempParseTextToAdd = new parsed_text(-1, -1);
                                    }
                                    else
                                    {
                                        foundGPEFunction =main_syntax_highlighter->find_gpe_function(currstringToRender,currPosToParse);
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
                                                foundGPEFunction= main_syntax_highlighter->find_gpe_function(currstringToRender,currPosToParse);
                                            }
                                            tempParseTextToAdd->textEnd = currPosToParse;
                                            functionLineText->foundParses.push_back(tempParseTextToAdd);
                                            tempParseTextToAdd = new parsed_text(-1, -1);
                                        }
                                        else
                                        {
                                            foundGPEVariable = main_syntax_highlighter->find_gpe_variable(currstringToRender,currPosToParse);
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
                                                    foundGPEVariable = main_syntax_highlighter->find_gpe_variable(currstringToRender,currPosToParse);
                                                }
                                                tempParseTextToAdd->textEnd = currPosToParse;
                                                variableLineText->foundParses.push_back(tempParseTextToAdd);
                                                tempParseTextToAdd = new parsed_text(-1, -1);
                                            }
                                            else
                                            {
                                                foundGPEKeyword = main_syntax_highlighter->find_gpe_keyword(currstringToRender,currPosToParse);
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
                                                        foundGPEKeyword = main_syntax_highlighter->find_gpe_keyword(currstringToRender,currPosToParse);
                                                    }
                                                    tempParseTextToAdd->textEnd = currPosToParse;
                                                    keywordLineText->foundParses.push_back(tempParseTextToAdd);
                                                    tempParseTextToAdd = new parsed_text(-1, -1);
                                                }
                                                else if ( stg_ex::char_is_alpha(currstringToRender[currPosToParse],false,true) )
                                                {
                                                    //color = pawgui::theme_main->text_box_font_color;
                                                    tempParseTextToAdd->textStart = currPosToParse;
                                                    currPosToParse++;
                                                    while (currPosToParse < lineEnd && stg_ex::char_is_alnum(currstringToRender[currPosToParse],false,true) )
                                                    {
                                                        currPosToParse++;
                                                    }
                                                    tempParseTextToAdd->textEnd = currPosToParse;
                                                    normalLineText->foundParses.push_back(tempParseTextToAdd);
                                                    tempParseTextToAdd = new parsed_text(-1, -1);
                                                }
                                                else
                                                {
                                                    if(currstringToRender[currPosToParse]!=' ')
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
                        endBlockCommentPos = currstringToRender.find("*/");
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
                        endDQuoteCommentPos = currstringToRender.find('"',currPosToParse);
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
                        endSQuoteCommentPos = currstringToRender.find("'",currPosToParse);
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
                    normalLineText->render_tokens( font_textinput,currstringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    color = pawgui::theme_main->text_box_font_number_color;
                    numberLineText->render_tokens( font_textinput_number,currstringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color,true );

                    color = pawgui::theme_main->text_box_font_symbols_color;
                    symbolLineText->render_tokens( font_textinput_symbol,currstringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    //
                    if( colorblind_mode_on )
                    {
                        color = pawgui::theme_main->text_box_font_function_alt_color;
                    }
                    else
                    {
                        color = pawgui::theme_main->text_box_font_function_color;
                    }
                    functionLineText->render_tokens( font_textinput_function,currstringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    //
                    if( colorblind_mode_on)
                    {
                        color = pawgui::theme_main->text_box_font_keyword_alt_color;
                    }
                    else
                    {
                        color = pawgui::theme_main->text_box_font_keyword_color;
                    }
                    keywordLineText->render_tokens( font_textinput_keyword,currstringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    if( colorblind_mode_on)
                    {
                        color = pawgui::theme_main->text_box_project_keyword_alt_color;
                    }
                    else
                    {
                        color = pawgui::theme_main->text_box_project_keyword_color;
                    }
                    projectKeywordLineText->render_tokens( font_textinput,currstringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    if( colorblind_mode_on)
                    {
                        color = pawgui::theme_main->text_box_project_function_alt_color;
                    }
                    else
                    {
                        color = pawgui::theme_main->text_box_project_function_color;
                    }
                    projectFunctionLineText->render_tokens( font_textinput,currstringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );



                    if( colorblind_mode_on)
                    {
                        color = pawgui::theme_main->text_box_font_variable_alt_color;
                    }
                    else
                    {
                        color = pawgui::theme_main->text_box_font_variable_color;
                    }
                    variableLineText->render_tokens( font_textinput,currstringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    color = pawgui::theme_main->text_box_font_datatype_color;
                    datatypeLineText->render_tokens( font_textinput,currstringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    color = pawgui::theme_main->text_box_font_datatype_color;
                    datatypeLineText->render_tokens( font_textinput,currstringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    color = pawgui::theme_main->text_box_font_squote_color;
                    sQuoteLineText->render_tokens( font_textinput_quote,currstringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    color = pawgui::theme_main->text_box_font_dquote_color;
                    dQuoteLineText->render_tokens( font_textinput_quote,currstringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );

                    color = pawgui::theme_main->text_box_font_comment_color;
                    commentLineText->render_tokens( font_textinput_comment,currstringToRender,textRenderXPos,textRenderYPos,lineStartXPos,lineStartXPos+charactersWithinView,color );
                }
            }

            if( mLineComment!=nullptr)
            {
                delete mLineComment;
                mLineComment = nullptr;
            }
            if( dqLineComment!=nullptr)
            {
                delete dqLineComment;
                dqLineComment = nullptr;
            }
            if( sqLineComment!=nullptr)
            {
                delete sqLineComment;
                sqLineComment = nullptr;
            }
            if( tempParseTextToAdd!=nullptr)
            {
                delete tempParseTextToAdd;
                tempParseTextToAdd = nullptr;
            }
        }
    }

    void widget_text_editor::render_linebox( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( view_space!=nullptr && cam!=nullptr )
        {
            //draws the line count box
            if( lineCountBoxWidth>0 && main_settings->showTextEditorLineCount )
            {
                gpe::gcanvas->render_rect(lineCountBox,pawgui::theme_main->text_box_outer_color,false);
                int lineSize = (editorZoomLevel*defaultLineHeight);
                int lineYPos = lineCountBox->y;
                for(  int i=lineStartYPos; i <= lineStartYPos+linesWithinView && i < (int)listOfstrings.size(); i++)
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
        if( view_space!=nullptr && cam!=nullptr && has_content() )
        {
            gpe::color * text_color = pawgui::theme_main->text_box_font_color;
            int foundSpecialLogPos = 0;
            int subCopyStartPos = 0;
            std::string stringToRender = "";
            for( int iLine=lineStartYPos; iLine <= lineStartYPos+linesWithinView && iLine < (int)listOfstrings.size(); iLine++)
            {
                stringToRender = listOfstrings[ iLine ];
                if( (int)stringToRender.size() > lineStartXPos )
                {
                    subCopyStartPos = 0;

                    text_color = pawgui::theme_main->text_box_font_color;
                    if( isTextLog)
                    {
                        text_color = pawgui::theme_main->text_box_font_color;
                        foundSpecialLogPos = stringToRender.find("Error:");
                        if( foundSpecialLogPos!=(int)std::string::npos)
                        {
                            text_color = pawgui::theme_main->text_box_font_color;
                            subCopyStartPos =7;
                        }
                        else
                        {
                            foundSpecialLogPos = stringToRender.find("Warning:");
                            if( foundSpecialLogPos!=(int)std::string::npos)
                            {
                                text_color = pawgui::theme_main->text_box_font_keyword_color;
                                subCopyStartPos =8;
                            }
                            else
                            {
                                foundSpecialLogPos = stringToRender.find("Comment:");
                                if( foundSpecialLogPos!=(int)std::string::npos)
                                {
                                    text_color = pawgui::theme_main->text_box_font_comment_color;
                                    subCopyStartPos =8;
                                }
                            }
                        }
                    }
                    stringToRender = stg_ex::get_substring(stringToRender,lineStartXPos,charactersWithinView );
                    //gpe::error_log->report("Rendering ["+stringToRender+"]..." );
                    gpe::gfs->render_text( renderBox->x+2,renderBox->y+4+(iLine-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                     stringToRender,text_color,font_textinput,gpe::fa_left,gpe::fa_top,255 );
                }
            }
        }
    }

    void widget_text_editor::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        setup_editor(view_space,cam);
        if( view_space!=nullptr &&  cam!=nullptr  )
        {
            int subCopyStartPos = 0;
            if( (int)listOfstrings.size()==0)
            {
                listOfstrings.push_back("");
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
                        if( (int)listOfstrings.size() > minHighlightYPos && minHighlightYPos>=lineStartYPos && minHighlightYPos < lineStartYPos+linesWithinView )
                        {
                            if( (int)listOfstrings.at(minHighlightYPos).size() > lineStartXPos && ( minHighlightXPos <= lineStartXPos+charactersWithinView) )
                            {
                                gpe::gcanvas->render_rectangle(
                                    renderBox->x+2+std::max(0, calculatedMax)*TEXTBOX_FONT_SIZE_WIDTH,
                                    renderBox->y+(minHighlightYPos-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                    renderBox->x+2+( std::min(mostCharactersOfText,(int)listOfstrings.at(minHighlightYPos).size() )-lineStartXPos+1)*TEXTBOX_FONT_SIZE_WIDTH,
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
                            for( i = minHighlightYPos+1; i <=maxHighlightYPos-1 && i < (int)listOfstrings.size() && i <= lineStartYPos+linesWithinView; i++ )
                            {
                                if( i >=lineStartYPos  && minHighlightYPos < lineStartYPos+linesWithinView)
                                {
                                    if( (int)listOfstrings[i].size() > lineStartXPos )
                                    {
                                        gpe::gcanvas->render_rectangle(
                                            renderBox->x+2,
                                            renderBox->y+(i-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                            renderBox->x+2+( std::min( mostCharactersOfText,(int)listOfstrings[i].size() )-lineStartXPos )*TEXTBOX_FONT_SIZE_WIDTH,
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

                        if( (int)listOfstrings.size() > maxHighlightYPos && maxHighlightYPos>=lineStartYPos  && maxHighlightYPos < lineStartYPos+linesWithinView )
                        {
                            std::string lastStrToHighlight = listOfstrings.at(maxHighlightYPos);
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
                std::string currstringToRender = "";
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
                if( showXScroll && textXScroll!=nullptr)
                {
                    textXScroll->render_self( view_space,cam);
                }
                if( showYScroll && textYScroll!=nullptr)
                {
                    textYScroll->render_self( view_space,cam);
                }

                //attempt to draw the cursor
                if( cursorYPos < (int) listOfstrings.size() )
                {
                    currentLineInView = listOfstrings[cursorYPos];
                }
            }



            gpe::gcanvas->render_rect( renderBox,pawgui::theme_main->text_box_outline_color,true);
            if( !isReadOnly && textEditor_buttonBar!=nullptr && show_buttonBar )
            {
                textEditor_buttonBar->render_self( view_space,cam);
            }

            //Renders the linebox seen on the left
            render_linebox( view_space, cam );
            if( isInUse && (prevCursorXPos >=lineStartXPos && prevCursorXPos <= lineStartXPos+charactersWithinView) &&  ( prevCursorYPos >=lineStartYPos && prevCursorYPos <= lineStartYPos+linesWithinView ) )
            {
                if( prevCursorXPos!=cursorXPos || prevCursorYPos!=cursorYPos )
                {
                    gpe::gcanvas->render_vertical_line_color( renderBox->x+(prevCursorXPos-lineStartXPos)*TEXTBOX_FONT_SIZE_WIDTH,
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
                    gpe::gcanvas->render_vertical_line_color( renderBox->x+(cursorXPos-lineStartXPos)*TEXTBOX_FONT_SIZE_WIDTH,
                                                     renderBox->y+(cursorYPos-lineStartYPos)*(editorZoomLevel*defaultLineHeight),
                                                     renderBox->y+(cursorYPos-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight),
                                                     pawgui::theme_main->text_box_font_color);
                }
                else
                {
                    gpe::gcanvas->render_vertical_line_color( renderBox->x+(cursorXPos-lineStartXPos)*TEXTBOX_FONT_SIZE_WIDTH,
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
                    if( main_statusbar!=nullptr)
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
                    syntax_compiler_term * cTerm = nullptr;
                    std::string fullPhraseToRender = "";
                    int suggestionRenderYPos = widget_box.y+view_space->y;
                    if( cursorYPos >= lineStartYPos)
                    {
                        if( cursorYPos+suggestedTextMaxInViewCount <= lineStartYPos+linesWithinView && suggestedTextMaxInViewCount < linesWithinView )
                        {
                            suggestionRenderYPos +=(cursorYPos-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight);
                            suggestionRenderYPos+=textEditor_buttonBar->get_height();
                        }
                        else if( suggestedTextMaxInViewCount >= (int)suggestedCompilerTerms.size() )
                        {
                            suggestionRenderYPos += (cursorYPos-lineStartYPos-suggestedTextMaxInViewCount)*(editorZoomLevel*defaultLineHeight);
                            suggestionRenderYPos+=textEditor_buttonBar->get_height();
                        }
                        else
                        {
                            suggestionRenderYPos+=(cursorYPos-lineStartYPos-(int)suggestedCompilerTerms.size() )*(editorZoomLevel*defaultLineHeight);
                            suggestionRenderYPos+=textEditor_buttonBar->get_height();
                        }

                        int iSuggestedEntry = 0;
                        for( int iSuggestedEntry = iSuggestedStartPos;iSuggestedEntry < (int)suggestedCompilerTerms.size() && iSuggestedEntry <  iSuggestedStartPos+suggestedTextMaxInViewCount;iSuggestedEntry++ )
                        {
                            cTerm = suggestedCompilerTerms[iSuggestedEntry];
                            if( cTerm!=nullptr)
                            {
                                if( cTerm->termType==cterm_function)
                                {
                                    if( (int)cTerm->termScope.size() > 0 && cTerm->termScope!="User Global" )
                                    {
                                        fullPhraseToRender = cTerm->termFunctionReturnType+" "+cTerm->termScope+"."+cTerm->termstring+"("+cTerm->get_parameters()+")";
                                    }
                                    else
                                    {
                                        fullPhraseToRender = cTerm->termFunctionReturnType+" "+cTerm->termstring+"("+cTerm->get_parameters()+")";
                                    }
                                }
                                else if( (int)cTerm->termScope.size() > 0 && cTerm->termScope!="User Global" )
                                {
                                    fullPhraseToRender = cTerm->termScope+"."+cTerm->termstring;
                                }
                                else
                                {
                                    fullPhraseToRender = cTerm->termstring;
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


    int widget_text_editor::replace_all_found(std::string strToReplace, std::string newstring)
    {
        widget_text_anchor * cAnchor = nullptr;
        std::string tempLine = "";
        std::string changedLine = "";
        int copiesDeleted = 0;
        export_text_anchors( gpe::app_directory_name+"replace_test.txt");
        for( int iReplace = (int)anchorPositions.size()-1; iReplace >=0; iReplace--)
        {
            cAnchor = anchorPositions.at( iReplace);
            if( cAnchor!=nullptr)
            {
                if( (int)listOfstrings.size() > cAnchor->lineNumber )
                {
                    tempLine = listOfstrings.at( cAnchor->lineNumber);
                    if( (int)tempLine.size() >= cAnchor->characterNumber+(int)strToReplace.size() )
                    {
                        changedLine = tempLine.substr(0,cAnchor->characterNumber)+newstring+tempLine.substr(cAnchor->characterNumber+(int)strToReplace.size() );
                        listOfstrings[cAnchor->lineNumber] = changedLine;
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
        if( listOfstrings.size() <=0)
        {
            listOfstrings.push_back("");
            cursorXPos = 0;
            cursorYPos = 0;
        }
        if( cursorYPos >= (int)listOfstrings.size() )
        {

            cursorYPos= (int)listOfstrings.size() - 1;
            lineToEdit = listOfstrings[cursorYPos];
            cursorXPos = (int)lineToEdit.size();
        }
        else if( cursorXPos > (int)listOfstrings[cursorYPos].size() )
        {
            cursorXPos = (int)listOfstrings[cursorYPos].size();
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
            if( tempPlace!=nullptr)
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
        widget_text_editor * fTextArea = nullptr;
        if( currentPositionInHistory > 0 && (int)savedHistory.size() > currentPositionInHistory-1 )
        {
            fTextArea = savedHistory.at(currentPositionInHistory-1);
            if( fTextArea!=nullptr)
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
            if( fTextArea!=nullptr)
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
            if( fTextArea!=nullptr)
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
        int listSize = (int)listOfstrings.size() ;
        if( listSize > 0)
        {
            std::string tempStr = listOfstrings.back();
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
            if( lineStartYPos+linesWithinView >= (int)listOfstrings.size() )
            {
                lineStartYPos =  (int)listOfstrings.size() - linesWithinView;
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
        if( cursorYPos >= lineStartYPos && cursorYPos <= lineStartYPos+linesWithinView && cursorYPos < (int)listOfstrings.size() )
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
        textEditor_buttonBar->set_height(0);
    }

    void widget_text_editor::set_writeable()
    {
        isReadOnly = true;
        textEditor_buttonBar->set_height( 32 );
    }

    void widget_text_editor::set_string(std::string newstring)
    {

    }

    void widget_text_editor::set_placeholder(std::string newstring)
    {

    }

    void widget_text_editor::set_xcursor(int newPos)
    {
        cursorXPos = newPos;
        std::string cLine = listOfstrings[cursorYPos];
        if( cursorXPos < 0 || cursorXPos < (int)cLine.size() )
        {
            cursorXPos = 0;
        }
        showCursor = true;
        cursorTimer = 0;
    }

    void widget_text_editor::set_ycursor(int newPos)
    {
        if( newPos >=0 && newPos < (int)listOfstrings.size() )
        {
            cursorYPos = newPos;
            std::string cLine = listOfstrings.at(newPos);
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
        if( view_space!=nullptr && cam!=nullptr && isCodeEditor && main_syntax_highlighter!=nullptr && !isReadOnly && isInUse )
        {
            if( main_syntax_highlighter->codeBeingSuggested && (int)main_syntax_highlighter->suggestedCompilerTerms.size() > 0 )
            {
                int suggestionRenderYPos = renderBox->y +view_space->y;
                if( cursorYPos >= lineStartYPos)
                {
                    if( cursorYPos+main_syntax_highlighter->suggestedTextMaxInViewCount <= lineStartYPos+linesWithinView && main_syntax_highlighter->suggestedTextMaxInViewCount < linesWithinView )
                    {
                        suggestionRenderYPos +=(cursorYPos-lineStartYPos+1)*(editorZoomLevel*defaultLineHeight);
                        suggestionRenderYPos+=textEditor_buttonBar->get_height();
                    }
                    else if( main_syntax_highlighter->suggestedTextMaxInViewCount >= (int)main_syntax_highlighter->suggestedCompilerTerms.size() )
                    {
                        suggestionRenderYPos += (cursorYPos-lineStartYPos-main_syntax_highlighter->suggestedTextMaxInViewCount)*(editorZoomLevel*defaultLineHeight);
                        suggestionRenderYPos+=textEditor_buttonBar->get_height();
                    }
                    else
                    {
                        suggestionRenderYPos+=(cursorYPos-lineStartYPos-(int)main_syntax_highlighter->suggestedCompilerTerms.size() )*(editorZoomLevel*defaultLineHeight);
                        suggestionRenderYPos+=textEditor_buttonBar->get_height();
                    }
                }
                main_syntax_highlighter->highlightedTermXPos = renderBox->x+view_space->x;
                main_syntax_highlighter->highlightedTermYPos = suggestionRenderYPos;
            }
            else if( main_syntax_highlighter->highlightedTerm!=nullptr && highlightYPos >=lineStartYPos && highlightYPos <= lineStartYPos+linesWithinView+3 )
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

        if( textEditor_buttonBar!=nullptr)
        {
            textEditor_buttonBar->set_coords(widget_box.x,widget_box.y);
            //sets the buttonbar to the width of the text editor( minus width of yScroll width[16 ).
            textEditor_buttonBar->set_width(widget_box.w);
            if( !isReadOnly)
            {
                textEditor_buttonBar->set_height( 32 );
                textEditor_buttonBar->enable_self();
            }
            else
            {
                textEditor_buttonBar->set_height(0);
                textEditor_buttonBar->disable_self();
            }

            renderBox->x = widget_box.x+lineCountBoxWidth -cam->x;
            renderBox->y = widget_box.y+ textEditor_buttonBar->get_height() -cam->y;
            lineCountBox->y = widget_box.y + textEditor_buttonBar->get_height() - cam->y;
            lineCountBox->h = renderBox->h = widget_box.h - textEditor_buttonBar->get_height();
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
            widget_text_editor * tempPlace = nullptr;
            if( currentPositionInHistory-1 >=0 && currentPositionInHistory-1 < (int)savedHistory.size() )
            {
                tempPlace = savedHistory.at(currentPositionInHistory-1);
                if( tempPlace!=nullptr)
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
        lineToEdit = listOfstrings[cursorYPos];
        showCursor = true;
        cursorTimer = 0;
        adjust_fortabs();
    }

    bool widget_text_editor::write_data_into_file(std::ofstream * fileTarget, int leftTabAmount, bool useNewLines, bool useLastNewLine )
    {
        if( fileTarget!=nullptr && leftTabAmount >=0)
        {
            std::string nestedTabsStr = generate_tabs( leftTabAmount  );
            if( fileTarget->is_open() )
            {
                int strSize = (int)listOfstrings.size();

                for( int i = 0; i < strSize-1; i++)
                {
                    if( useNewLines)
                    {
                        *fileTarget << nestedTabsStr+listOfstrings[i] << "\n";
                    }
                    else
                    {
                        *fileTarget <<nestedTabsStr+listOfstrings[i] << " ";
                    }
                }
                if( useNewLines)
                {
                    if( useLastNewLine)
                    {
                        *fileTarget << nestedTabsStr+listOfstrings[strSize-1] << "\n";
                    }
                    else
                    {
                        *fileTarget << nestedTabsStr+listOfstrings[strSize-1];
                    }
                }
                else
                {
                    *fileTarget <<nestedTabsStr+listOfstrings[strSize-1] << " ";
                }
                return true;
            }
        }
        return false;
    }

    bool widget_text_editor::write_short_data_into_file(std::ofstream * fileTarget, int leftTabAmount, bool useNewLines, bool useLastNewLine )
    {
        if( fileTarget!=nullptr && leftTabAmount >=0)
        {
            std::string nestedTabsStr = generate_tabs( leftTabAmount  );
            if( fileTarget->is_open() )
            {
                int strSize = (int)listOfstrings.size();
                std::string currentLineToExport = "";
                for( int i = 0; i < strSize-1; i++)
                {
                    currentLineToExport= listOfstrings[i];
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
                        *fileTarget << nestedTabsStr+listOfstrings[strSize-1] << "\n";
                    }
                    else
                    {
                        *fileTarget << nestedTabsStr+listOfstrings[strSize-1];
                    }
                }
                else
                {
                    *fileTarget <<nestedTabsStr+listOfstrings[strSize-1] << " ";
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

    void widget_text_editor_wrapped::set_string( std::string str_in)
    {
        if( paragraphText!=str_in)
        {
            paragraphText = str_in;
            update_paragraph();
        }
    }

    void widget_text_editor_wrapped::set_width(int new_width)
    {
        if( widget_box.w!=new_width)
        {
            widget_box.w = new_width;
            update_paragraph();
        }
    }

    void widget_text_editor_wrapped::set_height( int new_height)
    {
        if( widget_box.h!=new_height)
        {
            widget_box.h = new_height;
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
                if( font_textinput!=nullptr)
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
