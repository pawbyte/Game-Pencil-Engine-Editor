/*
gpe_log_manager.cpp
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

#include "gpe_log_manager.h"

GPE_LogManager * GPE_Main_Logs = NULL;

GPE_LogManager::GPE_LogManager()
{
    tabsAtBottom = true;
    /*if( gpe!=NULL )
    {
        int foundLineCount = gpe->runtimeLog.size();
        if( foundLineCount > 0 )
        {
            gpe->runtimeLog.clear();
        }
    }*/

    if( GPE_ANCHOR_GC==NULL  )
    {
        GPE_ANCHOR_GC = new GPE_TextAnchorGBCollector();
    }

    minLogHeight = 64;
    isVisible = true;
    logTabs = new GPE_TabBar( true );
    add_default_tab("Game Pencil Log", true);
    add_default_tab("Build Log");
    add_default_tab("Search");
    add_default_tab("Search Results");
    add_default_tab("Other");

    generalTextLog = new GPE_TextAreaInputBasic();
    generalTextLog->isCodeEditor = false;
    generalTextLog->isTextLog = true;
    generalTextLog->set_read_only();
    generalTextLog->clear_all_lines();

    debugTextLog = new GPE_TextAreaInputBasic();
    debugTextLog->isCodeEditor = false;
    debugTextLog->isTextLog = true;
    debugTextLog->set_read_only();
    debugTextLog->clear_all_lines();

    buildTextLog = new GPE_TextAreaInputBasic();
    buildTextLog->isCodeEditor = false;
    buildTextLog->isTextLog = true;
    buildTextLog->set_read_only();
    buildTextLog->clear_all_lines();

    otherLog = new GPE_TextAreaInputBasic();
    otherLog->isCodeEditor = false;
    otherLog->isTextLog = true;
    otherLog->set_read_only();
    otherLog->clear_all_lines();
    handle_error_log();

    logToView = NULL;
    bottomInfoList = new GPE_GuiElementList();
    emptyResultsLabel = new GPE_Label_Text("No results found","No results found");
    previousHeight = elementBox.h;
}

GPE_LogManager::~GPE_LogManager()
{
    if( bottomInfoList!=NULL)
    {
        bottomInfoList->clear_list();
        delete bottomInfoList;
        bottomInfoList = NULL;
    }
    if( logTabs!=NULL)
    {
        delete logTabs;
        logTabs = NULL;
    }

    if( generalTextLog!=NULL)
    {
        delete generalTextLog;
        generalTextLog = NULL;
    }

    if( debugTextLog!=NULL)
    {
        delete debugTextLog;
        debugTextLog = NULL;
    }

    if( buildTextLog!=NULL)
    {
        delete buildTextLog;
        buildTextLog = NULL;
    }
    if( emptyResultsLabel!=NULL)
    {
        delete emptyResultsLabel;
        emptyResultsLabel = NULL;
    }
}

void GPE_LogManager::add_default_tab(  std::string name,  bool open )
{
    if( (int)name.size() == 0 )
    {
        return;
    }
    GPE_KeyPair * newDefaultTab = new GPE_KeyPair(-1, name, name );
    defaultTabs.push_back( newDefaultTab);
    logTabs->add_new_tab(name,open);
}

void GPE_LogManager::clear_defaults()
{
    GPE_KeyPair * tempPair = NULL;
    for( int i = (int)defaultTabs.size()-1; i >=0; i--)
    {
        tempPair = defaultTabs[i];
        if( tempPair !=NULL)
        {
            delete tempPair;
            tempPair = NULL;
        }
    }
    defaultTabs.clear();
}

void GPE_LogManager::handle_error_log()
{
    if( gpe!=NULL )
    {
        int foundLineCount = gpe->runtimeLog.size();
        if( foundLineCount > 0 )
        {
            for( int i = 0; i < foundLineCount; i++ )
            {
                log_other_line( gpe->runtimeLog[i] );
            }
            gpe->runtimeLog.clear();
        }
    }

}

void GPE_LogManager::clear_search_anchors()
{
    if( (int)searchAnchors.size() > 0)
    {
        GPE_TextAnchor * tAnchor = NULL;
        for( int i = (int)searchAnchors.size()-1; i >=0; i--)
        {
            tAnchor = searchAnchors[i];
            delete tAnchor;
            tAnchor = NULL;
        }
        searchAnchors.clear();
    }
    if( FONT_LABEL_ANCHOR!=NULL)
    {
        FONT_LABEL_ANCHOR->clear_cache();
    }
}

void GPE_LogManager::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    viewedSpace = GPE_find_camera( viewedSpace );
    cam = GPE_find_camera( cam );
    GPE_GeneralGuiElement::process_self( viewedSpace, cam );
    handle_error_log();
    if( !isEnabled )
    {
        log_general_comment("Log manager is disabled....");
        return;
    }
    previousHeight = elementBox.h;
    //Robs the GC of text anchors
    //This is just to leave less to be depended on by text editor class
    if( GPE_ANCHOR_GC!=NULL  )
    {
        int gcSize = GPE_ANCHOR_GC->loggedAnchors.size();
        if( gcSize > 0 )
        {
            GPE_TextAnchor * tempAnchor = NULL;
            for( int gcItr = 0; gcItr < gcSize; gcItr++ )
            {
                tempAnchor = GPE_ANCHOR_GC->loggedAnchors.at( gcItr );
                if( tempAnchor !=NULL )
                {
                    searchAnchors.push_back( tempAnchor );
                }
            }
            GPE_ANCHOR_GC->clear_list();
        }
    }
    else
    {
        log_general_comment("GPE_Anchor_GC = NULL; //....");
    }
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( elementBox.h < 32 && MAIN_SEARCH_CONTROLLER!=NULL )
    {
        MAIN_SEARCH_CONTROLLER->close_finder();
    }

    if( isEnabled && isVisible && viewedSpace!=NULL && cam!=NULL && elementBox.h >8 )
    {
        std::string pastTab = logTabs->get_selected_name();
        if( pastTab!="Search" && MAIN_SEARCH_CONTROLLER!=NULL )
        {
            MAIN_SEARCH_CONTROLLER->close_finder();
        }

        if( elementBox.h > 32)
        {
            if( elementBox.y < 96)
            {
                //elementBox.h = SCREEN_HEIGHT-GPE_Main_Statusbar->get_height()-96;
            }

            if( logTabs!=NULL )
            {
                GPE_GeneralGuiElement::process_self(viewedSpace,cam);

                logTabs->set_width(elementBox.w);
                if( tabsAtBottom )
                {
                    logTabs->set_coords(elementBox.x, elementBox.y+elementBox.h- logTabs->get_height() );
                }
                else
                {
                    logTabs->set_coords(elementBox.x, elementBox.y+GENERAL_GPE_PADDING );
                }
                logTabs->process_self(viewedSpace,cam);

                if( logTabs->is_rightclicked() || ( logTabs->get_tab_count() ==0 && isRightClicked) )
                {
                    logTabs->tabIsRightClicked = false;
                    GPE_open_context_menu(input->mouse_x,input->mouse_y-32,256);
                    MAIN_CONTEXT_MENU->add_menu_option("Copy Log to Clipboard",0);
                    MAIN_CONTEXT_MENU->add_menu_option("Clear THIS Log",1);
                    MAIN_CONTEXT_MENU->add_menu_option("Clear ALL Logs",2);
                    MAIN_CONTEXT_MENU->add_menu_option("Clear Search Results",3);
                    GPE_PopUpMenu_Option * toggleMenuOption = MAIN_CONTEXT_MENU->add_menu_option("Toggle",-1);

                    GPE_KeyPair * tempPair = NULL;
                    int iTab = 0;
                    for( int iTab = 0; iTab < (int)defaultTabs.size(); iTab++)
                    {
                        tempPair = defaultTabs[iTab];
                        if( tempPair !=NULL)
                        {
                            if( logTabs->contains_tab( tempPair->keyString))
                            {
                                toggleMenuOption->add_menu_option("Toggle "+tempPair->keyString,1000+iTab,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/check-square.png"),-1,NULL,false);
                            }
                            else
                            {
                                toggleMenuOption->add_menu_option("Toggle "+tempPair->keyString,1000+iTab,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);
                            }
                        }
                    }
                    if( tabsAtBottom )
                    {
                        MAIN_CONTEXT_MENU->add_menu_option("Tabs at Top",20);
                    }
                    else
                    {
                        MAIN_CONTEXT_MENU->add_menu_option("Tabs at Bottom",20);
                    }
                    MAIN_CONTEXT_MENU->add_menu_option("Hide Logs",42);
                    //MAIN_CONTEXT_MENU->add_menu_option("Tabs at Top",3);
                    int foundResult = GPE_Get_Context_Result();
                    switch(foundResult)
                    {
                    case 0:
                        if( logToView!=NULL )
                        {
                            logToView->copy_all();
                        }
                    break;

                    case 1:
                        if( logToView!=NULL )
                        {
                            logToView->clear_all_lines();
                        }
                    break;

                    case 2:
                        clear_all_logs();
                    break;

                    case 3:
                        clear_search_anchors();
                    break;
                    case 20:
                        tabsAtBottom = !tabsAtBottom;
                    break;
                    case 42:
                        toggle_manager();
                    break;
                    default:
                        logToView = NULL;
                    break;
                    }
                }

                if( pastTab != logTabs->get_selected_name() )
                {
                    bottomInfoList->reset_self();
                }

                //Processes logToView
                logToView = NULL;

                //if( elementBox.h >= 64 && FONT_TEXTINPUT!=NULL )
                {
                    if( logTabs->get_selected_name()=="Game Pencil Log")
                    {
                        logToView = generalTextLog;
                        MAIN_SEARCH_CONTROLLER->close_finder();
                    }
                    else if( logTabs->get_selected_name()=="Build Log")
                    {
                        logToView = buildTextLog;
                        MAIN_SEARCH_CONTROLLER->close_finder();
                    }
                    else if( logTabs->get_selected_name()=="Debug Log")
                    {
                        logToView = generalTextLog;
                        MAIN_SEARCH_CONTROLLER->close_finder();
                    }
                    else if( logTabs->get_selected_name()=="Other" || logTabs->get_selected_name()=="Other..." )
                    {
                        logToView = otherLog;
                        MAIN_SEARCH_CONTROLLER->close_finder();
                    }
                    else
                    {
                        logToView= NULL;
                        if( logTabs->get_selected_name()=="Search" && bottomInfoList!=NULL )
                        {
                            if( pastTab != "Search" )
                            {
                                input->reset_all_input();
                                //elementBox.h = 96;
                                if( MAIN_SEARCH_CONTROLLER->previousSearchMode != MAIN_SEARCH_CONTROLLER->textSearchMode )
                                {
                                    MAIN_SEARCH_CONTROLLER->textSearchMode = MAIN_SEARCH_CONTROLLER->previousSearchMode;
                                    if( MAIN_SEARCH_CONTROLLER->textSearchMode==SEARCH_MODE_FIND || MAIN_SEARCH_CONTROLLER->textSearchMode==SEARCH_MODE_FINDALL )
                                    {
                                        MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
                                        MAIN_SEARCH_CONTROLLER->findTextStringBox->inputSubmitted = false;
                                        MAIN_SEARCH_CONTROLLER->findTextStringBox->resubmissionAllowed = true;
                                        MAIN_SEARCH_CONTROLLER->findTextStringBox->set_clicked( true);
                                        MAIN_SEARCH_CONTROLLER->findButton->set_clicked( false);
                                        MAIN_SEARCH_CONTROLLER->findAllButton->set_clicked( false);
                                    }
                                    else if( MAIN_SEARCH_CONTROLLER->textSearchMode==SEARCH_MODE_GOTO )
                                    {
                                        MAIN_SEARCH_CONTROLLER->goToLineStringBox->switch_inuse( true);
                                        MAIN_SEARCH_CONTROLLER->goToLineStringBox->inputSubmitted = false;
                                        MAIN_SEARCH_CONTROLLER->goToLineStringBox->resubmissionAllowed = true;
                                        MAIN_SEARCH_CONTROLLER->goToLineStringBox->set_clicked( true);
                                        MAIN_SEARCH_CONTROLLER->goToButton->set_clicked( false);
                                    }
                                    else
                                    {
                                        MAIN_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
                                        MAIN_SEARCH_CONTROLLER->findTextStringBox->set_clicked( false);
                                        MAIN_SEARCH_CONTROLLER->findButton->set_clicked( true);
                                        MAIN_SEARCH_CONTROLLER->findAllButton->set_clicked( false);
                                        MAIN_SEARCH_CONTROLLER->findTextStringBox->inputSubmitted = false;
                                        MAIN_SEARCH_CONTROLLER->findTextStringBox->resubmissionAllowed = true;
                                    }
                                }
                                //open_search_mode();
                            }
                            if( tabsAtBottom)
                            {
                                bottomInfoList->set_coords(elementBox.x,elementBox.y);
                            }
                            else
                            {
                                bottomInfoList->set_coords(elementBox.x,elementBox.y + logTabs->get_height() +GENERAL_GPE_PADDING );
                            }
                            bottomInfoList->set_width(elementBox.w);
                            bottomInfoList->set_height(elementBox.h-32);
                            bottomInfoList->barYMargin = bottomInfoList->barYPadding = GENERAL_GPE_PADDING/2;
                            bottomInfoList->barXMargin = bottomInfoList->barXPadding = GENERAL_GPE_PADDING;
                            bottomInfoList->clear_list();
                            switch( MAIN_SEARCH_CONTROLLER->textSearchMode )
                            {
                            //find case
                            case SEARCH_MODE_FIND:
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findTextStringBox,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findButton,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findMatchCase,false);
                                break;

                            case SEARCH_MODE_FINDALL:
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findTextStringBox,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findButton,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findMatchCase,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->scopeLabel,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findScope,false);

                                break;

                            //goto case
                            case SEARCH_MODE_GOTO:
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->goToLineStringBox,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->goToButton,false);
                                break;

                            case SEARCH_MODE_REPLACE:
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findTextStringBox,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findButton,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findMatchCase,true);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->replaceTextStringBox,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->replaceButton,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->replaceAllButton,false);
                                break;

                            case SEARCH_MODE_REPLACEALL:
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findTextStringBox,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findButton,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findMatchCase,true);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->replaceTextStringBox,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->replaceButton,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->replaceAllButton,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->scopeLabel,false);
                                bottomInfoList->add_gui_element(MAIN_SEARCH_CONTROLLER->findScope,false);
                                break;

                            //find/replace case
                            default:
                                MAIN_SEARCH_CONTROLLER->close_finder();
                                bottomInfoList->clear_list();
                                /**/
                                break;
                            }
                            bottomInfoList->process_self(viewedSpace,cam);
                            if( MAIN_SEARCH_CONTROLLER->textSearchMode == SEARCH_MODE_FINDALL )
                            {
                                if( MAIN_SEARCH_CONTROLLER->findButton->is_clicked() || MAIN_SEARCH_CONTROLLER->findTextStringBox->was_submitted() )
                                {
                                    if( (int)MAIN_SEARCH_CONTROLLER->findTextStringBox->get_string().size() > 0 )
                                    {
                                        if( MAIN_SEARCH_CONTROLLER->findScope!=NULL)
                                        {
                                            clear_search_anchors();
                                            if( MAIN_SEARCH_CONTROLLER->findScope->get_selected_tag()=="Open Tabs" && GPE_Main_TabManager!=NULL )
                                            {
                                                log_general_comment("Searching Tabs for ["+MAIN_SEARCH_CONTROLLER->findTextStringBox->get_string()+"]...");
                                                GPE_Main_TabManager->search_for_string(MAIN_SEARCH_CONTROLLER->findTextStringBox->get_string() );
                                            }
                                            /*
                                            else if( MAIN_SEARCH_CONTROLLER->findScope->get_selected_tag()=="Project Resources" && CURRENT_PROJECT!=NULL )
                                            {
                                                if( CURRENT_PROJECT->RESC_PROJECT_FOLDER!=NULL)
                                                {
                                                    log_general_comment("Searching Project for ["+MAIN_SEARCH_CONTROLLER->findTextStringBox->get_string()+"]...");
                                                    CURRENT_PROJECT->RESC_PROJECT_FOLDER->search_for_string(MAIN_SEARCH_CONTROLLER->findTextStringBox->get_string() );
                                                }
                                            }
                                            */
                                            input->reset_all_input();
                                            open_search_results();
                                        }
                                    }
                                    else
                                    {
                                        display_user_alert("Search Error","You can not search for an empty string!");
                                    }
                                }
                            }
                        }
                        else if( logTabs->get_selected_name()=="Search Results" && bottomInfoList!=NULL)
                        {
                            MAIN_SEARCH_CONTROLLER->close_finder();
                            GPE_TextAnchor * fAnchor = NULL;

                            if( tabsAtBottom)
                            {
                                bottomInfoList->set_coords(elementBox.x,elementBox.y);
                            }
                            else
                            {
                                bottomInfoList->set_coords(elementBox.x,elementBox.y + logTabs->get_height() +GENERAL_GPE_PADDING );
                            }
                            bottomInfoList->set_width(elementBox.w);
                            bottomInfoList->set_height(elementBox.h-32);
                            bottomInfoList->clear_list();
                            if( (int)searchAnchors.size()== 0 )
                            {
                                bottomInfoList->add_gui_element(emptyResultsLabel,true);
                            }
                            else
                            {
                                bottomInfoList->barYMargin = bottomInfoList->barXMargin =  0;
                                bottomInfoList->barYPadding = bottomInfoList->barXPadding = 0;
                                for( int iAnchor = 0; iAnchor < (int)searchAnchors.size(); iAnchor++)
                                {
                                    fAnchor = searchAnchors[iAnchor];
                                    if( fAnchor!=NULL)
                                    {
                                        bottomInfoList->add_gui_element(fAnchor,true);
                                    }
                                }
                            }
                            bottomInfoList->set_full_width();
                            bottomInfoList->process_self(viewedSpace,cam);
                        }
                        else
                        {
                            MAIN_SEARCH_CONTROLLER->close_finder();
                        }
                    }

                    if( logToView!=NULL)
                    {
                        if( tabsAtBottom )
                        {
                            logToView->set_coords(elementBox.x,elementBox.y+GENERAL_GPE_PADDING );
                        }
                        else
                        {
                            logToView->set_coords(elementBox.x,logTabs->get_y2pos() );
                        }
                        logToView->set_width( elementBox.w );
                        logToView->set_height(elementBox.h-logTabs->get_height()- GENERAL_GPE_PADDING );
                        logToView->process_self(viewedSpace,cam);
                    }
                }

            }
        }
    }
    else
    {
        log_general_comment("Not enable, visible, or viewedSpace/camera or box too short...");
    }
}

void GPE_LogManager::render_self( GPE_Rect * viewedSpace,GPE_Rect *cam, bool forceRedraw)
{
    MAIN_RENDERER->reset_viewpoint();
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( isEnabled && elementBox.h >8 )
    {
        //gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Text_Box_Color,false);
        if( elementBox.h > 32 && logTabs!=NULL )
        {
            //if( elementBox.h >= 64 && FONT_TEXTINPUT!=NULL)
            {
                if( logToView!=NULL)
                {
                    logToView->render_self( viewedSpace,cam,forceRedraw);
                }
                else if( bottomInfoList!=NULL)
                {
                    bottomInfoList->render_self( viewedSpace,cam,forceRedraw);
                }
            }
            logTabs->render_self( viewedSpace,cam,forceRedraw);
        }
        gcanvas->render_rect( &elementBox,GPE_MAIN_THEME->Text_Box_Outline_Color,true);
    }
    MAIN_RENDERER->reset_viewpoint();
}

void GPE_LogManager::clear_all_logs()
{
    clear_build_log();
    clear_debug_log();
    clear_general_log();

}

void GPE_LogManager::clear_debug_log()
{
    GPE_Log_Entry * tempLogEntry = NULL;
    for( int i = (int)debugLog.size()-1; i >=0; i--)
    {
        tempLogEntry = debugLog[i];
        if( tempLogEntry!=NULL)
        {
            delete tempLogEntry;
            tempLogEntry = NULL;
        }
    }
    debugLog.clear();
    if( debugTextLog!=NULL)
    {
        debugTextLog->clear_all_lines();
    }
}

void GPE_LogManager::clear_build_log()
{
    GPE_Log_Entry * tempLogEntry = NULL;
    for( int i = (int)buildLog.size()-1; i >=0; i--)
    {
        tempLogEntry = buildLog[i];
        if( tempLogEntry!=NULL)
        {
            delete tempLogEntry;
            tempLogEntry = NULL;
        }
    }
    buildLog.clear();
    if( buildTextLog!=NULL)
    {
        buildTextLog->clear_all_lines();
    }
}

void GPE_LogManager::clear_general_log()
{
    GPE_Log_Entry * tempLogEntry = NULL;
    for( int i = (int)generalLog.size()-1; i >=0; i--)
    {
        tempLogEntry = generalLog[i];
        if( tempLogEntry!=NULL)
        {
            delete tempLogEntry;
            tempLogEntry = NULL;
        }
    }
    generalLog.clear();
    if( generalTextLog!=NULL)
    {
        generalTextLog->clear_all_lines();
    }
}

void GPE_LogManager::log_general_line(std::string newLogLine)
{
    if( generalTextLog!=NULL)
    {
        generalTextLog->add_line(newLogLine, true );
        open_general_log();
    }
}

void GPE_LogManager::log_general_comment(std::string newLogLine)
{
    if( generalTextLog!=NULL)
    {
        generalTextLog->add_line("Comment: "+newLogLine, true );
        open_general_log();
    }
}


void GPE_LogManager::log_general_error(std::string newLogLine)
{
    if( generalTextLog!=NULL)
    {
        generalTextLog->add_line("Error: "+newLogLine, true );
        open_general_log();
    }
}

void GPE_LogManager::log_general_warning(std::string newLogLine)
{
    if( generalTextLog!=NULL)
    {
        generalTextLog->add_line("Warning: "+newLogLine, true );
        open_general_log();
    }
}

void GPE_LogManager::log_debug_line(std::string newLogLine)
{
    if( generalTextLog!=NULL)
    {
        generalTextLog->add_line("Error: "+newLogLine, true );
        open_general_log();
    }
}


void GPE_LogManager::log_build_line(std::string newLogLine)
{
    if( buildTextLog!=NULL)
    {
        buildTextLog->add_line(newLogLine, true );
        open_build_log();
    }
}

void GPE_LogManager::log_build_comment(std::string newLogLine)
{
    if( buildTextLog!=NULL)
    {
        buildTextLog->add_line("Comment: "+newLogLine, true );
        open_build_log();
    }
}

void GPE_LogManager::log_build_error(std::string newLogLine)
{
    if( buildTextLog!=NULL)
    {
        buildTextLog->add_line("Error: "+newLogLine, true );
        open_build_log();
    }
}

void GPE_LogManager::log_build_warning(std::string newLogLine)
{
    if( buildTextLog!=NULL)
    {
        buildTextLog->add_line("Warning: "+newLogLine, true );
        open_build_log();
    }
}


void GPE_LogManager::log_other_line(std::string newLogLine)
{
    GPE_Report(newLogLine);
    if( otherLog!=NULL)
    {
        otherLog->add_line(newLogLine, true );
        //open_other_log();
    }
}

void GPE_LogManager::open_general_log()
{
    if( elementBox.h < minLogHeight )
    {
        elementBox.h = minLogHeight;
    }
    if( logTabs!=NULL)
    {
        logTabs->open_tab("Game Pencil Log");
    }
}

void GPE_LogManager::open_build_log()
{
    if( elementBox.h < minLogHeight )
    {
        elementBox.h = minLogHeight ;
    }
    if( logTabs!=NULL)
    {
        logTabs->open_tab("Build Log");
    }
}

void GPE_LogManager::open_other_log()
{
    if( elementBox.h < minLogHeight )
    {
        elementBox.h = minLogHeight;
    }
    if( logTabs!=NULL)
    {
        logTabs->open_tab("Other");
    }
}

void GPE_LogManager::open_replace_mode()
{
    if( elementBox.h < 128 )
    {
        elementBox.h = 128;
    }
    bottomInfoList->reset_self();
    if( logTabs!=NULL)
    {
        logTabs->open_tab("Search");
    }
}

void GPE_LogManager::open_search_mode()
{
    if( elementBox.h < 96)
    {
        elementBox.h = 96;
    }
    bottomInfoList->reset_self();
    if( logTabs!=NULL)
    {
        logTabs->open_tab("Search");
    }
}

void GPE_LogManager::open_search_results()
{
    if( elementBox.h < 128)
    {
        elementBox.h = 128;
    }
    if( logTabs!=NULL)
    {
        logTabs->open_tab("Search Results");
    }
    bottomInfoList->reset_self();
    if( MAIN_GUI_SETTINGS!=NULL)
    {
        MAIN_SEARCH_CONTROLLER->close_finder();
    }
}

void GPE_LogManager::toggle_manager()
{
    if( isEnabled )
    {
        previousHeight = elementBox.h;
        elementBox.h  = 0;
        minLogHeight  = 0;
    }
    else
    {
        elementBox.h = previousHeight;
        minLogHeight = 64;
    }
    isEnabled = !isEnabled;
}
