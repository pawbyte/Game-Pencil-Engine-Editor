/*
gpe_editor_state.cpp
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

#include "gpe_editor_state.h"
#include "../extra/gpe_extra_tools.h"

gpe_editor_state::gpe_editor_state(std::string s_name)
{
    if( (int)s_name.size() > 0)
    {
        state_name = s_name;
    }
    else
    {
        state_name = "gpe_editor";
    }
}

gpe_editor_state::~gpe_editor_state()
{
    if( message !=nullptr )
    {
        delete message;
        message = nullptr;
    }
    if( editor_gui_main!=nullptr)
    {
        delete editor_gui_main;
        editor_gui_main = nullptr;
    }
}

void gpe_editor_state::process_input()
{
    if( gpe::input->check_mouse_released( mb_anybutton ) )
    {
        pawgui::main_syntax_highlighter->clear_suggestions();
    }
    gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::arrow) );
    //if( renderer_main->window_changed() )
    {
        //updates toolbars and whatnots info
        gpe::camera_reset();
        main_toolbar->set_coords(0,0);
        main_toolbar->set_width( gpe::screen_width );
        main_toolbar->set_height( 32 );
        //main_buttonbar->set_width(gpe::screen_width);
        editor_gui_main->main_notification_holder->set_coords(0, main_toolbar->get_y2() );
        editor_gui_main->main_notification_holder->set_width( gpe::screen_width );
        main_buttonbar->set_coords( 0, editor_gui_main->main_notification_holder->get_y2() );
        if( main_editor_settings!=nullptr)
        {
            int found_button_size = 32;
            if( main_editor_settings->ide_buttonBarSize!=nullptr)
            {
                found_button_size =  main_editor_settings->ide_buttonBarSize->get_selected_value();
                if( found_button_size < 24 )
                {
                    found_button_size = 24;
                }
            }
            else
            {
                found_button_size = 32;
            }
            main_buttonbar->set_height( found_button_size );
            if( main_editor_settings->ide_buttonBarAlignment !=nullptr && found_button_size > 0)
            {
                int found_buttonBarAlignment = main_editor_settings->ide_buttonBarAlignment->get_selected_value();
                if( found_buttonBarAlignment==1)
                {
                    main_buttonbar->set_coords( (gpe::screen_width-main_buttonbar->get_width() )/2, editor_gui_main->main_notification_holder->get_y2()  );
                }
                else if( found_buttonBarAlignment==2)
                {
                    main_buttonbar->set_coords( (gpe::screen_width-main_buttonbar->get_width() ), editor_gui_main->main_notification_holder->get_y2() );
                }
                else
                {
                    main_buttonbar->set_coords( 0, editor_gui_main->main_notification_holder->get_y2() );
                }
            }
            else
            {
                main_buttonbar->set_coords( 0, editor_gui_main->main_notification_holder->get_y2() );
            }
        }
        pawgui::main_statusbar->set_coords(0,gpe::screen_height-24 );
        pawgui::main_statusbar->set_height( 24 );
        pawgui::main_statusbar->set_width(gpe::screen_width);
    }
}

void gpe_editor_state::apply_logic()
{
    int dockX = 0, dockY = 0;

    if( main_buttonbar!=nullptr )
    {
        dockY = main_buttonbar->get_y2() + pawgui::padding_default;

        if( !main_toolbar->is_open() && !pawgui::main_context_menu->is_open() )
        {
            main_buttonbar->process_self();
            if( main_buttonbar->selectedOption>= 0 )
            {
                switch( main_buttonbar->selectedOption)
                {
                    case 1:
                        editor_gui_main->launch_new_project();
                    break;
                    case 2:
                        editor_gui_main->open_new_project();
                    break;

                    case 3:
                        editor_gui_main->save_current_project();
                    break;

                    case 4:
                        editor_gui_main->save_all_projects();
                    break;

                    case 5:
                        if( current_project!=nullptr && current_project->RESC_PROJECT_SETTINGS!=nullptr )
                        {
                            pawgui::main_tab_resource_bar->add_new_tab(current_project->RESC_PROJECT_SETTINGS->get_held_resource() );
                            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)current_project->RESC_PROJECT_SETTINGS->get_held_resource();
                            tProjectProps->project_settingsBar->set_selected_option("Platforms");
                        }
                    break;

                    case 6:
                        if( current_project!=nullptr && current_project->RESC_PROJECT_SETTINGS!=nullptr )
                        {
                            current_project->export_and_play_native();
                        }
                    break;
                    case 7:
                        process_view_mode();
                    break;

                    default:

                    break;
                }
            }
        }
    }

    if( gpe_dock!=nullptr )
    {
        gpe_dock->set_coords( dockX, dockY );
        gpe_dock->set_width( gpe::screen_width );
        if( pawgui::main_statusbar != nullptr )
        {
            gpe_dock->set_height( gpe::screen_height  - gpe_dock->get_ypos() - pawgui::main_statusbar->get_height() );
        }
        else
        {
            gpe_dock->set_height( gpe::screen_height - gpe_dock->get_ypos() );
        }
    }
    editor_gui_main->apply_logic();

    if( pawgui::seeked_project_name != pawgui::project_current_name )
    {
        set_current_gpe_project_from_name( pawgui::seeked_project_name );
    }
}

void gpe_editor_state::clean_up()
{

}

void gpe_editor_state::end_state()
{

}

void gpe_editor_state::process_view_mode()
{
    if( editor_view_icon == nullptr )
    {
        return;
    }

    std::string editor_icon_name = "Editor View-Mode 2D";
    if( editor_view_icon->is_clicked() )
    {
        pawgui::context_menu_open(gpe::input->mouse_position_x, gpe::input->mouse_position_y, 255);

        for( int i = 0; i < gpe::render_mode_userdefined0; i++ )
        {
            if( gpe::renderer_main->is_render_mode_supported( i ) == 1 )
            {
                pawgui::main_context_menu->add_menu_option("View Mode "+gpe::renderer_main->get_render_mode_name(i),i,nullptr,-1,nullptr,false,true);
            }
            else if( gpe::renderer_main->is_render_mode_supported( i ) == 0 )
            {
                pawgui::main_context_menu->add_menu_option("View Mode "+gpe::renderer_main->get_render_mode_name(i)+" - BETA",i,nullptr,-1,nullptr,false,true );
            }
            else
            {
                pawgui::main_context_menu->add_menu_option("View Mode "+gpe::renderer_main->get_render_mode_name(i)+" - Editor Backend NOT supported",i,nullptr,-1,nullptr,false,false );
            }
        }
        int menuSelection = pawgui::context_menu_process( );
        if( menuSelection>=0 && menuSelection < gpe::render_mode_userdefined0 )
        {
            //if( gpe::renderer_main->is_render_mode_supported( menuSelection ) )
            {
                editor_view_icon->descriptionText = "View Mode "+gpe::renderer_main->get_render_mode_name( menuSelection );
                main_editor_settings->current_editor_view_mode = menuSelection;
                gpe::gcanvas->set_render_mode(  menuSelection );
            }
        }
    }
}

void gpe_editor_state::render()
{
    uint64_t sTicks, eTicks;
    sTicks = gpe::time_keeper->get_ticks();
    pawgui::theme_main->render_background(nullptr, nullptr);
    eTicks = gpe::time_keeper->get_ticks();
    gpe::error_log->log_ms_action("pawgui::theme_main->render_background()",eTicks - sTicks,10 );

    sTicks = gpe::time_keeper->get_ticks();
    if( main_buttonbar!=nullptr  )
    {
       main_buttonbar->render_self( nullptr, nullptr);
    }
    eTicks = gpe::time_keeper->get_ticks();
    gpe::error_log->log_ms_action( "main_buttonbar->render_self)",eTicks - sTicks,10 );

    sTicks = gpe::time_keeper->get_ticks();
    if( editor_gui_main!=nullptr )
    {
        editor_gui_main->render_foreground_engine();
    }
    eTicks = gpe::time_keeper->get_ticks();
    gpe::error_log->log_ms_action("editor_gui_main->render_foreground_engine()",eTicks - sTicks,10 );

}

void gpe_editor_state::start_state()
{
    main_editor_log->log_general_line("Starting Game Pencil Engine Version "+ stg_ex::float_to_string( gpe::version_number_total )+"...");

    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Loading Icons & Widgets", "Please wait..." );
        main_gpe_splash_page->set_load_percent( 10 );
    }

    gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::arrow) );
    //Gets SDL Version
    //SDL_compiled_version.major = 0;
    //SDL_VERSION(&SDL_compiled_version);
    SDL_VersionText = "SDL Version: ";//+ /stg_ex::int_to_string(SDL_compiled_version.major)+"."+ stg_ex::int_to_string(SDL_compiled_version.minor)+"."+ stg_ex::int_to_string(SDL_compiled_version.patch);
    int animationDataLabelWidth =  0;
    int hh=0; // the 8th position is the widest

    main_toolbar = editor_gui_main->init_toolbar();

    gpe::animation2d * mainExportOptionsanimation = pawgui::rsm_gui->animation_add("guiExportOptions", gpe::app_directory_name+"resources/gfx/animations/main_export_options_icons.png",12,true,0,0,false);
    main_buttonbar = new pawgui::widget_button_iconbar( 32);
    main_buttonbar->set_coords( 0, main_toolbar->get_height() );
    main_buttonbar->widthAutoResizes = true;
    main_buttonbar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file.png","New Project",1,true );
    main_buttonbar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder.png","Open Project",2,true );
    main_buttonbar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/save.png","Save Project",3,false );
    main_buttonbar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/book.png","Save All Projects",4,true );
    main_buttonbar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cog.png","Build Project",5,false);
    main_buttonbar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png","Build & PLAY Project",6,false);
    editor_view_icon = main_buttonbar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/eye.png","Editor View-Mode: 2D",7,false);


    main_gpe_splash_page->set_load_percent( 15 ); //arbituarily set....

    pawgui::popup_menu_option * newOption = nullptr;
    pawgui::popup_menu_option * newOptionLayer2 = nullptr;
    pawgui::popup_menu_option * newOptionLayer3 = nullptr;
    newOption = main_toolbar->add_menu_option("File");

    pawgui::popup_menu_option * newProjectOption = nullptr;

    newProjectOption = newOption->add_menu_option("New....",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file.png"),-1,nullptr,false,false,false );
    newProjectOption->add_menu_option("New Game Project",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file.png"),-1,nullptr,false,true,false,kb_ctrl,kb_n );

    //Used for other "Related programs made by us [ BEGIN ]
    newProjectOption->add_menu_option("New Art Project",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/paint-brush.png"),-1,nullptr,false,false,false );
    newProjectOption->add_menu_option("New Web Project",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-code.png"),-1,nullptr,false,false,false );
    newProjectOption->add_menu_option("New Comic Project",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-photo.png"),-1,nullptr,false,false,false);
    //Used for other "Related programs made by us [ END ]


    newOption->add_menu_option("Open Project",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder.png"),-1,nullptr,false,true,false,kb_ctrl,kb_o );
    newOption->add_menu_option("Browse Projects",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/binoculars.png") );
    newOptionLayer2 = main_TOOLBAR_RECENT_PROJECTS = newOption->add_menu_option("Recent Projects",-1,nullptr,-1,nullptr,true);
    editor_gui_main->update_recent_project_list(false);
    //Adds the recent projects to the list

    //Adds the recent files to the list
    newOption->add_menu_option("Save Project",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/save.png"),-1,nullptr,false,true,false,kb_ctrl,kb_s  );
    newOption->add_menu_option("Save All Projects",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/save.png"),-1,nullptr,false,true,false,kb_ctrl,kb_s,kb_shift );
    /*
    newOption->add_menu_option("Export",-1,-1,nullptr,false);
    newOption->add_menu_option("Import Resources");
    newOption->add_menu_option("Export Resources");
    newOption->add_menu_option("Properties",-1,5,nullptr,true);
    */
    newOption->add_menu_option("Quit Editor",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/truck.png"),76,nullptr,false,true,false,kb_ctrl,kb_q);


    /*
    newOption = main_toolbar->add_menu_option("Edit");
    newOption->add_menu_option("Undo",-1,-1,nullptr,false);
    newOption->add_menu_option("Redo",-1,-1,nullptr,false);
    newOption->add_menu_option("Cut",-1,-1,nullptr,false);
    newOption->add_menu_option("Copy",-1,-1,nullptr,false,false);
    newOption->add_menu_option("Paste",-1,-1,nullptr,false);
    */

    newOption = main_toolbar->add_menu_option("View");
    //newOption->add_menu_option("Reset View",-1,-1,nullptr,false);

    newOption->add_menu_option("Toggle Fullscreen Mode",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/crop.png"),-1,nullptr,false,true,false, kb_f11);


    newOptionLayer2 = newOption->add_menu_option("Text Area Settings",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cogs.png"),-1,nullptr,false,false);
    newOptionLayer2->add_menu_option("Toggle Line Count",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,nullptr,false);
    newOptionLayer2->add_menu_option("Toggle Syntax Highlighting",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,nullptr,false);
    //newOptionLayer2->add_menu_option("",-1,-1,nullptr,false);

    newOptionLayer2 = newOption->add_menu_option("Dock Settings",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/columns.png"),-1,nullptr,false,false);

    newOptionLayer2->add_menu_option("Toggle Resource viewer",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/toggle-on.png"),-1,nullptr,false);

    newOption->add_menu_option("Start Page",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cube.png"),-1,nullptr,false);
    newOption->add_menu_option("Tip of the Day",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/info.png"),-1,nullptr,false);

    //newOption = main_toolbar->add_menu_option("Tools");
    //newOption = main_toolbar->add_menu_option("Resource Browser");

    newOption = main_toolbar->add_menu_option("Search");
    newOption->add_menu_option("Find...",-1,nullptr,-1,nullptr,false,true,false, kb_ctrl,kb_f);
    newOption->add_menu_option("Find in files...",-1,nullptr,-1,nullptr,false,true,false, kb_ctrl,kb_f,kb_shift);
    newOption->add_menu_option("Replace...",-1,nullptr,-1,nullptr,false,true,false, kb_ctrl,kb_r);
    newOption->add_menu_option("Replace in files....",-1,nullptr,-1,nullptr,false,true,false, kb_ctrl,kb_r,kb_shift);
    newOption->add_menu_option("Goto Line...",-1,nullptr,-1,nullptr,false,true,false, kb_ctrl,kb_g);
    newOption->add_menu_option("Find Project Resource...",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/search.png"),-1,nullptr,false);

    newOption = main_toolbar->add_menu_option("Import");
    for( int tempResType = gpe::resource_type_animation; tempResType <= gpe::resource_type_font; tempResType++ )
    {
        if( (int) gpe::resource_type_names[tempResType].size() > 2 )
            newOption->add_menu_option("Import "+ gpe::resource_type_names[tempResType]);
    }

    newOption = main_toolbar->add_menu_option("Build");
    // HTML5/ Web platforms

    newOption->add_menu_option("Build HTML5 GAME",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/html5.png"),-1,nullptr,true,true,false,kb_ctrl,kb_f5);
    newOption->add_menu_option("RUN HTML5 GAME",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play-circle.png"),-1,nullptr,true,true,false,kb_f8);
    newOption->add_menu_option("Build HTML5 GAME & RUN",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png"),-1,nullptr,true,true,false,kb_f5);
    //desktop / laptop platforms
    if( gpe::system_found_os == gpe::system_os_windows)
    {
        newOption->add_menu_option("Build WINDOWS GAME",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/windows.png"),-1,nullptr,false,true );
        newOption->add_menu_option("Build OSX GAME",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/apple.png"),-1,nullptr,false,true );
        newOption->add_menu_option("Build LINUX GAME",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/linux.png"),-1,nullptr,true,true );
    }
    else if( gpe::system_found_os== gpe::system_os_mac )
    {
        newOption->add_menu_option("Build OSX GAME",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/apple.png"),-1,nullptr,false,true );
        newOption->add_menu_option("Build LINUX GAME",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/linux.png"),-1,nullptr,true,true );
        newOption->add_menu_option("Build WINDOWS GAME",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/windows.png"),-1,nullptr,false,true );
    }
    else
    {
        newOption->add_menu_option("ExpBuildort LINUX GAME",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/linux.png"),-1,nullptr,true,true );
        newOption->add_menu_option("Build WINDOWS GAME",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/windows.png"),-1,nullptr,false,true );
        newOption->add_menu_option("Build OSX GAME",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/apple.png"),-1,nullptr,false,true );
    }
    /*
    newOption->add_menu_option("Compile Current Tab",-1);
    newOption->add_menu_option("OUYA- P2",-1,5,mainExportOptionsanimation,false,false);
    */

    //console platforms
    /*
    newOptionLayer2 =newOption->add_menu_option("Build Nintendo Switch GAME",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/animations/nintendo_switch_logo.png"),-1,nullptr,true,true);
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
    newOptionLayer2->add_menu_option("Android - P4",-1,11,nullptr,false,false);
    newOptionLayer2->add_menu_option("Blackberry - P5",-1,11,nullptr,false,false);
    newOptionLayer2->add_menu_option("iPhones - P5",-1,11,nullptr,false,false);
    newOptionLayer2->add_menu_option("Tizen Phones - P6",-1,11,nullptr,false,false);
    newOptionLayer2->add_menu_option("Ubuntu Phones - P6",-1,11,nullptr,false,false);
    newOptionLayer2->add_menu_option("Windows Phones - P4",-1,11,nullptr,false,false);
    */

    newOptionLayer2 = newOption->add_menu_option("Clean Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [HTML5] Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [Windows] Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [Linux] Build Folder",-1);
    newOptionLayer2->add_menu_option("Clean [OSX] Build Folder",-1);
    //newOptionLayer2->add_menu_option("Clean [HTML5] Build Folder",-1,-1,mainExportOptionsanimation,false);

    newOption = main_toolbar->add_menu_option("Tools");
    newOption->add_menu_option("Game Controller Tester",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/gamepad.png"),-1,nullptr,false,true,false);
    newOption->add_menu_option("Renderer Tester",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cubes.png"),-1,nullptr,false,true,false);
    newOption->add_menu_option("Extra Tools",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/briefcase.png"),-1,nullptr,false,true,false);

    newOption = main_toolbar->add_menu_option("Settings");
    newOption->add_menu_option("User Settings",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cog.png"),-1, nullptr,false,true,false,kb_f2);
    newOption->add_menu_option("JS Compiler Settings",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/code.png"),-1,nullptr,false,true,false);
    //C++ _settings
    newOption->add_menu_option("C++ Builder Settings",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/code.png"),-1,nullptr,false,true,false);

    newOption = main_toolbar->add_menu_option("Help" );
    //newOption->add_menu_option("Online Documentation",-1,17,mainMenuanimation,false);
    //newOption->add_menu_option("Tips",-1,20,mainMenuanimation,false);
    //newOption->add_menu_option("Forums",-1);

    newOption->add_menu_option("Online Documentation",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/book.png"),-1,nullptr,false);
    newOption->add_menu_option("Online Forums",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/group.png"),-1,nullptr,false);
    newOption->add_menu_option("Community Chat",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/comments.png"),-1,nullptr,false);
    newOption->add_menu_option("Tip of the Day",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/infO.png"),-1,nullptr,false);
    newOption->add_menu_option("Report Issue",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/bug.png"),-1,nullptr,false);
    newOption->add_menu_option("Check Updates",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/question-circle.png"),-1,nullptr,false);
    newOption->add_menu_option("About Game Pencil Engine",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/pencil.png"),-1,nullptr,false,true,false, kb_f1 );
    //newOption->add_menu_option("Licenses",-1,27,mainMenuanimation,false);

    main_gpe_splash_page->set_load_percent( 20 ); //arbituarily set....

    pawgui::main_tab_resource_bar->set_coords(-1,main_buttonbar->get_y2()+pawgui::padding_default );

    main_gpe_splash_page->set_load_percent( 25 ); //arbituarily set....
    editor_gui_main->init_resourcebar();
    //main resource bar elements

    country_language_image = gpe::rph->get_new_texture();
    if(country_language_image!=nullptr)
    {
        //country_language_image->load_new_texture(  gpe::app_directory_name+"resources/gfx/flags/24/us.png",-1,true);
    }

    //Render the intro message
    message = nullptr;
    main_gpe_splash_page->set_load_percent( 30 ); //arbituarily set....
    if( editor_gui_main!=nullptr)
    {
        editor_gui_main->load_settings();
        if( editor_gui_main->showStartPageAtStartUp)
        {
            pawgui::main_tab_resource_bar->add_new_tab(main_START_PAGE);
            main_START_PAGE->set_name("Start Page");
        }



        /*
        GPE_ProjectFolder * newProj = new GPE_ProjectFolder("New Project","here/","newproject.tgif");
        editor_gui_main->gpeProjects.push_back(newProj);
        GPE_ProjectFolder * newExampleProj = new GPE_ProjectFolder("Examples","here/","examples.tgif");
        editor_gui_main->gpeProjects.push_back(newExampleProj);
        */
    }

    main_gpe_splash_page->set_load_percent( 35 ); //arbituarily set....
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Loading Editor Settings", "Please wait..." );
    }


    if( gpe_dock != nullptr )
    {
        gpe::error_log->report("Loading in dock settings..");

        main_gpe_splash_page->set_load_percent( 40 ); //arbituarily set....
        if( gpe_dock->load_dock_setings( gpe::get_user_settings_folder()+"dock_settings.gpf") )
        {
            gpe::error_log->report( "Loading dock from [dock_settings.gpf]");
        }
        else
        {
            gpe::error_log->report( "Unable to load dock settings from [dock_settings.gpf]");
        }


        main_gpe_splash_page->set_load_percent( 45 ); //arbituarily set....
        //Check if we at least have the editor panel for dock after loading settings
        pawgui::widget_dock_panel * found_main_panel = gpe_dock->contains_panel("Main");
        pawgui::widget_dock_panel * found_editor_panel = gpe_dock->contains_panel("Editor");
        pawgui::widget_dock_panel * found_resources_panel = gpe_dock->contains_panel("Resource Browser");
        if( found_editor_panel == nullptr || found_resources_panel == nullptr || found_main_panel == nullptr )
        {
            gpe::error_log->report( "Unable to detect panels");

            //if not we reset the dock ( Typically for 1st time users or glitches)
            if( editor_gui_main!=nullptr )
            {
                gpe::error_log->report( "Resetting dock thru editor_gui_main");

                editor_gui_main->dock_reset();
            }
            else
            {
                gpe::error_log->report( "Resetting dock...");
                gpe_dock->resize_dock( 5, 3 );
            }
        }
        else
        {
            gpe::error_log->report( "Main 3 panels detected...");
        }
    }
    else
    {
        gpe::error_log->report("[gpe_dock] is not declared, so can not load...");
    }

    if( main_gpe_splash_page!=nullptr )
    {
        while( main_gpe_splash_page->exit_startup_mode() == false )
        {
            main_gpe_splash_page->render();
        }
    }
}

bool init_gpe_editor( int argc, char* args[] )
{
    editor_view_mode = new pawgui::widget_dropdown_menu("Editor View Mode", true );
    editor_view_mode->add_menu_option("2D","2D mode", gpe::render_mode_2D, true );
    editor_view_mode->add_menu_option("2D ISO","2D isometric mode", gpe::render_mode_2D_iso , false );
    editor_view_mode->add_menu_option("2.5D Sprite-Stacked","2D Sprite-Stacked", gpe::render_mode_psuedo3d_stacked, false );
    editor_view_mode->add_menu_option("2.5D Mode7"," 2.5D mode7", gpe::render_mode_pseudo3d_m7, false );
    editor_view_mode->add_menu_option("2.5D raycast"," 2.5D raycasting", gpe::render_mode_psuedo3d_raycast, false );
    editor_view_mode->add_menu_option("3D view mode"," 3D view mode", gpe::render_mode_3d, false );

    //Disables scaling as it's a full width application
    gpe::window_controller_main->disable_scaling();

    gpe::window_controller_main->set_window_size( 720, 480 );
    gpe::window_controller_main->set_window_position( -1 , -1  );
    gpe::error_log->report("Starting GPE Editor...");
    init_gpe_editor_globals();

    std::string font_file_location;
    int font_size = 14;
    gpe::error_log->report( "Starting GUI..." );
    if( !pawgui::init_gui( font_file_location, font_size ) )
    {
        gpe::error_log->report("Unable to start Paw_GUI LIBRARY!\n");
        return false;
    }

    main_gpe_splash_page = new gpe_splash_page();
    editor_gui_main = new GPE_Gui_Engine();
    //If everything initialized fine
    pawgui::main_tab_resource_bar = new pawgui::widget_tab_resource_bar();
    gpe::error_log->report("Tab manager added");

    main_editor_settings = new gpe_editor_settings_resource();
    main_editor_settings->set_global_rid(-1);
    main_editor_settings->set_name("User Settings");
    main_editor_settings->load_resource();
    gpe::error_log->report("Main editor added");

    main_about_page = new gamePencilAboutPageResource();
    main_about_page->set_global_rid(-2);
    main_about_page->set_name("About...");
    gpe::error_log->report("About page added");

    main_START_PAGE = new gamePencilStartPageResource();
    main_START_PAGE->set_global_rid(-4);
    main_START_PAGE->set_name("Start Page");
    gpe::error_log->report("Start page added");

    main_EXTRA_TOOLS = new gamePencilExtraTools();
    main_EXTRA_TOOLS->set_global_rid(-5);
    main_EXTRA_TOOLS->set_name("Extra Tools");
    gpe::error_log->report("Extra Tools page added");

    GPE_JS_COMPILER_SETTINGS = new gameJSCompiler_settingsResource();
    GPE_JS_COMPILER_SETTINGS->set_global_rid(-6);
    GPE_JS_COMPILER_SETTINGS->set_name("JS Compiler Settings");
    GPE_JS_COMPILER_SETTINGS->load_resource();
    gpe::error_log->report("JS compiler settings added");

    GPE_CPP_BUILDER_SETTINGS = new gameCPPBuilder_settingsResource();
    GPE_CPP_BUILDER_SETTINGS->set_global_rid(-7);
    GPE_CPP_BUILDER_SETTINGS->set_name("C++ Builder Settings");
    GPE_CPP_BUILDER_SETTINGS->load_resource();
    gpe::error_log->report("C++ Builder _settings added");

    main_gamepad_tester = new gamePencilgamepadTesterResource();
    main_gamepad_tester->set_global_rid(-8);
    main_gamepad_tester->set_name("Game Controller Tester");
    gpe::error_log->report("Game Controller Tester added");

    //pawgui::main_tab_resource_bar->add_new_tab(main_START_PAGE);

    pawgui::main_statusbar = new pawgui::widget_statusbar();
    pawgui::main_statusbar->set_coords(0,0);
    pawgui::main_statusbar->set_width(gpe::screen_width);
    pawgui::main_statusbar->set_height(24);
    gpe::error_log->report("Status Bar added");

    main_editor_log = new log_manager();
    main_editor_log->set_coords(0,gpe::screen_height-64);
    main_editor_log->set_width(gpe::screen_width);
    main_editor_log->set_height(32);
    gpe::error_log->report("Log Manager added");

    pawgui::main_syntax_highlighter = new pawgui::syntax_highlighter();
    gpe::error_log->report("Syntax Highlighter added");

    gpe::error_log->report("Adding Docking System...");
    gpe_dock = new pawgui::widget_dock();
    gpe_dock->set_width( gpe::screen_width );
    gpe_dock->set_height( gpe::screen_height );


        //add_to_panel( "Resource Browser", DOCK_TOP_LEFT,  true );
        //add_to_panel( "Editor", DOCK_TOP_LEFT_CENTER, true );
        //add_to_panel( "Inspector", DOCK_BOTTOM_RIGHT, true );
        //add_to_panel( "Meta", DOCK_BOTTOM_RIGHT, true );
        //add_to_panel("Logs", DOCK_BOTTOM_MIDDLE, true );
    gpe::error_log->report("Docking System added...");

    spm = new GPE_SceneEditorHelper();


    gpe::game_runtime->loading_data = false;
    gpe_editor_state * newEditor = new gpe_editor_state( "gpe_editor");
    gpe::game_runtime->state_add( newEditor );
    gpe::game_runtime->state_set( newEditor->get_state_name() );

    //Opens projects based on args[0] being the application name.
    if( argc > 1)
    {
        for( int iArgc = 1; iArgc <argc; iArgc++ )
        {
            editor_gui_main->open_project( args[iArgc] );
            gpe::error_log->report( args[iArgc] );
        }
    }
    else if( main_editor_settings!=nullptr && main_editor_settings->launchLastProjectBox!=nullptr )
    {
        if( main_editor_settings->launchLastProjectBox->is_clicked() )
        {
            int iRPList = 0;
            int maxRPList = editor_gui_main->get_recent_project_list_size();
            if( maxRPList > 0 )
            {
                if( sff_ex::file_exists(editor_gui_main->get_recent_project_name(0) ) )
                {
                    //editor_gui_main->open_project( editor_gui_main->get_recent_project_name(0) );
                }
            }
        }
    }

    if( main_editor_settings!=nullptr && main_editor_settings->ide_settingsFPSRate!=nullptr )
    {
        gpe::time_keeper->set_fps( main_editor_settings->ide_settingsFPSRate->get_selected_value() );
    }
    return true;
}


bool quit_gpe_editor()
{
    if( pawgui::main_tab_resource_bar!=nullptr)
    {
        gpe::error_log->report("Closing all tabs....");
        pawgui::main_tab_resource_bar->close_all_tabs();
    }
    if( main_editor_settings!=nullptr)
    {
        gpe::error_log->report("Deleting _settings....");
        main_editor_settings->save_resource();
        delete main_editor_settings;
        main_editor_settings = nullptr;
    }
    if( main_about_page!=nullptr)
    {
        gpe::error_log->report("Deleting about page....");
        delete main_about_page;
        main_about_page = nullptr;
    }
    if( main_START_PAGE!=nullptr)
    {
        gpe::error_log->report("Deleting start page....");
        delete main_START_PAGE;
        main_START_PAGE = nullptr;
    }
    if( main_EXTRA_TOOLS!=nullptr)
    {
        gpe::error_log->report("Deleting extra tools....");
        delete main_EXTRA_TOOLS;
        main_EXTRA_TOOLS = nullptr;
    }
    if( pawgui::main_context_menu!=nullptr)
    {
        gpe::error_log->report("Deleting context menu....");
        delete pawgui::main_context_menu;
        pawgui::main_context_menu = nullptr;
    }
    if( main_TOOLBAR_RECENT_PROJECTS!=nullptr)
    {
        gpe::error_log->report("Deleting toolbar recent projects....");
        delete main_TOOLBAR_RECENT_PROJECTS;
        main_TOOLBAR_RECENT_PROJECTS = nullptr;
    }

    if( gpe_dock!=nullptr )
    {
        gpe_dock->save_dock_setings( gpe::get_user_settings_folder()+"dock_settings.gpf");
        delete gpe_dock;
        gpe_dock = nullptr;
    }

    if( editor_gui_main!=nullptr)
    {
        gpe::error_log->report("Deleting gui....");
        editor_gui_main->save_settings();
        delete editor_gui_main;
        editor_gui_main = nullptr;
    }

    if( main_gpe_splash_page !=nullptr)
    {
        delete main_gpe_splash_page;
        main_gpe_splash_page = nullptr;
    }

    gpe::error_log->report("Deleting Gui...");
    pawgui::quit_gui();

    return true;
}
