/*
gpe_editor_settings.cpp
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

#include "gpe_editor_settings.h"

gpe_editor_settings_resource * main_editor_settings = nullptr;

theme_holder::theme_holder( std::string tName, std::string tFileName, bool isCustomTheme )
{
    theme_name = tName;
    templateFileName = tFileName;
    nameInEditor = new pawgui::widget_input_text(tName,"");
    themeIconImg = new pawgui::widget_label_image("","");
    theme_bg_location = new pawgui::widget_input_text("","");
    non_default_theme = isCustomTheme;
}

theme_holder::~theme_holder()
{
    clear_colors();
    if( nameInEditor!=nullptr )
    {
        delete nameInEditor;
        nameInEditor = nullptr;
    }
    if( themeIconImg!=nullptr )
    {
        delete themeIconImg;
        themeIconImg = nullptr;
    }
    if( theme_bg_location!=nullptr )
    {
        delete theme_bg_location;
        theme_bg_location = nullptr;
    }
}

void theme_holder::clear_colors()
{
    pawgui::gpe_widget_color_picker * tColor = nullptr;
    for( int i = (int)colorsInEditor.size()-1; i >=0; i-- )
    {
        tColor = colorsInEditor[i];
        if( tColor!=nullptr )
        {
            delete tColor;
            tColor = nullptr;
        }
    }
    colorsInEditor.clear();
}


bool theme_holder::copy_theme( pawgui::gui_theme * systemTheme, bool copyToSelf )
{
    if( systemTheme == nullptr)
    {
        gpe::error_log->report("Unable to find system theme...");
        return false;
    }
    gpe::color * fThemeColor = nullptr;
    pawgui::gpe_widget_color_picker * myColorInEditor = nullptr;
    int jThemeColor = 0;
    int jThemeColorMax = 0;

    if( copyToSelf )
    {
        jThemeColorMax = systemTheme->get_color_count();
        clear_colors();
        non_default_theme = systemTheme->is_custom_theme();
        load_background( systemTheme->theme_bg_location );
    }
    else
    {
        jThemeColorMax = (int)colorsInEditor.size();
        systemTheme->load_background( templateBgFileName );
    }

    if( jThemeColorMax == 0)
    {
        return false;
    }

    for(  jThemeColor = 0; jThemeColor < jThemeColorMax; jThemeColor++ )
    {
        if(copyToSelf )
        {
            fThemeColor = systemTheme->get_color( jThemeColor );
            if( fThemeColor !=nullptr )
            {
                myColorInEditor = new pawgui::gpe_widget_color_picker(fThemeColor->get_name(),"" );
                myColorInEditor->set_color_from_rgb( fThemeColor );
                myColorInEditor->set_label( fThemeColor->get_name() );
                myColorInEditor->set_width( 512 );
                colorsInEditor.push_back( myColorInEditor );
            }
            else
            {
                gpe::error_log->report("Unable to find color...");
            }
        }
        else
        {
            //modifies the actual template
            myColorInEditor = colorsInEditor[jThemeColor];
            if( myColorInEditor!=nullptr )
            {
                fThemeColor = systemTheme->find_color( myColorInEditor->get_name() );
                if( fThemeColor!=nullptr)
                {
                    fThemeColor->change_rgba( myColorInEditor->get_r(), myColorInEditor->get_g(), myColorInEditor->get_b(), 255 );
                }
                else
                {
                    gpe::error_log->report("Unable to find color...");
                }
            }
        }
    }
    return true;
}

bool theme_holder::load_background( std::string bg_textureLocation, bool allOrNothing)
{
    std::string miniBGFileName = "";
    if( sff_ex::file_exists(bg_textureLocation ) && stg_ex::file_is_image(bg_textureLocation) )
    {
        if(themeIconImg!=nullptr)
        {
            themeIconImg->load_label_image( bg_textureLocation );
            themeIconImg->set_height(64);
        }
        miniBGFileName = templateBgFileName = stg_ex::get_local_from_global_file( bg_textureLocation  );

        if( non_default_theme )
        {
            miniBGFileName = gpe::app_directory_name+"themes/custom/"+miniBGFileName;
        }
        else
        {
            miniBGFileName = gpe::app_directory_name+"themes/"+miniBGFileName;
        }
        sff_ex::file_copy( bg_textureLocation, miniBGFileName );
        templateBgFileName = miniBGFileName;
        theme_bg_location->set_string( miniBGFileName );
        return true;
    }
    else if( allOrNothing ==false )
    {
        // in case the file is loaded elsewhere, we see if we can find it on the local themes
        bg_textureLocation = stg_ex::get_local_from_global_file(bg_textureLocation);
        if( non_default_theme )
        {
            miniBGFileName = gpe::app_directory_name+"themes/custom/"+bg_textureLocation;
        }
        else
        {
            miniBGFileName = gpe::app_directory_name+"themes/"+bg_textureLocation;
        }
        if( sff_ex::file_exists(miniBGFileName ) && stg_ex::file_is_image(miniBGFileName) )
        {
            //performs "guaranteed recursion after finding local file"
            return load_background( miniBGFileName, true );
        }
    }
    return false;
}

void theme_holder::remove_background()
{
    if( themeIconImg !=nullptr )
    {
        themeIconImg->change_texture( nullptr );
    }
    templateBgFileName = "";
    theme_bg_location->set_string( "" );
}

gpe_editor_settings_resource::gpe_editor_settings_resource()
{
    current_editor_view_mode = gpe::render_mode_2D;

    confirmResource_button = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/check.png","Confirm Changes","Confirm and Save Changes");
    cancelResource_button =  new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/times.png","Cancel Changes","Cancel Changes and Revert to previous settings");

    isFullScreenResource = true;

    resourceFileName = "";
    resource_name = "Editor Settings";
    parentProjectName = "";
    parentProjectDirectory = "";

    sideAreaPanel = new pawgui::widget_selectbox("Mode");
    sideAreaPanel->set_width(256);
    sideAreaPanel->set_option_height(32);
    sideAreaPanel->add_option("General",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cubes.png"),nullptr,2, false, false);
    sideAreaPanel->add_option("Dock Settings",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/columns.png"),nullptr,2, false, false);
    sideAreaPanel->add_option("External Editors",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/external-link-square.png"),nullptr,2, false, false);
    sideAreaPanel->add_option("Coding Languages",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/code-fork.png"),nullptr,2, false, false);
    sideAreaPanel->add_option("Themes",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/eye.png"),nullptr,2, false, false);
    sideAreaPanel->add_option("Login",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/users.png"),nullptr,2, false, false);
    sideAreaPanel->add_option("Advanced",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/stethoscope.png"),nullptr,2, false, false);

    sidePanelRect = new gpe::shape_rect();

    CodingLanguageTitle = new pawgui::widget_label_title("Coding Language Editor","Modify how the editor reacts to your language" );
    CodingLanguageLabel = new pawgui::widget_label_text ("Basic language syntax");
    CodingLanguageParagraph = new pawgui::widget_label_paragraph("Coding Language","Unsure how to add a new language to the editor? Click the link below to add a new language. Please note to use new languages you may need to reset the editor.");
    CodingLanguageURL = new pawgui::widget_text_url("See our online guide for editing languages","Opens in browser","https://docs.pawbyte.com/");
    codingLanguageSelector = new pawgui::widget_selectbox("Coding Languages");
    codingLanguageSelector->set_option_height( 32 );
    codingLanguageSelector->set_width( 128 );

    codingLanguageSelector->add_option("JavaScript",pawgui::program_language_js );
    codingLanguageSelector->add_option("C++",pawgui::program_language_cpp );
    codingLanguageSelector->add_option("C#",pawgui::program_language_cs );
    codingLanguageSelector->add_option("TypeScript",pawgui::program_language_tst );
    codingLanguageSelector->add_option("Haxe",pawgui::program_language_haxe );
    codingLanguageSelector->add_option("Lua",pawgui::program_language_lua );

    openEditorFolder = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder-open.png","Open Editor Folder...","Be very super careful!");
    openLocalSettingsFolder = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder-open.png","Open Settings Folder...","Be very super careful!");

    userInvertMouseScroll = new pawgui::widget_checkbox("Invert Mouse Scroll","Inverts the direction the mouse scrolls in", false );
    autoSaveScreenshots = new pawgui::widget_checkbox("Save screenshot on Print-Screen","Saves PrintScreen images to Screenshots Folder", true);
    makeMetaScreenshots = new pawgui::widget_checkbox("Add Project Meta to Screenshots","Adds Project Name to screenshots", true);
    showShortProjectNames = new pawgui::widget_checkbox("Show Shorter Project Names","Show Shorter Project Names[Recommended]", true);

    externalEditorsTitle = new pawgui::widget_label_title("External Editors","" );
    for( int i = 0; i < GPE_EXTERNAL_EDITOR_MAX; i++)
    {
        pencilExternalEditorsFile[i] = new pawgui::widget_input_text("","");
        pencilExternalEditorsLoad_button[i] = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/binoculars.png","Find Editor..",-1,32);
    }
    pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->set_label("Image Editor");
    pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_AUD]->set_label("Audio Editor");
    pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_VID]->set_label("Video Editor");
    pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_FONT]->set_label("Font Editor");

    showStatupTipsBox = new pawgui::widget_checkbox("Show Tips at Startup","Leave checked to automatically launch Tips at StartUp", true);
    launchLastProjectBox = new pawgui::widget_checkbox("Open last project on startup","Open last project on startup", true);
    editorStatupTipsLabel = new pawgui::widget_label_title("Startup  Settings","Startup Settings");
    editorGui_settingsLabel = new pawgui::widget_label_title("Editor & Gui Settings","Editor & Gui Settings");

    editorNormalDelayTime = new pawgui::widget_input_number("Valid from 15 to 1000",true, 15, 1000);
    editorNormalDelayTime->set_label("User Input Delay Time: (15-1000 ms)[100 Recommended]");
    editorNormalDelayTime->set_number( pawgui::main_settings->normalInputDelayTime );

    editorTextInputDelayTime = new pawgui::widget_input_number("Valid from 15 to 1000",true, 15,1000);
    editorTextInputDelayTime->set_label("Input Field Delay Time: (15-1000 ms)[50 Recommended]");
    editorTextInputDelayTime->set_number( pawgui::main_settings->textInputDelayTime );


    editorTextAreaDelayTime = new pawgui::widget_input_number("Valid from 15 to 1000",true, 15, 1000);
    editorTextAreaDelayTime->set_label("Text Area KeyDelay Time: (15-1000 ms)[50 Recommended]");
    editorTextAreaDelayTime->set_number( pawgui::main_settings->textAreaDelayTime );

    editorCursorBlinkDelayTime = new pawgui::widget_input_number("Valid from 15 to 6000",true, 15, 6000);
    editorCursorBlinkDelayTime->set_label("Cursor Blink Time: (15-6000 ms)[300 Recommended]");
    editorCursorBlinkDelayTime->set_number( pawgui::main_settings->cursorBlinkTime );

    advancedAreaLabel = new pawgui::widget_label_title("Advanced","Advanged Editor Settings");
    forceFrameRedraw = new pawgui::widget_checkbox("Redraw every frame[Not reccommended]","Redraws the Editor every frame regardless of user input", false);
    showHiddenFilesInBrowser = new pawgui::widget_checkbox("Show Hidden Files in Browser","(Linux only feature", true);
    clearCache_button = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/close.png","Clear Font Cache","Clears the cache of pre-rendered texts");
    ide_settingsFPSRateLabel = new pawgui::widget_label_title("IDE Frame Rate","Sets the (max) framerate set for the IDE(Usually 30 or 60)");
    ideFPSRatioLabel = new pawgui::widget_label_text ( stg_ex::float_to_string( gpe::fps_ratio*100)+"%","The FPS Ratio compared to 30 FPS");
    ide_settingsFPSRate = new pawgui::widget_dropdown_menu( "FPS Cap",true);
    ide_settingsFPSRate->add_menu_option("20 FPS","",20);
    ide_settingsFPSRate->add_menu_option("24 FPS","",24);
    ide_settingsFPSRate->add_menu_option("25 FPS","",25);
    ide_settingsFPSRate->add_menu_option("30 FPS","",30,true);
    ide_settingsFPSRate->add_menu_option("45 FPS","",45);
    ide_settingsFPSRate->add_menu_option("48 FPS","",48);
    ide_settingsFPSRate->add_menu_option("50 FPS","",50);
    ide_settingsFPSRate->add_menu_option("55 FPS","",55);
    ide_settingsFPSRate->add_menu_option("58 FPS","",58);
    ide_settingsFPSRate->add_menu_option("59 FPS","",59);
    ide_settingsFPSRate->add_menu_option("60 FPS","",60);
    ide_settingsFPSRate->add_menu_option("72 FPS","",72);
    ide_settingsFPSRate->add_menu_option("88 FPS","",88);
    ide_settingsFPSRate->add_menu_option("89 FPS","",89);
    ide_settingsFPSRate->add_menu_option("90 FPS","",90);
    ide_settingsFPSRate->add_menu_option("100 FPS","",100);
    ide_settingsFPSRate->add_menu_option("125 FPS","",125);
    ide_settingsFPSRate->add_menu_option("150 FPS","",150);
    ide_settingsFPSRate->add_menu_option("200 FPS","",200);
    ide_settingsFPSRate->add_menu_option("225 FPS","",225);
    ide_settingsFPSRate->add_menu_option("250 FPS","",250);
    ide_settingsFPSRate->add_menu_option("275 FPS","",275);
    ide_settingsFPSRate->add_menu_option("300 FPS","",300);
    ide_settingsFPSRate->add_menu_option("400 FPS","",400);
    ide_settingsFPSRate->add_menu_option("500 FPS","",500);
    ide_settingsFPSRate->add_menu_option("600 FPS","",600);
    ide_settingsFPSRate->add_menu_option("700 FPS","",700);
    ide_settingsFPSRate->add_menu_option("800 FPS","",800);
    ide_settingsFPSRate->add_menu_option("900 FPS","",900);
    ide_settingsFPSRate->add_menu_option("1000 FPS","",1000);
    showFPSOnEditor = new pawgui::widget_checkbox("Show FPS Counter","Show FPS Counter", gpe::fps_show_counter );
    vsyncEditor = new pawgui::widget_checkbox("VSync On","VSync On", gpe::fps_show_counter );

    ide_buttonBarSizeLabel= new pawgui::widget_label_text ("Top Buttonbar Size","Changes the size of the buttonbar");
    ide_buttonBarSize = new pawgui::widget_dropdown_menu( "Top Buttonbar Size",true);
    ide_buttonBarSize->add_menu_option("Small","",24);
    ide_buttonBarSize->add_menu_option("Medium","",32);
    ide_buttonBarSize->add_menu_option("Large","",48);
    ide_buttonBarAlignment = new pawgui::widget_radio_button_controller("Top Buttonbar Alignment");
    ide_buttonBarAlignment->add_menu_option("Left","Aligns to the left size of window",0,true);
    ide_buttonBarAlignment->add_menu_option("Center","Aligns to the center size of window",1,false);
    ide_buttonBarAlignment->add_menu_option("Right","Aligns to the right size of window",2,false);
    projectAutoSaveRateLabel = new pawgui::widget_label_text ("Auto Save Rate","Sets how often the editor auto-saves your projects");
    projectAutoSaveRate = new pawgui::widget_dropdown_menu( "Auto Save Rate",true);
    projectAutoSaveRate->add_menu_option("Every 5 minutes","",5);
    projectAutoSaveRate->add_menu_option("Every 10 minutes","",10);
    projectAutoSaveRate->add_menu_option("Every 15 minutes","",15);
    projectAutoSaveRate->add_menu_option("Every 20 minutes","",20);
    projectAutoSaveRate->add_menu_option("Every 25 minutes","",25,true);
    projectAutoSaveRate->add_menu_option("Every 30 minutes","",30);
    projectAutoSaveRate->add_menu_option("Every 45 minutes","",45);
    projectAutoSaveRate->add_menu_option("Hourly Saves","",60);
    projectAutoSaveRate->add_menu_option("Every 2 hours","",120);
    projectAutoSaveRate->add_menu_option("Every 3 hours","",180);
    projectAutoSaveRate->add_menu_option("Every 4 hours","",240);
    projectAutoSaveRate->add_menu_option("Every 5 hours","",300);
    projectAutoSaveRate->add_menu_option("Every 6 hours","",480);
    projectAutoSaveRate->add_menu_option("Every 10 hours","",600);
    projectAutoSaveRate->add_menu_option("Every 12 hours","",720);
    projectAutoSaveRate->add_menu_option("Disabled","",-1);

    renderSceneBGColor = new pawgui::widget_checkbox("Preview Scene Background Color","Use to display scene bg color instead of transparent tiles in scene editor", false);

    //Dock _settings [ start ]
    dock_settingsLabel = new pawgui::widget_label_title( "Dock Settings", "Dock Settings" );
    dockDefaultPanel = new pawgui::widget_dropdown_menu("dock_default_panel" , true );

    //Dock _settings [ end ]

    //Login Section [ BEGIN ]
    assetLibTitleLabel = new pawgui::widget_label_text("Game Pencil Asset Library Account", "Game Pencil Asset Library Account");

    assetLibUserName = new pawgui::widget_input_text("Username", "");
    assetLibUserName->set_label("Username");
    assetLibPassword = new pawgui::widget_input_text("Password", "");
    assetLibPassword->set_label("Password");
    assetLibPassword->inputIsPassword = true;
    showLibPasswordText = new pawgui::widget_checkbox("Show Password","Show Password", true );
    assetLibSignInButton = new pawgui::widget_button_push("","Sign In", "Sign into Asset Library");
    assetLibSignInButton = new pawgui::widget_button_push("","Sign In", "Sign into Asset Library");
    assetLibSignOutButton = new pawgui::widget_button_push("","Sign Out", "Sign Out...");
    assetLibSignUpURL = new pawgui::widget_text_url("Create Account","Will open web page","https://gamepencil.net/signup");
    //Login Section [ END ]

    //Themes Section
    currentThemeInEdit = 0;
    themesLabel = new pawgui::widget_label_title("Themes");
    themeSystemDescription = new pawgui::widget_label_paragraph("System Based Theme( WIP)","This is an emulation of your current operating system's theme. As such it is not modifiable in this editor.", "System Based Theme(WIP)");
    themeBgLabel = new pawgui::widget_label_text ("Theme Background:    ");
    themeBgBrowse_button = new pawgui::widget_button_label( "Browse...","Browse for theme background...");
    themeBgRemove_button = new pawgui::widget_button_label( "Remove Background","Removes background image from theme");
    themePicker = new pawgui::widget_dropdown_menu("Themes", true );
    themeLoad_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder.png","Load Custom Theme");
    themeAdd_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/plus.png","Create Custom Theme");
    themeRemove_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/minus.png","Remove Custom Theme");
    themeSetDefault_button = new pawgui::widget_button_label( "Set as default theme","Set as default theme");
    itchLinkForThemes = new pawgui::widget_text_url("Find Themes on Itch.io","Browse the GPE Themes category on itch.io","https://itch.io/game-assets/tag-gamepencil/tag-theme");
    defaultTemplateEndPos = (int)editor_theme_controller->theme_list.size();

    theme_holder * newLoadedTheme = nullptr;
    pawgui::gui_theme * currentThemeBeingLoaded = nullptr;
    for( int iTheme = 0; iTheme < defaultTemplateEndPos; iTheme++ )
    {
        currentThemeBeingLoaded = editor_theme_controller->theme_list[iTheme];
        if( currentThemeBeingLoaded !=nullptr )
        {
            newLoadedTheme = new theme_holder( currentThemeBeingLoaded->theme_name, currentThemeBeingLoaded->theme_local_location );

            themePicker->add_menu_option( currentThemeBeingLoaded->theme_name,currentThemeBeingLoaded->theme_local_location, (int)themeHolders.size(), (iTheme==0) );
            newLoadedTheme->copy_theme( currentThemeBeingLoaded );
            themeHolders.push_back( newLoadedTheme );
        }
    }
    load_themes_from_folder( gpe::app_directory_name+"themes");
    load_themes_from_folder( gpe::app_directory_name+"themes/custom");
    //Tab Section
    tabSectionLabel = new pawgui::widget_label_title("Tabs");
    mouseAutoFindTabs = new pawgui::widget_checkbox("Mouse detect tabs");
    tabSpaceSize = new pawgui::widget_input_number("Range: 1 to 16",true,1,16);
    tabSpaceSize->set_number(4);
    tabSpaceSize->set_label("Tab SIZE in spaces:");
    pawgui::main_settings->tabSpaceCount = 4;

    editorPageList = new pawgui::widget_panel_list();
    editorPageList->panelAlignType = pawgui::panel_align_left;
    editorPageList->barXPadding = pawgui::padding_default;
    editorPageList->barXMargin = 0;

    subViewedSpace.x = 0;
    subViewedSpace.y = 0;
    subViewedSpace.w = gpe::screen_width;
    subViewedSpace.h = gpe::screen_height;

    subEditorSelectBox = nullptr; //Now stop crashing until I add your feature! :-)

}

gpe_editor_settings_resource::~gpe_editor_settings_resource()
{
    if( openEditorFolder!=nullptr)
    {
        delete openEditorFolder;
        openEditorFolder = nullptr;
    }
    if( openLocalSettingsFolder!=nullptr)
    {
        delete openLocalSettingsFolder;
        openLocalSettingsFolder = nullptr;
    }
    if( themeSetDefault_button!=nullptr)
    {
        delete themeSetDefault_button;
        themeSetDefault_button = nullptr;
    }

    if( themeBgLabel!=nullptr)
    {
        delete themeBgLabel;
        themeBgLabel = nullptr;
    }

    if( themeBgBrowse_button!=nullptr)
    {
        delete themeBgBrowse_button;
        themeBgBrowse_button = nullptr;
    }

    if( themeBgRemove_button!=nullptr)
    {
        delete themeBgRemove_button;
        themeBgRemove_button = nullptr;
    }

    if( itchLinkForThemes!=nullptr)
    {
        delete itchLinkForThemes;
        itchLinkForThemes = nullptr;
    }

    if( editorPageList!=nullptr)
    {
        editorPageList->clear_list();
        delete editorPageList;
        editorPageList = nullptr;
    }

    if( editorStatupTipsLabel!=nullptr)
    {
        delete editorStatupTipsLabel;
        editorStatupTipsLabel = nullptr;
    }

    if( clearCache_button!=nullptr)
    {
        delete clearCache_button;
        clearCache_button = nullptr;
    }

    if( ide_settingsFPSRate!=nullptr)
    {
        delete ide_settingsFPSRate;
        ide_settingsFPSRate = nullptr;
    }

    if( ide_settingsFPSRateLabel!=nullptr)
    {
        delete ide_settingsFPSRateLabel;
        ide_settingsFPSRateLabel = nullptr;
    }
    if( ideFPSRatioLabel!=nullptr)
    {
        delete ideFPSRatioLabel;
        ideFPSRatioLabel = nullptr;
    }

    if( ide_buttonBarSize!=nullptr)
    {
        delete ide_buttonBarSize;
        ide_buttonBarSize = nullptr;
    }
    if( ide_buttonBarAlignment!=nullptr)
    {
        delete ide_buttonBarAlignment;
        ide_buttonBarAlignment = nullptr;
    }

    if( ide_buttonBarSizeLabel!=nullptr)
    {
        delete ide_buttonBarSizeLabel;
        ide_buttonBarSizeLabel = nullptr;
    }

    if( editorGui_settingsLabel!=nullptr)
    {
        delete editorGui_settingsLabel;
        editorGui_settingsLabel = nullptr;
    }

    if( editorTextAreaDelayTime!=nullptr)
    {
        delete editorTextAreaDelayTime;
        editorTextAreaDelayTime = nullptr;
    }
    if( editorTextInputDelayTime!=nullptr)
    {
        delete editorTextInputDelayTime;
        editorTextInputDelayTime = nullptr;
    }
    if( editorNormalDelayTime!=nullptr)
    {
        delete editorNormalDelayTime;
        editorNormalDelayTime = nullptr;
    }

    if( advancedAreaLabel!=nullptr)
    {
        delete advancedAreaLabel;
        advancedAreaLabel = nullptr;
    }

    if( showHiddenFilesInBrowser!=nullptr)
    {
        delete showHiddenFilesInBrowser;
        showHiddenFilesInBrowser = nullptr;
    }
    if( forceFrameRedraw!=nullptr)
    {
        delete forceFrameRedraw;
        forceFrameRedraw = nullptr;
    }

    if( subEditorSelectBox!=nullptr)
    {
        delete subEditorSelectBox;
        subEditorSelectBox = nullptr;
    }
    if( renderSceneBGColor!=nullptr)
    {
        delete renderSceneBGColor;
        renderSceneBGColor = nullptr;
    }

    if( dock_settingsLabel!=nullptr )
    {
        delete dock_settingsLabel;
        dock_settingsLabel = nullptr;
    }
    if( dockDefaultPanel!=nullptr )
    {
        delete dockDefaultPanel;
        dockDefaultPanel = nullptr;
    }
    pawgui::widget_input_text * tempInput = nullptr;
    pawgui::widget_button_icon * tempIcon = nullptr;
    if( externalEditorsTitle!=nullptr)
    {
        delete externalEditorsTitle;
        externalEditorsTitle = nullptr;
    }

    for( int i =GPE_EXTERNAL_EDITOR_MAX-1; i>=0; i--)
    {
        if( pencilExternalEditorsFile[i]!=nullptr)
        {
            tempInput = pencilExternalEditorsFile[i];
            if( tempInput!=nullptr)
            {
                delete tempInput;
                tempInput = nullptr;
            }
            pencilExternalEditorsFile[i] = nullptr;
        }
        if( pencilExternalEditorsLoad_button[i]!=nullptr)
        {
            tempIcon = pencilExternalEditorsLoad_button[i];
            if( tempIcon!=nullptr)
            {
                delete tempIcon;
                tempIcon = nullptr;
            }
            pencilExternalEditorsLoad_button[i] = nullptr;
        }
    }
}

bool gpe_editor_settings_resource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}

void gpe_editor_settings_resource::load_themes_from_folder( std::string themeFolder )
{
    //Used for searching directories
    int iFile;
    std::string file_nameDiscovered = "";
    gpe::file_directory_class * dir = new gpe::file_directory_class();
    gpe::file_object * file = nullptr;
    theme_holder * newLoadedTheme;
    pawgui::gui_theme * newSystemTheme;
    if( dir!=nullptr)
    {
        dir->open_directory(themeFolder);
        for (iFile = 0; iFile < (int)dir->get_count(); iFile++)
        {
            file = dir->get_file(iFile);
            if( file!=nullptr)
            {
                file_nameDiscovered = file->get_name();
                if( file_nameDiscovered!="." && file_nameDiscovered!="..")
                {
                    if( file->is_directory()==false && file->get_type()=="gpf" )
                    {
                        newSystemTheme = editor_theme_controller->add_theme( file_nameDiscovered, true );
                        newSystemTheme->load_theme( themeFolder+"/"+file_nameDiscovered );
                        newLoadedTheme = new theme_holder( newSystemTheme->theme_name, newSystemTheme->theme_local_location );
                        themePicker->add_menu_option( newSystemTheme->theme_name,newSystemTheme->theme_local_location, (int)themeHolders.size(), false );
                        newLoadedTheme->copy_theme(newSystemTheme );
                        themeHolders.push_back( newLoadedTheme );
                    }
                }
            }
        }
        delete dir;
        dir = nullptr;
    }
}

void gpe_editor_settings_resource::prerender_self( )
{

}

void gpe_editor_settings_resource::load_resource(std::string file_path)
{
    //showStatupTipsBox->set_clicked( editor_gui_main->showTipsAtStartUp );
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Loading Editor Settings", "Please wait..." );
    }

    std::string otherColContainerName = "";

    std::string newFileIn =  gpe::get_user_settings_folder()+"gpe_ide_local_settings.txt";
    std::ifstream gameResourceFileIn( newFileIn.c_str() );

    gpe::error_log->report("Loading Local settings - "+newFileIn);
    pawgui::gui_theme * foundDefaultTheme = nullptr;
    //If the level file could be loaded
    if( !gameResourceFileIn.fail() )
    {
        //makes sure the file is open
        if (gameResourceFileIn.is_open())
        {
            int equalPos = 0;
            std::string firstChar="";
            std::string key_string="";
            std::string valstring="";
            std::string subValstring="";
            std::string currLine="";
            std::string currLineToBeProcessed;
            std::string colortheme_name;
            float foundFileVersion = 0;
            int foundDelayTime = 0;
            int foundFPSValue = gpe::fps_cap;
            while ( gameResourceFileIn.good() )
            {
                getline (gameResourceFileIn,currLine); //gets the next line of the file
                currLineToBeProcessed = stg_ex::trim_left_inplace(currLine);
                currLineToBeProcessed = stg_ex::trim_right_inplace(currLineToBeProcessed);

                if( foundFileVersion <=0)
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
                                key_string = currLineToBeProcessed.substr(0,equalPos);
                                valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                if( key_string=="Version")
                                {
                                    foundFileVersion = stg_ex::string_to_float(valstring);
                                }
                            }
                        }
                    }
                }
                else if( foundFileVersion <= 2)
                {
                    //Begin processing the file.
                    if(!currLineToBeProcessed.empty() )
                    {
                        equalPos=currLineToBeProcessed.find_first_of("=");
                        if(equalPos!=(int)std::string::npos)
                        {
                            //if the equalPos is present, then parse on through and carryon
                            key_string = currLineToBeProcessed.substr(0,equalPos);
                            valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                            //gpe::error_log->report("Detected Key:     "+key_string);
                            if( key_string=="DefaultTheme" && editor_theme_controller!=nullptr )
                            {
                                foundDefaultTheme = editor_theme_controller->find_theme( valstring);
                                if( foundDefaultTheme!=nullptr )
                                {
                                    pawgui::theme_main = foundDefaultTheme;
                                }
                            }
                            if( key_string=="ExternalImageEditor")
                            {
                                pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->set_string(valstring);
                            }
                            else if( key_string=="ExternalAudioEditor")
                            {
                                pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_AUD]->set_string(valstring);
                            }
                            else if( key_string=="ExternalVideoEditor")
                            {
                                pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_VID]->set_string(valstring);
                            }
                            else if( key_string=="ExternalFontEditor")
                            {
                                pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_FONT]->set_string(valstring);
                            }
                            else if( key_string=="AutoSaveScreenshots")
                            {
                                autoSaveScreenshots->set_clicked( stg_ex::string_to_bool(valstring ) );
                            }
                            else if( key_string=="InvertMouseScroll")
                            {
                                userInvertMouseScroll->set_clicked( stg_ex::string_to_bool(valstring ) );
                                gpe::input->mouse_scroll_inverted = userInvertMouseScroll->is_clicked();
                            }
                            else if( key_string=="PreviewSceneBGColor")
                            {
                                renderSceneBGColor->set_clicked( stg_ex::string_to_bool(valstring ) );
                            }
                            else if( key_string=="MakeMetaScreenshots")
                            {
                                makeMetaScreenshots->set_clicked( stg_ex::string_to_bool(valstring ) );
                            }
                            else if( key_string=="ShowShortProjectNames")
                            {
                                pawgui::main_settings->useShortProjectNames = stg_ex::string_to_bool(valstring );
                                showShortProjectNames->set_clicked(  pawgui::main_settings->useShortProjectNames );
                            }
                            else if( key_string=="LaunchLastProject")
                            {
                                launchLastProjectBox->set_clicked( stg_ex::string_to_bool(valstring ) );
                            }
                            else if( key_string=="ShowFPSCounter")
                            {
                                gpe::fps_show_counter = stg_ex::string_to_bool(valstring);
                                showFPSOnEditor->set_clicked( gpe::fps_show_counter );
                            }
                            else if( key_string=="ShowTipsAtStart")
                            {
                                showStatupTipsBox->set_clicked( stg_ex::string_to_bool(valstring ) );
                            }
                            else if( key_string=="BrowseHiddenFiles")
                            {
                                showHiddenFilesInBrowser->set_clicked( stg_ex::string_to_bool(valstring ) );
                            }
                            else if( key_string=="ProjectFolderList" )
                            {
                                projectFolderListLocation = valstring;
                            }
                            else if( key_string=="NormalDelayMS")
                            {
                                foundDelayTime = stg_ex::string_to_int(valstring, 60 );
                                if( foundDelayTime <= 15)
                                {
                                    foundDelayTime = 15;
                                }
                                else if( foundDelayTime > 1000)
                                {
                                    foundDelayTime = 1000;
                                }
                                pawgui::main_settings->normalInputDelayTime = foundDelayTime;
                                editorNormalDelayTime->set_number( foundDelayTime );
                            }
                            else if( key_string=="InputDelayMS")
                            {
                                foundDelayTime = stg_ex::string_to_int(valstring, 30 );
                                if( foundDelayTime <= 15 )
                                {
                                    foundDelayTime = 15;
                                }
                                else if( foundDelayTime > 1000)
                                {
                                    foundDelayTime = 1000;
                                }
                                pawgui::main_settings->textInputDelayTime = foundDelayTime;
                                editorTextInputDelayTime->set_number( foundDelayTime );
                            }
                            else if( key_string=="TextAreaDelayMS")
                            {
                                foundDelayTime = stg_ex::string_to_int(valstring, 30 );
                                if( foundDelayTime <= 15 )
                                {
                                    foundDelayTime = 15;
                                }
                                else if( foundDelayTime > 1000 )
                                {
                                    foundDelayTime = 1000;
                                }
                                pawgui::main_settings->textAreaDelayTime = foundDelayTime;
                                editorTextAreaDelayTime->set_number( foundDelayTime );
                            }
                            else if( key_string=="CursorBlinkDelayMS")
                            {
                                foundDelayTime = stg_ex::string_to_int(valstring, 300 );
                                if( foundDelayTime <= 15 )
                                {
                                    foundDelayTime = 15;
                                }
                                else if( foundDelayTime > 6000 )
                                {
                                    foundDelayTime = 6000;
                                }
                                pawgui::main_settings->cursorBlinkTime = foundDelayTime;
                                editorCursorBlinkDelayTime->set_number( foundDelayTime );
                            }
                            else if( key_string=="FPS")
                            {
                                foundFPSValue = stg_ex::string_to_int(valstring,30);
                                foundFPSValue = std::max( foundFPSValue, 30);
                                foundFPSValue = std::min( foundFPSValue, 300);
                                gpe::time_keeper->set_fps(foundFPSValue);
                                ide_settingsFPSRate->set_option_value(foundFPSValue);
                            }
                            else if( key_string=="VSync")
                            {
                                vsyncEditor->set_clicked( stg_ex::string_to_bool(valstring ) );
                                gpe::time_keeper->set_fps( vsyncEditor->is_clicked() );

                            }
                            else if( key_string=="_buttonBarSize")
                            {
                                ide_buttonBarSize->set_option_value( stg_ex::string_to_int( valstring) );
                            }
                            else if( key_string=="_buttonBarAlignment")
                            {
                                ide_buttonBarAlignment->set_selection( stg_ex::string_to_int( valstring) );
                            }
                        }
                    }
                }
                else
                {
                    gpe::error_log->report("Invalid FoundFileVersion ="+ stg_ex::float_to_string(foundFileVersion)+".");
                }
            }
            //Sets default value based on new found current theme
            if( pawgui::theme_main!=nullptr && themePicker!=nullptr )
            {
                themePicker->set_option_named( pawgui::theme_main->theme_name );
            }
        }
    }
}

void gpe_editor_settings_resource::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
{
    cam = gpe::camera_find(cam);
    view_space = gpe::camera_find(view_space);
    if( cam!=nullptr && editorPageList!=nullptr && sideAreaPanel!=nullptr && view_space!=nullptr)
    {
        int prevTab = sideAreaPanel->get_selection();
        if( panel_main_editor!=nullptr )
        {
            subViewedSpace.x = 0;
            subViewedSpace.y = 0;
            subViewedSpace.w = view_space->w;
            subViewedSpace.h = view_space->h;
            panel_main_editor->add_gui_element_fullsize( sideAreaPanel );
            panel_main_editor->process_self();

        }
        else
        {
            sideAreaPanel->set_coords(0, 0 );
            sideAreaPanel->set_height( view_space->h );
            sideAreaPanel->process_self( view_space, cam );
            subViewedSpace.x = sideAreaPanel->get_x2();
            subViewedSpace.y = 0;
            subViewedSpace.w = view_space->w - sideAreaPanel->get_width();
            subViewedSpace.h = view_space->h;
        }

        if( prevTab!=sideAreaPanel->get_selection() )
        {
            editorPageList->reset_self();
        }

        editorPageList->clear_list();
        editorPageList->set_coords( subViewedSpace.x, subViewedSpace.y );
        editorPageList->set_width(subViewedSpace.w);
        editorPageList->set_height(subViewedSpace.h );
        editorPageList->barXPadding = pawgui::padding_default;
        editorPageList->barXMargin = pawgui::padding_default;
        editorPageList->alignment_h = gpe::fa_center;
        int i = 0;
        theme_holder * currentThemeBeingEdited = nullptr;
        int iColor = 0, iThemeColorMax = 0;
        if(sideAreaPanel->get_selected_name()=="General")
        {
            //editorPageList->add_gui_element(allowAutoSaveBox,true);
            editorPageList->add_gui_element(editorGui_settingsLabel,true);
            editorPageList->add_gui_element(autoSaveScreenshots,true);
            editorPageList->add_gui_element(makeMetaScreenshots,true);
            editorPageList->add_gui_element(showShortProjectNames,true);
            editorPageList->add_gui_element(renderSceneBGColor,true);
            editorPageList->add_gui_element(projectAutoSaveRateLabel,true);
            editorPageList->add_gui_element(projectAutoSaveRate,true);

            editorPageList->add_gui_element(ide_buttonBarAlignment,true);
            editorPageList->add_gui_element(ide_buttonBarSizeLabel,true);
            editorPageList->add_gui_element(ide_buttonBarSize,true);

            editorPageList->add_gui_element(ide_settingsFPSRateLabel,true);
            editorPageList->add_gui_element(ide_settingsFPSRate,true);
            editorPageList->add_gui_element(vsyncEditor,true);
            ideFPSRatioLabel->set_name("FPS Ratio: "+ stg_ex::float_to_string( gpe::fps_ratio*100)+"%");
            editorPageList->add_gui_element(ideFPSRatioLabel,true);
            editorPageList->add_gui_element(showFPSOnEditor,true);


            editorPageList->add_gui_element(editorStatupTipsLabel,true);
            editorPageList->add_gui_element(showStatupTipsBox,true);
            editorPageList->add_gui_element(launchLastProjectBox,true);

            editorPageList->add_gui_element(tabSectionLabel,true);
            editorPageList->add_gui_element(mouseAutoFindTabs,true);
            editorPageList->add_gui_element(tabSpaceSize,true);

            editorPageList->add_gui_element(confirmResource_button,false);
            editorPageList->add_gui_element(cancelResource_button,true);
        }
        else if( sideAreaPanel->get_selected_name()=="Login")
        {
            editorPageList->alignment_h = gpe::fa_center;
            editorPageList->add_gui_element(assetLibTitleLabel,true);
            editorPageList->add_gui_element(assetLibUserName,true);
            assetLibPassword->inputIsPassword = showLibPasswordText->is_clicked();
            editorPageList->add_gui_element(assetLibPassword,true);
            editorPageList->add_gui_element(showLibPasswordText,true);
            editorPageList->add_gui_element(assetLibSignInButton,false);
            editorPageList->add_gui_element(assetLibSignOutButton,true);
            editorPageList->add_gui_element(assetLibSignUpURL,true);
        }
        else if( sideAreaPanel->get_selected_name()=="Dock ")
        {
            editorPageList->alignment_h = gpe::fa_center;
            editorPageList->add_gui_element(dock_settingsLabel,true);
            editorPageList->add_gui_element(dockDefaultPanel,true);
        }
        else if( sideAreaPanel->get_selected_name()=="Colors" || sideAreaPanel->get_selected_name()=="Themes" )
        {
            editorPageList->alignment_h = gpe::fa_center;
            editorPageList->add_gui_element(themesLabel,true);

            editorPageList->add_gui_element(themeLoad_button,false);
            editorPageList->add_gui_element(themeAdd_button,false);
            editorPageList->add_gui_element(themeRemove_button,false);
            editorPageList->add_gui_element(themePicker,false);
            editorPageList->add_gui_element(themeSetDefault_button,true);
            editorPageList->add_gui_element(itchLinkForThemes,true);

            //Dynamic theme data
            if( currentThemeInEdit >=0 && currentThemeInEdit < (int)themeHolders.size() )
            {
                currentThemeBeingEdited = themeHolders[currentThemeInEdit];
            }

            //256 is an estimated width of a color picker with a label...
            /*int colorsPerLine = (subViewedSpace.w-pawgui::padding_default*4)/256;
            int colorsInLine = 0;*/
            bool goToNextColorLine = false;

            if( currentThemeBeingEdited!=nullptr )
            {
                if( themePicker->get_selected_name() == "system_based" )
                {
                    editorPageList->add_gui_element( themeSystemDescription, true  );
                }
                else
                {
                    editorPageList->add_gui_element( themeBgLabel, false );
                    editorPageList->add_gui_element( currentThemeBeingEdited->theme_bg_location, true );
                    editorPageList->add_gui_element( themeBgBrowse_button, false );
                    editorPageList->add_gui_element( themeBgRemove_button, true );
                    editorPageList->add_gui_element( currentThemeBeingEdited->themeIconImg, true );

                    iThemeColorMax =  (int)currentThemeBeingEdited->colorsInEditor.size();
                    for( iColor = 0; iColor < iThemeColorMax; iColor++ )
                    {
                        editorPageList->add_gui_element(currentThemeBeingEdited->colorsInEditor[iColor], true );
                    }
                }

            }

            editorPageList->add_gui_element(confirmResource_button,false);
            editorPageList->add_gui_element(cancelResource_button,true);
        }
        else if( sideAreaPanel->get_selected_name()=="External Editors")
        {
            editorPageList->alignment_h = gpe::fa_center;
            editorPageList->add_gui_element( externalEditorsTitle, true );
            for( i = 0; i < GPE_EXTERNAL_EDITOR_MAX; i++)
            {
                editorPageList->add_gui_element(pencilExternalEditorsFile[i],false);
                editorPageList->add_gui_element(pencilExternalEditorsLoad_button[i],true);
            }
            editorPageList->add_gui_element(confirmResource_button,true);
        }
        else if( sideAreaPanel->get_selected_name()=="Coding Languages")
        {
            codingLanguageSelector->set_coords(subViewedSpace.x, 0 );
            codingLanguageSelector->set_height(subViewedSpace.h);
            codingLanguageSelector->optionIconWidth = 0;
            codingLanguageSelector->process_self(view_space,cam);
            subViewedSpace.x+= codingLanguageSelector->get_width();
            subViewedSpace.w-= codingLanguageSelector->get_width();

            editorPageList->set_coords( subViewedSpace.x, subViewedSpace.y );
            editorPageList->set_width( subViewedSpace.w  );
            editorPageList->set_height(subViewedSpace.h );

            editorPageList->add_gui_element(CodingLanguageTitle,true);
            editorPageList->add_gui_element(CodingLanguageLabel,true);
            editorPageList->add_gui_element(CodingLanguageParagraph,true);
            editorPageList->add_gui_element(CodingLanguageURL,true);
        }
        else if( sideAreaPanel->get_selected_name()=="Advanced")
        {
            editorPageList->add_gui_element(advancedAreaLabel,true);
            editorPageList->add_gui_element(openEditorFolder,true);
            editorPageList->add_gui_element(openLocalSettingsFolder,true);
            editorPageList->add_gui_element(clearCache_button,true);
            editorPageList->add_gui_element(showHiddenFilesInBrowser,true);
            editorPageList->add_gui_element(forceFrameRedraw,true);

            editorPageList->add_gui_element(editorNormalDelayTime,true);
            editorPageList->add_gui_element(editorTextAreaDelayTime,true);
            editorPageList->add_gui_element(editorTextInputDelayTime,true);
            editorPageList->add_gui_element(editorCursorBlinkDelayTime,true);

            editorPageList->add_gui_element(userInvertMouseScroll,true);

            editorPageList->add_gui_element(confirmResource_button,false);
            editorPageList->add_gui_element(cancelResource_button,true);
        }
        else
        {
            editorPageList->clear_list();
        }

        editorPageList->process_self( view_space,cam);

        //Process results of last process...
        if( sideAreaPanel->get_selected_name()=="Colors" || sideAreaPanel->get_selected_name()=="Themes" )
        {
            bool themeChanged = false;
            if( themePicker!=nullptr )
            {
                int potentiallyselectedTheme = themePicker->get_selected_value();
                if( currentThemeInEdit!= potentiallyselectedTheme && potentiallyselectedTheme >=0 && potentiallyselectedTheme < (int)themeHolders.size() )
                {
                    currentThemeInEdit = potentiallyselectedTheme;
                    themeChanged = true;
                    /*input->reset_all_input();
                    input->input_received = true;
                    process_self( view_space, cam );
                    return;
                    */
                }
            }

            if( !themeChanged )
            {
                if( themeSetDefault_button!=nullptr && themeSetDefault_button->is_clicked() )
                {
                    if( currentThemeInEdit >=0 && currentThemeInEdit < (int)editor_theme_controller->theme_list.size() )
                    {
                        pawgui::theme_main = editor_theme_controller->theme_list[currentThemeInEdit];
                        sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_ide_local_settings.txt","DefaultTheme="+pawgui::theme_main->theme_name );

                    }
                }
                else if( themeAdd_button!=nullptr && themeAdd_button->is_clicked() )
                {
                    //Adds new theme if its a unique name
                    std::string newtheme_name = pawgui::get_string_from_popup("New Theme name","Only unique names allowed");
                    if( newtheme_name.size() > 0 && newtheme_name!="default" )
                    {
                        if( editor_theme_controller->find_theme( newtheme_name)==nullptr )
                        {
                            pawgui::gui_theme * newGPETheme = editor_theme_controller->add_theme( newtheme_name, true );
                            if( newGPETheme !=nullptr )
                            {
                                //Now adds it to our editor here...
                                theme_holder * newThemeHolder =  new theme_holder(newtheme_name, newtheme_name+".gpf" );
                                newThemeHolder->copy_theme( newGPETheme, true );
                                themePicker->add_menu_option( newtheme_name+".gpf", newtheme_name+".gpf", (int)themeHolders.size(), true );
                                themeHolders.push_back( newThemeHolder );
                                newGPETheme->save_theme();
                            }
                        }
                        else
                        {
                            pawgui::display_user_alert("Theme creation stopped","Sorry, but you must use a unique name for this theme.");
                        }
                    }
                }
                else if( currentThemeBeingEdited!=nullptr )
                {
                    if( themeBgBrowse_button!=nullptr && themeBgBrowse_button->is_clicked() )
                    {
                        if( (int)editor_gui_main->fileOpenProjectIconDir.size()==0 && current_project!=nullptr )
                        {
                            editor_gui_main->fileOpenProjectIconDir = current_project->get_project_directory();
                        }
                        std::string newThemeBGFileName = pawgui::get_filename_open_from_popup("Find Theme Background","Image",editor_gui_main->fileOpenProjectIconDir );
                        currentThemeBeingEdited->load_background( newThemeBGFileName );
                    }
                    else if( themeBgRemove_button!=nullptr && themeBgRemove_button->is_clicked() )
                    {
                        currentThemeBeingEdited->remove_background();
                    }
                }


                if( confirmResource_button!=nullptr && confirmResource_button->is_clicked() )
                {
                    if( currentThemeBeingEdited!=nullptr )
                    {
                        pawgui::gui_theme * fThemeToChange = editor_theme_controller->find_theme( currentThemeBeingEdited->theme_name );
                        if( fThemeToChange !=nullptr)
                        {
                            currentThemeBeingEdited->copy_theme( fThemeToChange, false );
                            fThemeToChange->save_theme();
                            //currentThemeBeingEdited->copy_theme( fThemeToChange, true );
                        }
                    }
                }
            }
        }
        else if(sideAreaPanel->get_selected_name()=="Advanced")
        {
            if( clearCache_button->is_clicked() )
            {
                if( pawgui::display_prompt_message("Clear Font Cache?","Are you sure you will like to clear the font cache? Current Size: "+ stg_ex::int_to_string(gpe::gfs->get_font_cache_size() ) +" textures. This operation may be time consuming",false)==pawgui::display_query_yes)
                {
                    gpe::gfs->clear_font_cache();
                }
            }
            else if( openEditorFolder->is_clicked() )
            {
                gpe::external_open_program( gpe::app_directory_name );
            }
            else if( openLocalSettingsFolder->is_clicked() )
            {
                gpe::external_open_program(  gpe::get_user_settings_folder() );
            }
        }
        else if( sideAreaPanel->get_selected_name()=="External Editors")
        {
            for( i = 0; i < GPE_EXTERNAL_EDITOR_MAX; i++)
            {
                if( pencilExternalEditorsLoad_button[i]->is_clicked() )
                {
                    std::string new_file_name = pawgui::get_filename_open_from_popup("Find External File Editor","",pawgui::main_settings->fileOpenDefaultDir);
                    if( sff_ex::file_exists(new_file_name) )
                    {
                        pencilExternalEditorsFile[i]->set_string(new_file_name);
                    }
                }
            }
        }
        else if( sideAreaPanel->get_selected_name()=="Dock Settings")
        {

        }
        if( confirmResource_button!=nullptr && confirmResource_button->is_clicked() )
        {
            gpe::fps_show_counter = showFPSOnEditor->is_clicked();
            editor_gui_main->showTipsAtStartUp = showStatupTipsBox->is_clicked();
            editor_gui_main->save_settings();

            //Re-used variable for setting delays.
            int foundDelTime = 0;

            //Sets the input field delay time
            foundDelTime = editorTextAreaDelayTime->get_held_number();
            if( foundDelTime < 15 )
            {
                foundDelTime = 15;
            }
            else if( foundDelTime > 1000 )
            {
                foundDelTime = 1000;
            }
            pawgui::main_settings->textAreaDelayTime = foundDelTime;
            editorTextAreaDelayTime->set_number(foundDelTime);

            //Sets the input field delay time
            foundDelTime = editorTextInputDelayTime->get_held_number();
            if( foundDelTime <= 15)
            {
                foundDelTime = 15;
            }
            else if( foundDelTime > 1000)
            {
                foundDelTime = 1000;
            }
            pawgui::main_settings->useShortProjectNames = showShortProjectNames->is_clicked();
            pawgui::main_settings->textInputDelayTime = foundDelTime;
            editorTextInputDelayTime->set_number( foundDelTime );

            //Sets the input delay for other elements
            foundDelTime = editorNormalDelayTime->get_held_number();
            if( foundDelTime <= 15)
            {
                foundDelTime = 15;
            }
            else if( foundDelTime > 1000)
            {
                foundDelTime = 1000;
            }

            pawgui::main_settings->normalInputDelayTime = foundDelTime;
            editorNormalDelayTime->set_number(foundDelTime );

            //Sets the cursor blink delay
            foundDelTime = editorCursorBlinkDelayTime->get_held_number();
            if( foundDelTime <= 15)
            {
                foundDelTime = 15;
            }
            else if( foundDelTime > 1000)
            {
                foundDelTime = 1000;
            }
            pawgui::main_settings->cursorBlinkTime = foundDelTime;
            editorCursorBlinkDelayTime->set_number(foundDelTime );

            gpe::input->mouse_scroll_inverted = userInvertMouseScroll->is_clicked();
            gpe::time_keeper->set_vysnc( vsyncEditor->is_clicked() );
            gpe::window_controller_main->set_vysnc( vsyncEditor->is_clicked() );
            gpe::renderer_main->set_vysnc( vsyncEditor->is_clicked() );
            save_resource();
        }

        if( cancelResource_button!=nullptr && cancelResource_button->is_clicked() )
        {
            load_resource();
            editor_gui_main->showTipsAtStartUp = showStatupTipsBox->is_clicked();
            editor_gui_main->save_settings();
        }
    }
}

void gpe_editor_settings_resource::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
{
    cam = gpe::camera_find(cam);
    view_space = gpe::camera_find(view_space);
    if( cam!=nullptr && view_space!=nullptr)
    {
        if( sideAreaPanel!=nullptr && panel_main_editor==nullptr )
        {
            sideAreaPanel->render_self( view_space,cam);
        }

        if(  codingLanguageSelector!=nullptr && sideAreaPanel!=nullptr  && sideAreaPanel->get_selected_name()=="Coding Languages" )
        {
            codingLanguageSelector->render_self( view_space,cam);
        }

        if( editorPageList!=nullptr )
        {
            editorPageList->render_self( view_space,cam);
        }
    }
}

void gpe_editor_settings_resource::save_resource(std::string file_path, int backupId)
{
    bool usingAltSaveSource = false;
    isModified = false;
    std::string newSaveDataFilename =  gpe::get_user_settings_folder()+"gpe_ide_local_settings.txt";
    std::ofstream newSaveDataFile( newSaveDataFilename.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            write_header_on_file(&newSaveDataFile);
            newSaveDataFile << "#     \n";
            newSaveDataFile << "DefaultTheme=" << pawgui::theme_main->theme_name << "\n";
            newSaveDataFile << "ExternalImageEditor=" << pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string() << "\n";
            newSaveDataFile << "ExternalAudioEditor=" << pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_AUD]->get_string() << "\n";
            newSaveDataFile << "ExternalVideoEditor=" << pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_VID]->get_string() << "\n";
            newSaveDataFile << "ExternalFontEditor=" << pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_FONT]->get_string() << "\n";

            newSaveDataFile << "AutoSaveScreenshots=" << autoSaveScreenshots->is_clicked() << "\n";
            newSaveDataFile << "InvertMouseScroll=" << userInvertMouseScroll->is_clicked() << "\n";
            newSaveDataFile << "MakeMetaScreenshots=" << makeMetaScreenshots->is_clicked() << "\n";
            newSaveDataFile << "ShowFPSCounter=" << gpe::fps_show_counter << "\n";
            newSaveDataFile << "ShowShortProjectNames=" << showShortProjectNames->is_clicked() << "\n";
            newSaveDataFile << "LaunchLastProject=" << launchLastProjectBox->is_clicked() << "\n";
            newSaveDataFile << "ShowTipsAtStart=" << showStatupTipsBox->is_clicked() << "\n";
            newSaveDataFile << "InputDelayMS=" << editorTextInputDelayTime->get_held_number() << "\n";
            newSaveDataFile << "TextAreaDelayMS=" << editorTextAreaDelayTime->get_held_number() << "\n";
            newSaveDataFile << "CursorBlinkDelayMS=" << editorCursorBlinkDelayTime->get_held_number() << "\n";
            newSaveDataFile << "NormalDelayMS=" << editorNormalDelayTime->get_held_number() << "\n";
            newSaveDataFile << "BrowseHiddenFiles=" << showHiddenFilesInBrowser->is_clicked() << "\n";
            newSaveDataFile << "PreviewSceneBGColor=" << renderSceneBGColor->is_clicked() << "\n";
            newSaveDataFile << "FPS="+ stg_ex::int_to_string( ide_settingsFPSRate->get_selected_value() ) << "\n";
            newSaveDataFile << "VSync="+ stg_ex::int_to_string( vsyncEditor->is_clicked() ) << "\n";
            newSaveDataFile << "ProjectFolderList="+projectFolderListLocation << "\n";
            newSaveDataFile << "_buttonBarSize="+ stg_ex::int_to_string( ide_buttonBarSize->get_selected_value() ) << "\n";
            newSaveDataFile << "_buttonBarAlignment="+ stg_ex::int_to_string( ide_buttonBarAlignment->get_selected_value() ) << "\n";
            newSaveDataFile << "\n";
        }
    }
    gpe::time_keeper->set_fps( ide_settingsFPSRate->get_selected_value() );
}

bool gpe_editor_settings_resource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=nullptr)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "";
            //save_resource();
            return true;
        }
    }
    return false;
}
