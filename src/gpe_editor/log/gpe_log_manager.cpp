/*
gpe_log_manager.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

#include "../log/gpe_log_manager.h"
#include "../editor/gpe_editor.h"

log_manager * main_editor_log = nullptr;

log_manager::log_manager()
{
    tabsAtBottom = true;
    /*if( gpe::game_runtime!=nullptr )
    {
        int foundLineCount = gpe::game_runtime->system_log.size();
        if( foundLineCount > 0 )
        {
            gpe::game_runtime->system_log.clear();
        }
    }*/

    if( pawgui::main_anchor_controller==nullptr  )
    {
        pawgui::main_anchor_controller = new pawgui::widget_text_anchor_controller();
    }

    minLogHeight = 64;
    is_visible = true;
    logTabs = new pawgui::widget_tabbar( true );
    add_default_tab("Game Pencil Log", true);
    add_default_tab("Build Log");
    add_default_tab("Search");
    add_default_tab("Search Results");
    add_default_tab("Other Log");

    generalTextLog = new pawgui::widget_text_editor();
    generalTextLog->isCodeEditor = false;
    generalTextLog->isTextLog = true;
    generalTextLog->set_read_only();
    generalTextLog->clear_all_lines();

    debugTextLog = new pawgui::widget_text_editor();
    debugTextLog->isCodeEditor = false;
    debugTextLog->isTextLog = true;
    debugTextLog->set_read_only();
    debugTextLog->clear_all_lines();

    buildTextLog = new pawgui::widget_text_editor();
    buildTextLog->isCodeEditor = false;
    buildTextLog->isTextLog = true;
    buildTextLog->set_read_only();
    buildTextLog->clear_all_lines();

    otherLog = new pawgui::widget_text_editor();
    otherLog->isCodeEditor = false;
    otherLog->isTextLog = true;
    otherLog->set_read_only();
    otherLog->clear_all_lines();
    handle_error_log();

    logToView = nullptr;
    bottomInfoList = new pawgui::widget_panel_list();
    emptyResultsLabel = new pawgui::widget_label_text ("No results found","No results found");
    previousHeight = widget_box.h;
}

log_manager::~log_manager()
{
    if( bottomInfoList!=nullptr)
    {
        bottomInfoList->clear_list();
        delete bottomInfoList;
        bottomInfoList = nullptr;
    }
    if( logTabs!=nullptr)
    {
        delete logTabs;
        logTabs = nullptr;
    }

    if( generalTextLog!=nullptr)
    {
        delete generalTextLog;
        generalTextLog = nullptr;
    }

    if( debugTextLog!=nullptr)
    {
        delete debugTextLog;
        debugTextLog = nullptr;
    }

    if( buildTextLog!=nullptr)
    {
        delete buildTextLog;
        buildTextLog = nullptr;
    }
    if( emptyResultsLabel!=nullptr)
    {
        delete emptyResultsLabel;
        emptyResultsLabel = nullptr;
    }
}

void log_manager::add_default_tab(  std::string name,  bool open )
{
    if( (int)name.size() == 0 )
    {
        return;
    }
    gpe::key_pair * newDefaultTab = new gpe::key_pair(-1, name, name );
    defaultTabs.push_back( newDefaultTab);
    logTabs->add_new_tab(name,open);
}

void log_manager::clear_defaults()
{
    gpe::key_pair * tempPair = nullptr;
    for( int i = (int)defaultTabs.size()-1; i >=0; i--)
    {
        tempPair = defaultTabs[i];
        if( tempPair !=nullptr)
        {
            delete tempPair;
            tempPair = nullptr;
        }
    }
    defaultTabs.clear();
}

void log_manager::handle_error_log()
{
    if( gpe::error_log!=nullptr )
    {
        int foundLineCount = gpe::error_log->get_size();
        if( foundLineCount > 0 )
        {
            for( int i = 0; i < foundLineCount; i++ )
            {
                log_other_line( gpe::error_log->get_line( i ) );
            }
            gpe::error_log->clear_log();
        }
    }

}

void log_manager::clear_search_anchors()
{
    if( (int)searchAnchors.size() > 0)
    {
        pawgui::widget_text_anchor * tAnchor = nullptr;
        for( int i = (int)searchAnchors.size()-1; i >=0; i--)
        {
            tAnchor = searchAnchors[i];
            delete tAnchor;
            tAnchor = nullptr;
        }
        searchAnchors.clear();
    }
    if( pawgui::FONT_LABEL_ANCHOR!=nullptr)
    {
        pawgui::FONT_LABEL_ANCHOR->clear_cache();
    }
}

void log_manager::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
{
    view_space = gpe::camera_find( view_space );
    cam = gpe::camera_find( cam );
    pawgui::widget_basic::process_self( view_space, cam );
    handle_error_log();
    if( !isEnabled )
    {
        log_general_comment("Log manager is disabled....");
        return;
    }
    previousHeight = widget_box.h;
    //Robs the GC of text anchors
    //This is just to leave less to be depended on by text editor class
    if( pawgui::main_anchor_controller!=nullptr  )
    {
        int gcSize = pawgui::main_anchor_controller->loggedAnchors.size();
        if( gcSize > 0 )
        {
            pawgui::widget_text_anchor * tempAnchor = nullptr;
            for( int gcItr = 0; gcItr < gcSize; gcItr++ )
            {
                tempAnchor = pawgui::main_anchor_controller->loggedAnchors.at( gcItr );
                if( tempAnchor !=nullptr )
                {
                    searchAnchors.push_back( tempAnchor );
                }
            }
            pawgui::main_anchor_controller->clear_list();
        }
    }
    else
    {
        log_general_comment("pawgui::main_anchor_controller = nullptr; //....");
    }
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if( widget_box.h < 32 && pawgui::main_search_controller!=nullptr )
    {
        pawgui::main_search_controller->close_finder();
    }

    //exits if it's not in use or view
    if( !isEnabled || !is_visible || view_space==nullptr || cam==nullptr || widget_box.h <= 8 )
    {
        return;
    }

    std::string pastTab = logTabs->get_selected_name();
    if( pastTab!="Search" && pawgui::main_search_controller!=nullptr )
    {
        pawgui::main_search_controller->close_finder();
    }

    if( widget_box.h > 32)
    {
        if( widget_box.y < 96)
        {
            //widget_box.h = screen_height-pawgui::main_statusbar->get_height()-96;
        }

        if( logTabs!=nullptr )
        {
            pawgui::widget_basic::process_self(view_space,cam);

            logTabs->set_width(widget_box.w);
            if( tabsAtBottom )
            {
                logTabs->set_coords(widget_box.x, widget_box.y+widget_box.h- logTabs->get_height() );
            }
            else
            {
                logTabs->set_coords(widget_box.x, widget_box.y );
            }
            logTabs->process_self(view_space,cam);

            if( logTabs->is_rightclicked() || ( logTabs->get_tab_count() ==0 && isRightClicked) )
            {
                logTabs->tabIsRightClicked = false;
                pawgui::context_menu_open(gpe::input->mouse_position_x,gpe::input->mouse_position_y-32,256);
                pawgui::main_context_menu->add_menu_option("Copy Log to Clipboard",0);
                pawgui::main_context_menu->add_menu_option("Clear THIS Log",1);
                pawgui::main_context_menu->add_menu_option("Clear ALL Logs",2);
                pawgui::main_context_menu->add_menu_option("Clear Search Results",3);
                pawgui::popup_menu_option * toggleMenuOption = pawgui::main_context_menu->add_menu_option("Toggle",-1);

                gpe::key_pair * tempPair = nullptr;
                int iTab = 0;
                for( int iTab = 0; iTab < (int)defaultTabs.size(); iTab++)
                {
                    tempPair = defaultTabs[iTab];
                    if( tempPair !=nullptr)
                    {
                        if( logTabs->contains_tab( tempPair->key_string))
                        {
                            toggleMenuOption->add_menu_option("Toggle "+tempPair->key_string,1000+iTab,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/check-square.png"),-1,nullptr,false);
                        }
                        else
                        {
                            toggleMenuOption->add_menu_option("Toggle "+tempPair->key_string,1000+iTab,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,nullptr,false);
                        }
                    }
                }
                if( tabsAtBottom )
                {
                    pawgui::main_context_menu->add_menu_option("Tabs at Top",20);
                }
                else
                {
                    pawgui::main_context_menu->add_menu_option("Tabs at Bottom",20);
                }
                pawgui::main_context_menu->add_menu_option("Hide Logs",42);
                //pawgui::main_context_menu->add_menu_option("Tabs at Top",3);
                int foundResult = pawgui::context_menu_process();
                switch(foundResult)
                {
                case 0:
                    if( logToView!=nullptr )
                    {
                        logToView->copy_all();
                    }
                break;

                case 1:
                    if( logToView!=nullptr )
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
                    logToView = nullptr;
                break;
                }
            }

            if( pastTab != logTabs->get_selected_name() )
            {
                bottomInfoList->reset_self();
            }

            //Processes logToView
            logToView = nullptr;

            //if( widget_box.h >= 64 && font_textinput!=nullptr )
            {
                if( logTabs->get_selected_name()=="Game Pencil Log")
                {
                    logToView = generalTextLog;
                    pawgui::main_search_controller->close_finder();
                }
                else if( logTabs->get_selected_name()=="Build Log")
                {
                    logToView = buildTextLog;
                    pawgui::main_search_controller->close_finder();
                }
                else if( logTabs->get_selected_name()=="Debug Log")
                {
                    logToView = generalTextLog;
                    pawgui::main_search_controller->close_finder();
                }
                else if( logTabs->get_selected_name()=="Other" || logTabs->get_selected_name()=="Other..." || logTabs->get_selected_name() == "Other Log" )
                {
                    logToView = otherLog;
                    pawgui::main_search_controller->close_finder();
                }
                else
                {
                    logToView= nullptr;
                    if( logTabs->get_selected_name()=="Search" && bottomInfoList!=nullptr )
                    {
                        if( pastTab != "Search" )
                        {
                            gpe::input->reset_all_input();
                            //widget_box.h = 96;
                            if( pawgui::main_search_controller->previousSearchMode != pawgui::main_search_controller->textSearchMode )
                            {
                                pawgui::main_search_controller->textSearchMode = pawgui::main_search_controller->previousSearchMode;
                                if( pawgui::main_search_controller->textSearchMode==pawgui::search_mode::find_text || pawgui::main_search_controller->textSearchMode==pawgui::search_mode::find_text_all )
                                {
                                    pawgui::main_search_controller->findTextstringBox->switch_inuse( true);
                                    pawgui::main_search_controller->findTextstringBox->inputSubmitted = false;
                                    pawgui::main_search_controller->findTextstringBox->resubmissionAllowed = true;
                                    pawgui::main_search_controller->findTextstringBox->set_clicked( true);
                                    pawgui::main_search_controller->find_button->set_clicked( false);
                                    pawgui::main_search_controller->findAll_button->set_clicked( false);
                                }
                                else if( pawgui::main_search_controller->textSearchMode==pawgui::search_mode::goto_line )
                                {
                                    pawgui::main_search_controller->goToLinestringBox->switch_inuse( true);
                                    pawgui::main_search_controller->goToLinestringBox->inputSubmitted = false;
                                    pawgui::main_search_controller->goToLinestringBox->resubmissionAllowed = true;
                                    pawgui::main_search_controller->goToLinestringBox->set_clicked( true);
                                    pawgui::main_search_controller->goTo_button->set_clicked( false);
                                }
                                else
                                {
                                    pawgui::main_search_controller->findTextstringBox->switch_inuse( true);
                                    pawgui::main_search_controller->findTextstringBox->set_clicked( false);
                                    pawgui::main_search_controller->find_button->set_clicked( true);
                                    pawgui::main_search_controller->findAll_button->set_clicked( false);
                                    pawgui::main_search_controller->findTextstringBox->inputSubmitted = false;
                                    pawgui::main_search_controller->findTextstringBox->resubmissionAllowed = true;
                                }
                            }
                            //open_search_mode();
                        }
                        if( tabsAtBottom)
                        {
                            bottomInfoList->set_coords(widget_box.x,widget_box.y);
                        }
                        else
                        {
                            bottomInfoList->set_coords(widget_box.x,widget_box.y + logTabs->get_height() );
                        }
                        bottomInfoList->set_width(widget_box.w);
                        bottomInfoList->set_height(widget_box.h );
                        bottomInfoList->barYMargin = bottomInfoList->barYPadding = pawgui::padding_default/2;
                        bottomInfoList->barXMargin = bottomInfoList->barXPadding = pawgui::padding_default;
                        bottomInfoList->clear_list();
                        switch( pawgui::main_search_controller->textSearchMode )
                        {
                        //find case
                        case pawgui::search_mode::find_text:
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->findTextstringBox,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->find_button,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->findMatchCase,false);
                            break;

                        case pawgui::search_mode::find_text_all:
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->findTextstringBox,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->find_button,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->findMatchCase,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->scopeLabel,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->findScope,false);

                            break;

                        //goto case
                        case pawgui::search_mode::goto_line:
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->goToLinestringBox,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->goTo_button,false);
                            break;

                        case pawgui::search_mode::replace_text:
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->findTextstringBox,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->find_button,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->findMatchCase,true);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->replaceTextstringBox,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->replace_button,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->replaceAll_button,false);
                            break;

                        case pawgui::search_mode::replace_text_all:
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->findTextstringBox,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->find_button,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->findMatchCase,true);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->replaceTextstringBox,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->replace_button,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->replaceAll_button,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->scopeLabel,false);
                            bottomInfoList->add_gui_element(pawgui::main_search_controller->findScope,false);
                            break;

                        //find/replace case
                        default:
                            pawgui::main_search_controller->close_finder();
                            bottomInfoList->clear_list();
                            /**/
                            break;
                        }
                        bottomInfoList->process_self(view_space,cam);
                        if( pawgui::main_search_controller->textSearchMode == pawgui::search_mode::find_text_all )
                        {
                            if( pawgui::main_search_controller->find_button->is_clicked() || pawgui::main_search_controller->findTextstringBox->was_submitted() )
                            {
                                if( (int)pawgui::main_search_controller->findTextstringBox->get_string().size() > 0 )
                                {
                                    if( pawgui::main_search_controller->findScope!=nullptr)
                                    {
                                        clear_search_anchors();
                                        if( pawgui::main_search_controller->findScope->get_selected_tag()=="Open Tabs" && pawgui::main_tab_resource_bar!=nullptr )
                                        {
                                            log_general_comment("Searching Tabs for ["+pawgui::main_search_controller->findTextstringBox->get_string()+"]...");
                                            pawgui::main_tab_resource_bar->search_for_string(pawgui::main_search_controller->findTextstringBox->get_string() );
                                        }
                                        else if( pawgui::main_search_controller->findScope->get_selected_tag()=="Project Resources" && current_project!=nullptr )
                                        {
                                            if( current_project->RESC_PROJECT_FOLDER!=nullptr)
                                            {
                                                log_general_comment("Searching Project for ["+pawgui::main_search_controller->findTextstringBox->get_string()+"]...");
                                                current_project->RESC_PROJECT_FOLDER->search_for_string(pawgui::main_search_controller->findTextstringBox->get_string() );
                                            }
                                        }
                                        gpe::input->reset_all_input();
                                        open_search_results();
                                    }
                                }
                                else
                                {
                                    pawgui::display_user_alert("Search Error","You can not search for an empty string!");
                                }
                            }
                        }
                    }
                    else if( logTabs->get_selected_name()=="Search Results" && bottomInfoList!=nullptr)
                    {
                        pawgui::main_search_controller->close_finder();
                        pawgui::widget_text_anchor * fAnchor = nullptr;

                        if( tabsAtBottom)
                        {
                            bottomInfoList->set_coords(widget_box.x,widget_box.y);
                        }
                        else
                        {
                            bottomInfoList->set_coords(widget_box.x,widget_box.y + logTabs->get_height() );
                        }
                        bottomInfoList->set_width(widget_box.w);
                        bottomInfoList->set_height(widget_box.h );
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
                                fAnchor->set_width( widget_box.w );
                                if( fAnchor!=nullptr)
                                {
                                    bottomInfoList->add_gui_element(fAnchor,true);
                                }
                            }
                        }
                        bottomInfoList->set_full_width();
                        bottomInfoList->process_self(view_space,cam);

                        process_anchors();
                    }
                    else
                    {
                        pawgui::main_search_controller->close_finder();
                    }
                }

                if( logToView!=nullptr)
                {
                    if( tabsAtBottom )
                    {
                        logToView->set_coords(widget_box.x,widget_box.y );
                    }
                    else
                    {
                        logToView->set_coords(widget_box.x,logTabs->get_y2() );
                    }
                    logToView->set_width( widget_box.w );
                    logToView->set_height(widget_box.h-logTabs->get_height() );
                    logToView->process_self(view_space,cam);
                }
            }

        }
    }

}

void log_manager::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
{
    gpe::renderer_main->reset_viewpoint();
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if( isEnabled && widget_box.h >8 )
    {
        //gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->text_box_color,false);
        if( widget_box.h > 32 && logTabs!=nullptr )
        {
            //if( widget_box.h >= 64 && font_textinput!=nullptr)
            {
                if( logToView!=nullptr)
                {
                    logToView->render_self( view_space,cam);
                }
                else if( bottomInfoList!=nullptr)
                {
                    //bottomInfoList->render_self( view_space,cam);
                }
            }
            logTabs->render_self( view_space,cam);
        }
        //gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->text_box_outline_color,true);
    }
    gpe::renderer_main->reset_viewpoint();
}

void log_manager::clear_all_logs()
{
    clear_build_log();
    clear_debug_log();
    clear_general_log();

}

void log_manager::clear_debug_log()
{
    pawgui::log_entry * tempLogEntry = nullptr;
    for( int i = (int)debugLog.size()-1; i >=0; i--)
    {
        tempLogEntry = debugLog[i];
        if( tempLogEntry!=nullptr)
        {
            delete tempLogEntry;
            tempLogEntry = nullptr;
        }
    }
    debugLog.clear();
    if( debugTextLog!=nullptr)
    {
        debugTextLog->clear_all_lines();
    }
}

void log_manager::clear_build_log()
{
    pawgui::log_entry * tempLogEntry = nullptr;
    for( int i = (int)buildLog.size()-1; i >=0; i--)
    {
        tempLogEntry = buildLog[i];
        if( tempLogEntry!=nullptr)
        {
            delete tempLogEntry;
            tempLogEntry = nullptr;
        }
    }
    buildLog.clear();
    if( buildTextLog!=nullptr)
    {
        buildTextLog->clear_all_lines();
    }
}

void log_manager::clear_general_log()
{
    pawgui::log_entry * tempLogEntry = nullptr;
    for( int i = (int)generalLog.size()-1; i >=0; i--)
    {
        tempLogEntry = generalLog[i];
        if( tempLogEntry!=nullptr)
        {
            delete tempLogEntry;
            tempLogEntry = nullptr;
        }
    }
    generalLog.clear();
    if( generalTextLog!=nullptr)
    {
        generalTextLog->clear_all_lines();
    }
}

void log_manager::log_general_line(std::string newLogLine)
{
    if( generalTextLog!=nullptr)
    {
        generalTextLog->add_line(newLogLine, true );
        open_general_log();
    }
}

void log_manager::log_general_comment(std::string newLogLine)
{
    if( generalTextLog!=nullptr)
    {
        generalTextLog->add_line("Comment: "+newLogLine, true );
        open_general_log();
    }
}


void log_manager::log_general_error(std::string newLogLine)
{
    if( generalTextLog!=nullptr)
    {
        generalTextLog->add_line("Error: "+newLogLine, true );
        open_general_log();
    }
}

void log_manager::log_general_warning(std::string newLogLine)
{
    if( generalTextLog!=nullptr)
    {
        generalTextLog->add_line("Warning: "+newLogLine, true );
        open_general_log();
    }
}

void log_manager::log_debug_line(std::string newLogLine)
{
    if( generalTextLog!=nullptr)
    {
        generalTextLog->add_line("Error: "+newLogLine, true );
        open_general_log();
    }
}


void log_manager::log_build_line(std::string newLogLine)
{
    if( buildTextLog!=nullptr)
    {
        buildTextLog->add_line(newLogLine, true );
        open_build_log();
    }
}

void log_manager::log_build_comment(std::string newLogLine)
{
    if( buildTextLog!=nullptr)
    {
        buildTextLog->add_line("Comment: "+newLogLine, true );
        open_build_log();
    }
}

void log_manager::log_build_error(std::string newLogLine)
{
    if( buildTextLog!=nullptr)
    {
        buildTextLog->add_line("Error: "+newLogLine, true );
        open_build_log();
    }
}

void log_manager::log_build_warning(std::string newLogLine)
{
    if( buildTextLog!=nullptr)
    {
        buildTextLog->add_line("Warning: "+newLogLine, true );
        open_build_log();
    }
}


void log_manager::log_other_line(std::string newLogLine)
{
    gpe::error_log->report(newLogLine);
    if( otherLog!=nullptr)
    {
        otherLog->add_line(newLogLine, true );
        //open_other_log();
    }
}

void log_manager::open_general_log()
{
    if( widget_box.h < minLogHeight )
    {
        widget_box.h = minLogHeight;
    }
    if( logTabs!=nullptr)
    {
        logTabs->open_tab("Game Pencil Log");
    }
}

void log_manager::open_build_log()
{
    if( widget_box.h < minLogHeight )
    {
        widget_box.h = minLogHeight ;
    }
    if( logTabs!=nullptr)
    {
        logTabs->open_tab("Build Log");
    }
}

void log_manager::open_other_log()
{
    if( widget_box.h < minLogHeight )
    {
        widget_box.h = minLogHeight;
    }
    if( logTabs!=nullptr)
    {
        logTabs->open_tab("Other Log");
    }
}

void log_manager::open_replace_mode()
{
    if( widget_box.h < 128 )
    {
        widget_box.h = 128;
    }
    bottomInfoList->reset_self();
    if( logTabs!=nullptr)
    {
        logTabs->open_tab("Search");
    }
}

void log_manager::open_search_mode()
{
    if( widget_box.h < 96)
    {
        widget_box.h = 96;
    }
    bottomInfoList->reset_self();
    if( logTabs!=nullptr)
    {
        logTabs->open_tab("Search");
    }
}

void log_manager::open_search_results()
{
    if( widget_box.h < 128)
    {
        widget_box.h = 128;
    }
    if( logTabs!=nullptr)
    {
        logTabs->open_tab("Search Results");
    }
    bottomInfoList->reset_self();
    if( pawgui::main_settings!=nullptr)
    {
        pawgui::main_search_controller->close_finder();
    }
}

void log_manager::process_anchors()
{
    int anchorCount = (int)searchAnchors.size();
    if(anchorCount ==  0  || pawgui::main_anchor_controller== nullptr )
    {
        return;
    }
    pawgui::widget_text_anchor * tAnchor = nullptr;
    for( int i = 0; i < anchorCount; i++)
    {
        tAnchor = searchAnchors[i];
        if( tAnchor!=nullptr && tAnchor->is_clicked() )
        {
            if( (int) tAnchor->anchorProjectName.size() > 0 )
            {
                GPE_ProjectFolder * foundProject = editor_gui_main->find_project_from_filename( tAnchor->anchorProjectName);
                pawgui::widget_resource_container * foundResContainer = nullptr;
                pawgui::general_resource * foundGameResource = nullptr;
                if( foundProject!=nullptr && foundProject->RESC_PROJECT_FOLDER!=nullptr )
                {
                    if( tAnchor->anchorProjectResourceId >=0 )
                    {
                        foundResContainer = foundProject->RESC_PROJECT_FOLDER->find_resource_from_id( tAnchor->anchorProjectResourceId,true,false);
                    }
                    else if( tAnchor->anchorProjectResourceId < 0 )
                    {
                        foundResContainer = foundProject->RESC_PROJECT_SETTINGS;
                    }
                }

                if( foundResContainer!=nullptr)
                {
                    foundGameResource = foundResContainer->get_held_resource();
                }
                if( foundGameResource!=nullptr)
                {
                    foundGameResource->open_code( tAnchor->lineNumber, tAnchor->characterNumber, tAnchor->lineMessage );
                    if( pawgui::main_tab_resource_bar!=nullptr)
                    {
                        pawgui::main_tab_resource_bar->add_new_tab(foundGameResource);
                    }
                }
            }
            //gpe::input->reset_all_input();
        }
    }
}
void log_manager::toggle_manager()
{
    if( isEnabled )
    {
        previousHeight = widget_box.h;
        widget_box.h  = 0;
        minLogHeight  = 0;
    }
    else
    {
        widget_box.h = previousHeight;
        minLogHeight = 64;
    }
    isEnabled = !isEnabled;
}
