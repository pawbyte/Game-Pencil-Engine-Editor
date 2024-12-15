/*
project_properties.cpp
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

#include "project_properties.h"
#include "../resources/game_scene/game_scene_resource.h"

projectPropertiesResource::projectPropertiesResource(pawgui::widget_resource_container * pFolder)
{
    isFullScreenResource = false;
    for( int iLayerN = 0; iLayerN < 32; iLayerN++)
    {
        projectLayerNamesColumnTitles[iLayerN] = new pawgui::widget_label_rotated_90("","");
        projectLayerNamesRowTitles[iLayerN] = new pawgui::widget_label_text ("","");
    }
    for( int iMatrix = 0; iMatrix < 1024; iMatrix++)
    {
        projectCollisionLayerMatrix[iMatrix] = new pawgui::widget_checkbox("","");
        projectCollisionLayerMatrix[iMatrix]->set_checkbox_size(16);
    }
    int iMatrixRow = 0, iMatrixCol = 0;
    for( iMatrixRow = 0; iMatrixRow < 32; iMatrixRow++)
    {
        for( iMatrixCol = 16; iMatrixCol < 32; iMatrixCol++)
        {
            projectCollisionLayerMatrix[iMatrixRow*32+iMatrixCol]->set_checked(false);
        }
    }

    for( iMatrixRow = 16; iMatrixRow < 32; iMatrixRow++)
    {
        for( iMatrixCol = 0; iMatrixCol < 32; iMatrixCol++)
        {
            projectCollisionLayerMatrix[iMatrixRow*32+iMatrixCol]->set_checked(false);
        }
    }
    projectLayerEmptyTitle = new pawgui::widget_label_text ("","");
    selectAllCollisionBoxes = new pawgui::widget_button_label( "Select All Layers","Select All Layers");
    unselectAllCollisionBoxes = new pawgui::widget_button_label( "Unselect All Layers","Unselect All Layers");
    resourceFileName = "";
    resource_name = "Project Properties";
    common_buttonAlignment = 1;
    projectParentFolder = pFolder;

    //_settings Tab

    //Project language

    projectLanguageLabel = new pawgui::widget_label_text ("Programming Language:","Programming Language:");
    //Adds all available languages to drop down menu
    projectLanguage = new pawgui::widget_dropdown_menu( "Project Language",true);
    int addedLanguages = 0;
    if( pawgui::main_syntax_highlighter!=nullptr )
    {
        //short opertion so not "optimized"
        pawgui::syntax_language * tLanguage  = nullptr;
        int languageCount = (int)pawgui::main_syntax_highlighter->get_language_count();
        for( int cLanguage = 0; cLanguage < languageCount; cLanguage++ )
        {
            tLanguage = pawgui::main_syntax_highlighter->get_language_object(cLanguage);
            if( tLanguage!=nullptr && tLanguage->isCodingLanguage)
            {
                projectLanguage->add_menu_option(tLanguage->languageName+" ("+tLanguage->languageShortName+")",tLanguage->languageShortName,cLanguage,true);
                addedLanguages++;
            }
        }
    }
    //In the event something went wrong and we somehow didn't add JS and any other new coding language...
    if( addedLanguages == 0 )
    {
            projectLanguage->add_menu_option("JavaScript","JS", pawgui::program_language_js,true);
            projectLanguage->add_menu_option("C++","CPP", pawgui::program_language_cpp,true);
            projectLanguage->add_menu_option("C#","CS", pawgui::program_language_cs,true);
            projectLanguage->add_menu_option("HAXE","HAXE", pawgui::program_language_haxe,true);
    }
    iconFileName = "";
    projectIconLabel = new pawgui::widget_label_text ("Project Icon","Project Icon");
    projectIconImg = new pawgui::widget_label_image("roject_icon_image","" );
    projectIconImg->set_width(128);
    projectIconImg->set_height(128);
    browseIcon_button = new pawgui::widget_button_label( "Browse..","Find image icon");

    projectScreenWidthField = new pawgui::widget_input_number("Default: 640",true,64,4096);
    projectScreenWidthField->set_label("Screen Width");
    projectScreenWidthField->set_number(640);

    projectScreenHeightField = new pawgui::widget_input_number("Default: 480",true,64,4096);
    projectScreenHeightField->set_label("Screen Height");
    projectScreenHeightField->set_number(480);

    projectGameVersionField = new pawgui::widget_input_number("0");
    projectGameVersionField->set_label("Game Version Number");
    projectGameVersionField->set_number(1);

    projectGameSubVersionField = new pawgui::widget_input_text();
    projectGameSubVersionField->set_label("Game Sub-Version Number");
    projectGameSubVersionField->set_number(0);

    project_scale_settings = new pawgui::widget_radio_button_controller("_scale Settings", true,1);
    project_scale_settings->add_opton("Do Not scale");
    project_scale_settings->add_opton("Half-Size[50%]");
    project_scale_settings->add_opton("float-Size[200%]");
    project_scale_settings->add_opton("Keep Aspect Ratio[Recommended]");
    project_scale_settings->add_opton("Full Scale");

    project_settingsFPSRateLabel = new pawgui::widget_label_text ("Frame Rate Cap","Sets the (max) framerate set for the game(Usually 30 or 60)");
    project_settingsFPSRate = new pawgui::widget_dropdown_menu( "FPS Cap",true);
    project_settingsFPSRate->add_menu_option("15 FPS","",15);
    project_settingsFPSRate->add_menu_option("20 FPS","",20);
    project_settingsFPSRate->add_menu_option("24 FPS","",24);
    project_settingsFPSRate->add_menu_option("25 FPS","",25);
    project_settingsFPSRate->add_menu_option("30 FPS","",30,true);
    project_settingsFPSRate->add_menu_option("60 FPS","",60);
    project_settingsFPSRate->add_menu_option("Uncapped","",-1);

    //Publisher Section
    sectionTitleGame_settings = new pawgui::widget_label_title("Game Settings","Game Settings");
    sectionTitlePublisher = new pawgui::widget_label_title("Publishing","Publishing");
    projectGameTitleField = new pawgui::widget_input_text();
    projectGameTitleField->set_label("Game Title");

    projectGameShortTitleField = new pawgui::widget_input_text();
    projectGameShortTitleField->set_label("Game Short Title");

    projectGamePublisherField = new pawgui::widget_input_text();
    projectGamePublisherField->set_label("Game Publisher");

    projectGameDeveloperField = new pawgui::widget_input_text();
    projectGameDeveloperField->set_label("Game Developer");

    projectGameCopyrightField = new pawgui::widget_input_text();
    projectGameCopyrightField->set_label("Game Copyright Text");

    projectGameDateField = new pawgui::widget_input_text("2018");
    projectGameDateField->set_label("Game Copyright Date");

    projectGameDescriptionField = new pawgui::widget_input_text();
    projectGameDescriptionField->set_label("Game Description");

    projectGameWebsiteField = new pawgui::widget_input_text();
    projectGameWebsiteField->set_label("Game Website");

    projectGameEmailField= new pawgui::widget_input_text();
    projectGameEmailField->set_label("Game Email");

    checkBoxShowPublisherInfo = new pawgui::widget_checkbox("Display Info","Displays this information in build");
    checkBoxShowPublisherInfo->set_checked(true);

    //Colors Tab
    sectionWebColorsTitle = new pawgui::widget_label_title("Web Colors");
    projectBorderColor = new pawgui::gpe_widget_color_picker();
    projectBorderColor->set_label("Border");
    projectBorderColor->set_rgb(16,16,16);

    projectWebsiteBackgroundColor = new pawgui::gpe_widget_color_picker();
    projectWebsiteBackgroundColor->set_label("Website Background");
    projectWebsiteBackgroundColor->set_rgb(0,0,0);

    projectGameBackgroundColor = new pawgui::gpe_widget_color_picker();
    projectGameBackgroundColor->set_label("Game Background");
    projectGameBackgroundColor->set_rgb(0,0,0);

    projectTextHeaderColor = new pawgui::gpe_widget_color_picker();
    projectTextHeaderColor->set_label("Text(Header)");
    projectTextHeaderColor->set_rgb(0,0,228);

    projectTextParagraphColor = new pawgui::gpe_widget_color_picker();
    projectTextParagraphColor->set_label("Text");
    projectTextParagraphColor->set_rgb(192,192,192);

    projectTextLinkColor = new pawgui::gpe_widget_color_picker();
    projectTextLinkColor->set_label("Hyperlink");
    projectTextLinkColor->set_rgb(16,16,255);

    projectTextLinkActiveColor = new pawgui::gpe_widget_color_picker();
    projectTextLinkActiveColor->set_label("Hyperlink Active");
    projectTextLinkActiveColor->set_rgb(192,192,192);

    projectTextLinkHoverColor = new pawgui::gpe_widget_color_picker();
    projectTextLinkHoverColor->set_label("Hyperlink Hover");
    projectTextLinkHoverColor->set_rgb(192,255,192);

    projectTextLinkVisitedColor = new pawgui::gpe_widget_color_picker();
    projectTextLinkVisitedColor->set_label("Hyperlink Visited");
    projectTextLinkVisitedColor->set_rgb(192,16,192);

    //Macros Tab
    projectGameMacros = new pawgui::widget_text_editor();

    //Game Notes Tab
    projectGameNotes = new pawgui::widget_text_editor();
    projectGameNotes->isCodeEditor = false;

    //Export _settings Tab
    buildToSource_button = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-code-o.png","Build to Source","Packages code in preparation for building");
    buildPlatformdropdown = new pawgui::widget_dropdown_menu("Source Build Method", false);
    buildPlatformRefreshLabel = new pawgui::widget_text_url("Refresh methods","Click to re-load build platforms","#");
    exportPush_button = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cog.png","Build Project","Recompiles Project");
    exportAndPlayPush_button = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png","Build & Play Project","Recompiles Project & Runs it");

    cleanBuildFolderPush_button = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png","Clean Build Folders","Removes files from the build folder matching the criteria above.");
    cleanBuildAllFolderPush_button = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png","Clean ALL Build Folders","Removes files from all build folders of this project.");
    playProgramPush_button = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png","Play Game","Runs game matching the criteria above");

    project_settingsBar = new pawgui::widget_selectbox( "Settings");
    project_settingsBar->set_width(160);
    project_settingsBar->set_option_height(32);

    project_settingsBar->add_option("General",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/wrench.png"),nullptr,2, false, false);
    project_settingsBar->add_option("Dependencies",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/chain.png"),nullptr,2, false, false);
    project_settingsBar->add_option("Platforms",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cubes.png"),nullptr,2, false, false);
    project_settingsBar->add_option("Layers",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/map.png"),nullptr,2, false, false);
    project_settingsBar->add_option("Macros",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/tags.png"),nullptr,2, false, false);
    project_settingsBar->add_option("Notes",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/sticky-note.png"),nullptr,2, false, false);

    export_settingsBar = new pawgui::widget_selectbox( "Platforms");
    export_settingsBar->set_width(160);
    export_settingsBar->set_option_height(32);
    export_settingsBar->add_option("Windows", gpe::system_os_windows,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/windows.png"));
    export_settingsBar->add_option("Mac", gpe::system_os_mac,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/apple.png"));
    export_settingsBar->add_option("Linux", gpe::system_os_linux,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/linux.png"));
    export_settingsBar->add_option("HTML5", gpe::system_os_html5,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/html5.png"));
    export_settingsBar->add_option("Other", gpe::system_os_html5,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/question-circle.png"));


    otherPlatformLabel = new pawgui::widget_label_text ("Have another platform to extend GPE to?","Follow the instructions below:" );
    otherPlatformLabelParagraph = new pawgui::widget_label_paragraph("Extending GPE to new platforms","Were you able to get GPE working on a new platform besides the basic systems listed above? Great, using the guide below we have prepared a way to integrate our editor into your new system and build for it right away ( pending automation). Please note you may need to restart the editor for this to take place as well. "," ");
    otherPlatformLabelURL = new pawgui::widget_text_url("Documentation on adding new platforms","Will open in browser","http://docs.pawbyte.com/adding-platforms/");

    exportApplicationLabel = new pawgui::widget_label_title("Application Settings","Key Application Information");

    exportWindowsLabel = new pawgui::widget_label_title("Windows Settings","Windows  Export Settings");
    exportOSXLabel = new pawgui::widget_label_title("Mac Settings","Mac Export Settings");
    exportLinuxLabel = new pawgui::widget_label_title("Linux Settings","Linux Export Settings");

    win64BitCheckbox = new pawgui::widget_checkbox("64 Bit Build","Leave unchecked to use 32-bit build", true);
    osx64BitCheckbox = new pawgui::widget_checkbox("64 Bit Build","Leave unchecked to use 32-bit build", true);
    linux64BitCheckbox = new pawgui::widget_checkbox("64 Bit Build","Leave unchecked to use 32-bit build", true);

    html5DebugModeCheckbox = new pawgui::widget_checkbox("Debug Mode","Enter Debug mode for HTML5 build", true);
    osxDebugModeCheckbox = new pawgui::widget_checkbox("Debug Mode","Enter Debug mode for OSX build", true );
    winDebugModeCheckbox = new pawgui::widget_checkbox("Debug Mode","Enter Debug mode for Windows build",true);
    linuxDebugModeCheckbox = new pawgui::widget_checkbox("Debug Mode","Enter Debug mode for Linux build",true);

    html5nativeBuildTypeOptions = new pawgui::widget_radio_button_controller("Build Type", true);

    windowsNativeBuildTypeOptions = new pawgui::widget_radio_button_controller("Build Type", true);
    //windowsNativeBuildTypeOptions->add_menu_option("Use GPE Native-Build[Experimental]","gpe_native",native_option_runtime,false);
    windowsNativeBuildTypeOptions->add_menu_option("Use Electron","gpe_electron", native_option_electron,false);
    //windowsNativeBuildTypeOptions->add_menu_option("C++ Project","gpe_cpp",native_option_runtime_cpp,false);
    windowsNativeBuildTypeOptions->add_menu_option("Do not Export...","gpe_no_native", native_option_none,true);

    osxNativeBuildTypeOptions = new pawgui::widget_radio_button_controller("Build Type", true);
    //osxNativeBuildTypeOptions->add_menu_option("Use GPE Native-Build[Experimental]","gpe_native",native_option_runtime,false);
    osxNativeBuildTypeOptions->add_menu_option("Use Electron","gpe_electron", native_option_electron,false);
    //osxNativeBuildTypeOptions->add_menu_option("C++ Project","gpe_cpp",native_option_runtime_cpp,false);
    osxNativeBuildTypeOptions->add_menu_option("Do not Export...","gpe_no_native", native_option_none,true);

    linuxNativeBuildTypeOptions = new pawgui::widget_radio_button_controller("Build Type", true);
    //linuxNativeBuildTypeOptions->add_menu_option("Use GPE Native-Build[Experimental]","gpe_native",native_option_runtime,false);
    linuxNativeBuildTypeOptions->add_menu_option("Use Electron","gpe_electron",native_option_electron,false);
    //linuxNativeBuildTypeOptions->add_menu_option("C++ Project","gpe_cpp",native_option_runtime_cpp,false);
    linuxNativeBuildTypeOptions->add_menu_option("Do not Export...","gpe_no_native",native_option_none,true);

    electronCompileNote = new pawgui::widget_label_text ("Electron Build Guide");
    electronCompileUrl = new pawgui::widget_text_url("For instructions on compiling using Electron click here.","Instructions on compiling using Electron.","http://docs.pawbyte.com/using-electron/");

    switchComingSoonNote = new pawgui::widget_label_paragraph("Switch Note","We hope and intend to soon providing support to the Nintendo Switch. When we do, we will provide the port to approved Nintendo Switch developers free of charge. Thank you for your patience and time.","");
    nSwitchSideNote = new pawgui::widget_label_paragraph("Switch Side Note","We are heavily excited to soon provide support for multi-touch screens, new play modes, HD Rumble and built in multiplayer controls.","");
    nSwitchThirdNote = new pawgui::widget_label_paragraph("Switch Third Note","For more information feel free to contact Nintendo Developer's program:   ","");

    nSwitchContactUrl  = new pawgui::widget_text_url("Nintendo Developer contact page","Click here to visit Nintendo Developer contact page","https://developer.nintendo.com/web/development/contact");

    ps4ComingSoonNote = new pawgui::widget_label_paragraph("PS4 Note","We hope and intend to soon providing support to the PlayStation 4. When we do, we will provide the port to approved PlayStation 4 developers free of charge. Thank you for your patience and time.","");
    ps4SideNote = new pawgui::widget_label_paragraph("PS4 Side Note","For more information feel free to contact PlayStation 4 Developer's program:   ","");
    ps4ContactUrl  = new pawgui::widget_text_url("PlayStation Developer contact page","Click here to visit PlayStation Developer page","https://www.playstation.com/en-us/develop/");

    xboxOneComingSoonNote = new pawgui::widget_label_paragraph("Xbox One Note","We hope and intend to soon providing support to the Xbox One. When we do, we will provide the port to approved Xbox One developers free of charge. Thank you for your patience and time.","");
    xboxOneSideNote = new pawgui::widget_label_paragraph("Xbox One Side Note","For more information feel free to contact Xbox Developer's program:   ","");
    xboxOneContactUrl  = new pawgui::widget_text_url("Xbox Developer contact page","Click here to visit Xbox Developer contact page","http://www.xbox.com/en-US/developers");

    switchNativeBuildTypeOptions = new pawgui::widget_radio_button_controller("Build Type",true);
    /*
    switchNativeBuildTypeOptions->add_menu_option("Use GPE Native-Build[Experimental]","gpe_native",native_option_runtime,false);
    switchNativeBuildTypeOptions->add_menu_option("C++ Project","gpe_cpp",native_option_runtime_cpp,false);
    */
    switchNativeBuildTypeOptions->add_menu_option("Do not Export...","gpe_no_native",native_option_none,false);

    gpe::animation2d * mainExportOptionsanimation = pawgui::rsm_gui->animation_add("exportOptionsIcons", gpe::app_directory_name+"resources/gfx/animations/main_export_options_icons.png",12,true,0,0,false);

    gpe::texture_base * desktopLogoTexture = pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/desktop.png");
    gpe::texture_base * appleLogoTexture = pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/apple.png");
    gpe::texture_base * linuxLogoTexture = pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/font_awesome_linux.png");
    gpe::texture_base * windowsLogoTexture = pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/windows.png");
    gpe::texture_base * html5LogoTexture = pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/html5.png");

    //HTML5 _settings
    warnOnCloseCheckbox = new pawgui::widget_checkbox("Warn on Close","If checked warn user on close",true);

    desktop_settingsTabBar = new pawgui::widget_tabbar();
    desktop_settingsTabBar->set_coords(16, 16);
    desktop_settingsTabBar->add_new_tab("Windows");
    desktop_settingsTabBar->add_new_tab("OSX");
    desktop_settingsTabBar->add_new_tab("Linux");
    desktop_settingsTabBar->open_tab(0);

    renameBox->disable_self();
    loadResource_button->disable_self();
    //confirmResource_button->disable_self();
    //cancelResource_button->disable_self();
    saveResource_button->disable_self();
    resourcePostProcessed = false;
    project_settingsList = new pawgui::widget_panel_list();
    project_settingsList->panelAlignType = pawgui::panel_align_full;

    collisionLayerMatrixList = new pawgui::widget_panel_list();
    project_settingsList->barXPadding = pawgui::padding_default;
    project_settingsList->barXMargin = pawgui::padding_default;
    subViewedSpace.x = 0;
    subViewedSpace.y = 0;
    subViewedSpace.w = gpe::screen_width;
    subViewedSpace.h = gpe::screen_height;
    refresh_build_to_source_targets();
}

projectPropertiesResource::~projectPropertiesResource()
{
    gpe::error_log->report("Project Propertries Deletion #1");
    for( int iLayerN = 31; iLayerN >=0; iLayerN--)
    {
        /*projectLayerNamesColumnTitles[iLayerN] = nullptr;
        projectLayerNamesRowTitles[iLayerN] = nullptr;*/
    }
    if( projectLayerEmptyTitle!=nullptr)
    {
        delete projectLayerEmptyTitle;
        projectLayerEmptyTitle = nullptr;
    }
    //Macros Tab
    if( projectGameMacros!=nullptr)
    {
        delete projectGameMacros;
        projectGameMacros = nullptr;
    }
    //Game Notes Tab
    if( projectGameNotes!=nullptr)
    {
        delete projectGameNotes;
        projectGameNotes = nullptr;
    }
    gpe::error_log->report("Project Propertries Deletion #2");
    //Publisher Tab
    if( sectionTitleGame_settings!=nullptr)
    {
        delete sectionTitleGame_settings;
        sectionTitleGame_settings = nullptr;
    }
    if( sectionTitlePublisher!=nullptr)
    {
        delete sectionTitlePublisher;
        sectionTitlePublisher = nullptr;
    }
    if( projectGameTitleField!=nullptr)
    {
        delete projectGameTitleField;
        projectGameTitleField = nullptr;
    }
    if( projectGameDateField!=nullptr)
    {
        delete projectGameDateField;
        projectGameDateField = nullptr;
    }

    gpe::error_log->report("Project Propertries Deletion #3");
    if( projectGameDescriptionField!=nullptr)
    {
        delete projectGameDescriptionField;
        projectGameDescriptionField = nullptr;
    }
    if( projectGamePublisherField!=nullptr)
    {
        delete projectGamePublisherField;
        projectGamePublisherField = nullptr;
    }
    if( projectGameDeveloperField!=nullptr)
    {
        delete projectGameDeveloperField;
        projectGameDeveloperField = nullptr;
    }
    //_settings Tab
    if( projectScreenWidthField!=nullptr)
    {
        delete projectScreenWidthField;
        projectScreenWidthField = nullptr;
    }
    if( projectScreenHeightField!=nullptr)
    {
        delete projectScreenHeightField;
        projectScreenHeightField = nullptr;
    }
    if( projectGameVersionField!=nullptr)
    {
        delete projectGameVersionField;
        projectGameVersionField = nullptr;
    }
    if( projectGameSubVersionField!=nullptr)
    {
        delete projectGameSubVersionField;
        projectGameSubVersionField = nullptr;
    }
    gpe::error_log->report("Project Propertries Deletion #4");
    if( project_scale_settings!=nullptr)
    {
        delete project_scale_settings;
        project_scale_settings = nullptr;
    }
    if( project_settingsFPSRateLabel!=nullptr)
    {
        delete project_settingsFPSRateLabel;
        project_settingsFPSRateLabel = nullptr;
    }
    if( project_settingsFPSRate!=nullptr)
    {
        delete project_settingsFPSRate;
        project_settingsFPSRate = nullptr;
    }
    if( projectGameCopyrightField!=nullptr)
    {
        delete projectGameCopyrightField;
        projectGameCopyrightField = nullptr;
    }
    if( projectGameWebsiteField!=nullptr)
    {
        delete projectGameWebsiteField;
        projectGameWebsiteField = nullptr;
    }
    if( projectGameEmailField!=nullptr)
    {
        delete projectGameEmailField;
        projectGameEmailField = nullptr;
    }
    gpe::error_log->report("Proect Propertries Deletion #5");
    if( checkBoxShowPublisherInfo!=nullptr)
    {
        delete checkBoxShowPublisherInfo;
        checkBoxShowPublisherInfo = nullptr;
    }


    //Colors Tab
    if( sectionWebColorsTitle!=nullptr )
    {
        delete sectionWebColorsTitle;
        sectionWebColorsTitle = nullptr;
    }
    if(projectBorderColor!=nullptr)
    {
        delete projectBorderColor;
        projectBorderColor = nullptr;
    }
    if( projectWebsiteBackgroundColor!=nullptr)
    {
        delete projectWebsiteBackgroundColor;
        projectWebsiteBackgroundColor = nullptr;
    }
    if(projectGameBackgroundColor!=nullptr)
    {
        delete projectGameBackgroundColor;
        projectGameBackgroundColor = nullptr;
    }
    if(projectTextHeaderColor!=nullptr)
    {
        delete projectTextHeaderColor;
        projectTextHeaderColor = nullptr;
    }
    if(projectTextParagraphColor!=nullptr)
    {
        delete projectTextParagraphColor;
        projectTextParagraphColor = nullptr;
    }
    gpe::error_log->report("Project Propertries Deletion #6");
    if(projectTextLinkColor!=nullptr)
    {
        delete projectTextLinkColor;
        projectTextLinkColor = nullptr;
    }

    if(projectTextLinkActiveColor!=nullptr)
    {
        delete projectTextLinkActiveColor;
        projectTextLinkActiveColor = nullptr;
    }

    if(projectTextLinkHoverColor!=nullptr)
    {
        delete projectTextLinkHoverColor;
        projectTextLinkHoverColor = nullptr;
    }

    if(projectTextLinkVisitedColor!=nullptr)
    {
        delete projectTextLinkVisitedColor;
        projectTextLinkVisitedColor = nullptr;
    }
    gpe::error_log->report("Project Propertries Deletion #7");
    //Platforms tab
    //HTML5 Platforms
    if( warnOnCloseCheckbox!=nullptr)
    {
        delete warnOnCloseCheckbox;
        warnOnCloseCheckbox = nullptr;
    }

    if( windowsNativeBuildTypeOptions!=nullptr)
    {
        delete windowsNativeBuildTypeOptions;
        windowsNativeBuildTypeOptions = nullptr;
    }

    if( osxNativeBuildTypeOptions!=nullptr)
    {
        delete osxNativeBuildTypeOptions;
        osxNativeBuildTypeOptions = nullptr;
    }

    if( linuxNativeBuildTypeOptions!=nullptr)
    {
        delete linuxNativeBuildTypeOptions;
        linuxNativeBuildTypeOptions = nullptr;
    }

    if( switchNativeBuildTypeOptions!=nullptr)
    {
        delete switchNativeBuildTypeOptions;
        switchNativeBuildTypeOptions = nullptr;
    }
    gpe::error_log->report("Project Propertries Deletion #8");

    //Other variables
    if( project_settingsList!=nullptr)
    {
        delete project_settingsList;
        project_settingsList = nullptr;
    }
}

bool projectPropertiesResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void projectPropertiesResource::compile_cpp()
{

}

bool projectPropertiesResource::export_and_play_native(bool launchProgram)
{
    bool buildAndRunSuccessful = false;
    if( current_project!=nullptr)
    {
        int buildBits = 32;
        if( gpe::system_found_os == gpe::system_os_windows )
        {
            if( win64BitCheckbox->is_clicked() )
            {
                buildBits = 64;
            }
            current_project->save_project();
            buildAndRunSuccessful = current_project->export_project_windows("", buildPlatformdropdown->get_selected_name() ,buildBits,launchProgram, winDebugModeCheckbox->is_clicked(), windowsNativeBuildTypeOptions->get_selected_value() );
        }
        else if( gpe::system_found_os== gpe::system_os_linux )
        {
            if( linux64BitCheckbox->is_clicked() )
            {
                buildBits = 64;
            }
            current_project->save_project();
            buildAndRunSuccessful = current_project->export_project_linux("", buildPlatformdropdown->get_selected_name(),buildBits,launchProgram, linuxDebugModeCheckbox->is_clicked(), linuxNativeBuildTypeOptions->get_selected_value() );
        }
        else if( gpe::system_found_os == gpe::system_os_mac )
        {
            buildBits = 64;
            current_project->save_project();
            buildAndRunSuccessful = current_project->export_project_osx("", buildPlatformdropdown->get_selected_name(),buildBits,launchProgram, osxDebugModeCheckbox->is_clicked(), osxNativeBuildTypeOptions->get_selected_value()  );
        }
        else if( pawgui::main_tab_resource_bar!=nullptr )
        {
            pawgui::main_tab_resource_bar->add_new_tab(this );
            project_settingsBar->set_selected_option("Platforms");
        }
    }
    return buildAndRunSuccessful;
}

void projectPropertiesResource::integrate_into_syntax()
{
    //parses the macros text editor for joy...
    if( projectGameMacros!=nullptr && current_project!=nullptr)
    {
        std::string currentLineToRead = "";
        std::string foundVariable = "";
        std::string variablestring = "";
        bool isInBlockCommentMode = false;
        bool isInfloatQuoteMode = false;
        bool isInSingleQuoteMode = false;
        bool commentFoundInSymbols = false;
        int blockCommentStartPos = 0;
        int blockCommentEndPos = 0;
        int singleCommentStartPos = 0;
        int lineParseXCursorPos = 0;
        int endSQuoteCommentPos = 0;
        int endDQuoteCommentPos = 0;
        int currentLineSize = 0;
        int varDeclarationPos = 0;
        int commaPos = 0;
        int lastCommaPos = 0;
        int variableEndPos = 0;
        for( int i = 0; i < projectGameMacros->get_line_count() ; i++ )
        {
            currentLineToRead = projectGameMacros->get_line_string(i);
            lineParseXCursorPos = 0;
            currentLineSize = (int)currentLineToRead.size();
            if( currentLineSize > 0)
            {
                while( lineParseXCursorPos < currentLineSize )
                {
                    if( isInBlockCommentMode==false && isInfloatQuoteMode==false && isInSingleQuoteMode==false)
                    {
                        //skip empty spaces
                        while (lineParseXCursorPos < currentLineSize && currentLineToRead[lineParseXCursorPos]==' ')
                        {
                            lineParseXCursorPos++;
                        }
                        if (lineParseXCursorPos < currentLineSize)
                        {
                            if (lineParseXCursorPos+1<currentLineSize && currentLineToRead[lineParseXCursorPos] == '/' && lineParseXCursorPos + 1 < currentLineSize && currentLineToRead[lineParseXCursorPos + 1] == '/' )
                            {
                                lineParseXCursorPos = currentLineSize;
                            }
                            else if (lineParseXCursorPos+1<currentLineSize && currentLineToRead[lineParseXCursorPos] == '/' && currentLineToRead[lineParseXCursorPos + 1] == '*' )
                            {
                                isInBlockCommentMode = true;
                                lineParseXCursorPos+=2;
                            }
                            else if(currentLineToRead[lineParseXCursorPos] == '"')
                            {
                                isInfloatQuoteMode = true;
                                lineParseXCursorPos++;
                            }
                            else if(currentLineToRead[lineParseXCursorPos] == '\'')
                            {
                                isInSingleQuoteMode = true;
                                lineParseXCursorPos++;
                            }
                            else if (isdigit(currentLineToRead[lineParseXCursorPos]))
                            {
                                lineParseXCursorPos++;
                                while( (lineParseXCursorPos < currentLineSize && isdigit( currentLineToRead[lineParseXCursorPos] )  ) || currentLineToRead[lineParseXCursorPos] ==' ')
                                {
                                    lineParseXCursorPos++;
                                }
                            }
                            else if (pawgui::main_syntax_highlighter->charIsSymbol(currentLineToRead[lineParseXCursorPos]))
                            {
                                lineParseXCursorPos++;
                                commentFoundInSymbols = false;
                                while( ( commentFoundInSymbols==false && lineParseXCursorPos < currentLineSize && pawgui::main_syntax_highlighter->charIsSymbol(currentLineToRead[lineParseXCursorPos] ) )|| currentLineToRead[lineParseXCursorPos] ==' ')
                                {
                                    if( currentLineSize > lineParseXCursorPos+1)
                                    {
                                        if( currentLineToRead[lineParseXCursorPos] == '/' && (currentLineToRead[lineParseXCursorPos + 1] == '/' || currentLineToRead[lineParseXCursorPos + 1] == '*' ) )
                                        {
                                            commentFoundInSymbols = true;
                                        }
                                        else if (currentLineToRead[lineParseXCursorPos] == '*' && currentLineToRead[lineParseXCursorPos + 1] == '/' )
                                        {
                                            commentFoundInSymbols = true;
                                        }
                                        else
                                        {
                                            lineParseXCursorPos++;
                                        }
                                    }
                                    else
                                    {
                                        lineParseXCursorPos++;
                                    }
                                }
                            }
                            else
                            {
                                //checks for variable declaration
                                if (lineParseXCursorPos+5<currentLineSize )
                                {
                                    varDeclarationPos = currentLineToRead.find("var ",lineParseXCursorPos);
                                    if( varDeclarationPos!=(int)std::string::npos)
                                    {
                                        variableEndPos = 0;
                                        variablestring = stg_ex::get_substring(currentLineToRead,varDeclarationPos+4);
                                        variablestring = stg_ex::string_replace_all(variablestring," ","");
                                        if( (int)variablestring.size() > 3)
                                        {
                                            commaPos = variablestring.find(",",0);
                                            lastCommaPos = 0;
                                            if( commaPos > 0)
                                            {
                                                while( commaPos >=0)
                                                {
                                                    foundVariable = stg_ex::get_substring(variablestring,lastCommaPos, commaPos);
                                                    if( (int)foundVariable.size() > 3 )
                                                    {
                                                        while( stg_ex::char_is_alnum(foundVariable[variableEndPos],false,true) )
                                                        {
                                                            variableEndPos++;
                                                            lineParseXCursorPos++;
                                                        }
                                                        foundVariable = stg_ex::get_substring(foundVariable,0,variableEndPos);
                                                        if( (int)foundVariable.size() > 0 )
                                                        {
                                                            current_project->add_project_keyword(foundVariable,"Defined in Project Marcos",-1,"User Global");
                                                            gpe::error_log->report("Found user macro:   ["+foundVariable+"].");
                                                        }
                                                        lastCommaPos = commaPos;
                                                        commaPos = std::max(commaPos,lineParseXCursorPos);
                                                        if( commaPos > 0)
                                                        {
                                                            commaPos = variablestring.find(",",commaPos);
                                                            variableEndPos = 0;
                                                            lineParseXCursorPos = commaPos;
                                                            if( commaPos > 0)
                                                            {
                                                                commaPos++;
                                                            }
                                                            else
                                                            {
                                                                commaPos = (int)variablestring.size();
                                                            }
                                                        }
                                                        else
                                                        {
                                                            commaPos = (int)variablestring.size();
                                                            lineParseXCursorPos = currentLineSize;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        commaPos = -1;
                                                        lineParseXCursorPos = currentLineSize;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                while( stg_ex::char_is_alnum(variablestring[variableEndPos],false,true) )
                                                {
                                                    variableEndPos++;
                                                    lineParseXCursorPos++;
                                                }
                                                foundVariable = stg_ex::get_substring(variablestring,0,variableEndPos);
                                                if( (int)foundVariable.size() > 0 )
                                                {
                                                    current_project->add_project_keyword(foundVariable,"Defined in Project Marcos",-1,"User Global");
                                                    gpe::error_log->report("Found user macro:   ["+foundVariable+"].");
                                                }
                                            }
                                        }
                                    }
                                }
                                lineParseXCursorPos = currentLineSize;

                            }
                        }
                    }
                    if(isInBlockCommentMode)
                    {
                        blockCommentEndPos = currentLineToRead.find("*/");
                        if( blockCommentEndPos!=(int)std::string::npos)
                        {
                            isInBlockCommentMode = false;
                            lineParseXCursorPos = blockCommentEndPos+2;
                        }
                        else
                        {
                            lineParseXCursorPos = currentLineSize;
                        }
                    }
                    else if( isInfloatQuoteMode)
                    {
                        endDQuoteCommentPos = currentLineToRead.find('"',lineParseXCursorPos);
                        if( endDQuoteCommentPos==(int)std::string::npos)
                        {
                            lineParseXCursorPos = currentLineSize;
                        }
                        else
                        {
                            lineParseXCursorPos = endDQuoteCommentPos+1;
                            isInfloatQuoteMode = false;
                        }
                    }
                    else if( isInSingleQuoteMode)
                    {
                        endSQuoteCommentPos = currentLineToRead.find("'",lineParseXCursorPos);
                        if( endSQuoteCommentPos==(int)std::string::npos)
                        {
                            lineParseXCursorPos = currentLineSize;
                        }
                        else
                        {
                            lineParseXCursorPos = endSQuoteCommentPos+1;
                            isInSingleQuoteMode = false;
                        }
                    }
                }
            }
        }
    }
}

bool projectPropertiesResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}

bool projectPropertiesResource::is_build_ready()
{
    return true; //WIPNOTFUNCTIONALYET
}

void projectPropertiesResource::open_code(int lineNumb, int colNumb, std::string codeTitle )
{
    if( (int)codeTitle.size() > 0 )
    {
        pawgui::widget_text_editor * foundTextArea = nullptr;

        if( codeTitle == "Macros")
        {
            foundTextArea = projectGameMacros;
            project_settingsBar->set_selected_option("Macros");
        }
        else if( codeTitle == "Notes")
        {
            foundTextArea = projectGameNotes;
            project_settingsBar->set_selected_option("Notes");

        }

        if( foundTextArea!=nullptr && foundTextArea->has_content() )
        {
            foundTextArea->scroll_to_pos(lineNumb, colNumb);
        }
    }
}

void projectPropertiesResource::prerender_self(  )
{
    standardEditableGameResource::prerender_self();
    if( checkBoxShowPublisherInfo!=nullptr)
    {
        checkBoxShowPublisherInfo->prerender_self( );
    }
}

void projectPropertiesResource::load_resource(std::string file_path)
{
    if( resourcePostProcessed ==false  || sff_ex::file_exists(file_path) )
    {
        if( main_gpe_splash_page != nullptr )
        {
            main_gpe_splash_page->update_submessages( "Processing Project Properties","Please wait..." );
        }

        std::string otherColContainerName = "";

        std::string newFileIn ="";
        if( sff_ex::file_exists(file_path) )
        {
            newFileIn = file_path;
        }
        else
        {
            newFileIn = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/project_settings.gpf";
        }

        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        int equalPos = 0;
        std::string firstChar="";
        std::string key_string="";
        std::string valstring="";
        std::string subValstring="";
        std::string currLine="";
        std::string currLineToBeProcessed;
        std::string sprDataStr;
        float foundFileVersion = 0;

        gpe::error_log->report("Loading project properties - "+newFileIn);
        //If the level file could be loaded
        if( !gameResourceFileIn.fail() )
        {
            gpe::error_log->report("Processing project properties file...");
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
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

                                if( key_string=="ScreenWidth")
                                {
                                    projectScreenWidthField->set_number( std::max(160, stg_ex::string_to_int(valstring) )  );
                                }
                                else if( key_string=="ScreenHeight")
                                {
                                    projectScreenHeightField->set_number( std::max(120, stg_ex::string_to_int(valstring) ) );
                                }
                                else if( key_string=="ProjectVersion")
                                {
                                    projectGameVersionField->set_string(valstring);
                                }
                                else if( key_string=="ProjectSubVersion")
                                {
                                    projectGameSubVersionField->set_string(valstring);
                                }
                                else if( key_string=="ProjectIcon")
                                {
                                    iconFileName = stg_ex::file_to_dir(parentProjectName)+"/"+valstring;
                                    if( current_project!=nullptr )
                                    {
                                        current_project->projectIconName =  stg_ex::get_local_from_global_file( iconFileName);
                                    }
                                    if( sff_ex::file_exists(iconFileName) && stg_ex::file_is_image(iconFileName) )
                                    {
                                        if( projectIconImg!=nullptr)
                                        {
                                            projectIconImg->load_label_image(iconFileName);
                                            projectIconImg->set_height(128);
                                        }
                                    }
                                    else
                                    {
                                        iconFileName = "";
                                    }
                                }
                                else if( key_string=="Project_scaleSettings")
                                {
                                    project_scale_settings->set_selection( stg_ex::string_to_int(valstring) );
                                }
                                else if( key_string=="ProjectFPSCap")
                                {
                                    project_settingsFPSRate->set_id( stg_ex::string_to_int(valstring) );
                                }
                                else if( key_string=="ProjectTitle")
                                {
                                    projectGameTitleField->set_string(valstring);
                                }
                                else if( key_string=="ProjectShortTitle")
                                {
                                    projectGameShortTitleField->set_string(valstring);
                                }
                                else if( key_string=="ProjectDate")
                                {
                                    projectGameDateField->set_string(valstring);
                                }
                                else if( key_string=="ProjectDescription")
                                {
                                    projectGameDescriptionField->set_string(valstring);
                                }
                                else if( key_string=="ProjectLanguage")
                                {
                                    projectLanguage->set_option_subvalue(valstring);
                                }
                                //
                                else if( key_string=="ProjectPublisher")
                                {
                                    projectGamePublisherField->set_string(valstring);
                                }
                                else if( key_string=="ProjectDeveloper")
                                {
                                    projectGameDeveloperField->set_string(valstring);
                                }
                                else if( key_string=="ProjectCopyright")
                                {
                                    projectGameCopyrightField->set_string(valstring);
                                }
                                else if( key_string=="ProjectWebsite")
                                {
                                    projectGameWebsiteField->set_string(valstring);
                                }
                                else if( key_string=="ProjectEmail")
                                {
                                    projectGameEmailField->set_string(valstring);
                                }

                                else if( key_string=="BorderColor")
                                {
                                    projectBorderColor->set_color_from_rgb(valstring);
                                }
                                else if( key_string=="GameBackgroundColor")
                                {
                                    projectGameBackgroundColor->set_color_from_rgb(valstring);
                                }
                                else if( key_string=="WebsiteBackgroundColor")
                                {
                                    projectWebsiteBackgroundColor->set_color_from_rgb(valstring);
                                }
                                else if( key_string=="HeaderColor")
                                {
                                    projectTextHeaderColor->set_color_from_rgb(valstring);
                                }
                                else if( key_string=="ParagraphColor")
                                {
                                    projectTextParagraphColor->set_color_from_rgb(valstring);
                                }
                                else if( key_string=="LinkColor")
                                {
                                    projectTextLinkColor->set_color_from_rgb(valstring);
                                }
                                else if( key_string=="LinkActiveColor")
                                {
                                    projectTextLinkActiveColor->set_color_from_rgb(valstring);
                                }
                                else if( key_string=="LinkHoverColor")
                                {
                                    projectTextLinkHoverColor->set_color_from_rgb(valstring);
                                }
                                else if( key_string=="LinkVisitedColor")
                                {
                                    projectTextLinkVisitedColor->set_color_from_rgb(valstring);
                                }
                                else if( key_string=="HTML5WarnOnClose")
                                {
                                    warnOnCloseCheckbox->set_checked(stg_ex::string_to_bool(valstring) );
                                }
                            }
                        }
                    }
                }
            }
        }

        if( current_project!=nullptr)
        {
            for( int iLayerN = 0; iLayerN < 32; iLayerN++)
            {
                projectLayerNamesColumnTitles[iLayerN]->set_name(current_project->projectLayerNames[iLayerN] );
                projectLayerNamesRowTitles[iLayerN]->set_name(current_project->projectLayerNames[iLayerN] );
            }

            std::string projectCollisionMatrixFileName = projectParentFolder->parentProjectDirectory+"/gpe_project/project_layer_matrix.gpf";
            gpe::error_log->report("Loading project collision matrix to "+projectCollisionMatrixFileName);
            std::ofstream projectCollisionMatrixFile (projectCollisionMatrixFileName.c_str() );
            int jLayerCol = 0;
            int iLayerRow = 0;
            while (projectCollisionMatrixFile.is_open() )
            {
                getline (gameResourceFileIn,currLine); //gets the next line of the file
                currLineToBeProcessed = stg_ex::trim_left_inplace(currLine);
                currLineToBeProcessed = stg_ex::trim_right_inplace(currLineToBeProcessed);

                //Comment skipping is only allowed at the top of the file
                if(!currLineToBeProcessed.empty() &&  currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                {
                    //searches for an equal character and parses through the variable
                    equalPos=currLineToBeProcessed.find_first_of("=");
                    if(equalPos!=(int)std::string::npos)
                    {
                        //if the equalPos is present, then parse on through and carryon
                        key_string = currLineToBeProcessed.substr(0,equalPos);
                        valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                        for(  iLayerRow = 0; iLayerRow < 12; iLayerRow++)
                        {
                            if( key_string=="CollisionLayer["+ stg_ex::int_to_string(iLayerRow)+"]")
                            {
                                for(  jLayerCol = 0; jLayerCol < 12-iLayerRow; jLayerCol++)
                                {
                                    valstring = stg_ex::split_first_int(valstring,',');
                                    projectCollisionLayerMatrix[iLayerRow*12+jLayerCol]->set_checked( stg_ex::string_to_bool(valstring) );
                                }
                            }
                        }
                    }
                }
                projectCollisionMatrixFile.close();
            }
            for(  iLayerRow = 0; iLayerRow < 32; iLayerRow++)
            {
                for(  jLayerCol = 0; jLayerCol < 32-iLayerRow; jLayerCol++)
                {
                    valstring = stg_ex::split_first_int(valstring,',');
                    projectCollisionLayerMatrix[iLayerRow*32+jLayerCol]->descriptionText = "["+current_project->projectLayerNames[iLayerRow]+" * "+current_project->projectLayerNames[jLayerCol]+"]";
                }
            }
        }
    }
    //Project TextAreas
    if( projectGameMacros!=nullptr)
    {
        projectGameMacros->import_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_macros.gpf");
    }
    if( projectGameNotes!=nullptr)
    {
        projectGameNotes->import_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_notes.gpf");
    }
}

void projectPropertiesResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    cam = gpe::camera_find(cam);
    view_space = gpe::camera_find(view_space);
    bool isJSProject = true;
    if( current_project !=nullptr )
    {
        if( current_project->get_project_language_id() != pawgui::program_language_js )
        {
            isJSProject = false;
        }
    }

    if ( export_settingsBar!=nullptr )
    {
        int html5OptionId = export_settingsBar->get_option_id("HTML5");

        //Add/Remove the HTML5 option depending on current project language
        if( isJSProject )
        {
            if( html5OptionId < 0)
            {
                export_settingsBar->add_option("HTML5", gpe::system_os_html5,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/html5.png"));
            }
        }
        else if(  html5OptionId > 0 )
        {
            export_settingsBar->remove_option( html5OptionId );
        }
    }

    if( collisionLayerMatrixList != nullptr )
    {
        collisionLayerMatrixList->clear_list();
    }
    if( cam!=nullptr && project_settingsList!=nullptr && project_settingsBar!=nullptr && view_space!=nullptr)
    {
        std::string nExportOptionName = export_settingsBar->get_selected_name();

        std::string nExportDesktopOS = desktop_settingsTabBar->get_selected_name();

        int prevTab = project_settingsBar->get_selection();
        if( panel_main_editor!=nullptr )
        {
            subViewedSpace.x = 0;
            subViewedSpace.y = 0;
            subViewedSpace.w = view_space->w;
            subViewedSpace.h = view_space->h;
            panel_main_editor->add_gui_element_fullsize( project_settingsBar );
            panel_main_editor->process_self();
            export_settingsBar->set_coords( 0, 0 );

        }
        else
        {
            project_settingsBar->set_coords(0, 0 );
            project_settingsBar->set_height( view_space->h );
            project_settingsBar->process_self( view_space, cam );
            subViewedSpace.x = project_settingsBar->get_x2();
            subViewedSpace.y = 0;
            subViewedSpace.w = view_space->w - project_settingsBar->get_width();
            subViewedSpace.h = view_space->h;
            export_settingsBar->set_coords(project_settingsBar->get_x2(), 0 );

        }

        if( prevTab!= project_settingsBar->get_selection() )
        {
            project_settingsList->reset_self();
            gpe::input->reset_all_input();
            return;
        }
        project_settingsList->clear_list();
        project_settingsList->set_coords( subViewedSpace.x, subViewedSpace.y);
        project_settingsList->set_width(subViewedSpace.w);
        project_settingsList->set_height(subViewedSpace.h );
        project_settingsList->barXPadding = pawgui::padding_default;
        project_settingsList->barYPadding = pawgui::padding_default;
        project_settingsList->barXMargin = pawgui::padding_default;
        project_settingsList->barYMargin = pawgui::padding_default;

        if( project_settingsBar->get_selected_name()=="General" || project_settingsBar->get_selected_name()=="Settings")
        {
            project_settingsList->add_gui_element(sectionTitleGame_settings,true);
            project_settingsList->add_gui_element(projectScreenWidthField,true);
            project_settingsList->add_gui_element(projectScreenHeightField,true);
            project_settingsList->add_gui_element(projectGameVersionField,true);
            project_settingsList->add_gui_element(projectGameSubVersionField,true);

            project_settingsList->add_gui_element(projectLanguageLabel,true);
            project_settingsList->add_gui_element(projectLanguage,true);

            project_settingsList->add_gui_element(projectIconLabel,true);
            project_settingsList->add_gui_element(browseIcon_button,true);
            project_settingsList->add_gui_element(projectIconImg,true);

            project_settingsList->add_gui_element(project_scale_settings,true);
            project_settingsList->add_gui_element(project_settingsFPSRateLabel,true);
            project_settingsList->add_gui_element(project_settingsFPSRate,true);

            project_settingsList->add_gui_element(sectionTitlePublisher,true);
            project_settingsList->add_gui_element(projectGameTitleField,true);
            project_settingsList->add_gui_element(projectGameShortTitleField,true);
            project_settingsList->add_gui_element(projectGamePublisherField,true);
            project_settingsList->add_gui_element(projectGameDeveloperField,true);
            project_settingsList->add_gui_element(projectGameDescriptionField,true);
            project_settingsList->add_gui_element(projectGameCopyrightField,true);
            project_settingsList->add_gui_element(projectGameDateField,true);
            project_settingsList->add_gui_element(projectGameWebsiteField,true);
            project_settingsList->add_gui_element(projectGameEmailField,true);
            project_settingsList->add_gui_element(checkBoxShowPublisherInfo,true);

            project_settingsList->add_gui_element(confirmResource_button,true);
            project_settingsList->process_self( view_space,cam);

            if( browseIcon_button->is_clicked() )
            {
                if( (int)editor_gui_main->fileOpenProjectIconDir.size()==0 && current_project!=nullptr )
                {
                    editor_gui_main->fileOpenProjectIconDir = current_project->get_project_directory();
                }
                std::string newProjectIconName = pawgui::get_filename_open_from_popup("Find Project Icon","Image",editor_gui_main->fileOpenProjectIconDir );
                if( sff_ex::file_exists(newProjectIconName ) )
                {
                    if( stg_ex::file_is_image(newProjectIconName) )
                    {
                        if( projectIconImg!=nullptr)
                        {
                            projectIconImg->load_label_image( newProjectIconName );
                            projectIconImg->set_height(128);
                        }
                        if( current_project!=nullptr )
                        {
                            iconFileName = current_project->projectIconName = stg_ex::get_local_from_global_file(newProjectIconName );
                            sff_ex::file_copy(newProjectIconName, current_project->get_project_directory()+"/"+current_project->projectIconName  );
                        }
                    }
                }
            }
        }
        else if(project_settingsBar->get_selected_name()=="Platforms")
        {
            export_settingsBar->set_height(subViewedSpace.h);
            export_settingsBar->process_self(view_space,cam);
            subViewedSpace.x+= export_settingsBar->get_width();
            subViewedSpace.w-= export_settingsBar->get_width();
            project_settingsList->set_coords( subViewedSpace.x, subViewedSpace.y );
            project_settingsList->set_width( subViewedSpace.w  );
            project_settingsList->set_height(subViewedSpace.h );
            if( nExportOptionName=="HTML5" )
            {
               //NO LONGER SUPPORTED BY DESKTOP APPLICATION
            }
            else
            {
                int buildBits = 32;
                if( nExportOptionName=="Windows" )
                {
                    project_settingsList->add_gui_element(exportWindowsLabel,true);
                    project_settingsList->add_gui_element(win64BitCheckbox,true );
                    project_settingsList->add_gui_element(winDebugModeCheckbox, true);
                    if( isJSProject )
                    {
                        project_settingsList->add_gui_element(windowsNativeBuildTypeOptions, true);
                    }
                    if( gpe::system_found_os == gpe::system_os_windows )
                    {
                        project_settingsList->add_gui_auto(buildToSource_button );
                        project_settingsList->add_gui_auto(buildPlatformdropdown );
                        project_settingsList->add_gui_auto(buildPlatformRefreshLabel );
                        /*
                        project_settingsList->add_gui_auto(exportPush_button );
                        project_settingsList->add_gui_auto(exportAndPlayPush_button );
                        */
                        project_settingsList->add_gui_auto(playProgramPush_button );
                    }
                    else
                    {
                        project_settingsList->add_gui_element(exportPush_button,true);
                    }

                    if( isJSProject )
                    {
                        project_settingsList->add_gui_element(electronCompileNote,true);
                        project_settingsList->add_gui_element(electronCompileUrl,true);
                    }
                    project_settingsList->process_self( view_space,cam);

                    if( buildPlatformRefreshLabel->is_clicked() )
                    {
                        refresh_build_to_source_targets();
                    }

                    if( buildToSource_button->is_clicked() && current_project!=nullptr && buildPlatformdropdown!=nullptr )
                    {
                        if( buildPlatformdropdown->get_selected_id() >= 0 && buildPlatformdropdown->get_menu_size() > 0 )
                        {
                            current_project->save_project();
                            current_project->export_project_windows("",buildPlatformdropdown->get_selected_name(),buildBits,false, winDebugModeCheckbox->is_clicked(), windowsNativeBuildTypeOptions->get_selected_value() );
                            //winnativeBuildTypeOptions->set_checked(false);
                        }
                        else
                        {
                            main_editor_log->log_build_error("Unable to build to Source for Windows: Method not selected!" );
                        }
                    }

                    if( win64BitCheckbox->is_clicked() )
                    {
                        buildBits = 64;
                    }
                    if( exportPush_button->is_clicked() && current_project!=nullptr)
                    {
                        current_project->save_project();
                        current_project->export_project_windows("",buildPlatformdropdown->get_selected_name(),buildBits,false, winDebugModeCheckbox->is_clicked(), windowsNativeBuildTypeOptions->get_selected_value() );
                        //winnativeBuildTypeOptions->set_checked(false);
                    }
                    else if( gpe::system_found_os== gpe::system_os_windows && current_project!=nullptr )
                    {
                        if( exportAndPlayPush_button->is_clicked() )
                        {
                            export_and_play_native(true );
                        }
                        else if( playProgramPush_button->is_clicked() )
                        {
                            current_project->run_project("",gpe::system_os_windows,buildBits, winDebugModeCheckbox->is_clicked() );
                        }
                    }
                }
                else if( nExportOptionName=="Linux" )
                {
                    project_settingsList->add_gui_element(exportLinuxLabel,true);
                    project_settingsList->add_gui_element(linux64BitCheckbox,true);
                    project_settingsList->add_gui_element(linuxDebugModeCheckbox, true);
                    if( isJSProject )
                    {
                        project_settingsList->add_gui_element(linuxNativeBuildTypeOptions, true);
                    }
                    if( gpe::system_found_os == gpe::system_os_linux )
                    {
                        project_settingsList->add_gui_auto(buildToSource_button );
                        project_settingsList->add_gui_auto(buildPlatformdropdown );
                        project_settingsList->add_gui_auto(buildPlatformRefreshLabel );
                        /*
                        project_settingsList->add_gui_auto(exportPush_button );
                        project_settingsList->add_gui_auto(exportAndPlayPush_button );
                        */
                        project_settingsList->add_gui_auto(playProgramPush_button );
                    }
                    else
                    {
                        project_settingsList->add_gui_auto(buildToSource_button );
                        project_settingsList->add_gui_auto(buildPlatformdropdown );
                        project_settingsList->add_gui_auto(buildPlatformRefreshLabel );
                        project_settingsList->add_gui_element(exportPush_button,true);
                    }
                    if( isJSProject )
                    {
                        project_settingsList->add_gui_element(electronCompileNote,true);
                        project_settingsList->add_gui_element(electronCompileUrl,true);
                    }
                    project_settingsList->process_self( view_space,cam);

                    if( buildPlatformRefreshLabel->is_clicked() )
                    {
                        refresh_build_to_source_targets();
                    }

                    if( buildToSource_button->is_clicked() && current_project!=nullptr && buildPlatformdropdown!=nullptr )
                    {
                        if( buildPlatformdropdown->get_selected_id() >= 0 && buildPlatformdropdown->get_menu_size() > 0 )
                        {
                            current_project->save_project();
                            current_project->export_project_linux("",buildPlatformdropdown->get_selected_name(),buildBits,false, linuxDebugModeCheckbox->is_clicked(), linuxNativeBuildTypeOptions->get_selected_value() );
                            //winnativeBuildTypeOptions->set_checked(false);
                        }
                        else
                        {
                            main_editor_log->log_build_error("Unable to build to Source for Linux: Method not selected!" );
                        }
                    }
                    if( linux64BitCheckbox->is_clicked() )
                    {
                        buildBits = 64;
                    }
                    if( exportPush_button->is_clicked() && current_project!=nullptr)
                    {
                        current_project->save_project();
                        current_project->export_project_linux("","", buildBits,false, linuxDebugModeCheckbox->is_clicked(), linuxNativeBuildTypeOptions->get_selected_value() );
                        //linuxnativeBuildTypeOptions->set_checked(false);
                    }
                    else if( gpe::system_found_os == gpe::system_os_linux && current_project!=nullptr )
                    {
                        if( exportAndPlayPush_button->is_clicked() )
                        {
                            export_and_play_native(true);
                        }
                        else if( playProgramPush_button->is_clicked() )
                        {
                            current_project->run_project("",gpe::system_os_linux,buildBits, linuxDebugModeCheckbox->is_clicked() );
                        }
                    }
                }
                else if( nExportOptionName=="OSX" || nExportOptionName=="Mac" )
                {
                    project_settingsList->add_gui_element( exportOSXLabel,true);
                    project_settingsList->add_gui_element(osx64BitCheckbox,true);
                    project_settingsList->add_gui_element(osxDebugModeCheckbox, true);
                    if( isJSProject )
                    {
                        project_settingsList->add_gui_element(osxNativeBuildTypeOptions, true);
                    }
                    if( gpe::system_found_os == gpe::system_os_mac )
                    {
                        project_settingsList->add_gui_auto(buildToSource_button );
                        project_settingsList->add_gui_auto(buildPlatformdropdown );
                        project_settingsList->add_gui_auto(buildPlatformRefreshLabel );
                        /*
                        project_settingsList->add_gui_auto(exportPush_button );
                        project_settingsList->add_gui_auto(exportAndPlayPush_button );
                        */
                        project_settingsList->add_gui_auto(playProgramPush_button );
                    }
                    else
                    {
                        project_settingsList->add_gui_auto(buildToSource_button );
                        project_settingsList->add_gui_auto(buildPlatformdropdown );
                        project_settingsList->add_gui_auto(buildPlatformRefreshLabel );
                        project_settingsList->add_gui_element(exportPush_button,true);
                    }
                    if( isJSProject )
                    {
                        project_settingsList->add_gui_element(electronCompileNote,true);
                        project_settingsList->add_gui_element(electronCompileUrl,true);
                    }
                    project_settingsList->process_self( view_space,cam);

                    if( buildPlatformRefreshLabel->is_clicked() )
                    {
                        refresh_build_to_source_targets();
                    }

                    if( buildToSource_button->is_clicked() && current_project!=nullptr && buildPlatformdropdown!=nullptr )
                    {
                        if( buildPlatformdropdown->get_selected_id() >= 0 && buildPlatformdropdown->get_menu_size() > 0 )
                        {
                            current_project->save_project();
                            current_project->export_project_osx("",buildPlatformdropdown->get_selected_name(),buildBits,false, osxDebugModeCheckbox->is_clicked(), osxNativeBuildTypeOptions->get_selected_value() );
                            //winnativeBuildTypeOptions->set_checked(false);
                        }
                        else
                        {
                            main_editor_log->log_build_error("Unable to build to Source for OSX: Method not selected!" );
                        }
                    }

                    if( osx64BitCheckbox->is_clicked() )
                    {
                        buildBits = 64;
                    }

                    if( exportPush_button->is_clicked() && current_project!=nullptr)
                    {
                        current_project->save_project();
                        current_project->export_project_osx("","",buildBits,false, osxDebugModeCheckbox->is_clicked(), osxNativeBuildTypeOptions->get_selected_value()  );
                        //osxnativeBuildTypeOptions->set_checked(false);
                    }
                    else if( gpe::system_found_os == gpe::system_os_mac && current_project!=nullptr )
                    {
                        if( exportAndPlayPush_button->is_clicked() )
                        {
                            export_and_play_native(true);
                        }
                        else if( playProgramPush_button->is_clicked() )
                        {
                            current_project->run_project("",gpe::system_os_mac,buildBits, osxDebugModeCheckbox->is_clicked() );
                        }
                    }
                }
                else
                {
                    project_settingsList->add_gui_element(otherPlatformLabel,true);
                    project_settingsList->add_gui_element(otherPlatformLabelParagraph,true );
                    project_settingsList->add_gui_element(otherPlatformLabelURL, true);
                    project_settingsList->process_self( view_space,cam);
                }
            }
        }
        else if( project_settingsBar->get_selected_name()=="Macros")
        {
            projectGameMacros->set_width(subViewedSpace.w-32 );
            projectGameMacros->set_height(subViewedSpace.h-96 );
            project_settingsList->add_gui_element(projectGameMacros,true);
            project_settingsList->process_self( view_space,cam);

        }
        else if( project_settingsBar->get_selected_name()=="Notes")
        {
            projectGameNotes->set_width(subViewedSpace.w-32 );
            projectGameNotes->set_height(subViewedSpace.h-96 );
            project_settingsList->add_gui_element(projectGameNotes,true);
            project_settingsList->process_self( view_space,cam);
        }
        else if( project_settingsBar->get_selected_name() == "Layers")
        {
            int iLayerN = 0;
            int widestTitle = 0;
            int iColMatrixColumn = 0;
            int maxCollisionLayerCount = 32;
            for( iLayerN = 0; iLayerN < 32; iLayerN++)
            {
                if( widestTitle < projectLayerNamesRowTitles[iLayerN]->get_width() )
                {
                    widestTitle = projectLayerNamesRowTitles[iLayerN]->get_width();
                }
            }
            projectLayerEmptyTitle->set_width(widestTitle);
            projectLayerEmptyTitle->autoResizes = false;
            collisionLayerMatrixList->clear_list();
            collisionLayerMatrixList->set_coords( subViewedSpace.x, subViewedSpace.y);
            collisionLayerMatrixList->set_width(subViewedSpace.w);
            collisionLayerMatrixList->set_height(subViewedSpace.h );
            collisionLayerMatrixList->barXPadding = pawgui::padding_default;
            collisionLayerMatrixList->barYPadding = pawgui::padding_default;
            collisionLayerMatrixList->barXMargin = pawgui::padding_default;
            collisionLayerMatrixList->barYMargin = pawgui::padding_default;

            //Important to manipulate view of layer matrix with rows/columns using headers
            collisionLayerMatrixList->add_gui_element( projectLayerEmptyTitle,false );
            for( iLayerN = 0; iLayerN < maxCollisionLayerCount; iLayerN++)
            {
                projectLayerNamesColumnTitles[iLayerN]->set_height(widestTitle);
                //projectLayerNamesColumnTitles[iLayerN]->set_width(16);
                collisionLayerMatrixList->add_gui_element(projectLayerNamesColumnTitles[iLayerN],(iLayerN==maxCollisionLayerCount-1) );
            }
            for( iLayerN = 0; iLayerN < maxCollisionLayerCount; iLayerN++)
            {
                projectLayerNamesRowTitles[iLayerN]->set_width(widestTitle);
                collisionLayerMatrixList->add_gui_element(projectLayerNamesRowTitles[iLayerN],false );
                for( iColMatrixColumn = 0; iColMatrixColumn < maxCollisionLayerCount-iLayerN; iColMatrixColumn++)
                {
                    collisionLayerMatrixList->add_gui_element(projectCollisionLayerMatrix[iLayerN*32+iColMatrixColumn],(iColMatrixColumn==maxCollisionLayerCount-1-iLayerN) );
                }
            }
            collisionLayerMatrixList->add_gui_element(selectAllCollisionBoxes, true);
            collisionLayerMatrixList->add_gui_element(unselectAllCollisionBoxes, true);
            collisionLayerMatrixList->add_gui_element(confirmResource_button,true);
            collisionLayerMatrixList->process_self( view_space,cam);

            bool renameLayer = false;
            for( int iLayerVal = 0; iLayerVal < 32 && renameLayer==false; iLayerVal++)
            {
                if( projectLayerNamesRowTitles[iLayerVal]!=nullptr && ( projectLayerNamesRowTitles[iLayerVal]->is_clicked() || projectLayerNamesRowTitles[iLayerVal]->is_rightclicked() ) )
                {
                    renameLayer = true;
                }
                else if( projectLayerNamesColumnTitles[iLayerVal]!=nullptr && ( projectLayerNamesColumnTitles[iLayerVal]->is_clicked() || projectLayerNamesColumnTitles[iLayerVal]->is_rightclicked() ) )
                {
                    renameLayer = true;
                }
                if( renameLayer)
                {
                    std::string newLayerName = pawgui::get_string_from_popup("Rename this project layer","Please enter an unique alphanumeric name",projectLayerNamesRowTitles[iLayerVal]->get_name() );
                    bool layerValueAllowed = true;
                    if( stg_ex::is_alnum(newLayerName,true,true) )
                    {
                        for( int jLayerVal = 0; jLayerVal < 32; jLayerVal++)
                        {
                            if( iLayerVal!=jLayerVal)
                            {
                                if( projectLayerNamesColumnTitles[jLayerVal] )
                                {
                                    if( newLayerName==projectLayerNamesColumnTitles[jLayerVal]->get_name() )
                                    {
                                        pawgui::display_user_alert("Invalid name given","Layer names must all be unique!");
                                        layerValueAllowed= false;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        layerValueAllowed = false;
                        pawgui::display_user_alert("Invalid name given","Layer names must be alphanumerical in value.");
                    }
                    if( layerValueAllowed)
                    {
                        current_project->projectLayerNames[iLayerVal] = newLayerName;
                        update_project_layers();
                    }
                    gpe::input->reset_all_input();
                }

            }

            if( !renameLayer )
            {
                if(selectAllCollisionBoxes!=nullptr && selectAllCollisionBoxes->is_clicked() )
                {
                    for( int iMatrix = 0; iMatrix < 1024; iMatrix++)
                    {
                        projectCollisionLayerMatrix[iMatrix]->set_checked(true);
                    }
                }
                else if( unselectAllCollisionBoxes!=nullptr && unselectAllCollisionBoxes->is_clicked() )
                {
                    for( int iMatrix = 0; iMatrix < 1024; iMatrix++)
                    {
                        projectCollisionLayerMatrix[iMatrix]->set_checked(false);
                    }
                }
            }
        }

        //project_settingsList->set_maxed_out_width();

        if( confirmResource_button->is_clicked() )
        {
            save_resource();
        }
        if( nExportOptionName != export_settingsBar->get_selected_name() )
        {
            //process_self(view_space,cam);
        }
    }
}

void projectPropertiesResource::refresh_build_to_source_targets()
{
    if( main_editor_log==nullptr)
    {
        gpe::error_log->report("Unable to detect [main_editor_log].");
        return;
    }
    main_editor_log->log_build_line("Attempting to refresh build platforms");
    main_editor_log->log_build_line("Build platforms refreshed!");

    if( buildPlatformdropdown==nullptr )
    {
        main_editor_log->log_build_error("Error finding [buildPlatformdropdown] object!");
        return;
    }

    if( GPE_CPP_BUILDER_SETTINGS==nullptr )
    {
        main_editor_log->log_build_error("Error finding [GPE_CPP_BUILDER_SETTINGS] global object!");
        return;
    }

    int currentDropDownPos = buildPlatformdropdown->get_selected_id();
    buildPlatformdropdown->clear_dropdown();

    gpeCPPBuildHolder * tempCBS = nullptr;
    int builderSystems = GPE_CPP_BUILDER_SETTINGS->cppBuildSystems.size();
    for( int i = 0; i <builderSystems ; i++)
    {
        tempCBS = GPE_CPP_BUILDER_SETTINGS->cppBuildSystems[i];
        if( tempCBS!=nullptr )
        {
            buildPlatformdropdown->add_menu_option(tempCBS->builderName,tempCBS->builderName, -1, false );
        }
    }
    main_editor_log->log_build_line("Added ["+ stg_ex::int_to_string(builderSystems)+"] builder systems");
}

void projectPropertiesResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if( cam!=nullptr && view_space!=nullptr)
    {
        if(  project_settingsBar!=nullptr && panel_main_editor==nullptr )
        {
            project_settingsBar->render_self( view_space,cam);
        }
        if(  export_settingsBar!=nullptr && project_settingsBar!=nullptr  && project_settingsBar->get_selected_name()=="Platforms" )
        {
            export_settingsBar->render_self( view_space,cam);
        }

        //Seperate render process since the layer is aligned differently then rest of menus
        if( project_settingsBar->get_selected_name()=="Layers")
        {
            collisionLayerMatrixList->render_self( view_space,cam );
        }
        else if( project_settingsList!=nullptr )
        {
            project_settingsList->render_self( view_space,cam);
        }
    }
}

void projectPropertiesResource::save_resource(std::string file_path, int backupId)
{
    bool usingAltSaveSource = false;
    isModified = false;
    if( projectParentFolder!=nullptr)
    {
        std::string project_settingsFileName = projectParentFolder->parentProjectDirectory+"/gpe_project/project_settings.gpf";
        gpe::error_log->report("Saving project settings to "+project_settingsFileName);
        std::ofstream newSaveDataFile (project_settingsFileName.c_str() );
        if (newSaveDataFile.is_open() )
        {
            write_header_on_file(&newSaveDataFile);
            //newSaveDataFile << "ResourceName=" << resource_name << "\n";
            newSaveDataFile << "#Project _settings     \n";
            newSaveDataFile << "ProjectIcon=" << stg_ex::get_local_from_global_file(iconFileName) << "\n";
            if( projectScreenWidthField!=nullptr)
            {
                newSaveDataFile << "ScreenWidth=" << projectScreenWidthField->get_held_number() << "\n";
            }
            if( projectScreenHeightField!=nullptr)
            {
                newSaveDataFile << "ScreenHeight=" << projectScreenHeightField->get_held_number() << "\n";
            }

            if( projectGameVersionField!=nullptr)
            {
                newSaveDataFile << "ProjectVersion=" << projectGameVersionField->get_held_number() << "\n";
            }
            if( projectGameSubVersionField!=nullptr)
            {
                newSaveDataFile << "ProjectSubVersion=" << projectGameSubVersionField->get_string() << "\n";
            }
            if( project_scale_settings!=nullptr)
            {
                newSaveDataFile << "Project_scale_settings=" << project_scale_settings->get_selected_id() << "\n";
            }

            if( project_settingsFPSRate!=nullptr)
            {
                newSaveDataFile << "ProjectFPSCap=" << project_settingsFPSRate->get_selected_id() << "\n";
            }

            newSaveDataFile << "#Project Publishing MetaData     \n";
            if( projectGameTitleField!=nullptr)
            {
                newSaveDataFile << "ProjectTitle=" << projectGameTitleField->get_string() << "\n";
            }
            if( projectGameShortTitleField!=nullptr)
            {
                newSaveDataFile << "ProjectShortTitle=" << projectGameShortTitleField->get_string() << "\n";
            }

            //Saves the project language
            if( projectLanguage!=nullptr )
            {
                newSaveDataFile << "ProjectLanguage=" << projectLanguage->get_selected_tag() << "\n";
                if( current_project!=nullptr )
                {
                    current_project->set_project_language( projectLanguage->get_selected_tag() );
                }
            }

            if( projectGameDateField!=nullptr)
            {
                newSaveDataFile << "ProjectDate=" << projectGameDateField->get_string() << "\n";
            }
            if( projectGameDescriptionField!=nullptr)
            {
                newSaveDataFile << "ProjectDescription=" << projectGameDescriptionField->get_string() << "\n";
            }
            if( projectGamePublisherField!=nullptr)
            {
                newSaveDataFile << "ProjectPublisher=" << projectGamePublisherField->get_string() << "\n";
            }
            if( projectGameDeveloperField!=nullptr)
            {
                newSaveDataFile << "ProjectDeveloper=" << projectGameDeveloperField->get_string() << "\n";
            }
            if( projectGameCopyrightField!=nullptr)
            {
                newSaveDataFile << "ProjectCopyright=" << projectGameCopyrightField->get_string() << "\n";
            }
            if( projectGameWebsiteField!=nullptr)
            {
                newSaveDataFile << "ProjectWebsite=" << projectGameWebsiteField->get_string() << "\n";
            }
            if( projectGameEmailField!=nullptr )
            {
                newSaveDataFile << "ProjectEmail=" << projectGameEmailField->get_string() << "\n";
            }

            //Colors
            newSaveDataFile << "#Project Colors     \n";
            if( projectBorderColor!=nullptr)
            {
                newSaveDataFile << "BorderColor=" << projectBorderColor->get_rgb_string() << "\n";
            }
            if( projectGameBackgroundColor!=nullptr)
            {
                newSaveDataFile << "GameBackgroundColor=" << projectGameBackgroundColor->get_rgb_string() << "\n";
            }
            if( projectWebsiteBackgroundColor!=nullptr)
            {
                newSaveDataFile << "WebsiteBackgroundColor=" << projectWebsiteBackgroundColor->get_rgb_string() << "\n";
            }
            if( projectTextHeaderColor!=nullptr)
            {
                newSaveDataFile << "HeaderColor=" << projectTextHeaderColor->get_rgb_string() << "\n";
            }
            if( projectTextParagraphColor!=nullptr)
            {
                newSaveDataFile << "ParagraphColor=" << projectTextParagraphColor->get_rgb_string() << "\n";
            }
            if( projectTextLinkColor!=nullptr)
            {
                newSaveDataFile << "LinkColor=" << projectTextLinkColor->get_rgb_string() << "\n";
            }

            if( projectTextLinkColor!=nullptr)
            {
                newSaveDataFile << "LinkActiveColor=" << projectTextLinkActiveColor->get_rgb_string() << "\n";
            }
            if( projectTextLinkColor!=nullptr)
            {
                newSaveDataFile << "LinkHoverColor=" << projectTextLinkHoverColor->get_rgb_string() << "\n";
            }
            if( projectTextLinkColor!=nullptr)
            {
                newSaveDataFile << "LinkVisitedColor=" << projectTextLinkVisitedColor->get_rgb_string() << "\n";
            }
            if( warnOnCloseCheckbox!=nullptr)
            {
                newSaveDataFile << "HTML5WarnOnClose=" << warnOnCloseCheckbox->is_clicked() << "\n";
            }

            //Project TextAreas
            if( projectGameMacros!=nullptr)
            {
                projectGameMacros->export_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_macros.gpf");
            }
            if( projectGameNotes!=nullptr)
            {
                projectGameNotes->export_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_notes.gpf");
            }

            newSaveDataFile.close();
        }

        //Saves the Project Layer Collision Matrix to a seperate file
        std::string projectCollisionMatrixFileName = projectParentFolder->parentProjectDirectory+"/gpe_project/project_layer_matrix.gpf";
        gpe::error_log->report("Saving project collision matrix to "+projectCollisionMatrixFileName);
        std::ofstream projectCollisionMatrixFile (projectCollisionMatrixFileName.c_str() );
        if (projectCollisionMatrixFile.is_open() )
        {
            write_header_on_file(&projectCollisionMatrixFile);
            //newSaveDataFile << "ResourceName=" << resource_name << "\n";
            projectCollisionMatrixFile << "#Project Layer Collision Matrix _settings     \n";
            int jLayerCol = 0;
            for( int iLayerRow = 0; iLayerRow < 32; iLayerRow++)
            {
                projectCollisionMatrixFile << "CollisionLayer["+ stg_ex::int_to_string(iLayerRow)+"]=";
                for(  jLayerCol = 0; jLayerCol < 32-iLayerRow; jLayerCol++)
                {
                    projectCollisionMatrixFile << stg_ex::int_to_string( projectCollisionLayerMatrix[iLayerRow*32+jLayerCol]->is_clicked() )+",";
                }
                projectCollisionMatrixFile << "\n";
            }
            projectCollisionMatrixFile.close();
        }
    }
}


int projectPropertiesResource::search_for_string(std::string needle)
{
    int foundstrings = 0;
    main_editor_log->log_general_comment("Searching [Project _settings]..");
    if( pawgui::main_anchor_controller!=nullptr)
    {
        pawgui::main_anchor_controller->searchResultProjectName = parentProjectName;
        pawgui::main_anchor_controller->searchResultResourceId = globalResouceIdNumber;
        pawgui::main_anchor_controller->searchResultResourceName = resource_name;
        if( projectGameMacros!=nullptr && projectGameMacros->has_content() )
        {
            foundstrings+=projectGameMacros->find_all_strings(needle,pawgui::main_search_controller->findMatchCase->is_clicked(),true,"Macros");
        }

        if( projectGameNotes!=nullptr && projectGameNotes->has_content() )
        {
            foundstrings+=projectGameNotes->find_all_strings(needle,pawgui::main_search_controller->findMatchCase->is_clicked(),true,"Notes");
        }
    }
    return foundstrings;
}

int projectPropertiesResource::search_and_replace_string(std::string needle, std::string newStr )
{
    int foundstrings = 0;
    int tempFoundCount = 0;

    main_editor_log->log_general_comment("Searching [Project _settings]..");
    if( pawgui::main_anchor_controller!=nullptr)
    {
        pawgui::main_anchor_controller->searchResultProjectName = parentProjectName;
        pawgui::main_anchor_controller->searchResultResourceId = globalResouceIdNumber;
        pawgui::main_anchor_controller->searchResultResourceName = resource_name;
        if( projectGameMacros!=nullptr && projectGameMacros->has_content() )
        {
            tempFoundCount = projectGameMacros->find_all_strings(needle,pawgui::main_search_controller->findMatchCase->is_clicked(),true,"Macros");
            if( tempFoundCount > 0)
            {
                foundstrings+=tempFoundCount;
                //displayMessagestring = "Replaced "+ stg_ex::int_to_string( projectGameMacros->replace_all_found(needle, newStr ) )+" copies.";
            }
        }

        if( projectGameNotes!=nullptr && projectGameNotes->has_content() )
        {
            tempFoundCount = projectGameNotes->find_all_strings(needle,pawgui::main_search_controller->findMatchCase->is_clicked(),true,"Notes");
            if( tempFoundCount > 0)
            {
                foundstrings+=tempFoundCount;
                //displayMessagestring = "Replaced "+ stg_ex::int_to_string( projectGameNotes->replace_all_found(needle, newStr ) )+" copies.";
            }
        }
    }
    return foundstrings;
}


void projectPropertiesResource::update_project_layers()
{
    if( current_project!=nullptr && current_project->RESC_SCENES!=nullptr)
    {
        int iLN = 0;
        for(  iLN = 0; iLN < 32; iLN++)
        {
            if( projectLayerNamesRowTitles[iLN]!=nullptr)
            {
                projectLayerNamesRowTitles[iLN]->set_name( current_project->projectLayerNames[iLN] );
            }
            if( projectLayerNamesColumnTitles[iLN]!=nullptr)
            {
                projectLayerNamesColumnTitles[iLN]->set_name( current_project->projectLayerNames[iLN] );
            }
        }

        int jLayerCol = 0;
        for(  iLN = 0; iLN < 32; iLN++)
        {
            for(  jLayerCol = 0; jLayerCol < 32-iLN; jLayerCol++)
            {
                projectCollisionLayerMatrix[iLN*32+jLayerCol]->descriptionText = "["+current_project->projectLayerNames[iLN]+" * "+current_project->projectLayerNames[jLayerCol]+"]";
            }
        }

        std::vector <pawgui::widget_resource_container *> projectScenes;
        pawgui::widget_resource_container * tempContainer = nullptr;
        gameSceneResource * tempScnRes = nullptr;
        current_project->RESC_SCENES->grab_useable_resources(projectScenes);
        for( int iRes = (int)projectScenes.size()-1; iRes >=0; iRes--)
        {
            tempContainer = projectScenes[iRes];
            if( tempContainer!=nullptr)
            {
                if( tempContainer!=nullptr && tempContainer->get_held_resource()!=nullptr)
                {
                    tempScnRes = (gameSceneResource * ) tempContainer->get_held_resource();
                    if( tempScnRes!=nullptr)
                    {
                        tempScnRes->update_project_layers();
                    }
                }
            }
        }
    }
}

bool projectPropertiesResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=nullptr)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            //*fileTarget << "Testing";
            return true;
        }
    }
    return false;
}
