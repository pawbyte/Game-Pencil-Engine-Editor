/*
gpe_editor_state.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#include "gpe_editor_state.h"

GPE_Editor_State::GPE_Editor_State(std::string sName)
{
    if( (int)sName.size() > 0)
    {
        stateName = sName;
    }
    else
    {
        stateName = "GPE_Editor";
    }
}

GPE_Editor_State::~GPE_Editor_State()
{
    //Free the surfaces
    if( background !=NULL )
    {
        delete background;
        background = NULL;
    }
    if( message !=NULL )
    {
        delete message;
        message = NULL;
    }
    if( GPE_MAIN_GUI!=NULL)
    {
        delete GPE_MAIN_GUI;
        GPE_MAIN_GUI = NULL;
    }
}

void GPE_Editor_State::process_input()
{
    if( input->check_mouse_released( mb_anybutton ) )
    {
        GPE_MAIN_HIGHLIGHTER->clear_suggestions();
    }
    gpe->cursor_change( gpe->cursor_system_name( GPE_CURSOR_ARROW) );
    //if( GPE_MAIN_RENDERER->window_changed() )
    {
        //updates toolbars and whatnots info
        reset_camera();
        mainToolBar->set_width(SCREEN_WIDTH);
        //mainButtonBar->set_width(SCREEN_WIDTH);
        mainButtonBar->set_coords( 0,32 );
        if( MAIN_EDITOR_SETTINGS!=NULL)
        {
            if( MAIN_EDITOR_SETTINGS->ideButtonBarSize!=NULL)
            {
                int foundButtonSize =  MAIN_EDITOR_SETTINGS->ideButtonBarSize->get_selected_value();
                mainButtonBar->set_height( foundButtonSize );
                if( MAIN_EDITOR_SETTINGS->ideButtonBarAlignment !=NULL && foundButtonSize > 0)
                {
                    int foundButtonBarAlignment = MAIN_EDITOR_SETTINGS->ideButtonBarAlignment->get_selected_value();
                    if( foundButtonBarAlignment==1)
                    {
                        mainButtonBar->set_coords( (SCREEN_WIDTH-mainButtonBar->get_width() )/2, 32 );
                    }
                    else if( foundButtonBarAlignment==2)
                    {
                        mainButtonBar->set_coords( (SCREEN_WIDTH-mainButtonBar->get_width() ), 32 );
                    }
                    else
                    {
                        mainButtonBar->set_coords( 0, 32 );
                    }
                }
                else
                {
                    mainButtonBar->set_coords( 0, 24 );
                }
            }
        }

        GPE_Main_Statusbar->set_coords(0,SCREEN_HEIGHT-24 );
        GPE_Main_Statusbar->set_height( 24 );
        GPE_Main_Statusbar->set_width(SCREEN_WIDTH);

        if( GPE_DOCK!=NULL )
        {
            GPE_DOCK->set_coords( 0,mainButtonBar->get_y2pos() + GENERAL_GPE_GUI_PADDING);
            GPE_DOCK->set_width( SCREEN_WIDTH );
            GPE_DOCK->set_height( SCREEN_HEIGHT-GPE_DOCK->get_ypos() - GPE_Main_Statusbar->get_height() );
        }
    }
}

void GPE_Editor_State::apply_logic()
{
    GPE_MAIN_GUI->apply_logic();

    if( !mainToolBar->is_open() && !MAIN_CONTEXT_MENU->is_open() )
    {
        if( mainButtonBar!=NULL )
        {
            mainButtonBar->process_self();
            if( mainButtonBar->selectedOption>= 0 )
            {
                switch( mainButtonBar->selectedOption)
                {
                case 1:
                    GPE_MAIN_GUI->launch_new_project();
                    break;
                case 2:
                    GPE_MAIN_GUI->open_new_project();
                    break;

                case 3:
                    GPE_MAIN_GUI->save_current_project();
                    break;

                case 4:
                    GPE_MAIN_GUI->save_all_projects();
                    break;

                case 5:
                    if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
                    {
                        GPE_Main_TabManager->add_new_tab(CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource() );
                        projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
                        tProjectProps->projectSettingsBar->set_selected_option("Platforms");
                    }
                    break;

                case 6:
                    if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
                    {
                        CURRENT_PROJECT->export_and_play_native();
                    }
                    break;
                default:

                    break;
                }
            }
        }
    }

    if( SEEKED_PROJECT_NAME != CURRENT_PROJECT_NAME )
    {
        set_current_gpe_project_from_name( SEEKED_PROJECT_NAME );
    }
    /*
    //language tooltip
    //place elsewhere once language support is added
    if( point_between(input->mouse_x,input->mouse_y,SCREEN_WIDTH-32,0,SCREEN_WIDTH,32) )
    {
        MAIN_OVERLAY->update_tooltip(input->mouse_x,input->mouse_y+GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING,"Language Settings: American English! USA! ",FA_RIGHT);
    }
    */
}

void GPE_Editor_State::clean_up()
{

}

void GPE_Editor_State::end_state()
{

}

void GPE_Editor_State::render()
{
    Uint32 sTicks, eTicks;
    sTicks = SDL_GetTicks();
    GPE_MAIN_THEME->render_background(NULL, NULL);
    eTicks = SDL_GetTicks();
    GPE_Log_MS_Action("GPE_MAIN_THEME->render_background()",eTicks - sTicks,10 );

    sTicks = SDL_GetTicks();
    if( mainButtonBar!=NULL  )
    {
       mainButtonBar->render_self( NULL, NULL);
    }
    eTicks = SDL_GetTicks();
    GPE_Log_MS_Action("mainButtonBar->render_self)",eTicks - sTicks,10 );

    sTicks = SDL_GetTicks();
    if( GPE_MAIN_GUI!=NULL )
    {
        GPE_MAIN_GUI->render_foreground_engine();
    }
    eTicks = SDL_GetTicks();
    GPE_Log_MS_Action("GPE_MAIN_GUI->render_foreground_engine()",eTicks - sTicks,10 );

}

void GPE_Editor_State::start_state()
{
    if( GPE_VERSION_UPDATE_NUMBER==0)
    {
        GPE_Main_Logs->log_general_line("Starting Game Pencil Engine Version "+float_to_string(GPE_VERSION_FLOAT_NUMBER)+".0...");
    }
    else
    {
        GPE_Main_Logs->log_general_line("Starting Game Pencil Engine Version "+float_to_string(GPE_VERSION_FLOAT_NUMBER)+"...");
    }

    gpe->cursor_change( gpe->cursor_system_name( GPE_CURSOR_ARROW) );
    //Gets SDL Version
    SDL_compiled_version.major = 0;
    SDL_VERSION(&SDL_compiled_version);
    SDL_VersionText = "SDL Version: "+int_to_string(SDL_compiled_version.major)+"."+int_to_string(SDL_compiled_version.minor)+"."+int_to_string(SDL_compiled_version.patch);
    int animationDataLabelWidth =  0;
    int hh=0; // the 8th position is the widest

    mainToolBar = GPE_MAIN_GUI->init_toolbar();

    GPE_Animation * mainExportOptionsanimation = guiRCM->animation_add(APP_DIRECTORY_NAME+"resources/gfx/animations/main_export_options_icons.png",12,true,0,0,false);
    mainButtonBar = new GPE_ToolIconButtonBar(24);
    mainButtonBar->set_coords( 0, mainToolBar->get_height() );
    mainButtonBar->widthAutoResizes = true;
    mainButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/file.png","New Project",1,true );
    mainButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/folder.png","Open Project",2,true );
    mainButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/save.png","Save Project",3,false );
    mainButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/book.png","Save All Projects",4,true );
    mainButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/cog.png","Build Project",5,false);
    mainButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/play.png","Build & PLAY Project",6,false);

    GPE_PopUpMenu_Option * newOption = NULL;
    GPE_PopUpMenu_Option * newOptionLayer2 = NULL;
    GPE_PopUpMenu_Option * newOptionLayer3 = NULL;
    newOption = mainToolBar->add_menu_option("File");
    newOption->add_menu_option("New Project",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/file.png"),-1,NULL,false,true,false,kb_ctrl,kb_n );
    newOption->add_menu_option("Open Project",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/folder.png"),-1,NULL,false,true,false,kb_ctrl,kb_o );
    newOption->add_menu_option("Browse Projects",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/binoculars.png") );
    newOptionLayer2 = MAIN_TOOLBAR_RECENT_PROJECTS = newOption->add_menu_option("Recent Projects",-1,NULL,-1,NULL,true);
    GPE_MAIN_GUI->update_recent_project_list(false);
    //Adds the recent projects to the list

    //Adds the recent files to the list
    newOption->add_menu_option("Save Project",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/save.png"),-1,NULL,false,true,false,kb_ctrl,kb_s  );
    newOption->add_menu_option("Save All Projects",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/save.png"),-1,NULL,false,true,false,kb_ctrl,kb_s,kb_shift );
    /*
    newOption->add_menu_option("Export",-1,-1,NULL,false);
    newOption->add_menu_option("Import Resources");
    newOption->add_menu_option("Export Resources");
    newOption->add_menu_option("Properties",-1,5,NULL,true);
    */
    newOption->add_menu_option("Quit Editor",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/truck.png"),76,NULL,false,true,false,kb_ctrl,kb_q);


    /*
    newOption = mainToolBar->add_menu_option("Edit");
    newOption->add_menu_option("Undo",-1,-1,NULL,false);
    newOption->add_menu_option("Redo",-1,-1,NULL,false);
    newOption->add_menu_option("Cut",-1,-1,NULL,false);
    newOption->add_menu_option("Copy",-1,-1,NULL,false,false);
    newOption->add_menu_option("Paste",-1,-1,NULL,false);
    */

    newOption = mainToolBar->add_menu_option("View");
    //newOption->add_menu_option("Reset View",-1,-1,NULL,false);

    newOption->add_menu_option("Toggle Fullscreen Mode",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/crop.png"),-1,NULL,false,true,false, kb_f11);
    if( GPE_DOCK!=NULL)
    {
        newOption->add_option( GPE_DOCK->toolbarOptonsHolder );
    }

    newOptionLayer2 = newOption->add_menu_option("Text Area Settings",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/cogs.png"),-1,NULL,false,false);
    newOptionLayer2->add_menu_option("Toggle Line Count",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);
    newOptionLayer2->add_menu_option("Toggle Syntax Highlighting",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,NULL,false);
    //newOptionLayer2->add_menu_option("",-1,-1,NULL,false);
    newOption->add_menu_option("Start Page",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/cube.png"),-1,NULL,false);
    newOption->add_menu_option("Tip of the Day",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/info.png"),-1,NULL,false);

    //newOption = mainToolBar->add_menu_option("Tools");
    //newOption = mainToolBar->add_menu_option("Resources");

    newOption = mainToolBar->add_menu_option("Search");
    newOption->add_menu_option("Find...",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_f);
    newOption->add_menu_option("Find in files...",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_f,kb_shift);
    newOption->add_menu_option("Replace...",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_r);
    newOption->add_menu_option("Replace in files....",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_r,kb_shift);
    newOption->add_menu_option("Goto Line...",-1,NULL,-1,NULL,false,true,false, kb_ctrl,kb_g);
    newOption->add_menu_option("Find Project Resource...",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/search.png"),-1,NULL,false);

    newOption = mainToolBar->add_menu_option("Import");
    for( int tempResType = RESOURCE_TYPE_ANIMATION; tempResType <= RESOURCE_TYPE_FONT; tempResType++ )
    {
        if( (int)RESOURCE_TYPE_NAMES[tempResType].size() > 2 )
            newOption->add_menu_option("Import "+RESOURCE_TYPE_NAMES[tempResType]);
    }

    newOption = mainToolBar->add_menu_option("Build");
    // HTML5/ Web platforms

    newOption->add_menu_option("Build HTML5 GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/html5.png"),-1,NULL,true,true,false,kb_ctrl,kb_f5);
    newOption->add_menu_option("RUN HTML5 GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/play-circle.png"),-1,NULL,true,true,false,kb_f8);
    newOption->add_menu_option("Build HTML5 GAME & RUN",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/play.png"),-1,NULL,true,true,false,kb_f5);
    //desktop / laptop platforms
    if( GPE_FOUND_OS==GPE_IDE_WINDOWS)
    {
        newOption->add_menu_option("Build WINDOWS GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/windows.png"),-1,NULL,false,true );
        newOption->add_menu_option("Build OSX GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/apple.png"),-1,NULL,false,true );
        newOption->add_menu_option("Build LINUX GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/linux.png"),-1,NULL,true,true );
    }
    else if( GPE_FOUND_OS==GPE_IDE_MAC)
    {
        newOption->add_menu_option("Build OSX GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/apple.png"),-1,NULL,false,true );
        newOption->add_menu_option("Build LINUX GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/linux.png"),-1,NULL,true,true );
        newOption->add_menu_option("Build WINDOWS GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/windows.png"),-1,NULL,false,true );
    }
    else
    {
        newOption->add_menu_option("ExpBuildort LINUX GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/linux.png"),-1,NULL,true,true );
        newOption->add_menu_option("Build WINDOWS GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/windows.png"),-1,NULL,false,true );
        newOption->add_menu_option("Build OSX GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/apple.png"),-1,NULL,false,true );
    }
    /*
    newOption->add_menu_option("Compile Current Tab",-1);
    newOption->add_menu_option("OUYA- P2",-1,5,mainExportOptionsanimation,false,false);
    */

    //console platforms
    /*
    newOptionLayer2 =newOption->add_menu_option("Build Nintendo Switch GAME",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/animations/nintendo_switch_logo.png"),-1,NULL,true,true);
    newOptionLayer2->renderWhite = true;*/
    /*
    newOption->add_menu_option("Build Xbox One GAME",-1,3,mainExportOptionsanimation,true,false);
    */

    /*
    //handheld consoles
    newOptionLayer2 = newOption->add_menu_option("Handheld Platforms (Coming Soon)",-1);
    newOptionLayer2->add_menu_option("GCW-Zero- P3",-1,8,mainExportOptionsanimation,false);
    newOptionLayer2->add_menu_option("Nintendo 3DS & New 3DS- P3",-1,9,mainExportOptionsanimation,false,false);
    newOptionLayer2->add_menu_option("Open Pandora- P3",-1,10,mainExportOptionsanimation,false,false);
    newOptionLayer2->add_menu_option("Playstation Vita- P3",-1,11,mainExportOptionsanimation,false,false);

    //mobile platforms
    newOptionLayer2 = newOption->add_menu_option("Mobile Platforms(Coming Soon)",-1);
    newOptionLayer2->add_menu_option("Android - P4",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("Blackberry - P5",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("iPhones - P5",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("Tizen Phones - P6",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("Ubuntu Phones - P6",-1,11,NULL,false,false);
    newOptionLayer2->add_menu_option("Windows Phones - P4",-1,11,NULL,false,false);
    */

    newOptionLayer2 = newOption->add_menu_option("Clean Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [HTML5] Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [Windows] Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [Linux] Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [OSX] Build Folder",-1);
    //newOptionLayer2->add_menu_option("Clean [HTML5] Build Folder",-1,-1,mainExportOptionsanimation,false);

    newOption = mainToolBar->add_menu_option("Tools");
    newOption->add_menu_option("Game Controller Tester",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/gamepad.png"),-1,NULL,false,true,false);
    newOption->add_menu_option("Extra Tools",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/briefcase.png"),-1,NULL,false,true,false);

    newOption = mainToolBar->add_menu_option("Settings");
    newOption->add_menu_option("User Settings",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/cog.png"),-1, NULL,false,true,false,kb_f2);
    newOption->add_menu_option("JS Compiler Settings",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/code.png"),-1,NULL,false,true,false);
    //C++ Settings
    newOption->add_menu_option("C++ Builder Settings",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/code.png"),-1,NULL,false,true,false);

    newOption = mainToolBar->add_menu_option("Help" );
    //newOption->add_menu_option("Online Documentation",-1,17,mainMenuanimation,false);
    //newOption->add_menu_option("Tips",-1,20,mainMenuanimation,false);
    //newOption->add_menu_option("Forums",-1);
    newOption->add_menu_option("General Help",-1,NULL,-1,NULL,false,true,false,kb_f1);


    newOption->add_menu_option("Online Documentation",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/book.png"),-1,NULL,false);
    newOption->add_menu_option("Online Forums",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/group.png"),-1,NULL,false);
    newOption->add_menu_option("Community Chat",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/comments.png"),-1,NULL,false);
    newOption->add_menu_option("Tip of the Day",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/infO.png"),-1,NULL,false);
    newOption->add_menu_option("Report Issue",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/bug.png"),-1,NULL,false);
    newOption->add_menu_option("Check Updates",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/question-circle.png"),-1,NULL,false);
    newOption->add_menu_option("License",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/file-text.png"),-1,NULL,false);
    newOption->add_menu_option("About Game Pencil Engine",-1,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/pencil.png"),-1,NULL,false,true,false,kb_ctrl,kb_b);
    //newOption->add_menu_option("Licenses",-1,27,mainMenuanimation,false);
    GPE_Main_TabManager->set_coords(-1,mainButtonBar->get_y2pos()+GENERAL_GPE_GUI_PADDING );
    GPE_MAIN_GUI->init_resourcebar();
    //main resource bar elements

    country_language_image = gpeph->get_new_texture();
    if(country_language_image!=NULL)
    {
        //country_language_image->load_new_texture( GPE_MAIN_RENDERER,APP_DIRECTORY_NAME+"resources/gfx/flags/24/us.png",-1,true);
    }

    //Load the background
    background = NULL;

    //Render the intro message
    message = NULL;
    if( GPE_MAIN_GUI!=NULL)
    {
        GPE_MAIN_GUI->load_settings();
        if( GPE_MAIN_GUI->showStartPageAtStartUp)
        {
            GPE_Main_TabManager->add_new_tab(MAIN_START_PAGE);
            MAIN_START_PAGE->set_name("Start Page");
        }

        if( GPE_MAIN_GUI->showTipsAtStartUp)
        {
            gpe->cursor_change( gpe->cursor_system_name( GPE_CURSOR_ARROW) );
            input->handle_input(true);
            process_input();
            input->reset_all_input();
            apply_logic();
            render();
            if( GPE_MAIN_GUI->showTipsAtStartUp)
            {
                GPE_Show_Tip_Of_Day();
            }
        }


        /*
        GPE_ProjectFolder * newProj = new GPE_ProjectFolder("New Project","here/","newproject.tgif");
        GPE_MAIN_GUI->gpeProjects.push_back(newProj);
        GPE_ProjectFolder * newExampleProj = new GPE_ProjectFolder("Examples","here/","examples.tgif");
        GPE_MAIN_GUI->gpeProjects.push_back(newExampleProj);
        */
    }
}

bool GPE_Editor_Init( int argc, char* args[] )
{
    GPE_Report("Starting GPE Editor...");
    GPE_Editor_Init_Globals();

    GPE_Render_Package * debugWindow = gpeph->add_render_package( "sdl_debug_window" );
    debugWindow->packageWindow = new GPE_Window("Game Pencil Debug Window...",SCREEN_WIDTH, SCREEN_HEIGHT, true, false, false, true );
    debugWindow->packageRenderer = new GPE_Renderer_SDL( debugWindow->packageWindow->get_window_id(), SCREEN_HEIGHT, SCREEN_HEIGHT );
    debugWindow->packageTexture = new GPE_Texture_SDL();
    debugWindow->packageWindow->hide_window();

    GPE_Report( "Starting GUI..." );
    if( !PAW_GUI_START() )
    {
        GPE_Report("Unable to start Paw_GUI LIBRARY!\n");
        return false;
    }
    GPE_MAIN_GUI = new GPE_Gui_Engine();
    //If everything initialized fine
    GPE_Main_TabManager = new GPE_TabManager();

    MAIN_EDITOR_SETTINGS = new gamePencilEditorSettingsResource();
    MAIN_EDITOR_SETTINGS->set_global_rid(-1);
    MAIN_EDITOR_SETTINGS->set_name("User Settings");
    MAIN_EDITOR_SETTINGS->preprocess_self();

    MAIN_ABOUT_PAGE = new gamePencilAboutPageResource();
    MAIN_ABOUT_PAGE->set_global_rid(-2);
    MAIN_ABOUT_PAGE->set_name("About...");

    MAIN_HELP_PAGE = new gamePencilHelpPageResource();
    MAIN_HELP_PAGE->set_global_rid(-3);
    MAIN_HELP_PAGE->set_name("Help...");


    MAIN_START_PAGE = new gamePencilStartPageResource();
    MAIN_START_PAGE->set_global_rid(-4);
    MAIN_START_PAGE->set_name("Start Page");

    MAIN_EXTRA_TOOLS = new gamePencilExtraTools();
    MAIN_EXTRA_TOOLS->set_global_rid(-5);
    MAIN_EXTRA_TOOLS->set_name("Extra Tools");

    GPE_JS_COMPILER_SETTINGS = new gameJSCompilerSettingsResource();
    GPE_JS_COMPILER_SETTINGS->set_global_rid(-6);
    GPE_JS_COMPILER_SETTINGS->set_name("JS Compiler Settings");
    GPE_JS_COMPILER_SETTINGS->preprocess_self();


    GPE_CPP_BUILDER_SETTINGS = new gameCPPBuilderSettingsResource();
    GPE_CPP_BUILDER_SETTINGS->set_global_rid(-7);
    GPE_CPP_BUILDER_SETTINGS->set_name("C++ Builder Settings");
    GPE_CPP_BUILDER_SETTINGS->preprocess_self();

    MAIN_CONTROLLER_TESTER = new gamePencilgamecontrollerTesterResource();
    MAIN_CONTROLLER_TESTER->set_global_rid(-8);
    MAIN_CONTROLLER_TESTER->set_name("Game Controller Tester");

    //GPE_Main_TabManager->add_new_tab(MAIN_START_PAGE);

    GPE_Main_Statusbar = new GPE_StatusBar();
    GPE_Main_Statusbar->set_coords(0,0);
    GPE_Main_Statusbar->set_width(SCREEN_WIDTH);
    GPE_Main_Statusbar->set_height(24);

    GPE_Main_Logs = new GPE_LogManager();
    GPE_Main_Logs->set_coords(0,SCREEN_HEIGHT-64);
    GPE_Main_Logs->set_width(SCREEN_WIDTH);
    GPE_Main_Logs->set_height(32);

    GPE_MAIN_HIGHLIGHTER = new GPE_Syntax_Highlighter();
    GPE_Report("Adding Docking System...");
    GPE_DOCK = new gpeEditorDock();
    GPE_DOCK->add_default_panel("Editor",DOCK_TOP_LEFT, true );
    GPE_DOCK->add_default_panel("Inspector",DOCK_BOTTOM_LEFT, true );
    GPE_DOCK->add_default_panel("Resources",DOCK_TOP_RIGHT, true );
    GPE_DOCK->add_default_panel("Tilesheet",DOCK_BOTTOM_RIGHT, true );
    GPE_DOCK->add_default_panel("Grid Settings",DOCK_BOTTOM_RIGHT, true );
    GPE_Report("Docking System added...");

    spm = new GPE_SceneEditorHelper();

    GPE_IS_LOADING = false;
    GPE_Editor_State * newEditor = new GPE_Editor_State( "gpe_editor");
    gpe->state_add( newEditor );
    gpe->state_set( newEditor->get_state_name() );

    //Opens projects based on args[0] being the application name.
    if( argc > 1)
    {
        for( int iArgc = 1; iArgc <argc; iArgc++ )
        {
            GPE_MAIN_GUI->open_project( args[iArgc] );
            GPE_Report( args[iArgc] );
        }
    }
    else if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->launchLastProjectBox!=NULL )
    {
        if( MAIN_EDITOR_SETTINGS->launchLastProjectBox->is_clicked() )
        {
            int iRPList = 0;
            int maxRPList = GPE_MAIN_GUI->get_recent_project_list_size();
            if( maxRPList > 0 )
            {
                if( file_exists(GPE_MAIN_GUI->get_recent_project_name(0) ) )
                {
                    //GPE_MAIN_GUI->open_project( GPE_MAIN_GUI->get_recent_project_name(0) );
                }
            }
        }
    }

    if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->ideSettingsFPSRate!=NULL )
    {
        gpe->set_fps( MAIN_EDITOR_SETTINGS->ideSettingsFPSRate->get_selected_value() );
    }
    GPE_Report("Loading in dock settings..");
    GPE_DOCK->load_dock_setings( "dock_settings.gpf");

    return true;
}


bool GPE_Editor_Quit()
{
    if( GPE_Main_TabManager!=NULL)
    {
        GPE_Report("Closing all tabs....");
        GPE_Main_TabManager->close_all_tabs();
    }
    if( MAIN_EDITOR_SETTINGS!=NULL)
    {
        GPE_Report("Deleting Settings....");
        MAIN_EDITOR_SETTINGS->save_resource();
        delete MAIN_EDITOR_SETTINGS;
        MAIN_EDITOR_SETTINGS = NULL;
    }
    if( MAIN_ABOUT_PAGE!=NULL)
    {
        GPE_Report("Deleting about page....");
        delete MAIN_ABOUT_PAGE;
        MAIN_ABOUT_PAGE = NULL;
    }
    if( MAIN_HELP_PAGE!=NULL)
    {
        GPE_Report("Deleting help page....");
        delete MAIN_HELP_PAGE;
        MAIN_HELP_PAGE = NULL;
    }
    if( MAIN_START_PAGE!=NULL)
    {
        GPE_Report("Deleting start page....");
        delete MAIN_START_PAGE;
        MAIN_START_PAGE = NULL;
    }
    if( MAIN_EXTRA_TOOLS!=NULL)
    {
        GPE_Report("Deleting extra tools....");
        delete MAIN_EXTRA_TOOLS;
        MAIN_EXTRA_TOOLS = NULL;
    }
    if( MAIN_CONTEXT_MENU!=NULL)
    {
        GPE_Report("Deleting context menu....");
        delete MAIN_CONTEXT_MENU;
        MAIN_CONTEXT_MENU = NULL;
    }
    if( MAIN_TOOLBAR_RECENT_PROJECTS!=NULL)
    {
        GPE_Report("Deleting toolbar recent projects....");
        delete MAIN_TOOLBAR_RECENT_PROJECTS;
        MAIN_TOOLBAR_RECENT_PROJECTS = NULL;
    }

    if( GPE_DOCK!=NULL )
    {
        delete GPE_DOCK;
        GPE_DOCK = NULL;
    }

    if( GPE_MAIN_GUI!=NULL)
    {
        GPE_Report("Deleting gui....");
        GPE_MAIN_GUI->save_settings();
        delete GPE_MAIN_GUI;
        GPE_MAIN_GUI = NULL;
    }
}
