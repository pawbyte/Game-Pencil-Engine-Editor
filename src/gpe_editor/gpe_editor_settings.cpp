/*
gpe_editor_settings.cpp
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

#include "gpe_editor_settings.h"

gamePencilEditorSettingsResource * main_EDITOR_SETTINGS = NULL;

GPE_Theme_Holder::GPE_Theme_Holder( std::string tName, std::string tFileName, bool isCustomTheme )
{
    theme_name = tName;
    templateFileName = tFileName;
    nameInEditor = new gpe_text_widget_string(tName,"");
    themeIconImg = new GPE_Label_Image("","");
    themeBgFileLocation = new gpe_text_widget_string("","");
    non_default_theme = isCustomTheme;
}

GPE_Theme_Holder::~GPE_Theme_Holder()
{
    clear_colors();
    if( nameInEditor!=NULL )
    {
        delete nameInEditor;
        nameInEditor = NULL;
    }
    if( themeIconImg!=NULL )
    {
        delete themeIconImg;
        themeIconImg = NULL;
    }
    if( themeBgFileLocation!=NULL )
    {
        delete themeBgFileLocation;
        themeBgFileLocation = NULL;
    }
}

void GPE_Theme_Holder::clear_colors()
{
    gpe_widget_color_picker * tColor = NULL;
    for( int i = (int)colorsInEditor.size()-1; i >=0; i-- )
    {
        tColor = colorsInEditor[i];
        if( tColor!=NULL )
        {
            delete tColor;
            tColor = NULL;
        }
    }
    colorsInEditor.clear();
}


bool GPE_Theme_Holder::copy_theme( GPE_Theme * systemTheme, bool copyToSelf )
{
    if( systemTheme == NULL)
    {
        gpe::error_log->report("Unable to find system theme...");
        return false;
    }
    gpe::color * fThemeColor = NULL;
    gpe_widget_color_picker * myColorInEditor = NULL;
    int jThemeColor = 0;
    int jThemeColorMax = 0;

    if( copyToSelf )
    {
        jThemeColorMax = systemTheme->get_color_count();
        clear_colors();
        non_default_theme = systemTheme->is_custom_theme();
        load_background( systemTheme->themeBgFileLocation );
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
            if( fThemeColor !=NULL )
            {
                myColorInEditor = new gpe_widget_color_picker(fThemeColor->get_name(),"" );
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
            if( myColorInEditor!=NULL )
            {
                fThemeColor = systemTheme->find_color( myColorInEditor->get_name() );
                if( fThemeColor!=NULL)
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

bool GPE_Theme_Holder::load_background( std::string bgTextureLocation, bool allOrNothing)
{
    std::string miniBGFileName = "";
    if( sff_ex::file_exists(bgTextureLocation ) && stg_ex::file_is_image(bgTextureLocation) )
    {
        if(themeIconImg!=NULL)
        {
            themeIconImg->load_label_image( bgTextureLocation );
            themeIconImg->set_height(64);
        }
        miniBGFileName = templateBgFileName = stg_ex::get_local_from_global_file( bgTextureLocation  );

        if( non_default_theme )
        {
            miniBGFileName = gpe::app_directory_name+"themes/custom/"+miniBGFileName;
        }
        else
        {
            miniBGFileName = gpe::app_directory_name+"themes/"+miniBGFileName;
        }
        sff_ex::file_copy( bgTextureLocation, miniBGFileName );
        templateBgFileName = miniBGFileName;
        themeBgFileLocation->set_string( miniBGFileName );
        return true;
    }
    else if( allOrNothing ==false )
    {
        // in case the file is loaded elsewhere, we see if we can find it on the local themes
        bgTextureLocation = stg_ex::get_local_from_global_file(bgTextureLocation);
        if( non_default_theme )
        {
            miniBGFileName = gpe::app_directory_name+"themes/custom/"+bgTextureLocation;
        }
        else
        {
            miniBGFileName = gpe::app_directory_name+"themes/"+bgTextureLocation;
        }
        if( sff_ex::file_exists(miniBGFileName ) && stg_ex::file_is_image(miniBGFileName) )
        {
            //performs "guaranteed recursion after finding local file"
            return load_background( miniBGFileName, true );
        }
    }
    return false;
}

gamePencilEditorSettingsResource::gamePencilEditorSettingsResource()
{
    confirmResourceButton = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/check.png","Confirm Changes","Confirm and Save Changes");
    cancelResourceButton =  new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/times.png","Cancel Changes","Cancel Changes and Revert to previous settings");

    isFullScreenResource = true;

    resourceFileName = "";
    resource_name = "Editor Settings";
    parentProjectName = "";
    parentProjectDirectory = "";

    sideAreaPanel = new GPE_SelectBoxBasic("Mode");
    sideAreaPanel->set_width(256);
    sideAreaPanel->set_option_height(64);
    sideAreaPanel->add_option("General",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cubes.png"),NULL,2, false, false);
    sideAreaPanel->add_option("External Editors",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/external-link-square.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Coding Languages",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/code-fork.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Themes",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/eye.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Advanced",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/stethoscope.png"),NULL,2, false, false);

    sidePanelRect = new gpe::shape_rect();

    CodingLanguageTitle = new GPE_Label_Title("Coding Language Editor","Modify how the editor reacts to your language" );
    CodingLanguageLabel = new GPE_Label_Text("Basic language syntax");
    CodingLanguageParagraph = new GPE_Label_Paragraph("Coding Language","Unsure how to add a new language to the editor? Click the link below to add a new language. Please note to use new languages you may need to reset the editor.");
    CodingLanguageURL = new GPE_TextURL("See our online guide for editing languages","Opens in browser","https://docs.pawbyte.com/");
    codingLanguageSelector = new GPE_SelectBoxBasic("CodingLanguages");
    codingLanguageSelector->set_width( 96 );
    codingLanguageSelector->add_option("JavaScript",PROGRAM_LANGUAGE_JS );
    codingLanguageSelector->add_option("C++",PROGRAM_LANGUAGE_CPP );
    codingLanguageSelector->add_option("C#",PROGRAM_LANGUAGE_CS );
    codingLanguageSelector->add_option("TypeScript",PROGRAM_LANGUAGE_TST );
    codingLanguageSelector->add_option("Haxe",PROGRAM_LANGUAGE_HAXE );
    codingLanguageSelector->add_option("Lua",PROGRAM_LANGUAGE_LUA );

    openEditorFolder = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder-open.png","Open Editor Folder...","Be very super careful!");
    openLocalSettingsFolder = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder-open.png","Open Settings Folder...","Be very super careful!");

    userInvertMouseScroll = new GPE_CheckBoxBasic("Invert Mouse Scroll","Inverts the direction the mouse scrolls in", false );
    autoSaveScreenshots = new GPE_CheckBoxBasic("Save screenshot on Print-Screen","Saves PrintScreen images to Screenshots Folder", true);
    makeMetaScreenshots = new GPE_CheckBoxBasic("Add Project Meta to Screenshots","Adds Project Name to screenshots", true);
    showShortProjectNames = new GPE_CheckBoxBasic("Show Shorter Project Names","Show Shorter Project Names[Recommended]", true);

    externalEditorsTitle = new GPE_Label_Title("External Editors","" );
    for( int i = 0; i < GPE_EXTERNAL_EDITOR_MAX; i++)
    {
        pencilExternalEditorsFile[i] = new gpe_text_widget_string("","");
        pencilExternalEditorsLoadButton[i] = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/binoculars.png","Find Editor..",-1,32);
    }
    pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->set_label("Image Editor");
    pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_AUD]->set_label("Audio Editor");
    pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_VID]->set_label("Video Editor");
    pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_FONT]->set_label("Font Editor");

    showStatupTipsBox = new GPE_CheckBoxBasic("Show Tips at Startup","Leave checked to automatically launch Tips at StartUp", true);
    launchLastProjectBox = new GPE_CheckBoxBasic("Open last project on startup","Open last project on startup", true);
    editorStatupTipsLabel = new GPE_Label_Title("Startup  Settings","Startup Settings");
    editorGuiSettingsLabel = new GPE_Label_Title("Editor & Gui Settings","Editor & Gui Settings");

    editorNormalDelayTime = new gpe_text_widget_number("Valid from 15 to 1000",true, 15, 1000);
    editorNormalDelayTime->set_label("User Input Delay Time: (15-1000 ms)[100 Recommended]");
    editorNormalDelayTime->set_number( main_GUI_SETTINGS->normalInputDelayTime );

    editorTextInputDelayTime = new gpe_text_widget_number("Valid from 15 to 1000",true, 15,1000);
    editorTextInputDelayTime->set_label("Input Field Delay Time: (15-1000 ms)[50 Recommended]");
    editorTextInputDelayTime->set_number( main_GUI_SETTINGS->textInputDelayTime );


    editorTextAreaDelayTime = new gpe_text_widget_number("Valid from 15 to 1000",true, 15, 1000);
    editorTextAreaDelayTime->set_label("Text Area KeyDelay Time: (15-1000 ms)[50 Recommended]");
    editorTextAreaDelayTime->set_number( main_GUI_SETTINGS->textAreaDelayTime );

    editorCursorBlinkDelayTime = new gpe_text_widget_number("Valid from 15 to 6000",true, 15, 6000);
    editorCursorBlinkDelayTime->set_label("Cursor Blink Time: (15-6000 ms)[300 Recommended]");
    editorCursorBlinkDelayTime->set_number( main_GUI_SETTINGS->cursorBlinkTime );

    advancedAreaLabel = new GPE_Label_Title("Advanced","Advanged Editor Settings");
    forceFrameRedraw = new GPE_CheckBoxBasic("Redraw every frame[Not reccommended]","Redraws the Editor every frame regardless of user input", false);
    showHiddenFilesInBrowser = new GPE_CheckBoxBasic("Show Hidden Files in Browser","(Linux only feature", true);
    clearCacheButton = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/close.png","Clear Font Cache","Clears the cache of pre-rendered texts");
    ideSettingsFPSRateLabel = new GPE_Label_Title("IDE Frame Rate","Sets the (max) framerate set for the IDE(Usually 30 or 60)");
    ideFPSRatioLabel = new GPE_Label_Text( stg_ex::float_to_string( gpe::fps_ratio*100)+"%","The FPS Ratio compared to 30 FPS");
    ideSettingsFPSRate = new GPE_DropDown_Menu( "FPS Cap",true);
    ideSettingsFPSRate->add_menu_option("20 FPS","",20);
    ideSettingsFPSRate->add_menu_option("24 FPS","",24);
    ideSettingsFPSRate->add_menu_option("25 FPS","",25);
    ideSettingsFPSRate->add_menu_option("30 FPS","",30,true);
    ideSettingsFPSRate->add_menu_option("45 FPS","",45);
    ideSettingsFPSRate->add_menu_option("48 FPS","",48);
    ideSettingsFPSRate->add_menu_option("50 FPS","",50);
    ideSettingsFPSRate->add_menu_option("55 FPS","",55);
    ideSettingsFPSRate->add_menu_option("58 FPS","",58);
    ideSettingsFPSRate->add_menu_option("59 FPS","",59);
    ideSettingsFPSRate->add_menu_option("60 FPS","",60);
    ideSettingsFPSRate->add_menu_option("72 FPS","",72);
    ideSettingsFPSRate->add_menu_option("88 FPS","",88);
    ideSettingsFPSRate->add_menu_option("89 FPS","",89);
    ideSettingsFPSRate->add_menu_option("90 FPS","",90);
    ideSettingsFPSRate->add_menu_option("100 FPS","",100);
    ideSettingsFPSRate->add_menu_option("125 FPS","",125);
    ideSettingsFPSRate->add_menu_option("150 FPS","",150);
    ideSettingsFPSRate->add_menu_option("200 FPS","",200);
    ideSettingsFPSRate->add_menu_option("225 FPS","",225);
    ideSettingsFPSRate->add_menu_option("250 FPS","",250);
    ideSettingsFPSRate->add_menu_option("275 FPS","",275);
    ideSettingsFPSRate->add_menu_option("300 FPS","",300);
    ideSettingsFPSRate->add_menu_option("400 FPS","",400);
    ideSettingsFPSRate->add_menu_option("500 FPS","",500);
    ideSettingsFPSRate->add_menu_option("600 FPS","",600);
    ideSettingsFPSRate->add_menu_option("700 FPS","",700);
    ideSettingsFPSRate->add_menu_option("800 FPS","",800);
    ideSettingsFPSRate->add_menu_option("900 FPS","",900);
    ideSettingsFPSRate->add_menu_option("1000 FPS","",1000);
    showFPSOnEditor = new GPE_CheckBoxBasic("Show FPS Counter","Show FPS Counter", gpe::fps_show_counter );

    ideButtonBarSizeLabel= new GPE_Label_Text("Buttonbar Size","Changes the size of the buttonbar");
    ideButtonBarSize = new GPE_DropDown_Menu( "Buttonbar size",true);
    ideButtonBarSize->add_menu_option("Small","",24);
    ideButtonBarSize->add_menu_option("Medium","",32);
    ideButtonBarSize->add_menu_option("Large","",48);
    ideButtonBarSize->add_menu_option("Invisible","",0);
    ideButtonBarAlignment = new GPE_RadioButtonControllerBasic("Buttonbar Alignment");
    ideButtonBarAlignment->add_menu_option("Left","Aligns to the left size of window",0,true);
    ideButtonBarAlignment->add_menu_option("Center","Aligns to the center size of window",1,false);
    ideButtonBarAlignment->add_menu_option("Right","Aligns to the right size of window",2,false);
    projectAutoSaveRateLabel = new GPE_Label_Text("Auto Save Rate","Sets how often the editor auto-saves your projects");
    projectAutoSaveRate = new GPE_DropDown_Menu( "Auto Save Rate",true);
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

    renderSceneBGColor = new GPE_CheckBoxBasic("Preview Scene Background Color","Use to display scene bg color instead of transparent tiles in scene editor", false);

    //Themes Section
    currentThemeInEdit = 0;
    themesLabel = new GPE_Label_Title("Themes");
    themeBgLabel = new GPE_Label_Text("Theme Background:    ");
    themeBgBrowseButton = new GPE_ToolLabelButton( "Browse...","Browse for theme background...");
    themePicker = new GPE_DropDown_Menu("Themes", true );
    themeLoadButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder.png","Load Custom Theme");
    themeAddButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/plus.png","Create Custom Theme");
    themeRemoveButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/minus.png","Remove Custom Theme");
    themeSetDefaultButton = new GPE_ToolLabelButton( "Set as default theme","Set as default theme");
    itchLinkForThemes = new GPE_TextURL("Find Themes on Itch.io","Browse the GPE Themes category on itch.io","https://itch.io/game-assets/tag-gamepencil/tag-theme");
    defaultTemplateEndPos = (int)editor_theme_controller->theme_list.size();

    GPE_Theme_Holder * newLoadedTheme = NULL;
    GPE_Theme * currentThemeBeingLoaded = NULL;
    for( int iTheme = 0; iTheme < defaultTemplateEndPos; iTheme++ )
    {
        currentThemeBeingLoaded = editor_theme_controller->theme_list[iTheme];
        if( currentThemeBeingLoaded !=NULL )
        {
            newLoadedTheme = new GPE_Theme_Holder( currentThemeBeingLoaded->theme_name, currentThemeBeingLoaded->theme_local_location );

            themePicker->add_menu_option( currentThemeBeingLoaded->theme_name,currentThemeBeingLoaded->theme_local_location, (int)themeHolders.size(), (iTheme==0) );
            newLoadedTheme->copy_theme( currentThemeBeingLoaded );
            themeHolders.push_back( newLoadedTheme );
        }
    }
    load_themes_from_folder( gpe::app_directory_name+"themes");
    load_themes_from_folder( gpe::app_directory_name+"themes/custom");
    //Tab Section
    tabSectionLabel = new GPE_Label_Title("Tabs");
    mouseAutoFindTabs = new GPE_CheckBoxBasic("Mouse detect tabs");
    tabSpaceSize = new gpe_text_widget_number("Range: 1 to 16",true,1,16);
    tabSpaceSize->set_number(4);
    tabSpaceSize->set_label("Tab SIZE in spaces:");
    main_GUI_SETTINGS->tabSpaceCount = 4;

    editorPageList = new GPE_GuiElementList();
    editorPageList->panelAlignType = GPE_PANEL_ALIGN_FULL_LEFT;
    editorPageList->barXPadding = GENERAL_GPE_GUI_PADDING;
    editorPageList->barXMargin = 0;

    subViewedSpace.x = 0;
    subViewedSpace.y = 0;
    subViewedSpace.w = gpe::screen_width;
    subViewedSpace.h = gpe::screen_height;

    subEditorSelectBox = NULL; //Now stop crashing until I add your feature! :-)

}

gamePencilEditorSettingsResource::~gamePencilEditorSettingsResource()
{
    if( openEditorFolder!=NULL)
    {
        delete openEditorFolder;
        openEditorFolder = NULL;
    }
    if( openLocalSettingsFolder!=NULL)
    {
        delete openLocalSettingsFolder;
        openLocalSettingsFolder = NULL;
    }
    if( themeSetDefaultButton!=NULL)
    {
        delete themeSetDefaultButton;
        themeSetDefaultButton = NULL;
    }

    if( themeBgLabel!=NULL)
    {
        delete themeBgLabel;
        themeBgLabel = NULL;
    }

    if( themeBgBrowseButton!=NULL)
    {
        delete themeBgBrowseButton;
        themeBgBrowseButton = NULL;
    }

    if( itchLinkForThemes!=NULL)
    {
        delete itchLinkForThemes;
        itchLinkForThemes = NULL;
    }

    if( editorPageList!=NULL)
    {
        editorPageList->clear_list();
        delete editorPageList;
        editorPageList = NULL;
    }

    if( editorStatupTipsLabel!=NULL)
    {
        delete editorStatupTipsLabel;
        editorStatupTipsLabel = NULL;
    }

    if( clearCacheButton!=NULL)
    {
        delete clearCacheButton;
        clearCacheButton = NULL;
    }

    if( ideSettingsFPSRate!=NULL)
    {
        delete ideSettingsFPSRate;
        ideSettingsFPSRate = NULL;
    }

    if( ideSettingsFPSRateLabel!=NULL)
    {
        delete ideSettingsFPSRateLabel;
        ideSettingsFPSRateLabel = NULL;
    }
    if( ideFPSRatioLabel!=NULL)
    {
        delete ideFPSRatioLabel;
        ideFPSRatioLabel = NULL;
    }

    if( ideButtonBarSize!=NULL)
    {
        delete ideButtonBarSize;
        ideButtonBarSize = NULL;
    }
    if( ideButtonBarAlignment!=NULL)
    {
        delete ideButtonBarAlignment;
        ideButtonBarAlignment = NULL;
    }

    if( ideButtonBarSizeLabel!=NULL)
    {
        delete ideButtonBarSizeLabel;
        ideButtonBarSizeLabel = NULL;
    }

    if( editorGuiSettingsLabel!=NULL)
    {
        delete editorGuiSettingsLabel;
        editorGuiSettingsLabel = NULL;
    }

    if( editorTextAreaDelayTime!=NULL)
    {
        delete editorTextAreaDelayTime;
        editorTextAreaDelayTime = NULL;
    }
    if( editorTextInputDelayTime!=NULL)
    {
        delete editorTextInputDelayTime;
        editorTextInputDelayTime = NULL;
    }
    if( editorNormalDelayTime!=NULL)
    {
        delete editorNormalDelayTime;
        editorNormalDelayTime = NULL;
    }

    if( advancedAreaLabel!=NULL)
    {
        delete advancedAreaLabel;
        advancedAreaLabel = NULL;
    }

    if( showHiddenFilesInBrowser!=NULL)
    {
        delete showHiddenFilesInBrowser;
        showHiddenFilesInBrowser = NULL;
    }
    if( forceFrameRedraw!=NULL)
    {
        delete forceFrameRedraw;
        forceFrameRedraw = NULL;
    }

    if( subEditorSelectBox!=NULL)
    {
        delete subEditorSelectBox;
        subEditorSelectBox = NULL;
    }
    if( renderSceneBGColor!=NULL)
    {
        delete renderSceneBGColor;
        renderSceneBGColor = NULL;
    }

    gpe_text_widget_string * tempInput = NULL;
    GPE_ToolIconButton * tempIcon = NULL;
    if( externalEditorsTitle!=NULL)
    {
        delete externalEditorsTitle;
        externalEditorsTitle = NULL;
    }

    for( int i =GPE_EXTERNAL_EDITOR_MAX-1; i>=0; i--)
    {
        if( pencilExternalEditorsFile[i]!=NULL)
        {
            tempInput = pencilExternalEditorsFile[i];
            if( tempInput!=NULL)
            {
                delete tempInput;
                tempInput = NULL;
            }
            pencilExternalEditorsFile[i] = NULL;
        }
        if( pencilExternalEditorsLoadButton[i]!=NULL)
        {
            tempIcon = pencilExternalEditorsLoadButton[i];
            if( tempIcon!=NULL)
            {
                delete tempIcon;
                tempIcon = NULL;
            }
            pencilExternalEditorsLoadButton[i] = NULL;
        }
    }
}

bool gamePencilEditorSettingsResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}

void gamePencilEditorSettingsResource::load_themes_from_folder( std::string themeFolder )
{
    //Used for searching directories
    int iFile;
    std::string fileNameDiscovered = "";
    gpe::GPE_FileDirectory * dir = new gpe::GPE_FileDirectory();
    gpe::GPE_File * file = NULL;
    GPE_Theme_Holder * newLoadedTheme;
    GPE_Theme * newSystemTheme;
    if( dir!=NULL)
    {
        dir->open_directory(themeFolder);
        for (iFile = 0; iFile < (int)dir->get_count(); iFile++)
        {
            file = dir->get_file(iFile);
            if( file!=NULL)
            {
                fileNameDiscovered = file->get_name();
                if( fileNameDiscovered!="." && fileNameDiscovered!="..")
                {
                    if( file->is_directory()==false && file->get_type()=="gpf" )
                    {
                        newSystemTheme = editor_theme_controller->add_theme( fileNameDiscovered, true );
                        newSystemTheme->load_theme( themeFolder+"/"+fileNameDiscovered );
                        newLoadedTheme = new GPE_Theme_Holder( newSystemTheme->theme_name, newSystemTheme->theme_local_location );
                        themePicker->add_menu_option( newSystemTheme->theme_name,newSystemTheme->theme_local_location, (int)themeHolders.size(), false );
                        newLoadedTheme->copy_theme(newSystemTheme );
                        themeHolders.push_back( newLoadedTheme );
                    }
                }
            }
        }
        delete dir;
        dir = NULL;
    }
}

void gamePencilEditorSettingsResource::prerender_self( )
{

}

void gamePencilEditorSettingsResource::load_resource(std::string file_path)
{
    //showStatupTipsBox->set_clicked( editor_gui_main->showTipsAtStartUp );
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Loading Editor Settings", "Please wait..." );
    }

    std::string otherColContainerName = "";

    std::string newFileIn =  gpe::get_user_settings_folder()+"gpe_ide_local_settings.txt";
    std::ifstream gameResourceFileIn( newFileIn.c_str() );

    gpe::error_log->report("Loading Local settings - "+newFileIn);
    GPE_Theme * foundDefaultTheme = NULL;
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
                else if( foundFileVersion <= 2)
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

                            //gpe::error_log->report("Detected Key:     "+keyString);
                            if( keyString=="DefaultTheme" && editor_theme_controller!=NULL )
                            {
                                foundDefaultTheme = editor_theme_controller->find_theme( valString);
                                if( foundDefaultTheme!=NULL )
                                {
                                    theme_main = foundDefaultTheme;
                                }
                            }
                            if( keyString=="ExternalImageEditor")
                            {
                                pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->set_string(valString);
                            }
                            else if( keyString=="ExternalAudioEditor")
                            {
                                pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_AUD]->set_string(valString);
                            }
                            else if( keyString=="ExternalVideoEditor")
                            {
                                pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_VID]->set_string(valString);
                            }
                            else if( keyString=="ExternalFontEditor")
                            {
                                pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_FONT]->set_string(valString);
                            }
                            else if( keyString=="AutoSaveScreenshots")
                            {
                                autoSaveScreenshots->set_clicked( stg_ex::string_to_bool(valString ) );
                            }
                            else if( keyString=="InvertMouseScroll")
                            {
                                userInvertMouseScroll->set_clicked( stg_ex::string_to_bool(valString ) );
                                gpe::input->mouse_scroll_inverted = userInvertMouseScroll->is_clicked();
                            }
                            else if( keyString=="PreviewSceneBGColor")
                            {
                                renderSceneBGColor->set_clicked( stg_ex::string_to_bool(valString ) );
                            }
                            else if( keyString=="MakeMetaScreenshots")
                            {
                                makeMetaScreenshots->set_clicked( stg_ex::string_to_bool(valString ) );
                            }
                            else if( keyString=="ShowShortProjectNames")
                            {
                                main_GUI_SETTINGS->useShortProjectNames = stg_ex::string_to_bool(valString );
                                showShortProjectNames->set_clicked(  main_GUI_SETTINGS->useShortProjectNames );
                            }
                            else if( keyString=="LaunchLastProject")
                            {
                                launchLastProjectBox->set_clicked( stg_ex::string_to_bool(valString ) );
                            }
                            else if( keyString=="ShowFPSCounter")
                            {
                                gpe::fps_show_counter = stg_ex::string_to_bool(valString);
                                showFPSOnEditor->set_clicked( gpe::fps_show_counter );
                            }
                            else if( keyString=="ShowTipsAtStart")
                            {
                                showStatupTipsBox->set_clicked( stg_ex::string_to_bool(valString ) );
                            }
                            else if( keyString=="BrowseHiddenFiles")
                            {
                                showHiddenFilesInBrowser->set_clicked( stg_ex::string_to_bool(valString ) );
                            }
                            else if( keyString=="ProjectFolderList" )
                            {
                                projectFolderListLocation = valString;
                            }
                            else if( keyString=="NormalDelayMS")
                            {
                                foundDelayTime = stg_ex::string_to_int(valString, 60 );
                                if( foundDelayTime <= 15)
                                {
                                    foundDelayTime = 15;
                                }
                                else if( foundDelayTime > 1000)
                                {
                                    foundDelayTime = 1000;
                                }
                                main_GUI_SETTINGS->normalInputDelayTime = foundDelayTime;
                                editorNormalDelayTime->set_number( foundDelayTime );
                            }
                            else if( keyString=="InputDelayMS")
                            {
                                foundDelayTime = stg_ex::string_to_int(valString, 30 );
                                if( foundDelayTime <= 15 )
                                {
                                    foundDelayTime = 15;
                                }
                                else if( foundDelayTime > 1000)
                                {
                                    foundDelayTime = 1000;
                                }
                                main_GUI_SETTINGS->textInputDelayTime = foundDelayTime;
                                editorTextInputDelayTime->set_number( foundDelayTime );
                            }
                            else if( keyString=="TextAreaDelayMS")
                            {
                                foundDelayTime = stg_ex::string_to_int(valString, 30 );
                                if( foundDelayTime <= 15 )
                                {
                                    foundDelayTime = 15;
                                }
                                else if( foundDelayTime > 1000 )
                                {
                                    foundDelayTime = 1000;
                                }
                                main_GUI_SETTINGS->textAreaDelayTime = foundDelayTime;
                                editorTextAreaDelayTime->set_number( foundDelayTime );
                            }
                            else if( keyString=="CursorBlinkDelayMS")
                            {
                                foundDelayTime = stg_ex::string_to_int(valString, 300 );
                                if( foundDelayTime <= 15 )
                                {
                                    foundDelayTime = 15;
                                }
                                else if( foundDelayTime > 6000 )
                                {
                                    foundDelayTime = 6000;
                                }
                                main_GUI_SETTINGS->cursorBlinkTime = foundDelayTime;
                                editorCursorBlinkDelayTime->set_number( foundDelayTime );
                            }
                            else if( keyString=="FPS")
                            {
                                foundFPSValue = stg_ex::string_to_int(valString,30);
                                foundFPSValue = std::max( foundFPSValue, 30);
                                foundFPSValue = std::min( foundFPSValue, 300);
                                gpe::time_keeper->set_fps(foundFPSValue);
                                ideSettingsFPSRate->set_option_value(foundFPSValue);
                            }
                            else if( keyString=="ButtonBarSize")
                            {
                                ideButtonBarSize->set_option_value( stg_ex::string_to_int( valString) );
                            }
                            else if( keyString=="ButtonBarAlignment")
                            {
                                ideButtonBarAlignment->set_selection( stg_ex::string_to_int( valString) );
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
            if( theme_main!=NULL && themePicker!=NULL )
            {
                themePicker->set_option_named( theme_main->theme_name );
            }
        }
    }
}

void gamePencilEditorSettingsResource::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    cam = gpe::camera_find(cam);
    viewedSpace = gpe::camera_find(viewedSpace);
    if( cam!=NULL && editorPageList!=NULL && sideAreaPanel!=NULL && viewedSpace!=NULL)
    {
        int prevTab = sideAreaPanel->get_selection();
        if( PANEL_GENERAL_EDITOR!=NULL )
        {
            subViewedSpace.x = 0;
            subViewedSpace.y = 0;
            subViewedSpace.w = viewedSpace->w;
            subViewedSpace.h = viewedSpace->h;
            PANEL_GENERAL_EDITOR->add_gui_element_fullsize( sideAreaPanel );
            PANEL_GENERAL_EDITOR->process_self();

        }
        else
        {
            sideAreaPanel->set_coords(0, 0 );
            sideAreaPanel->set_height( viewedSpace->h );
            sideAreaPanel->process_self( viewedSpace, cam );
            subViewedSpace.x = sideAreaPanel->get_x2pos();
            subViewedSpace.y = 0;
            subViewedSpace.w = viewedSpace->w - sideAreaPanel->get_width();
            subViewedSpace.h = viewedSpace->h;
        }

        if( prevTab!=sideAreaPanel->get_selection() )
        {
            editorPageList->reset_self();
        }

        editorPageList->clear_list();
        editorPageList->set_coords( subViewedSpace.x, subViewedSpace.y );
        editorPageList->set_width(subViewedSpace.w);
        editorPageList->set_height(subViewedSpace.h );
        editorPageList->barXPadding = GENERAL_GPE_GUI_PADDING;
        editorPageList->barXMargin = GENERAL_GPE_GUI_PADDING;
        editorPageList->hAlign = gpe::fa_center;
        int i = 0;
        GPE_Theme_Holder * currentThemeBeingEdited = NULL;
        int iColor = 0, iThemeColorMax = 0;
        if(sideAreaPanel->get_selected_name()=="General")
        {
            //editorPageList->add_gui_element(allowAutoSaveBox,true);
            editorPageList->add_gui_element(editorGuiSettingsLabel,true);
            editorPageList->add_gui_element(autoSaveScreenshots,true);
            editorPageList->add_gui_element(makeMetaScreenshots,true);
            editorPageList->add_gui_element(showShortProjectNames,true);
            editorPageList->add_gui_element(renderSceneBGColor,true);
            editorPageList->add_gui_element(projectAutoSaveRateLabel,true);
            editorPageList->add_gui_element(projectAutoSaveRate,true);

            editorPageList->add_gui_element(ideButtonBarAlignment,true);
            editorPageList->add_gui_element(ideButtonBarSizeLabel,true);
            editorPageList->add_gui_element(ideButtonBarSize,true);

            editorPageList->add_gui_element(ideSettingsFPSRateLabel,true);
            editorPageList->add_gui_element(ideSettingsFPSRate,true);
            ideFPSRatioLabel->set_name("FPS Ratio: "+ stg_ex::float_to_string( gpe::fps_ratio*100)+"%");
            editorPageList->add_gui_element(ideFPSRatioLabel,true);
            editorPageList->add_gui_element(showFPSOnEditor,true);


            editorPageList->add_gui_element(editorStatupTipsLabel,true);
            editorPageList->add_gui_element(showStatupTipsBox,true);
            editorPageList->add_gui_element(launchLastProjectBox,true);

            editorPageList->add_gui_element(tabSectionLabel,true);
            editorPageList->add_gui_element(mouseAutoFindTabs,true);
            editorPageList->add_gui_element(tabSpaceSize,true);

            editorPageList->add_gui_element(confirmResourceButton,false);
            editorPageList->add_gui_element(cancelResourceButton,true);
        }
        else if( sideAreaPanel->get_selected_name()=="Colors" || sideAreaPanel->get_selected_name()=="Themes" )
        {
            editorPageList->hAlign = gpe::fa_center;
            editorPageList->add_gui_element(themesLabel,true);

            editorPageList->add_gui_element(themeLoadButton,false);
            editorPageList->add_gui_element(themeAddButton,false);
            editorPageList->add_gui_element(themeRemoveButton,false);
            editorPageList->add_gui_element(themePicker,false);
            editorPageList->add_gui_element(themeSetDefaultButton,true);
            editorPageList->add_gui_element(itchLinkForThemes,true);

            //Dynamic theme data
            if( currentThemeInEdit >=0 && currentThemeInEdit < (int)themeHolders.size() )
            {
                currentThemeBeingEdited = themeHolders[currentThemeInEdit];
            }

            //256 is an estimated width of a color picker with a label...
            /*int colorsPerLine = (subViewedSpace.w-GENERAL_GPE_GUI_PADDING*4)/256;
            int colorsInLine = 0;*/
            bool goToNextColorLine = false;

            if( currentThemeBeingEdited!=NULL )
            {
                editorPageList->add_gui_element( themeBgLabel, false );
                editorPageList->add_gui_element( currentThemeBeingEdited->themeIconImg, false );
                editorPageList->add_gui_element( currentThemeBeingEdited->themeBgFileLocation, false );
                editorPageList->add_gui_element( themeBgBrowseButton, true );
                iThemeColorMax =  (int)currentThemeBeingEdited->colorsInEditor.size();
                for( iColor = 0; iColor < iThemeColorMax; iColor++ )
                {
                    /*
                    colorsInLine++;
                    if( colorsInLine >= colorsPerLine || iColor==iThemeColorMax-1 )
                    {
                        goToNextColorLine = true;
                        colorsInLine = 0;
                    }
                    else
                    {
                        goToNextColorLine = false;
                    }
                    editorPageList->add_gui_element(currentThemeBeingEdited->colorsInEditor[iColor] , goToNextColorLine );
                    */
                    editorPageList->add_gui_element(currentThemeBeingEdited->colorsInEditor[iColor], true );
                }
            }

            editorPageList->add_gui_element(confirmResourceButton,false);
            editorPageList->add_gui_element(cancelResourceButton,true);
        }
        else if( sideAreaPanel->get_selected_name()=="External Editors")
        {
            editorPageList->hAlign = gpe::fa_center;
            editorPageList->add_gui_element( externalEditorsTitle, true );
            for( i = 0; i < GPE_EXTERNAL_EDITOR_MAX; i++)
            {
                editorPageList->add_gui_element(pencilExternalEditorsFile[i],false);
                editorPageList->add_gui_element(pencilExternalEditorsLoadButton[i],true);
            }
            editorPageList->add_gui_element(confirmResourceButton,true);
        }
        else if( sideAreaPanel->get_selected_name()=="Coding Languages")
        {
            codingLanguageSelector->set_coords(subViewedSpace.x, 0 );
            codingLanguageSelector->set_height(subViewedSpace.h);
            codingLanguageSelector->optionIconWidth = 0;
            codingLanguageSelector->process_self(viewedSpace,cam);
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
            editorPageList->add_gui_element(clearCacheButton,true);
            editorPageList->add_gui_element(showHiddenFilesInBrowser,true);
            editorPageList->add_gui_element(forceFrameRedraw,true);

            editorPageList->add_gui_element(editorNormalDelayTime,true);
            editorPageList->add_gui_element(editorTextAreaDelayTime,true);
            editorPageList->add_gui_element(editorTextInputDelayTime,true);
            editorPageList->add_gui_element(editorCursorBlinkDelayTime,true);

            editorPageList->add_gui_element(userInvertMouseScroll,true);

            editorPageList->add_gui_element(confirmResourceButton,false);
            editorPageList->add_gui_element(cancelResourceButton,true);
        }
        else
        {
            editorPageList->add_gui_element(CodingLanguageTitle,true);
            editorPageList->add_gui_element(CodingLanguageLabel,true);
            editorPageList->add_gui_element(CodingLanguageParagraph,true);
            editorPageList->add_gui_element(CodingLanguageURL,true);
        }

        editorPageList->process_self( viewedSpace,cam);

        //Process results of last process...
        if( sideAreaPanel->get_selected_name()=="Colors" || sideAreaPanel->get_selected_name()=="Themes" )
        {
            bool themeChanged = false;
            if( themePicker!=NULL )
            {
                int potentiallyselectedTheme = themePicker->get_selected_value();
                if( currentThemeInEdit!= potentiallyselectedTheme && potentiallyselectedTheme >=0 && potentiallyselectedTheme < (int)themeHolders.size() )
                {
                    currentThemeInEdit = potentiallyselectedTheme;
                    themeChanged = true;
                    /*input->reset_all_input();
                    input->input_received = true;
                    process_self( viewedSpace, cam );
                    return;
                    */
                }
            }

            if( !themeChanged )
            {
                if( themeSetDefaultButton!=NULL && themeSetDefaultButton->is_clicked() )
                {
                    if( currentThemeInEdit >=0 && currentThemeInEdit < (int)editor_theme_controller->theme_list.size() )
                    {
                        theme_main = editor_theme_controller->theme_list[currentThemeInEdit];
                        sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_ide_local_settings.txt","DefaultTheme="+theme_main->theme_name );

                    }
                }
                else if( themeAddButton!=NULL && themeAddButton->is_clicked() )
                {
                    //Adds new theme if its a unique name
                    std::string newtheme_name = get_string_from_popup("New Theme name","Only unique names allowed");
                    if( newtheme_name.size() > 0 && newtheme_name!="default" )
                    {
                        if( editor_theme_controller->find_theme( newtheme_name)==NULL )
                        {
                            GPE_Theme * newGPETheme = editor_theme_controller->add_theme( newtheme_name, true );
                            if( newGPETheme !=NULL )
                            {
                                //Now adds it to our editor here...
                                GPE_Theme_Holder * newThemeHolder =  new GPE_Theme_Holder(newtheme_name, newtheme_name+".gpf" );
                                newThemeHolder->copy_theme( newGPETheme, true );
                                themePicker->add_menu_option( newtheme_name+".gpf", newtheme_name+".gpf", (int)themeHolders.size(), true );
                                themeHolders.push_back( newThemeHolder );
                                newGPETheme->save_theme();
                            }
                        }
                        else
                        {
                            display_user_alert("Theme creation stopped","Sorry, but you must use a unique name for this theme.");
                        }
                    }
                }
                else if( currentThemeBeingEdited!=NULL && themeBgBrowseButton!=NULL && themeBgBrowseButton->is_clicked() )
                {
                    if( (int)editor_gui_main->fileOpenProjectIconDir.size()==0 && current_project!=NULL )
                    {
                        editor_gui_main->fileOpenProjectIconDir = current_project->get_project_directory();
                    }
                    std::string newThemeBGFileName = GPE_GetOpenFileName("Find Theme Background","Image",editor_gui_main->fileOpenProjectIconDir );
                    currentThemeBeingEdited->load_background( newThemeBGFileName );
                }

                if( confirmResourceButton!=NULL && confirmResourceButton->is_clicked() )
                {
                    if( currentThemeBeingEdited!=NULL )
                    {
                        GPE_Theme * fThemeToChange = editor_theme_controller->find_theme( currentThemeBeingEdited->theme_name );
                        if( fThemeToChange !=NULL)
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
            if( clearCacheButton->is_clicked() )
            {
                if( GPE_Display_Basic_Prompt("Clear Font Cache?","Are you sure you will like to clear the font cache? Current Size: "+ stg_ex::int_to_string(gpe::gfs->get_font_cache_size() ) +" textures. This operation may be time consuming",false)==DISPLAY_QUERY_YES)
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
                if( pencilExternalEditorsLoadButton[i]->is_clicked() )
                {
                    std::string new_file_name = GPE_GetOpenFileName("Find External File Editor","",main_GUI_SETTINGS->fileOpenDefaultDir);
                    if( sff_ex::file_exists(new_file_name) )
                    {
                        pencilExternalEditorsFile[i]->set_string(new_file_name);
                    }
                }
            }
        }

        if( confirmResourceButton!=NULL && confirmResourceButton->is_clicked() )
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
            main_GUI_SETTINGS->textAreaDelayTime = foundDelTime;
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
            main_GUI_SETTINGS->useShortProjectNames = showShortProjectNames->is_clicked();
            main_GUI_SETTINGS->textInputDelayTime = foundDelTime;
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

            main_GUI_SETTINGS->normalInputDelayTime = foundDelTime;
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
            main_GUI_SETTINGS->cursorBlinkTime = foundDelTime;
            editorCursorBlinkDelayTime->set_number(foundDelTime );

            gpe::input->mouse_scroll_inverted = userInvertMouseScroll->is_clicked();
            save_resource();
        }

        if( cancelResourceButton!=NULL && cancelResourceButton->is_clicked() )
        {
            load_resource();
            editor_gui_main->showTipsAtStartUp = showStatupTipsBox->is_clicked();
            editor_gui_main->save_settings();
        }
    }
}

void gamePencilEditorSettingsResource::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    cam = gpe::camera_find(cam);
    viewedSpace = gpe::camera_find(viewedSpace);
    if( cam!=NULL && viewedSpace!=NULL)
    {
        if( sideAreaPanel!=NULL && PANEL_GENERAL_EDITOR==NULL )
        {
            sideAreaPanel->render_self( viewedSpace,cam);
        }

        if(  codingLanguageSelector!=NULL && sideAreaPanel!=NULL  && sideAreaPanel->get_selected_name()=="Coding Languages" )
        {
            codingLanguageSelector->render_self( viewedSpace,cam);
        }

        if( editorPageList!=NULL )
        {
            editorPageList->render_self( viewedSpace,cam);
        }
    }
}

void gamePencilEditorSettingsResource::save_resource(std::string file_path, int backupId)
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
            newSaveDataFile << "DefaultTheme=" << theme_main->theme_name << "\n";
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
            newSaveDataFile << "FPS="+ stg_ex::int_to_string( ideSettingsFPSRate->get_selected_value() ) << "\n";
            newSaveDataFile << "ProjectFolderList="+projectFolderListLocation << "\n";
            newSaveDataFile << "ButtonBarSize="+ stg_ex::int_to_string( ideButtonBarSize->get_selected_value() ) << "\n";
            newSaveDataFile << "ButtonBarAlignment="+ stg_ex::int_to_string( ideButtonBarAlignment->get_selected_value() ) << "\n";
            newSaveDataFile << "\n";
        }
    }
    gpe::time_keeper->set_fps( ideSettingsFPSRate->get_selected_value() );
}

bool gamePencilEditorSettingsResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "";
            //save_resource();
            return true;
        }
    }
    return false;
}
