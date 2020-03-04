/*
gpe_editor.cpp
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

#include "../gpe/gpe.h"
#include "gpe_editor.h"
#include "gpe_editor_start_page.h"
#include "gpe_project_resources.h"
#include "gpe_editor_helppage.h"
#include "gpe_cpp_builder_settings.h"
#include "gpe_gamepad_tester.h"


GPE_Gui_Engine * editor_gui_main = NULL;

GPE_PopUpMenu_Option * main_TOOLBAR_RECENT_PROJECTS = NULL;

//generalGameResource * selected_GENERAL_RESOUCE = NULL;
gpe::texture_base * GPE_LOGO = NULL;
gpe::texture_base * GPE_TEXTURE_TRANSPARENT_BG = NULL;

std::string GPE_BUILD_NAMES[ gpe::system_os_max ];

bool set_current_gpe_project( GPE_ProjectFolder * newMainProject )
{
    bool projectChanged = false;
    if( newMainProject!=NULL)
    {
        current_project = newMainProject;
        if( project_current_name != newMainProject->get_project_file_name() )
        {
            project_current_name = newMainProject->get_project_file_name();
            //Integrate syntax for syntax highlighter and such...
            current_project->refresh_gui_syntax();
        }
    }
    else if( current_project!=NULL)
    {
        projectChanged = true;
    }
    return projectChanged;
}

bool set_current_gpe_project_from_name( std::string newMainProjectName )
{
    bool projectChanged = false;
    if( (int)newMainProjectName.size() > 0  )
    {
        if( project_current_name != newMainProjectName)
        {
            projectChanged = true;
        }
        project_current_name = newMainProjectName;
        current_project = editor_gui_main->find_project_from_filename(project_current_name);
        if( current_project!=NULL && projectChanged)
        {
            //Integrate for syntax highlighter and such...
            editor_gui_main->process_window_title();
            current_project->refresh_gui_syntax();
        }
    }
    else if( current_project!=NULL)
    {
        projectChanged = true;
    }
    return projectChanged;
}

GPE_Gui_Engine::GPE_Gui_Engine()
{
    includeNintendoSwitchExport = false;
    includePlaystation4Export = false;
    includeXboxOneExport = false;
    if( gpe::window_controller_main!=NULL)
    {
        gpe::window_controller_main->set_window_title("Game Pencil Engine");
        gpe::error_log->report("gpe::window_controller_main is not NULL...");
    }
    else
    {
        gpe::error_log->report("gpe::window_controller_main is NULL...");
    }
    theme_default = editor_theme_controller->add_theme("default");
    theme_default->load_theme( gpe::app_directory_name+"themes/default.gpf");
    theme_main = theme_default;

    load_tips();
    fileOpenProjectDir = "";
    fileOpenProjectIconDir = "";
    fileSaveProjectDir = "";


    mainToolBar = NULL;
    mainResourceTree = NULL;

    //findTextStringBox->set_label("Find:");
    //goToLineStringBox->set_label("Go To Line:");
    //replaceTextStringBox->set_label("Replace:");

    showTipsAtStartUp = true;
    showStartPageAtStartUp = true;

    //loads the recent file list
    std::string recentFileListFileName =  gpe::get_user_settings_folder()+"recent_projects.txt";
    std::ifstream recentFileListFile( recentFileListFileName.c_str() );
    if( !recentFileListFile.fail() )
    {
        std::string currLine = "";
        //makes sure the file is open
        if (recentFileListFile.is_open())
        {
            while ( recentFileListFile.good() )
            {
                getline (recentFileListFile,currLine); //gets the next line of the file
                currLine = stg_ex::trim_left_inplace(currLine);
                currLine = stg_ex::trim_right_inplace(currLine);
                add_to_recent_project_list(currLine,false);
            }
            recentFileListFile.close();
        }
    }
}

GPE_Gui_Engine::~GPE_Gui_Engine()
{
    GPE_ProjectFolder * tempProject = NULL;
    for( int i = (int)gpeProjects.size()-1; i >=0; i--)
    {
        tempProject = gpeProjects[i];
        if( tempProject!=NULL)
        {
            delete tempProject;
            tempProject= NULL;
        }
    }
    gpeProjects.clear();

    if( mainToolBar!=NULL)
    {
        delete mainToolBar;
        mainToolBar = NULL;
    }

    if( mainResourceTree!=NULL)
    {
        delete mainResourceTree;
        mainResourceTree = NULL;
    }

    if( theme_main!=NULL)
    {
        delete theme_main;
        theme_main = NULL;
    }

    if( main_CONTEXT_MENU!=NULL)
    {
        delete main_CONTEXT_MENU;
        main_CONTEXT_MENU = NULL;
    }
    if(GPE_main_TabManager!=NULL)
    {
        delete GPE_main_TabManager;
        GPE_main_TabManager = NULL;
    }

    if(GPE_main_Logs!=NULL)
    {
        delete GPE_main_Logs;
        GPE_main_Logs = NULL;
    }
    if( GPE_ANCHOR_GC!=NULL )
    {
        GPE_ANCHOR_GC->clear_list( false );
        delete GPE_ANCHOR_GC;
        GPE_ANCHOR_GC = NULL;
    }

    if( texture_color_picker_gradient!=NULL)
    {
        delete texture_color_picker_gradient;
        texture_color_picker_gradient = NULL;
    }

    if( GPE_LOGO!=NULL)
    {
        delete GPE_LOGO;
        GPE_LOGO = NULL;
    }


}

void GPE_Gui_Engine::add_to_recent_project_list(std::string newProjectFileName, bool saveData)
{
    int cSize = (int)gpeRecentProjects.size();
    int i = 0;
    if( (int)newProjectFileName.size() > 0 )
    {
        cSize = (int)gpeRecentProjects.size();
        for(  i = cSize-1; i >=0; i--)
        {
            if( gpeRecentProjects[i]==newProjectFileName)
            {
                gpeRecentProjects.erase( gpeRecentProjects.begin()+i);
            }
        }
        cSize = (int)gpeRecentProjects.size();
        if( cSize >= 15)
        {
            int cDiff = cSize - 14;
            for(  i = cSize-1; i >=cDiff; i--)
            {
                gpeRecentProjects.erase( gpeRecentProjects.begin()+i);
            }
        }
        gpeRecentProjects.insert(gpeRecentProjects.begin(),newProjectFileName);
        gpe::error_log->report("Added ["+newProjectFileName+"] to Recent Project List");
    }
    if( saveData)
    {
        cSize = (int)gpeRecentProjects.size();
        std::string recentFileListFileName =  gpe::get_user_settings_folder()+"recent_projects.txt";
        std::ofstream recentFileListFile( recentFileListFileName.c_str() );
        if( !recentFileListFile.fail() )
        {
            //makes sure the file is open
            if (recentFileListFile.is_open())
            {
                for( i = cSize-1; i>=0; i--)
                {
                    recentFileListFile << gpeRecentProjects[i] <<  "\n";
                }
                if( current_project!=NULL)
                {
                    recentFileListFile << current_project->get_project_file_name() <<  "\n";
                }
                recentFileListFile.close();
            }
        }
    }
    update_recent_project_list(false);
}

void GPE_Gui_Engine::apply_logic()
{
    //always resetting the tooltip
    if( gpe::input->has_new_input() )
    {
        if( main_OVERLAY!=NULL)
        {
            main_OVERLAY->update_tooltip("");
        }
        update_popup_info();
    }
    process_window_title();

    if( mainToolBar!=NULL && main_CONTEXT_MENU!=NULL)
    {
        //all other gui elements are dependent on the toolbar and popup menus being closed
        if( !main_CONTEXT_MENU->is_open() )
        {
            //prevents the toolbar isnt activated when popup menu is open
            mainToolBar->process_toolbar();
        }

        if( mainToolBar->open_requested() )
        {
            GPE_DOCK->update_toolbar();
        }
        if( !mainToolBar->is_open() )
        {
            //prevents the popup menu from being processed while toolbar is open
            if( main_CONTEXT_MENU->is_open() )
            {
                //POPUP_MENU_VALUE = main_CONTEXT_MENU->process_menu_option();
                //if( gpe::input->check_mouse_released(kb_anykey) || window_controller_main->is_resized()  || gpe::input->kb_button_released[ESC] || gpe::input->kb_button_released[kb_enter])
                {
                    GPE_close_context_menu();
                }
            }
        }

        if( !mainToolBar->is_open() && !main_CONTEXT_MENU->is_open() )
        {
            if( GPE_main_Statusbar!=NULL)
            {
                GPE_main_Statusbar->process_self();
            }

            if( GPE_DOCK!=NULL)
            {
                GPE_DOCK->process_self();
                if( PANEL_RESOURCE_TREE!=NULL)
                {
                    if( gpe::input->check_kb_down( kb_ctrl) || gpe::input->check_kb_only_released(kb_1) )
                    {
                        //GPE_main_Logs->log_general_line( )
                    }
                }
            }
            else
            {
                gpe::error_log->report( "GPE_DOCK not initialized!" );
            }
        }
    }
    if( mainToolBar!=NULL && mainToolBar->just_closed() )
    {
        process_overlay_message();
    }

    if( gpe::input->check_kb_down(kb_ctrl) )
    {
        if( gpe::input->check_kb_released(kb_b) )
        {
            GPE_main_TabManager->add_new_tab(main_ABOUT_PAGE);
        }
        else if( gpe::input->kb_button_pressed[kb_f])
        {
            if( gpe::input->kb_button_down[kb_shift] )
            {
                main_SEARCH_CONTROLLER->textSearchMode = main_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_FINDALL;

            }
            else
            {
                main_SEARCH_CONTROLLER->textSearchMode = main_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_FIND;
            }
            main_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
            if( GPE_main_Logs!=NULL)
            {
                GPE_main_Logs->open_search_mode();
            }
        }
        else if( gpe::input->kb_button_pressed[kb_g])
        {
            main_SEARCH_CONTROLLER->textSearchMode = main_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_GOTO;
            main_SEARCH_CONTROLLER->goToLineStringBox->set_string("1");
            main_SEARCH_CONTROLLER->goToLineStringBox->switch_inuse(true);
            if( GPE_main_Logs!=NULL)
            {
                GPE_main_Logs->open_search_mode();
            }
        }
        else if( gpe::input->kb_button_pressed[kb_h])
        {
            main_SEARCH_CONTROLLER->textSearchMode = main_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_REPLACE;
            main_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true );
            if( GPE_main_Logs!=NULL)
            {
                GPE_main_Logs->open_replace_mode();
            }
        }
        else if( gpe::input->check_kb_released(kb_n) )
        {
            launch_new_project();
        }
        else if( gpe::input->check_kb_released(kb_o) )
        {
            open_new_project();
        }
        else if( gpe::input->check_kb_released(kb_q) )
        {
            GPE_Action_Message=="Quit Editor";
            gpe::input->exit_requested = true;
        }
        else if( gpe::input->kb_button_pressed[kb_r])
        {
            if( gpe::input->kb_button_down[kb_shift] )
            {
                main_SEARCH_CONTROLLER->textSearchMode = main_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_REPLACEALL;

            }
            else
            {
                main_SEARCH_CONTROLLER->textSearchMode = main_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_REPLACE;

            }
            main_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
            if( GPE_main_Logs!=NULL)
            {
                GPE_main_Logs->open_replace_mode();
            }
        }
        else if( gpe::input->check_kb_released(kb_s) )
        {
            if( gpe::input->check_kb_down(kb_shift) )
            {
                save_all_projects();
            }
            else
            {
                save_current_project();
            }
        }
        else if( gpe::input->check_kb_released(kb_u) )
        {
            GPE_main_TabManager->add_new_tab(main_EDITOR_SETTINGS);
        }
    }
    else if( gpe::input->check_kb_released(kb_f1) )
    {
        GPE_main_TabManager->add_new_tab(main_HELP_PAGE);
    }
    else if( gpe::input->check_kb_released(kb_f2) )
    {
        GPE_main_TabManager->add_new_tab(main_EDITOR_SETTINGS);
    }
    else if( gpe::input->check_kb_released(kb_f5) )
    {
        editor_gui_main->export_current_project_html5( gpe::input->check_kb_down(kb_ctrl)==false );
    }
    if( gpe::input->check_kb_released(kb_f6) )
    {
        GPE_main_TabManager->add_new_tab(main_START_PAGE);
    }
    else if( gpe::input->check_kb_released(kb_f8) && current_project!=NULL )
    {
        editor_gui_main->rum_current_project("", gpe::system_os_html5);
    }
    else if( gpe::input->check_kb_released(kb_f10) )
    {
        //gpe::gcanvas->get_font_cache_size(true);
    }
    else if( gpe::input->check_kb_released(kb_f11) )
    {
        if( !gpe::window_controller_main->is_fullscreen() )
        {
            main_OVERLAY->update_temporary_message("Entering Full-Screen","Press [F11] to exit","",1);
        }
        else
        {
            main_OVERLAY->update_temporary_message("Exiting Full-Screen","Press [F11] to toggle","",1);
        }
        gpe::window_controller_main->toggle_fullscreen();
    }
    else if( gpe::input->check_kb_released(kb_f12) )
    {
        editor_gui_main->take_live_screenshor();
    }
    else if( gpe::input->check_kb_released(kb_esc) )
    {
        main_SEARCH_CONTROLLER->close_finder();
    }

    int uInS = gpe::input->files_dropped_list.size();
    std::string temp_gpe_file = "";
    if( uInS > 0)
    {
        for( int iRFile = 0; iRFile < uInS; iRFile++)
        {
            temp_gpe_file = gpe::input->files_dropped_list.at( iRFile);
            if( sff_ex::file_exists(temp_gpe_file) )
            {
                if( stg_ex::get_file_ext(temp_gpe_file)=="gppf" )
                {
                    open_project(temp_gpe_file);
                }
                else
                {
                    display_user_alert("Error","Content dropped in window:"+temp_gpe_file+".");
                }
            }
            else
            {
                display_user_alert("Error","Content dropped in window:"+temp_gpe_file+" file not found.");
            }
        }
    }

    if( gpe::input->check_mouse_released( mb_anybutton ) )
    {
        RESOURCE_TO_DRAG = NULL;
        LAST_CLICKED_RESOURCE = NULL;
    }

    //If the user has Xed out the window
    if( gpe::window_controller_main->window_closed || gpe::input->exit_requested || GPE_Action_Message=="Quit Editor" || GPE_Action_Message=="Quit" )
    {
        if( (int)gpeProjects.size() > 0 )
        {
            int returnedAction = -1;
            int projectAction = -1;
            GPE_ProjectFolder * tempProjFolder;
            for( int i= (int)gpeProjects.size()-1; i>=0; i--)
            {
                tempProjFolder = gpeProjects[i];
                if( tempProjFolder!=NULL)
                {
                    projectAction = GPE_Display_Basic_Prompt("[Warning!] "+tempProjFolder->get_project_name()+" project not saved","Will you like to save and close before exiting?",true);
                    if( projectAction==DISPLAY_QUERY_YES)
                    {
                        tempProjFolder->save_project();
                        remove_project( tempProjFolder->get_project_file_name() );
                    }
                    else if( projectAction==DISPLAY_QUERY_CANCEL)
                    {
                        returnedAction = 2;
                        gpe::input->exit_requested = true;
                        gpe::window_controller_main->window_closed = false;
                    }
                    else
                    {
                        remove_project( tempProjFolder->get_project_file_name() );
                        gpe::input->exit_requested = true;
                    }
                }
            }
            if( returnedAction!=2)
            {
                //Quit the programs
                gpe::game_runtime->state_set( "exit" );
            }
        }
        else
        {
            //Quit the programs
            gpe::game_runtime->state_set( "exit" );
        }
    }
}


GPE_Toolbar * GPE_Gui_Engine::init_toolbar()
{
    if( mainToolBar!=NULL)
    {
        delete mainToolBar;
    }
    mainToolBar = new GPE_Toolbar();
    return mainToolBar;
}

int GPE_Gui_Engine::get_recent_project_list_size()
{
    return (int)gpeRecentProjects.size();
}

int GPE_Gui_Engine::get_tip_count()
{
    return (int)gpeTips.size();
}

int GPE_Gui_Engine::get_random_tip()
{
    return semath::random(0, (int)gpeTips.size()-1);
}

int GPE_Gui_Engine::get_previous_tip(int tipId)
{
    if( tipId >0 && tipId < (int)gpeTips.size() )
    {
        return tipId-1;
    }
    else
    {
        return (int)gpeTips.size()-1;
    }
}

int GPE_Gui_Engine::get_next_tip(int tipId)
{
    if( tipId >=0 && tipId < (int)gpeTips.size()-1 )
    {
        return tipId+1;
    }
    else
    {
        return 0;
    }
}

std::string GPE_Gui_Engine::get_tip(int tipId)
{
    if( tipId >=0 && tipId < (int)gpeTips.size() )
    {
        return gpeTips.at(tipId);
    }
    else
    {
        tipId = 0;
        if( (int)gpeTips.size() > 0)
        {
            return gpeTips.at(tipId);
        }
    }
    return "";
}

std::string GPE_Gui_Engine::get_recent_project_name( int pId )
{
    if( pId >=0 && pId < (int)gpeRecentProjects.size() )
    {
        return gpeRecentProjects.at(pId);
    }
    return "";
}

GPE_ResourceTree * GPE_Gui_Engine::init_resourcebar()
{
    if( mainResourceTree!=NULL)
    {
        delete mainResourceTree;
        mainResourceTree = NULL;
    }
    mainResourceTree = new GPE_ResourceTree();

    //update the tabbar and resourcemenu
    return mainResourceTree;
}

GPE_Toolbar * GPE_Gui_Engine::init_toolbar(std::string bName, gpe::shape_rect bRect)
{
    if( mainToolBar!=NULL)
    {
        delete mainToolBar;
    }
    mainToolBar = new GPE_Toolbar();
    mainToolBar->set_name( bName );
    mainToolBar->set_coords( bRect.x, bRect.y);
    mainToolBar->set_width( bRect.w) ;
    mainToolBar->set_height( bRect.h );
    return mainToolBar;
}

GPE_ProjectFolder * GPE_Gui_Engine::find_project_from_name(std::string projectName)
{
    if( (int)projectName.size()> 0)
    {
        GPE_ProjectFolder * tPFolder = NULL;
        GPE_ProjectFolder * foundProject = NULL;
        int i = 0;
        for( i = 0; i < (int)gpeProjects.size(); i++ )
        {
            tPFolder = gpeProjects[i];
            if( tPFolder!=NULL)
            {
                if( tPFolder->get_project_name().compare(projectName)==0 )
                {
                    foundProject = tPFolder;
                    break;
                }
            }
        }
        if( foundProject==NULL)
        {
            for( i = 0; i < (int)gpeProjects.size(); i++ )
            {
                tPFolder = gpeProjects[i];
                if( tPFolder!=NULL)
                {
                    if( tPFolder->get_project_file_name().compare(projectName)==0 )
                    {
                        foundProject = tPFolder;
                        break;
                    }
                }
            }
        }
        return foundProject;
    }
    return NULL;
}

GPE_ProjectFolder *  GPE_Gui_Engine::find_project_from_filename(std::string projectFileName)
{
    if( (int)projectFileName.size()> 0)
    {
        GPE_ProjectFolder * tPFolder = NULL;
        GPE_ProjectFolder * foundProject = NULL;
        for( int i = 0; i < (int)gpeProjects.size(); i++ )
        {
            tPFolder = gpeProjects[i];
            if( tPFolder!=NULL)
            {
                if( tPFolder->get_project_file_name().compare(projectFileName)==0 )
                {
                    foundProject = tPFolder;
                }
            }
        }
        return foundProject;
    }
    return NULL;
}

void GPE_Gui_Engine::clean_current_project_build_folder(int buildMetaTemplate)
{
    GPE_ProjectFolder * fFolder = find_project_from_filename(project_current_name);
    if( fFolder!=NULL)
    {
        fFolder->clean_build_folder(buildMetaTemplate);
    }
}

void GPE_Gui_Engine::clear_recent_project_list()
{
    gpeRecentProjects.clear();
    std::string recentFileListFileName =  gpe::get_user_settings_folder()+"recent_projects.txt";
    std::ofstream recentFileListFile( recentFileListFileName.c_str() );
    recentFileListFile.close();
    update_recent_project_list(false);
}

void GPE_Gui_Engine::close_project(std::string projectFileName )
{
    remove_project( projectFileName);
}

int GPE_Gui_Engine::find_project_id_from_name(std::string projectName, int ignoreId)
{
    if( (int)projectName.size()> 0)
    {
        GPE_ProjectFolder * tPFolder = NULL;
        int foundProject = -1;
        for( int i = 0; i < (int)gpeProjects.size(); i++ )
        {
            if( i!=ignoreId)
            {
                tPFolder = gpeProjects[i];
                if( tPFolder!=NULL)
                {
                    if( tPFolder->get_project_name().compare(projectName)==0 )
                    {
                        foundProject = i;
                        break;
                    }
                }
            }
        }
        return foundProject;
    }
    return -1;
}

int GPE_Gui_Engine::find_project_id_from_filename(std::string projectFileName, int ignoreId)
{
    if( (int)projectFileName.size()> 0)
    {
        GPE_ProjectFolder * tPFolder = NULL;
        int foundProject = -1;
        for( int i = 0; i < (int)gpeProjects.size(); i++ )
        {
            if( i!=ignoreId)
            {
                tPFolder = gpeProjects[i];
                if( tPFolder!=NULL)
                {
                    if( tPFolder->get_project_file_name().compare(projectFileName)==0 )
                    {
                        foundProject = i;
                    }
                }
            }
        }
        return foundProject;
    }
    return -1;
}

GPE_Toolbar * GPE_Gui_Engine::get_main_toolbar()
{
    return mainToolBar;
}

void GPE_Gui_Engine::launch_new_project()
{
    bool exitOperation = false;
    bool manualCancel = false;
    bool createButtonPushed = false;

    std::string newProjectFileName = "";
    std::string newProjectLocalFileName = "";
    std::string newProjectName = "";
    std::string foundFileName = "";
    RESOURCE_TO_DRAG = NULL;
    if( editor_gui_main!=NULL && gpe::renderer_main!=NULL)
    {
        gpe::error_log->report("Launching New Project");
        std::string popUpCaption = "Launch New Project";
        gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::arrow) );
        editor_gui_main->reset_gui_info();
        main_OVERLAY->take_frozen_screenshot( );

        int promptBoxWidth = gpe::settings->minWindowWidth;
        int promptBoxHeight = gpe::settings->minWindowHeight;
        gpe::shape_rect widget_box;


        gpe::input->reset_all_input();
        int currentTipId = editor_gui_main->get_random_tip();
        if( currentTipId < 0 || currentTipId > editor_gui_main->get_tip_count() )
        {
            currentTipId = 0;
        }
        std::string currentTipString = editor_gui_main->get_tip(currentTipId);

        GPE_Label_Title * mainMenuLabel = new GPE_Label_Title("Create A Project?","Create A Project");
        GPE_Label_Text * projectLocationLabel = new GPE_Label_Text("Project Location:","Project Location:");
        GPE_Label_Text * projectNameLabel = new GPE_Label_Text("Project Name:","Project Name:");

        GPE_Label_Text * projectMainExportLabel = new GPE_Label_Text("Primary Export Target:","Primary Export Target:");
        GPE_Label_Text * projectMainLanguageLabel = new GPE_Label_Text("Programming Language:","Programming Language:");
        int biggerLabelSize = std::max( projectMainExportLabel->get_width(), projectMainLanguageLabel->get_width() );
        biggerLabelSize = std::max( biggerLabelSize, projectNameLabel->get_width() );
        projectMainExportLabel->set_width(biggerLabelSize);
        projectNameLabel->set_width(biggerLabelSize);
        projectMainLanguageLabel->set_width(biggerLabelSize);

        //Adds all available languages to drop down menu
        GPE_DropDown_Menu * newprojectLanguage = new GPE_DropDown_Menu( "Project Language",true);
        int addedLanguages = 0;
        if( GPE_main_HIGHLIGHTER!=NULL )
        {
            //short operation so not "optimized"
            GPE_Gui_Engine_Language * tLanguage  = NULL;
            int languageCount = (int)GPE_main_HIGHLIGHTER->get_language_count();

            for( int cLanguage = 0; cLanguage < languageCount; cLanguage++ )
            {
                tLanguage = GPE_main_HIGHLIGHTER->get_language_object( cLanguage);
                if( tLanguage!=NULL && tLanguage->isCodingLanguage)
                {
                    newprojectLanguage->add_menu_option(tLanguage->languageName+" ("+tLanguage->languageShortName+")",tLanguage->languageShortName,cLanguage,true);
                    addedLanguages++;
                }
            }
        }
        //In the event something went wrong and we somehow didn't add JS and any other new coding language...
        if( addedLanguages == 0 )
        {
            newprojectLanguage->add_menu_option("JavaScript","JS", PROGRAM_LANGUAGE_JS,true);
            newprojectLanguage->add_menu_option("C++","CPP", PROGRAM_LANGUAGE_CPP,true);
        }

        GPE_DropDown_Menu * newprojectMainExport= new GPE_DropDown_Menu( "Main Export Target",true);
        newprojectMainExport->add_menu_option("HTML5","HTML5" , gpe::system_os_html5,true);
        newprojectMainExport->add_menu_option("WINDOWS","WINDOWS", gpe::system_os_windows);
        newprojectMainExport->add_menu_option("MAC","MAC", gpe::system_os_mac);
        newprojectMainExport->add_menu_option("LINUX","LINUX", gpe::system_os_linux);


        GPE_GuiElementList * popupMenuList = new GPE_GuiElementList();
        gpe_text_widget_string * projectNameField = new gpe_text_widget_string("","New Project");
        GPE_Label_Text * fileToCreateField = new GPE_Label_Text("No file selected","No file selected" );
        fileToCreateField->set_width( gpe::settings->minWindowWidth -32 );
        GPE_ToolLabelButton * closeButton = new GPE_ToolLabelButton( "Cancel","Cancel");
        GPE_ToolLabelButton * createButton = new GPE_ToolLabelButton( "Create","Create");
        GPE_CheckBoxBasic * matchProjectFileName = new GPE_CheckBoxBasic("Same as file name","Makes project name same as file name", true );
        GPE_Label_Error * projectCreateErrorLabel = new GPE_Label_Error("Please gpe::input all fields above","");
        GPE_ToolPushButton * fileFindButton = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder.png","Browse Projects...");
        fileToCreateField->set_width( gpe::settings->minWindowWidth -32 );
        gpe::renderer_main->reset_viewpoint( );
        //main_OVERLAY->render_frozen_screenshot( );
        while(exitOperation==false)
        {
            gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::arrow) );
            //gpe::error_log->report("Processing tip of the day");
            gpe::game_runtime->start_loop();

            if( gpe::screen_width > gpe::settings->defaultWindowWidth*2 )
            {
                promptBoxWidth = gpe::settings->defaultWindowWidth*2;
            }
            else
            {
                promptBoxWidth = gpe::screen_width;
            }

            if( gpe::screen_height > gpe::settings->defaultWindowWidth*2 )
            {
                promptBoxHeight = gpe::settings->defaultWindowWidth*2;
            }
            else
            {
                promptBoxHeight = gpe::screen_height;
            }

            widget_box.x = (gpe::screen_width-promptBoxWidth)/2;
            widget_box.y = (gpe::screen_height-promptBoxHeight)/2;
            widget_box.w = promptBoxWidth;
            widget_box.h = promptBoxHeight;
            popupMenuList->set_coords(widget_box.x, widget_box.y+32);
            popupMenuList->set_width(widget_box.w);
            popupMenuList->set_height(widget_box.h-32);
            popupMenuList->barXMargin = GENERAL_GPE_GUI_PADDING;
            popupMenuList->barYMargin = GENERAL_GPE_GUI_PADDING;
            popupMenuList->barXPadding = GENERAL_GPE_GUI_PADDING;
            popupMenuList->barYPadding = GENERAL_GPE_GUI_PADDING;
            editor_gui_main->reset_gui_info();
            popupMenuList->clear_list();
            popupMenuList->add_gui_element(mainMenuLabel,true);

            popupMenuList->add_gui_element(projectLocationLabel,false);
            popupMenuList->add_gui_element(fileFindButton, true);

            popupMenuList->add_gui_element(fileToCreateField,true);
            popupMenuList->add_gui_element(matchProjectFileName, true);

            popupMenuList->add_gui_element(projectNameLabel,false);
            popupMenuList->add_gui_element(projectNameField,true);

            popupMenuList->add_gui_element(projectMainExportLabel, false);
            popupMenuList->add_gui_element(newprojectMainExport, true);

            popupMenuList->add_gui_element(projectMainLanguageLabel, false);
            popupMenuList->add_gui_element(newprojectLanguage, true);

            popupMenuList->add_gui_element(projectCreateErrorLabel, true);
            popupMenuList->add_gui_element(createButton,false);
            popupMenuList->add_gui_element(closeButton,false);
            popupMenuList->process_self();
            if( gpe::input->check_kb_released(kb_esc) || closeButton->is_clicked() )
            {
                exitOperation = true;
                manualCancel = true;
                newProjectFileName = "";
            }
            else if( createButton->is_clicked() )
            {
                newProjectName = projectNameField->get_string();
                //Checks if a file is actually here or is it still just a folder...
                newProjectLocalFileName = stg_ex::get_local_from_global_file( newProjectName );
                if( (int)newProjectLocalFileName.size() > 0 )
                {
                    if( stg_ex::is_alnum(newProjectName,true, true) )
                    {
                        /*
                        If the path exists, file name checks, is alpha numeric and such
                        We exit this while loop and skip to end of function ready to create
                        a new project.
                        */
                        if( sff_ex::path_exists( stg_ex::get_path_from_file(newProjectFileName)  ) )
                        {
                            manualCancel = false;
                            createButtonPushed = true;
                            exitOperation = true;
                        }
                        else if( (int)newProjectFileName.size() > 0 )
                        {
                            projectCreateErrorLabel->set_name("Empty path given...");
                        }
                        else
                        {
                            projectCreateErrorLabel->set_name("Path does not exist...");
                        }
                    }
                    else
                    {
                        projectCreateErrorLabel->set_name("Project Name must be alphanumeric...");
                    }
                }
                else
                {
                    projectCreateErrorLabel->set_name("Empty project name given...");
                }
            }
            else if( fileFindButton->is_clicked() )
            {
                foundFileName = GPE_GetSaveFileName("Create a New Project File","Game Pencil Projects",editor_gui_main->fileOpenProjectDir);
                if( (int)foundFileName.size() > 0 )
                {
                    newProjectFileName = foundFileName;
                    fileToCreateField->set_name(foundFileName);
                    if( matchProjectFileName!=NULL && matchProjectFileName->is_clicked() )
                    {
                        projectNameField->set_string( stg_ex::get_file_noext( stg_ex::get_local_from_global_file(foundFileName) ) );
                    }
                }

            }

            //gpe::error_log->report("Rendering tip of the day");
            gpe::renderer_main->reset_viewpoint( );
            if( !gpe::window_controller_main->is_resized() )
            {
                //if( gpe::input->window_input_received )
                {
                    main_OVERLAY->render_frozen_screenshot( );
                }
                //Update screen
                gpe::gcanvas->render_rectangle( widget_box.x-4,widget_box.y-4,widget_box.x+widget_box.w+8,widget_box.y+widget_box.h+8, gpe::c_blgray,false,64);
                gpe::gcanvas->render_rect( &widget_box,theme_main->popup_box_color,false);

                gpe::gcanvas->render_rectangle( widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+32,theme_main->popup_box_highlight_color,false);
                gpe::gcanvas->render_rect( &widget_box,theme_main->button_box_highlight_color,true);
                gpe::gfs->render_text( widget_box.x+widget_box.w/2,widget_box.y+GENERAL_GPE_GUI_PADDING,popUpCaption,theme_main->popup_box_highlight_font_color,font_default,gpe::fa_center,gpe::fa_top);
                popupMenuList->render_self( );
                //editor_gui_main-render_gui_info(renderer_main, true);

                gpe::gcanvas->render_rect( &widget_box,theme_main->popup_box_border_color,true);
                if( editor_gui_main!= NULL )
                {
                    editor_gui_main->render_gui_info();
                }
            }
            gpe::game_runtime->end_loop();
        }

        gpe::input->reset_all_input();

        //Creates project if authorized to do so....
        if( manualCancel==false && (int)newProjectFileName.size()> 0 )
        {
            GPE_ProjectFolder * tPFolder = find_project_from_filename(newProjectFileName);
            if(tPFolder==NULL)
            {
                std::string newProjectDir= stg_ex::get_path_from_file(newProjectFileName);
                std::string newProjectLocalFileName = stg_ex::get_local_from_global_file(newProjectFileName);
                if( (int)newProjectLocalFileName.size()>0 && (int)newProjectName.size() > 0 )
                {
                    std::string newProjectTitle = stg_ex::get_file_noext(newProjectLocalFileName);
                    if( (int)newProjectTitle.size() > 0)
                    {
                        newProjectFileName = newProjectDir+"/"+newProjectTitle+".gppf";
                        newProjectDir = newProjectDir+"/"+newProjectTitle;
                        setup_project_directory(newProjectDir);
                        GPE_main_Logs->log_general_line("Creating new Project["+newProjectName+"] at ["+newProjectFileName+"] location.");
                        GPE_main_Logs->log_general_line("Detected Language for new project: ["+newprojectLanguage->get_selected_tag() + "]." );
                        GPE_ProjectFolder * newProject = new GPE_ProjectFolder(newProjectName,newProjectDir,newProjectFileName, newprojectLanguage->get_selected_tag(), true );
                        display_user_alert("Project Opened",newProjectFileName.c_str() );
                        gpeProjects.push_back(newProject);
                        newProject->save_project();
                    }
                }
            }
        }

        //Cleans up all of the UI elements.
        if( popupMenuList!=NULL)
        {
            delete popupMenuList;
            popupMenuList = NULL;
        }

        if( mainMenuLabel!=NULL)
        {
            delete mainMenuLabel;
            mainMenuLabel = NULL;
        }
        if( newprojectMainExport!=NULL)
        {
            delete newprojectMainExport;
            newprojectMainExport = NULL;
        }
        if( projectMainExportLabel!=NULL)
        {
            delete projectMainExportLabel;
            projectMainExportLabel = NULL;
        }
        if( newprojectLanguage!=NULL)
        {
            delete newprojectLanguage;
            newprojectLanguage = NULL;
        }
        if( projectMainLanguageLabel!=NULL)
        {
            delete projectMainLanguageLabel;
            projectMainLanguageLabel = NULL;
        }
        if( projectLocationLabel!=NULL)
        {
            delete projectLocationLabel;
            projectLocationLabel = NULL;
        }
        if( fileToCreateField!=NULL)
        {
            delete fileToCreateField;
            fileToCreateField = NULL;
        }
        if( matchProjectFileName!=NULL)
        {
            delete matchProjectFileName;
            matchProjectFileName = NULL;
        }
        if( closeButton!=NULL)
        {
            delete closeButton;
            closeButton = NULL;
        }
        if( fileFindButton!=NULL)
        {
            delete fileFindButton;
            fileFindButton = NULL;
        }
        gpe::game_runtime->start_loop();
    }
}

void GPE_Gui_Engine::load_tips()
{
    gpeTips.clear();
    gpeTips.push_back("Save your projects often to avoid lost work or memory in the event of a malfunction or crash!");
    gpeTips.push_back("In the Scene Editor, by holding 'M' you are you able to place multiple-objects at once.");
    gpeTips.push_back("Use the [kb_ctrl] KEY plus mouse wheel to horizontally scroll areas.");
    gpeTips.push_back("Be sure to take a look at the documentation for learning to make the most of Game Pencil.");
    gpeTips.push_back("You can have multiple projects open at once.");
    gpeTips.push_back("You can indent / unindent blocks of text/code using the [TAB] and [kb_shift-TAB] keys respectively");
    gpeTips.push_back("Objects inherit variables and functions from their parents. Very helpful for larger projects.");
    gpeTips.push_back("Use the arrow keys in gpe::input fields to traverse through previous entries.");
    gpeTips.push_back("Use the shortcut [kb_ctrl]+[O] to open a project to edit.");

    gpeTips.push_back("You are able to remove the background color, grayscale and invert an image using the transform button in the animation, Texture and Tilesheet editors.");
    gpeTips.push_back("Use the project settings editor to edit vital meta-data for your projects.");
    gpeTips.push_back("Need help or resources for your game? Check out the new Asset Store at http://store.pawbyte.com .");
    gpeTips.push_back("You can create a duplicate of your selected text, by pressing [kb_ctrl]+[D] keys.");
    gpeTips.push_back("You are able to give your objects custom functions in the object editor.");
    gpeTips.push_back("Save your current project folder using [kb_ctrl]+[S] keys together.");
    gpeTips.push_back("Assets are copied from their source directory to their project folder.");
    gpeTips.push_back("You can close a tab by clicking it with the middle mouse button or simply clicking on the X-icon.");
    gpeTips.push_back("The Game Pencil Editor is made via the SDL Library and some keyboard shortcuts may not behave 'normally to you'. If you experience any inconveniences please email help@pawbyte.com .");
    gpeTips.push_back("Save your projects often! There may be a few un-found errors in the  Game Pencil Engine and we will hate for a bug to be the cause of the loss of hours of work!");
    gpeTips.push_back("You can create a duplicate of the line the caret is on, by pressing [kb_ctrl]+[D] keys.");
    gpeTips.push_back("Version 1.1 of the GPE allows you to code in pure JavaScript.");
    gpeTips.push_back("You can add macros in your project's settings code editor.");
    gpeTips.push_back("You can debug your program using your web browser's console log.");
    gpeTips.push_back("You can save all open projects at once using the toolbar or buttonbar.");
    gpeTips.push_back("Project source code is provided free of obfuscation.");
    gpeTips.push_back("Use the shortcut [kb_ctrl]+[S] to save the current project you're editing.");
    gpeTips.push_back("You can get the most up to date documentation via http://docs.pawbyte.com");
}

void GPE_Gui_Engine::load_settings()
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_messages( "Loading Gui Settings", "Please Wait...","..." );
    }

    std::string otherColContainerName = "";

    std::string newFileIn =  gpe::get_user_settings_folder()+"gpe_ide_settings.txt";
    std::ifstream gameResourceFileIn( newFileIn.c_str() );

    gpe::error_log->report("Loading settings - "+newFileIn);
    //If the level file could be loaded
    if( !gameResourceFileIn.fail() )
    {
        //makes sure the file is open
        if (gameResourceFileIn.is_open())
        {
            int equalPos = 0;
            std::string firstChar="";
            std::string keyString="";
            std::string valString="";
            std::string subValString="";
            std::string currLine="";
            std::string currLineToBeProcessed;
            std::string colortheme_name;
            float foundFileVersion = 0;
            while ( gameResourceFileIn.good() )
            {
                getline (gameResourceFileIn,currLine); //gets the next line of the file
                currLineToBeProcessed = stg_ex::trim_left_inplace(currLine);
                currLineToBeProcessed = stg_ex::trim_right_inplace(currLineToBeProcessed);

                if( foundFileVersion <=0 )
                {
                    //Empty Line skipping is only allowed at the top of the file
                    if(!currLineToBeProcessed.empty() )
                    {
                        //Comment skipping is only allowed at the top of the file
                        if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                        {
                            //searches for an equal character and parses through the variable
                            equalPos=currLineToBeProcessed.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                keyString = currLineToBeProcessed.substr(0,equalPos);
                                valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                if( keyString=="Version")
                                {
                                    foundFileVersion = stg_ex::string_to_float(valString);
                                }
                            }
                        }
                    }
                }
                else if( foundFileVersion <= 2 )
                {
                    //Begin processing the file.
                    if(!currLineToBeProcessed.empty() )
                    {
                        equalPos=currLineToBeProcessed.find_first_of("=");
                        if(equalPos!=(int)std::string::npos)
                        {
                            //if the equalPos is present, then parse on through and carryon
                            keyString = currLineToBeProcessed.substr(0,equalPos);
                            valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());


                            if( keyString == "BottomLogsHeight")
                            {
                                if( GPE_main_Logs!=NULL )
                                {
                                    GPE_main_Logs->set_height( stg_ex::string_to_int( valString,32 ) );
                                    gpe::error_log->report("Logs Height = "+valString+";");
                                }
                                else
                                {
                                    gpe::error_log->report("main logs = NULL...");
                                }
                            }
                            else if( keyString=="TextAreaViewLineCount")
                            {
                                main_GUI_SETTINGS->showTextEditorLineCount = stg_ex::string_to_bool(valString);
                            }
                            else if( keyString=="TextAreaViewSyntaxHighlighting")
                            {
                                main_GUI_SETTINGS->showTextEditorSyntaxHightlight = stg_ex::string_to_bool(valString);
                            }

                            else if(keyString=="ShowStartPageAtStart")
                            {
                                showStartPageAtStartUp = stg_ex::string_to_bool(valString);
                            }
                            else if(keyString=="ShowTipOfDayAtStart")
                            {
                                showTipsAtStartUp = stg_ex::string_to_bool(valString);
                                main_EDITOR_SETTINGS->showStatupTipsBox->set_clicked(showTipsAtStartUp);
                            }
                            else if( keyString=="ShowFPSCounter")
                            {
                                gpe::fps_show_counter = stg_ex::string_to_bool( valString);
                            }
                            else if(keyString=="ColorTheme")
                            {
                                colortheme_name = valString;
                                if( sff_ex::file_exists( gpe::app_directory_name+"themes/"+colortheme_name) )
                                {

                                }
                            }
                        }
                    }
                }
                else
                {
                    gpe::error_log->report("Invalid FoundFileVersion ="+ stg_ex::float_to_string(foundFileVersion)+".");
                }
            }
        }
    }
}

void GPE_Gui_Engine::open_new_project()
{
    std::string newProjectFileName = GPE_GetOpenFileName("Load Your Project","Game Pencil Projects",fileOpenProjectDir);
    open_project( newProjectFileName);
}

void GPE_Gui_Engine::open_project(std::string newProjectFileName)
{
    if( (int)newProjectFileName.size()> 5 )
    {
        newProjectFileName = stg_ex::string_replace_all(newProjectFileName,"\\","/");
        std::string newProjectDir = stg_ex::get_file_noext(newProjectFileName);
        GPE_ProjectFolder * fFolder = find_project_from_filename( newProjectFileName );
        if( fFolder==NULL)
        {
            if( stg_ex::get_file_ext(newProjectFileName)=="gppf" )
            {
                if( sff_ex::file_exists(newProjectFileName) )
                {
                    if( main_GUI_SETTINGS && main_GUI_SETTINGS->useShortProjectNames )
                    {
                        GPE_main_Logs->log_general_line("Attempting to load ["+ stg_ex::get_local_from_global_file( newProjectFileName )+"]...");
                    }
                    else
                    {
                        GPE_main_Logs->log_general_line("Attempting to load ["+newProjectFileName+"]...");
                    }
                    std::string previousProjectInView = project_current_name;
                    std::string newProjectTitle = "Loaded Project";
                    GPE_ProjectFolder * newProject = new GPE_ProjectFolder(newProjectTitle,newProjectDir,newProjectFileName);
                    project_current_name = newProjectFileName;
                    gpeProjects.push_back(newProject);
                    gpe::error_log->report("New Project added into editor.");
                    set_current_gpe_project( newProject );
                    bool projectLoadedSuccessfuly = newProject->load_project_file(newProjectFileName);

                    if( projectLoadedSuccessfuly )
                    {
                        newProject->integrate_syntax();
                        set_current_gpe_project_from_name( newProjectFileName );
                        gpe::error_log->report("Checking with project list: ["+newProject->get_project_name()+"].");
                        add_to_recent_project_list(newProjectFileName,true );
                        gpe::error_log->report("Project added to GPE ["+newProject->get_project_name()+"]");
                        GPE_main_Logs->log_general_line("Done.");
                        if( main_GUI_SETTINGS && main_GUI_SETTINGS->useShortProjectNames )
                        {
                            GPE_main_Logs->log_general_line("Project ["+ stg_ex::get_local_from_global_file( newProjectFileName )+"] processed...");
                        }
                        else
                        {
                            GPE_main_Logs->log_general_line("Project ["+newProjectFileName+"] processed...");
                        }
                        mainResourceTree->add_resource_container( newProject->RESC_project_FOLDER );
                    }
                    else
                    {
                        set_current_gpe_project_from_name( previousProjectInView );
                        remove_project(newProjectFileName);
                    }
                }
                else
                {
                    display_user_alert("Project Open Error","File does not exist.");
                    GPE_main_Logs->log_debug_line("Project Open Error - File does not exist ("+newProjectFileName+")...");
                }
            }
            else
            {
                display_user_alert("Project Open Error","Invalid Project File Type given.");
            }
        }
        else
        {
            //display_user_alert("Project Open Error","This project is already open!");
            display_user_alert("Project Open Error","This project is already open!");
        }
    }
}


void GPE_Gui_Engine::prerender_gui( )
{
    mainResourceTree->prerender_self( );
    mainToolBar->prerender_self( );
    main_CONTEXT_MENU->prerender_self( );
}


void GPE_Gui_Engine::process_overlay_message()
{
    if( (int)GPE_Action_Message.size()== 0 )
    {
        return;
    }
    if( GPE_Action_Message=="Add Folder")
    {
        GPE_ProjectFolder * fFolder = find_project_from_filename(project_current_name);
        if( fFolder!=NULL)
        {
            std::string nFolderName = get_string_from_popup("Name Your Folder","Please enter the new name","newFolder" );
            if( (int)nFolderName.size()>0)
            {
                fFolder->create_blank_folder(LAST_CLICKED_RESOURCE, nFolderName);
            }
        }
    }
    else if( GPE_Action_Message=="Rename Folder" && LAST_CLICKED_RESOURCE!=NULL)
    {
        std::string nFolderName = get_string_from_popup("Rename Your Folder","Please enter the new name",LAST_CLICKED_RESOURCE->get_name() );
        if( (int)nFolderName.size()>0)
        {
            LAST_CLICKED_RESOURCE->set_name( nFolderName);
        }
    }
    else if( GPE_Action_Message=="Rename Resource" && LAST_CLICKED_RESOURCE!=NULL )
    {
        if( LAST_CLICKED_RESOURCE->get_resource_type()!= gpe::resource_type_project_settings)
        {
            std::string nresource_name = get_string_from_popup("Name Your Resource","Please enter the new name",LAST_CLICKED_RESOURCE->get_name() );
            if( (int)nresource_name.size()>0)
            {
                LAST_CLICKED_RESOURCE->set_name(nresource_name);
            }
        }
    }
    else if( GPE_Action_Message=="Clear Resource Folder" || GPE_Action_Message=="Clear Resource Folder Contents")
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->parentResource!=NULL)
        {
            if( GPE_Display_Basic_Prompt("Warning","Are you sure you will like to clear this folder's contents?")==DISPLAY_QUERY_YES)
            {
                /*Coming in Version 1.1.3...
                GPE_GeneralResourceContainer * parRes = LAST_CLICKED_RESOURCE->parentResource;
                parRes->delete_resource(LAST_CLICKED_RESOURCE);
                LAST_CLICKED_RESOURCE = NULL;
                RESOURCE_TO_DRAG = NULL;*/
            }
        }
    }
    else if( GPE_Action_Message=="Delete Folder" || GPE_Action_Message=="Remove Folder")
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->parentResource!=NULL)
        {
            if( GPE_Display_Basic_Prompt("Warning","Are you sure you will like to delete this folder?")==DISPLAY_QUERY_YES)
            {
                GPE_GeneralResourceContainer * parRes = LAST_CLICKED_RESOURCE->parentResource;
                parRes->delete_resource(LAST_CLICKED_RESOURCE);
                LAST_CLICKED_RESOURCE = NULL;
                RESOURCE_TO_DRAG = NULL;
            }
        }
    }
    else if( GPE_Action_Message=="Delete Resource" || GPE_Action_Message=="Remove Resource")
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->parentResource!=NULL)
        {
            if( LAST_CLICKED_RESOURCE->get_resource_type()!= gpe::resource_type_project_settings)
            {
                if( GPE_Display_Basic_Prompt("Warning","Are you sure you will like to delete this resource?")==DISPLAY_QUERY_YES)
                {
                    GPE_GeneralResourceContainer * parRes = LAST_CLICKED_RESOURCE->parentResource;
                    if( parRes!=NULL)
                    {
                        parRes->delete_resource(LAST_CLICKED_RESOURCE);
                        LAST_CLICKED_RESOURCE = NULL;
                        RESOURCE_TO_DRAG = NULL;
                    }
                }
            }
        }
    }
    else if( GPE_Action_Message=="Find Resource..." || GPE_Action_Message=="Find Local Resource..." )
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->get_resource_type()!= gpe::resource_type_project_settings )
        {
            std::string resourceNeedle = get_string_from_popup("Project Resource Search","Resource name:","");
            if( (int)resourceNeedle.size() > 0 )
            {
                GPE_GeneralResourceContainer * foundRes = LAST_CLICKED_RESOURCE->find_resource_from_name(resourceNeedle);
                if( foundRes!=NULL)
                {
                    generalGameResource * foundHeldRes = foundRes->get_held_resource();
                    if( GPE_main_TabManager!=NULL && foundHeldRes!=NULL )
                    {
                        GPE_main_TabManager->add_new_tab(foundHeldRes);
                    }
                }
            }
        }
    }
    else if( GPE_Action_Message=="Add Resource")
    {
        GPE_ProjectFolder * fFolder = find_project_from_filename(project_current_name);
        if( fFolder!=NULL)
        {
            int tempResType = LAST_CLICKED_RESOURCE->get_resource_type();
            if( tempResType > gpe::resource_type_project_settings && tempResType < gpe::resource_type_max )
            {
                std::string newresource_name = get_string_from_popup("Please name your new "+gpe::resource_type_names[tempResType]+".","Please use an alphanumeric value","" );
                if( (int)newresource_name.size() > 0 && stg_ex::is_alnum( newresource_name) )
                {
                    GPE_GeneralResourceContainer * newResource = fFolder->create_blank_resource(tempResType,LAST_CLICKED_RESOURCE, newresource_name );
                    if( newResource!=NULL)
                    {
                        GPE_main_TabManager->add_new_tab(newResource->get_held_resource() );
                        editor_gui_main->mainResourceTree->selectedSubOption = newResource->get_global_id();
                    }
                }
            }
        }
    }
    else if( GPE_Action_Message=="Duplicate Resource" || GPE_Action_Message=="Clone Resource")
    {
        if( LAST_CLICKED_RESOURCE!=NULL && LAST_CLICKED_RESOURCE->parentResource!=NULL)
        {
            if( !LAST_CLICKED_RESOURCE->is_folder() && !LAST_CLICKED_RESOURCE->is_super_project_folder() && !LAST_CLICKED_RESOURCE->is_super_project_folder() )
            {
                int tempResType = LAST_CLICKED_RESOURCE->get_resource_type();

                if( tempResType!=gpe::resource_type_texture && tempResType!=gpe::resource_type_tilesheet && tempResType!=gpe::resource_type_animation && tempResType!=gpe::resource_type_audio && tempResType!=gpe::resource_type_video && tempResType!=gpe::resource_type_project_settings )
                {
                    GPE_ProjectFolder * fFolder = find_project_from_filename(project_current_name);
                    if( fFolder!=NULL)
                    {
                        std::string newresource_name = get_string_from_popup("Please name your new copy of " + LAST_CLICKED_RESOURCE->get_name() + " ["+gpe::resource_type_names[tempResType]+"].","Please use an alphanumeric value","" );
                        if( (int)newresource_name.size() > 0 && stg_ex::is_alnum( newresource_name) )
                        {
                            GPE_GeneralResourceContainer * newResource = fFolder->create_blank_resource( tempResType,LAST_CLICKED_RESOURCE->parentResource,newresource_name );
                            if( newResource!=NULL)
                            {
                                LAST_CLICKED_RESOURCE->save_container(gpe::get_user_temp_folder()+"temp_duplicated_resource.gpf");
                                newResource->preprocess_container( gpe::get_user_temp_folder()+"temp_duplicated_resource.gpf");
                                newResource->set_name( newresource_name );
                                newResource->detect_name_change(true);
                                GPE_main_TabManager->add_new_tab(newResource->get_held_resource() );
                                editor_gui_main->mainResourceTree->selectedSubOption = newResource->get_global_id();
                            }
                            else
                            {
                                gpe::error_log->report("Unable to duplicate resource - new resource is NULL...");
                            }
                        }
                    }
                    else
                    {
                        gpe::error_log->report("Unable to duplicate resource - Unable to find project for resource ");
                    }
                }
            }
            else
            {
                gpe::error_log->report("Unable to duplicate resource - Last clicked resource is a folder...");
            }
        }
        else
        {
            gpe::error_log->report("Unable to duplicate resource - Last Clicked Resource or is parent equals NULL.");
        }
    }
    else if( GPE_Action_Message=="Close Project")
    {
        GPE_ProjectFolder * fFolder = find_project_from_filename(project_current_name);
        if( fFolder!=NULL)
        {
            int projectSaveState = GPE_Display_Basic_Prompt("Project Warning","Will you like to save this project before closing(Recommended)?",true);

            if( projectSaveState==DISPLAY_QUERY_YES)
            {
                save_current_project();
            }
            if( projectSaveState!=DISPLAY_QUERY_CANCEL)
            {
                //Close the project
                std::string projectNameToDelete = fFolder->get_project_file_name();
                if( GPE_LOADER != NULL )
                {
                    GPE_LOADER->update_messages( "Closing Game Project",projectNameToDelete, "Please wait..." );
                }

                GPE_main_TabManager->close_tabs_from_project( projectNameToDelete );

                remove_project(projectNameToDelete);
            }
        }
    }
    else if( GPE_Action_Message=="New Project")
    {
        launch_new_project();
    }
    else if( GPE_Action_Message=="Open Project")
    {
        open_new_project();
    }
    else if( GPE_Action_Message== "Open Project Directory" )
    {
        if( current_project!=NULL)
        {
            gpe::external_open_program(current_project->get_project_directory() );
        }
    }
    else if( GPE_Action_Message=="Save Project")
    {
        save_current_project();
    }
    else if( GPE_Action_Message=="Save All Projects")
    {
        save_all_projects();
    }
    else if( GPE_Action_Message=="Build Project" || GPE_Action_Message=="Build GAME")
    {
        if( current_project!=NULL)
        {
            GPE_main_TabManager->add_new_tab(current_project->RESC_project_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)current_project->RESC_project_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
        }
        else if( GPE_main_Logs!=NULL)
        {
            GPE_main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Clean [HTML5] Build Folder")
    {
        if( current_project!=NULL)
        {
            current_project->clean_build_folder( gpe::system_os_html5);
        }
    }
    else if( GPE_Action_Message=="Clean [Linux] Build Folder")
    {
        if( current_project!=NULL)
        {
            current_project->clean_build_folder( gpe::system_os_linux);
        }
    }
    else if( GPE_Action_Message=="Clean [OSX] Build Folder")
    {
        if( current_project!=NULL)
        {
            current_project->clean_build_folder( gpe::system_os_mac);
        }
    }
    else if( GPE_Action_Message=="Build HTML5 APP" || GPE_Action_Message=="Build HTML5 GAME")
    {
        if( current_project!=NULL)
        {
            GPE_main_TabManager->add_new_tab(current_project->RESC_project_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)current_project->RESC_project_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("HTML5");
        }
        else if( GPE_main_Logs!=NULL)
        {
            GPE_main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Build HTML5 APP & RUN" || GPE_Action_Message=="Build HTML5 GAME & RUN")
    {
        if( current_project!=NULL)
        {
            /*
            GPE_main_TabManager->add_new_tab(current_project->RESC_project_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)current_project->RESC_project_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("HTML5");
            */
            editor_gui_main->export_current_project_html5(true);
        }
        else if( GPE_main_Logs!=NULL)
        {
            GPE_main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if(GPE_Action_Message=="RUN HTML5 GAME")
    {
        if( current_project!=NULL)
        {
            rum_current_project("", gpe::system_os_html5);
        }
        else if( GPE_main_Logs!=NULL)
        {
            GPE_main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Build Nintendo Switch GAME")
    {
        if( current_project!=NULL && current_project->RESC_project_SETTINGS!=NULL )
        {
            GPE_main_TabManager->add_new_tab(current_project->RESC_project_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)current_project->RESC_project_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("Switch");
        }
        else if( GPE_main_Logs!=NULL)
        {
            GPE_main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Build WINDOWS GAME")
    {
        if( current_project!=NULL && current_project->RESC_project_SETTINGS!=NULL )
        {
            GPE_main_TabManager->add_new_tab(current_project->RESC_project_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)current_project->RESC_project_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("Windows");
        }
        else if( GPE_main_Logs!=NULL)
        {
            GPE_main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Build OSX GAME")
    {
        if( current_project!=NULL && current_project->RESC_project_SETTINGS!=NULL )
        {
            GPE_main_TabManager->add_new_tab(current_project->RESC_project_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)current_project->RESC_project_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("OSX");
        }
        else if( GPE_main_Logs!=NULL)
        {
            GPE_main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Build LINUX GAME")
    {
        if( current_project!=NULL && current_project->RESC_project_SETTINGS!=NULL )
        {
            GPE_main_TabManager->add_new_tab(current_project->RESC_project_SETTINGS->get_held_resource() );
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)current_project->RESC_project_SETTINGS->get_held_resource();
            tProjectProps->projectSettingsBar->set_selected_option("Platforms");
            tProjectProps->exportSettingsBar->set_selected_option("Linux");
        }
        else if( GPE_main_Logs!=NULL)
        {
            GPE_main_Logs->log_build_comment("You need to first open a project to use this option!");
        }
    }
    else if( GPE_Action_Message=="Clean [HTML5] Build Folder")
    {
        clean_current_project_build_folder( gpe::system_os_html5);
    }
    else if( GPE_Action_Message=="Clean [Windows] Build Folder")
    {
        clean_current_project_build_folder( gpe::system_os_windows );
    }
    else if( GPE_Action_Message=="Clean [OSX] Build Folder")
    {
        clean_current_project_build_folder( gpe::system_os_mac );
    }
    else if( GPE_Action_Message=="Clean [Linux] Build Folder")
    {
        clean_current_project_build_folder( gpe::system_os_linux );
    }
    else if( GPE_Action_Message=="Online Documentation"|| GPE_Action_Message=="Documentation")
    {
        gpe::external_open_url("http://docs.pawbyte.com/");
    }
    else if( GPE_Action_Message=="EULA" || GPE_Action_Message=="License")
    {
        gpe::external_open_url("http://gamepencil.pawbyte.com/eula/");
    }
    else if( GPE_Action_Message=="Check Updates"|| GPE_Action_Message=="Check Version")
    {
        gpe::external_open_url("http://gamepencil.pawbyte.com/get-game-pencil-engine/");
    }

    else if( GPE_Action_Message=="Community" || GPE_Action_Message=="Online Forums" || GPE_Action_Message=="Community" || GPE_Action_Message=="Forums")
    {
        gpe::external_open_url("http://community.pawbyte.com/");
    }
    else if( GPE_Action_Message=="Community Chat")
    {
        gpe::external_open_url("https://discord.gg/bua3rDW");
    }
    else if( GPE_Action_Message=="Toggle Fullscreen Mode")
    {
        if( !gpe::window_controller_main->is_fullscreen() )
        {
            main_OVERLAY->update_temporary_message("Entering Full-Screen","Press [F11] to exit","",1);
        }
        gpe::window_controller_main->toggle_fullscreen();
    }
    else if( GPE_Action_Message=="Toggle Logs")
    {
        GPE_DOCK->add_to_panel("Logs", DOCK_BOTTOM_MIDDLE );
        GPE_main_Logs->toggle_manager();
    }
    else if( GPE_Action_Message=="Reset Dock" )
    {
        GPE_DOCK->reset_dock();
        if( GPE_main_Logs->is_enabled() ==false )
        {
            GPE_main_Logs->toggle_manager();
        }
        if( GPE_main_TabManager->requests_fullscreen() )
        {
            GPE_main_TabManager->toggle_full_width();
        }
    }
    else if( GPE_Action_Message=="Clear Recent Projects List")
    {
        editor_gui_main->clear_recent_project_list();
    }
    else if( GPE_Action_Message=="Toggle Line Count")
    {
        main_GUI_SETTINGS->showTextEditorLineCount = !main_GUI_SETTINGS->showTextEditorLineCount;
        save_settings();
    }
    else if(GPE_Action_Message=="Toggle Syntax Highlighting")
    {
        main_GUI_SETTINGS->showTextEditorSyntaxHightlight = !main_GUI_SETTINGS->showTextEditorSyntaxHightlight;
        save_settings();
    }
    else if( GPE_Action_Message=="Find...")
    {
        if( GPE_main_Logs!=NULL)
        {
            GPE_main_Logs->open_search_mode();
        }
        main_SEARCH_CONTROLLER->textSearchMode = main_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_FIND;
        main_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
    }
    else if( GPE_Action_Message=="Find in files...")
    {
        if( GPE_main_Logs!=NULL)
        {
            GPE_main_Logs->open_search_mode();
        }
        main_SEARCH_CONTROLLER->textSearchMode = main_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_FINDALL;
        main_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true);
    }
    else if( GPE_Action_Message=="Goto Line...")
    {
        main_SEARCH_CONTROLLER->textSearchMode = SEARCH_MODE_GOTO;
        main_SEARCH_CONTROLLER->goToLineStringBox->set_string("1");
        main_SEARCH_CONTROLLER->goToLineStringBox->switch_inuse(true);
        if( GPE_main_Logs!=NULL)
        {
            GPE_main_Logs->open_search_mode();
        }
    }
    else if( GPE_Action_Message=="Find Project Resource...")
    {
        if( current_project!=NULL&& current_project->RESC_project_FOLDER!=NULL)
        {
            std::string resourceNeedle = get_string_from_popup("Project Resource Search","Resource name:","");
            if( (int)resourceNeedle.size() > 0 )
            {
                GPE_GeneralResourceContainer * foundRes = current_project->RESC_project_FOLDER->find_resource_from_name(resourceNeedle);
                if( foundRes!=NULL)
                {
                    generalGameResource * foundHeldRes = foundRes->get_held_resource();
                    if( GPE_main_TabManager!=NULL && foundHeldRes!=NULL)
                    {
                        GPE_main_TabManager->add_new_tab(foundHeldRes);
                    }
                }
            }
        }
    }
    else if( GPE_Action_Message=="Replace...")
    {
        main_SEARCH_CONTROLLER->textSearchMode = main_SEARCH_CONTROLLER->previousSearchMode = SEARCH_MODE_REPLACE;
        main_SEARCH_CONTROLLER->findTextStringBox->switch_inuse( true );
        if( GPE_main_Logs!=NULL)
        {
            GPE_main_Logs->open_replace_mode();
        }
    }
    else if( GPE_Action_Message=="Use Default Color Theme")
    {
        theme_main = theme_default;
        prerender_gui();
        save_settings();
    }
    else if( GPE_Action_Message=="Start Page")
    {
        GPE_main_TabManager->add_new_tab(main_START_PAGE);
    }
    else if( GPE_Action_Message=="Tip of the Day")
    {
        GPE_Show_Tip_Of_Day();
    }
    else if( GPE_Action_Message=="Report Issue")
    {
        gpe::external_open_url("https://github.com/pawbyte/Game-Pencil-Engine/issues");
    }
    else if( GPE_Action_Message=="About" || GPE_Action_Message=="About..." || GPE_Action_Message=="About Game Pencil Engine" )
    {
        GPE_main_TabManager->add_new_tab(main_ABOUT_PAGE);
    }
    else if( GPE_Action_Message=="Help" || GPE_Action_Message=="Help..." || GPE_Action_Message=="General Help"  )
    {
        GPE_main_TabManager->add_new_tab(main_HELP_PAGE);
    }
    else if( GPE_Action_Message=="User Settings" || GPE_Action_Message=="User Settings..." || GPE_Action_Message=="Game Pencil User Settings" )
    {
        GPE_main_TabManager->add_new_tab(main_EDITOR_SETTINGS);
    }
    else if( GPE_Action_Message=="Game Controller Tester" )
    {
        GPE_main_TabManager->add_new_tab( main_gamepad_tester );
    }
    else if( GPE_Action_Message=="JS Compiler Settings" )
    {
        GPE_main_TabManager->add_new_tab( GPE_JS_COMPILER_SETTINGS );
    }
    else if( GPE_Action_Message=="Extra Tools" )
    {
        GPE_main_TabManager->add_new_tab( main_EXTRA_TOOLS );
    }
    else if( GPE_Action_Message=="C++ Builder Settings" )
    {
        GPE_main_TabManager->add_new_tab( GPE_CPP_BUILDER_SETTINGS );
    }
    else if( stg_ex::string_starts(GPE_Action_Message, "Import ") )
    {
        //Import Resources ImportResources
        if( current_project!=NULL)
        {
            std::string remainingType = stg_ex::get_substring(GPE_Action_Message, 7);
            int tempResType = 0;
            bool foundResType = false;
            for( int iResType = 0; iResType < gpe::resource_type_max; iResType++ )
            {
                if( remainingType == gpe::resource_type_names[iResType] )
                {
                    foundResType = true;
                    tempResType = iResType;
                    break;
                }
            }

            if( foundResType )
            {
                std::string fileToImportName = GPE_GetOpenFileName( GPE_Action_Message,".gpf", main_GUI_SETTINGS->fileOpenImportFileDir );
                if( sff_ex::file_exists(fileToImportName) && stg_ex::get_file_ext(fileToImportName) =="gpf" )
                {
                    GPE_GeneralResourceContainer * newResource = current_project->create_blank_resource(tempResType,NULL, stg_ex::get_file_noext( stg_ex::get_local_from_global_file( fileToImportName) ) );
                    if( newResource!=NULL)
                    {
                        GPE_main_TabManager->add_new_tab(newResource->get_held_resource() );
                        newResource->preprocess_container(fileToImportName);
                        editor_gui_main->mainResourceTree->selectedSubOption = newResource->get_global_id();
                        newResource->detect_name_change(true);
                    }
                    else
                    {
                        display_user_alert("Import file error","Unable to create new "+gpe::resource_type_names[tempResType]+" resource.");
                    }
                }
                else
                {
                    display_user_alert("Import file error","Unable to import "+gpe::resource_type_names[tempResType]+" file ["+stg_ex::get_local_from_global_file(fileToImportName)+"]");
                }
            }
        }
    }
    else if( stg_ex::string_starts(GPE_Action_Message, "Toggle ") && stg_ex::string_ends(GPE_Action_Message, " Pane") )
    {
        GPE_Action_Message = stg_ex::string_replace_all(GPE_Action_Message,"Toggle ","");
        GPE_Action_Message = stg_ex::string_replace_all(GPE_Action_Message," Pane","");
        GPE_DOCK->toggle_default_pane( GPE_Action_Message );
    }
    else if( sff_ex::file_exists(GPE_Action_Message) )
    {
        if( stg_ex::get_file_ext(GPE_Action_Message)=="gppf")
        {
            open_project(GPE_Action_Message);
        }
    }
    else
    {
        gpe::error_log->report( "Unknown Command: ["+GPE_Action_Message+"]" );
    }
}

void GPE_Gui_Engine::process_window_title()
{
    if( gpe::renderer_main!=NULL )
    {
        std::string windowCurrentTitle = "";
        if( current_project!=NULL)
        {
            windowCurrentTitle = current_project->get_project_name();
        }
        if( gpe::version_number_update==0)
        {
            windowCurrentTitle+= "Game Pencil Engine   "+ stg_ex::float_to_string( gpe::version_number_total )+".0 -";
        }
        else
        {
            windowCurrentTitle+= "Game Pencil Engine   "+ stg_ex::float_to_string( gpe::version_number_total )+" -";
        }

        if( release_current_mode==relesae_type_production)
        {
            windowCurrentTitle+=" [Production]";
        }
        else if( release_current_mode==release_type_rc)
        {
            windowCurrentTitle+=" [Release Candidate]";
        }
        else if( release_current_mode== release_type_beta)
        {
            windowCurrentTitle+=" [Beta]";
        }
        else if( release_current_mode== release_type_alpha)
        {
            windowCurrentTitle+=" [Alpha]";
        }
        if( !gpe::window_controller_main->window_has_focus)
        {
            windowCurrentTitle += " Out of Focus |";
        }
        if( !gpe::window_controller_main->window_has_mouse)
        {
            windowCurrentTitle += " Mouse Outside |";
        }
        gpe::window_controller_main->set_window_title(windowCurrentTitle);
    }
}

void GPE_Gui_Engine::render_foreground_engine()
{
    if(gpe::game_runtime->loading_data)
    {
        gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::wait ) );
    }

    gpe::renderer_main->reset_viewpoint();

    if( mainToolBar!=NULL && main_CONTEXT_MENU!=NULL )
    {
        if( !mainToolBar->is_open() && !main_CONTEXT_MENU->is_open() )
        {
            if( GPE_DOCK!=NULL )
            {
                GPE_DOCK->render_self( NULL, NULL );
            }

            render_gui_info();
            main_OVERLAY->render_temporary_message();
            GPE_main_HIGHLIGHTER->render_code_highlights();
            GPE_main_HIGHLIGHTER->render_code_suggestions();
            if( RESOURCE_TO_DRAG!=NULL)
            {
                RESOURCE_TO_DRAG->render_option(  gpe::input->mouse_position_x+32, gpe::input->mouse_position_y, -1,NULL, NULL,false, true);
                gpe::gfs->render_text_boxed( gpe::input->mouse_position_x+32,gpe::input->mouse_position_y,RESOURCE_TO_DRAG->get_name(),theme_main->popup_box_font_color,theme_main->popup_box_color,FONT_TEXTINPUT,gpe::fa_left,gpe::fa_top );
            }
        }
        mainToolBar->render_toolbar( NULL);
        main_CONTEXT_MENU->render_self( );
        ////if( forceRedraw )
        {
            gpe::gfs->render_fps(FONT_TEXTINPUT, theme_main->main_box_font_color, gpe::screen_width, 32 );
        }
    }
}

void GPE_Gui_Engine::render_gui_info()
{
    if( GPE_main_Statusbar!=NULL)
    {
        GPE_main_Statusbar->render_self( NULL, NULL);
    }
    main_OVERLAY->render_tooltip(  gpe::input->mouse_position_x,gpe::input->mouse_position_y+32);
}

void GPE_Gui_Engine::remove_project(std::string projectFileName )
{
    if( (int)projectFileName.size()>0 )
    {
        GPE_ProjectFolder * tPFolder = NULL;
        for( int i = (int)gpeProjects.size()-1; i>=0; i--)
        {
            tPFolder = gpeProjects[i];
            if( tPFolder!=NULL )
            {
                if( tPFolder->get_project_file_name().compare(projectFileName)==0 )
                {
                    if( GPE_LOADER != NULL )
                    {
                        GPE_LOADER->update_messages( "Closing Game Project ["+tPFolder->get_project_name()+"]", projectFileName,"PLEASE WAIT" );
                    }
                    GPE_main_TabManager->close_tabs_from_project( projectFileName );
                    if(current_project!=NULL)
                    {
                        if( current_project->get_project_name()==tPFolder->get_project_name() )
                        {
                            current_project = NULL;
                            project_current_name = "";
                        }
                    }

                    if( editor_gui_main!=NULL && editor_gui_main->mainResourceTree!=NULL)
                    {
                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_messages( "Closing Game Project", projectFileName,"Removing from Projects Menu.." );
                        }

                        editor_gui_main->mainResourceTree->remove_project_resources(projectFileName);
                        if( GPE_DOCK != NULL)
                        {
                            GPE_DOCK->clear_all_panels();
                        }

                        if( GPE_LOADER != NULL )
                        {
                            GPE_LOADER->update_messages( "Closing Game Project", projectFileName,"Removed from Projects Menu.." );
                        }
                    }

                    delete tPFolder;
                    tPFolder = NULL;
                    gpeProjects.erase(gpeProjects.begin()+i );

                    GPE_LOADER->update_messages( "Closing Game Project", projectFileName,"Clearing cache..." );
                    gpe::gfs->clear_font_cache();
                    GPE_LOADER->update_messages( "Closing Game Project", projectFileName,"Cache Cleared..." );
                }
            }
        }
    }
}

void GPE_Gui_Engine::reset_gui_info()
{
    //update_tooltip("");
    if( GPE_main_Statusbar!=NULL)
    {
        GPE_main_Statusbar->reset_statusbar();
        /*GPE_main_Statusbar->set_coords(0,screen_height-32);
        GPE_main_Statusbar->set_width(screen_width);
        GPE_main_Statusbar->set_height(32);*/
    }
}

void GPE_Gui_Engine::save_all_projects()
{
    GPE_ProjectFolder * tPFolder = NULL;
    GPE_ProjectFolder * past_project = current_project;
    for( int i = 0; i < (int)gpeProjects.size(); i++ )
    {
        tPFolder = gpeProjects[i];
        if( tPFolder!=NULL)
        {
            set_current_gpe_project( tPFolder );
            tPFolder->save_project();
        }
    }
    set_current_gpe_project( past_project );

}

void GPE_Gui_Engine::export_current_project_html5(bool runGameOnCompile)
{
    GPE_ProjectFolder * fFolder = find_project_from_filename(project_current_name);
    if( fFolder!=NULL)
    {
        fFolder->save_project();
        fFolder->export_project_html5("","", gpe::system_os_html5,runGameOnCompile);
    }
}

void GPE_Gui_Engine::rum_current_project(std::string projectBuildDirectory, int buildMetaTemplate)
{
    GPE_ProjectFolder * fFolder = find_project_from_filename(project_current_name);
    if( fFolder!=NULL)
    {
        fFolder->run_project(projectBuildDirectory, buildMetaTemplate);
    }
}


void GPE_Gui_Engine::save_current_project()
{
    GPE_ProjectFolder * fFolder = find_project_from_filename(project_current_name);
    if( fFolder!=NULL)
    {
        fFolder->save_project();
    }
}

void GPE_Gui_Engine::save_settings()
{
    std::string newSaveDataFilename =  gpe::get_user_settings_folder()+"gpe_ide_settings.txt";
    std::ofstream newSaveDataFile( newSaveDataFilename.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            //write_header_on_file(&newSaveDataFile);
            newSaveDataFile << "Version=" << gpe::version_number_total << "\n";
            if( GPE_main_Logs!=NULL )
            {
                newSaveDataFile << "BottomLogsHeight=" << GPE_main_Logs->get_height() << "\n";

            }
            newSaveDataFile << "ShowFPSCounter=" << gpe::fps_show_counter << "\n";
            newSaveDataFile << "ShowStartPageAtStart=" << showStartPageAtStartUp << "\n";
            newSaveDataFile << "ShowTipOfDayAtStart=" << showTipsAtStartUp <<  "\n";

            if( main_GUI_SETTINGS!=NULL )
            {
                newSaveDataFile << "TextAreaViewLineCount=" << main_GUI_SETTINGS->showTextEditorLineCount << "\n";
            }
            if( main_GUI_SETTINGS!=NULL )
            {
                newSaveDataFile << "TextAreaViewSyntaxHighlighting=" << main_GUI_SETTINGS->showTextEditorSyntaxHightlight << "\n";
            }
            if( theme_main!=NULL)
            {
                newSaveDataFile << "ColorTheme=" << theme_main->theme_local_location << "\n";
            }
            newSaveDataFile.close();
        }
    }
}

void GPE_Gui_Engine::take_live_screenshor()
{

    if( gpe::renderer_main!=NULL)
    {
        if( main_EDITOR_SETTINGS!=NULL && main_EDITOR_SETTINGS->autoSaveScreenshots!=NULL && main_EDITOR_SETTINGS->autoSaveScreenshots->is_clicked() )
        {
            if( main_EDITOR_SETTINGS->makeMetaScreenshots!=NULL && main_EDITOR_SETTINGS->makeMetaScreenshots->is_clicked() )
            {
                if( (int)project_current_name.size() > 0 )
                {
                    std::time_t rawtime;
                    std::tm* timeinfo;
                    char timeBuffer [80];

                    std::time(&rawtime);
                    timeinfo = std::localtime(&rawtime);

                    std::strftime(timeBuffer,80,"%y%m%d_%H%M%S",timeinfo);
                    std::puts(timeBuffer);

                    bool foundBlank = false;
                    std::string screenshotNewLocation  = "";


                    screenshotNewLocation = gpe::app_directory_name+"screenshots/"+ stg_ex::get_file_noext( stg_ex::get_local_from_global_file( project_current_name) )+"_"+timeBuffer+".png";
                    if( sff_ex::file_exists( screenshotNewLocation) ==false )
                    {
                        foundBlank = true;
                        gpe::renderer_main->save_screenshot(screenshotNewLocation);
                    }
                    for(int i = 1; foundBlank == false && i <= 1000; i++)
                    {
                        screenshotNewLocation = gpe::app_directory_name+"screenshots/"+stg_ex::get_file_noext( stg_ex::get_local_from_global_file( project_current_name) )+"_"+timeBuffer+"_"+ stg_ex::int_to_string(i)+".png";
                        if( sff_ex::file_exists( screenshotNewLocation) ==false )
                        {
                            foundBlank = true;
                            gpe::renderer_main->save_screenshot(screenshotNewLocation);
                            break;
                        }
                    }
                    if( foundBlank==false)
                    {
                        sff_ex::append_to_file( gpe::app_directory_name+"screenshots/screenshot_log.txt","Unable to save screenshot to "+screenshotNewLocation);
                        gpe::error_log->report("Unable to save screenshot to "+screenshotNewLocation);
                    }
                }
                else
                {
                    gpe::renderer_main->save_screenshot();
                }
            }
            else
            {
                gpe::renderer_main->save_screenshot();
            }
        }
    }
}

void GPE_Gui_Engine::update_recent_project_list(bool saveData)
{
    if( main_TOOLBAR_RECENT_PROJECTS!=NULL)
    {
        main_TOOLBAR_RECENT_PROJECTS->clear_menu();
        int iRPList = 0;
        int maxRPList = editor_gui_main->get_recent_project_list_size();
        for( iRPList = 0; iRPList < maxRPList; iRPList++)
        {
            if( iRPList==maxRPList-1)
            {
                main_TOOLBAR_RECENT_PROJECTS->add_menu_option(editor_gui_main->get_recent_project_name(iRPList),-1,NULL,-1,NULL, true );
            }
            else
            {
                main_TOOLBAR_RECENT_PROJECTS->add_menu_option(editor_gui_main->get_recent_project_name(iRPList),-1,NULL,-1,NULL,false );
            }
        }
        main_TOOLBAR_RECENT_PROJECTS->add_menu_option("Clear Recent Projects List");
    }
}

void init_gpe_editor_globals()
{
    //Let's start off with the build names
    for(int bI = 0; bI < gpe::system_os_max; bI++)
    {
        GPE_BUILD_NAMES[bI] = "";
    }
    GPE_BUILD_NAMES[ gpe::system_os_html5] = "HTML5";
    GPE_BUILD_NAMES[ gpe::system_os_windows] = "WINDOWS";
    GPE_BUILD_NAMES[ gpe::system_os_mac] = "OSX";
    GPE_BUILD_NAMES[ gpe::system_os_linux] = "LINUX";
    GPE_BUILD_NAMES[ gpe::system_os_xboxone ] = "XBOXONE";
    GPE_BUILD_NAMES[ gpe::system_os_ps4] = "PS4";
    GPE_BUILD_NAMES[ gpe::system_os_android] = "ANDROID";
    GPE_BUILD_NAMES[ gpe::system_os_ios] = "IOS";

    //animation Labels we will use
    animaton2d_LABELS[gpe::anim_frame_count] = "Frame Count";
    animaton2d_LABELS[gpe::anim_frame_width] = "Frame Width";
    animaton2d_LABELS[gpe::anim_frame_height] = "Frame Height";
    animaton2d_LABELS[gpe::anim_pixel_offset_hori] = "Horizontal Pixel  Offset";
    animaton2d_LABELS[gpe::anim_pixel_offset_vert] = "Vertical Pixel Offset";
    animaton2d_LABELS[gpe::anim_pixel_padding_hori] = "Horizontal Frame Padding";
    animaton2d_LABELS[gpe::anim_pixel_padding_vert] = "Vertictal Frame Padding";

    //Tilesheet labels we will use
    tsDataLabels[0] = "Width";
    tsDataLabels[1] = "Height";
    tsDataLabels[2] = "X-Offset";
    tsDataLabels[3] = "Y-Offset";
    tsDataLabels[4] = "H-Padding";
    tsDataLabels[5] = "Y-Padding";

    //Font Popup variables
    POPUP_FONT_SIZE_WIDTH = 12;
    POPUP_FONT_SIZE_HEIGHT = 12;
    if( FONT_POPUP!=NULL)
    {
        FONT_POPUP->get_metrics("A",&POPUP_FONT_SIZE_WIDTH,&POPUP_FONT_SIZE_HEIGHT);
    }

    //Scene globals
    DEFAULT_SCENE_SUBEDITOR_NAMES[0] = "Settings";
    DEFAULT_SCENE_SUBEDITOR_NAMES[1] = "Layers";
    DEFAULT_SCENE_SUBEDITOR_NAMES[2] = "Extras";
    DEFAULT_SCENE_SUBEDITOR_NAMES[3] = "Extras";
    DEFAULT_SCENE_SUBEDITOR_NAMES[3] = "Extras";

    //Video type names
    SUPPORTED_VIDEO_EXT[0] = "MP4";
    SUPPORTED_VIDEO_EXT[1] = "WEBM";
    SUPPORTED_VIDEO_EXT[2] = "OGG";

    //Font type names
    SUPPORTED_FONT_EXT[FONT_EOT]  = "EOT";
    SUPPORTED_FONT_EXT[FONT_SVG]  = "SVG";
    SUPPORTED_FONT_EXT[FONT_OTF] = "OTF";
    SUPPORTED_FONT_EXT[FONT_TTF]  = "TTF";
    SUPPORTED_FONT_EXT[FONT_WOFF] = "WOFF";
    SUPPORTED_FONT_EXT[FONT_WOFF2] = "WOFF2";
    editor_theme_controller = new themes_controller();
}

void GPE_Show_Tip_Of_Day()
{
    if( editor_gui_main!=NULL && gpe::renderer_main!=NULL )
    {
        gpe::game_runtime->end_loop();
        RESOURCE_TO_DRAG = NULL;
        gpe::error_log->report("Showing tip of the day");
        std::string popUpCaption = "Tip of the Day";
        gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::arrow ) );
        editor_gui_main->reset_gui_info();
        main_OVERLAY->take_frozen_screenshot( );

        int promptBoxWidth = 544;
        int promptBoxHeight = 384;
        gpe::shape_rect widget_box;

        bool exitOperation = false;
        gpe::input->reset_all_input();
        int currentTipId = editor_gui_main->get_random_tip();
        if( currentTipId < 0 || currentTipId > editor_gui_main->get_tip_count() )
        {
            currentTipId = 0;
        }
        std::string currentTipString = editor_gui_main->get_tip(currentTipId);

        GPE_Label_Title * doYouKnowLabel = new GPE_Label_Title("Do you know?","Do you know?");
        GPE_GuiElementList * showTipList = new GPE_GuiElementList();
        showTipList->hideXScroll = true;
        showTipList->hideYScroll = true;
        GPE_ToolLabelButton * closeButton = new GPE_ToolLabelButton( "Close","");
        GPE_ToolLabelButton * nextTipButton = new GPE_ToolLabelButton( "Next Tip","");
        GPE_ToolLabelButton * previousTipButton = new GPE_ToolLabelButton( "Previous Tip","");
        GPE_CheckBoxBasic * showAtStartUpButton = new GPE_CheckBoxBasic("Show tips at startup","Unclick to not see this popup automatically on start");
        showAtStartUpButton->set_clicked( editor_gui_main->showTipsAtStartUp);
        GPE_ToolLabelButton * randomTipButton = new GPE_ToolLabelButton( "Random Tip","");
        GPE_WrappedTextArea * tipParagraph = new GPE_WrappedTextArea();
        tipParagraph->set_string(currentTipString);
        tipParagraph->set_width(512-GENERAL_GPE_GUI_PADDING*2);
        tipParagraph->set_height(96);
        gpe::renderer_main->reset_viewpoint();
        //main_OVERLAY->render_frozen_screenshot( );
        while(exitOperation==false)
        {
            gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::arrow) );
            //gpe::error_log->report("Processing tip of the day");
            gpe::game_runtime->start_loop();

            widget_box.x = ( gpe::screen_width-promptBoxWidth)/2;
            widget_box.y = ( gpe::screen_height-promptBoxHeight)/2;
            widget_box.w = promptBoxWidth;
            widget_box.h = promptBoxHeight;
            showTipList->set_coords(widget_box.x, widget_box.y+32);
            showTipList->set_width(widget_box.w);
            showTipList->set_height(widget_box.h-32);
            showTipList->barXMargin = GENERAL_GPE_GUI_PADDING;
            showTipList->barYMargin = GENERAL_GPE_GUI_PADDING;
            showTipList->barXPadding = GENERAL_GPE_GUI_PADDING;
            showTipList->barYPadding = GENERAL_GPE_GUI_PADDING;

            editor_gui_main->reset_gui_info();
            showTipList->clear_list();
            showTipList->add_gui_element(doYouKnowLabel,true);
            showTipList->add_gui_element(tipParagraph,true);
            showTipList->add_gui_element(showAtStartUpButton, true);
            showTipList->add_gui_element(previousTipButton,false);
            showTipList->add_gui_element(nextTipButton,true);
            showTipList->add_gui_element(randomTipButton,false);
            showTipList->add_gui_element(closeButton,false);
            showTipList->process_self( NULL, NULL );

            if( gpe::input->check_kb_released(kb_esc) || closeButton->is_clicked() || gpe::window_controller_main->is_resized()  )
            {
                exitOperation = true;
            }
            else if( nextTipButton->is_clicked() )
            {
                currentTipId = editor_gui_main->get_next_tip(currentTipId);
                currentTipString = editor_gui_main->get_tip(currentTipId);
                tipParagraph->set_string(currentTipString);
            }
            else if( previousTipButton->is_clicked() )
            {
                currentTipId = editor_gui_main->get_previous_tip(currentTipId);
                currentTipString = editor_gui_main->get_tip(currentTipId);
                tipParagraph->set_string(currentTipString);
            }
            else if( randomTipButton->is_clicked() )
            {
                currentTipId = editor_gui_main->get_random_tip();
                currentTipString = editor_gui_main->get_tip(currentTipId);
                tipParagraph->set_string(currentTipString);
            }

            //gpe::error_log->report("Rendering tip of the day");
            gpe::renderer_main->reset_viewpoint();
            if( !gpe::window_controller_main->is_resized() )
            {
                //if( gpe::input->window_input_received )
                {
                    main_OVERLAY->render_frozen_screenshot( );
                }
                //Update screen
                gpe::gcanvas->render_rect( &widget_box,theme_main->main_box_color,false);

                gpe::gcanvas->render_rectangle( widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+32,theme_main->popup_box_color,false);
                gpe::gcanvas->render_rect( &widget_box,theme_main->popup_box_highlight_color,true);
                gpe::gfs->render_text( widget_box.x+widget_box.w/2,widget_box.y+GENERAL_GPE_GUI_PADDING,popUpCaption,theme_main->popup_box_font_color,font_default,gpe::fa_center,gpe::fa_top);
                showTipList->render_self( NULL, NULL );
                //editor_gui_main-render_gui_info(  true);

                gpe::gcanvas->render_rect( &widget_box,theme_main->popup_box_border_color,true);
                if( editor_gui_main!= NULL )
                {
                    editor_gui_main->render_gui_info( );
                }
            }
            gpe::game_runtime->end_loop();
        }

        if( showTipList!=NULL)
        {
            delete showTipList;
            showTipList = NULL;
        }

        if( doYouKnowLabel!=NULL)
        {
            delete doYouKnowLabel;
            doYouKnowLabel = NULL;
        }
        if( closeButton!=NULL)
        {
            delete closeButton;
            closeButton = NULL;
        }
        if( nextTipButton!=NULL)
        {
            delete nextTipButton;
            nextTipButton = NULL;
        }
        if( previousTipButton!=NULL)
        {
            delete previousTipButton;
            previousTipButton = NULL;
        }
        if( randomTipButton!=NULL)
        {
            delete randomTipButton;
            randomTipButton = NULL;
        }
        if( showAtStartUpButton!=NULL)
        {
            editor_gui_main->showTipsAtStartUp = showAtStartUpButton->is_clicked();
            editor_gui_main->save_settings();
            delete showAtStartUpButton;
            showAtStartUpButton = NULL;
        }
        if( tipParagraph!=NULL)
        {
            delete tipParagraph;
            tipParagraph = NULL;
        }
        gpe::input->reset_all_input();
    }
}
