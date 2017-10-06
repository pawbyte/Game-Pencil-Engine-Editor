/*
project_properties.cpp
This file is part of:
GAME PENCI ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

#include "project_properties.h"

projectPropertiesResource::projectPropertiesResource(GPE_ResourceContainer * pFolder)
{
    for( int iLayerN = 0; iLayerN < 32; iLayerN++)
    {
        projectLayerNamesColumnTitles[iLayerN] = new GPE_Label_90Degree("","");
        projectLayerNamesRowTitles[iLayerN] = new GPE_Label_Text("","");
    }
    for( int iMatrix = 0; iMatrix < 1024; iMatrix++)
    {
        projectCollisionLayerMatrix[iMatrix] = new GPE_CheckBoxBasic("","");
        projectCollisionLayerMatrix[iMatrix]->set_checkbox_size(16);
    }
    int iMatrixRow = 0, iMatrixCol = 0;
    for( iMatrixRow = 0; iMatrixRow < 32; iMatrixRow++)
    {
        for( iMatrixCol = 16; iMatrixCol < 32; iMatrixCol++)
        {
            projectCollisionLayerMatrix[iMatrixRow*32+iMatrixCol]->set_clicked(false);
        }
    }

    for( iMatrixRow = 16; iMatrixRow < 32; iMatrixRow++)
    {
        for( iMatrixCol = 0; iMatrixCol < 32; iMatrixCol++)
        {
            projectCollisionLayerMatrix[iMatrixRow*32+iMatrixCol]->set_clicked(false);
        }
    }
    projectLayerEmptyTitle = new GPE_Label_Text("","");
    selectAllCollisionBoxes = new GPE_ToolLabelButton(0,0,"Select All Layers","Select All Layers");
    unselectAllCollisionBoxes = new GPE_ToolLabelButton(0,0,"Unselect All Layers","Unselect All Layers");
    resourceFileName = "";
    resourceName = "Project Properties";
    commonButtonAlignment = 1;
    projectParentFolder = pFolder;

    //Settings Tab
    iconFileName = "";
    projectIconLabel = new GPE_Label_Text("Project Icon","Project Icon");
    projectIconImg = new GPE_Label_Image();
    projectIconImg->set_width(128);
    projectIconImg->set_height(128);
    browseIconButton = new GPE_ToolLabelButton(0,0,"Browse..","Find image icon");

    projectScreenWidthField = new GPE_TextInputNumber("Default: 640",true,64,4096);
    projectScreenWidthField->set_label("Screen Width");
    projectScreenWidthField->set_number(640);

    projectScreenHeightField = new GPE_TextInputNumber("Default: 480",true,64,4096);
    projectScreenHeightField->set_label("Screen Height");
    projectScreenHeightField->set_number(480);

    projectGameVersionField = new GPE_TextInputNumber("0");
    projectGameVersionField->set_label("Game Version Number");
    projectGameVersionField->set_number(1);

    projectGameSubVersionField = new GPE_TextInputBasic();
    projectGameSubVersionField->set_label("Game Sub-Version Number");
    projectGameSubVersionField->set_number(0);

    projectScaleSettings = new GPE_RadioButtonControllerBasic("Scale Settings",0,256,true,1);
    projectScaleSettings->add_opton("Do Not Scale");
    projectScaleSettings->add_opton("Half-Size[50%]");
    projectScaleSettings->add_opton("Double-Size[200%]");
    projectScaleSettings->add_opton("Keep Aspect Ratio[Recommended]");
    projectScaleSettings->add_opton("Full Scale");

    projectSettingsFPSRateLabel = new GPE_Label_Text("Frame Rate Cap","Sets the (max) framerate set for the game(Usually 30 or 60)");
    projectSettingsFPSRate = new GPE_DropDown_Menu(0,0,"FPS Cap",true);
    projectSettingsFPSRate->add_menu_option("15 FPS","",15);
    projectSettingsFPSRate->add_menu_option("20 FPS","",20);
    projectSettingsFPSRate->add_menu_option("24 FPS","",24);
    projectSettingsFPSRate->add_menu_option("25 FPS","",25);
    projectSettingsFPSRate->add_menu_option("30 FPS","",30,true);
    projectSettingsFPSRate->add_menu_option("60 FPS","",60);
    projectSettingsFPSRate->add_menu_option("Uncapped","",-1);

    //Publisher Section
    Title_GameSettings = new GPE_Label_Title("Game Settings","Game Settings");
    Title_Publisher = new GPE_Label_Title("Publishing","Publishing");
    projectGameTitleField = new GPE_TextInputBasic();
    projectGameTitleField->set_label("Game Title");

    projectGameShortTitleField = new GPE_TextInputBasic();
    projectGameShortTitleField->set_label("Game Short Title");

    projectGamePublisherField = new GPE_TextInputBasic();
    projectGamePublisherField->set_label("Game Publisher");

    projectGameDeveloperField = new GPE_TextInputBasic();
    projectGameDeveloperField->set_label("Game Developer");

    projectGameCopyrightField = new GPE_TextInputBasic();
    projectGameCopyrightField->set_label("Game Copyright Text");

    projectGameDateField = new GPE_TextInputBasic("2016");
    projectGameDateField->set_label("Game Copyright Date");

    projectGameDescriptionField = new GPE_TextInputBasic();
    projectGameDescriptionField->set_label("Game Description");

    projectGameWebsiteField = new GPE_TextInputBasic();
    projectGameWebsiteField->set_label("Game Website");

    projectGameEmailField= new GPE_TextInputBasic();
    projectGameEmailField->set_label("Game Email");

    checkBoxShowPublisherInfo = new GPE_CheckBoxBasic("Display Info","Displays this information in build");
    checkBoxShowPublisherInfo->set_clicked(true);

    //Colors Tab
    projectBorderColor= new GPE_Input_Field_Color();
    projectBorderColor->set_label("Border Color");
    projectBorderColor->set_rgb(16,16,16);

    projectWebsiteBackgroundColor= new GPE_Input_Field_Color();
    projectWebsiteBackgroundColor->set_label("Website Background Color");
    projectWebsiteBackgroundColor->set_rgb(0,0,0);

    projectGameBackgroundColor = new GPE_Input_Field_Color();
    projectGameBackgroundColor->set_label("Game Background Color");
    projectGameBackgroundColor->set_rgb(0,0,0);

    projectTextHeaderColor = new GPE_Input_Field_Color();
    projectTextHeaderColor->set_label("Text(Header) Color");
    projectTextHeaderColor->set_rgb(0,0,228);

    projectTextParagraphColor = new GPE_Input_Field_Color();
    projectTextParagraphColor->set_label("Text Color");
    projectTextParagraphColor->set_rgb(192,192,192);

    projectTextLinkColor = new GPE_Input_Field_Color();
    projectTextLinkColor->set_label("Hyperlink Color");
    projectTextLinkColor->set_rgb(16,16,255);

    projectTextLinkActiveColor = new GPE_Input_Field_Color();
    projectTextLinkActiveColor->set_label("Hyperlink Active Color");
    projectTextLinkActiveColor->set_rgb(192,192,192);

    projectTextLinkHoverColor = new GPE_Input_Field_Color();
    projectTextLinkHoverColor->set_label("Hyperlink Hover Color");
    projectTextLinkHoverColor->set_rgb(192,255,192);

    projectTextLinkVisitedColor = new GPE_Input_Field_Color();
    projectTextLinkVisitedColor->set_label("Hyperlink Visited Color");
    projectTextLinkVisitedColor->set_rgb(192,16,192);

    //Macros Tab
    projectGameMacros= new GPE_TextAreaInputBasic();

    //Game Notes Tab
    projectGameNotes = new GPE_TextAreaInputBasic();
    projectGameNotes->isCodeEditor = false;

    //Export Settings Tab
    exportPushButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/cog.png","Build Project","Recompiles Project");
    exportAndPlayPushButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/play.png","Build & Play Project","Recompiles Project & Runs it");

    cleanBuildFolderPushButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/play.png","Clean Build Folders","Removes files from the build folder matching the criteria above.");
    cleanBuildAllFolderPushButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/play.png","Clean ALL Build Folders","Removes files from all build folders of this project.");
    playProgramPushButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/play.png","Play Game","Runs game matching the criteria above");

    exportSettingsBar = new GPE_SelectBoxBasic(0,0,"Platforms");
    exportSettingsBar->set_width(160);
    exportSettingsBar->set_option_height(64);

    exportApplicationLabel = new GPE_Label_Text("Application","Key Application Information");

    exportWindowsLabel = new GPE_Label_Text("Windows Export Settings","Windows  Export Settings");
    exportOSXLabel = new GPE_Label_Text("OSX Export Settings","OSX  Export Settings");
    exportLinuxLabel = new GPE_Label_Text("Linux Export Settings","Linux Export Settingsation");

    win64BitCheckbox = new GPE_CheckBoxBasic("64 Bit Build","Leave unchecked to use 32-bit build",0,0,true);
    osx64BitCheckbox = new GPE_CheckBoxBasic("64 Bit Build","Leave unchecked to use 32-bit build",0,0,true);
    linux64BitCheckbox = new GPE_CheckBoxBasic("64 Bit Build","Leave unchecked to use 32-bit build",0,0,true);

    html5DebugModeCheckbox = new GPE_CheckBoxBasic("Debug Mode","Enter Debug mode for HTML5 build",0,0,true);
    wiiuDebugModeCheckbox = new GPE_CheckBoxBasic("Debug Mode","Enter Debug mode for WiiU build",0,0,true);
    winDebugModeCheckbox = new GPE_CheckBoxBasic("Debug Mode","Enter Debug mode for Windows build",0,0,true);
    osxDebugModeCheckbox = new GPE_CheckBoxBasic("Debug Mode","Enter Debug mode for OSX build",0,0,true);
    linuxDebugModeCheckbox = new GPE_CheckBoxBasic("Debug Mode","Enter Debug mode for Linux build",0,0,true);

    html5nativeBuildTypeOptions = new GPE_RadioButtonControllerBasic("Build Type",0,0,true);

    windowsNativeBuildTypeOptions = new GPE_RadioButtonControllerBasic("Build Type",0,0,true);
    //windowsNativeBuildTypeOptions->add_menu_option("Use GPE Native-Build[Experimental]","gpe_native",Native_GamePencil,false);
    windowsNativeBuildTypeOptions->add_menu_option("Use Electron","gpe_electron",Native_Electron,false);
    //windowsNativeBuildTypeOptions->add_menu_option("C++ Project","gpe_cpp",Native_GamePencil_CPP,false);
    windowsNativeBuildTypeOptions->add_menu_option("Do not Export...","gpe_no_native",Native_None,true);

    osxNativeBuildTypeOptions = new GPE_RadioButtonControllerBasic("Build Type",0,0,true);
    //osxNativeBuildTypeOptions->add_menu_option("Use GPE Native-Build[Experimental]","gpe_native",Native_GamePencil,false);
    osxNativeBuildTypeOptions->add_menu_option("Use Electron","gpe_electron",Native_Electron,false);
    //osxNativeBuildTypeOptions->add_menu_option("C++ Project","gpe_cpp",Native_GamePencil_CPP,false);
    osxNativeBuildTypeOptions->add_menu_option("Do not Export...","gpe_no_native",Native_None,true);

    linuxNativeBuildTypeOptions = new GPE_RadioButtonControllerBasic("Build Type",0,0,true);
    //linuxNativeBuildTypeOptions->add_menu_option("Use GPE Native-Build[Experimental]","gpe_native",Native_GamePencil,false);
    linuxNativeBuildTypeOptions->add_menu_option("Use Electron","gpe_electron",Native_Electron,false);
    //linuxNativeBuildTypeOptions->add_menu_option("C++ Project","gpe_cpp",Native_GamePencil_CPP,false);
    linuxNativeBuildTypeOptions->add_menu_option("Do not Export...","gpe_no_native",Native_None,true);

    electronCompileNote = new GPE_Label_Text("Electron Build Guide");
    electronCompileUrl = new GPE_TextURL("For instructions on compiling using Electron click here.","Instructions on compiling using Electron.","http://docs.pawbyte.com/using-electron/");

    comingSoonTitle = new GPE_Label_Title("Coming Soon","Please wait for future versions of GPE...");
    wiiuCompileNote = new GPE_Label_Text("Nintendo Developer Account required","Nintendo Developer Account required");
    wiiuCompileUrl  = new GPE_TextURL("Click here to join the program","https://developer.nintendo.com","https://developer.nintendo.com");

    switchComingSoonNote = new GPE_Label_Paragraph("Switch Note","We hope and intend to soon providing support to the Nintendo Switch. When we do, we will provide the port to approved Nintendo Switch developers free of charge. Thank you for your patience and time.","");
    nSwitchSideNote = new GPE_Label_Paragraph("Switch Side Note","We are heavily excited to soon provide support for multi-touch screens, new play modes, HD Rumble and built in multiplayer controls.","");
    nSwitchThirdNote = new GPE_Label_Paragraph("Switch Third Note","For more information feel free to contact Nintendo Developer's program:   ","");

    nSwitchContactUrl  = new GPE_TextURL("Nintendo Developer contact page","Click here to visit Nintendo Developer contact page","https://developer.nintendo.com/web/development/contact");

    ps4ComingSoonNote = new GPE_Label_Paragraph("PS4 Note","We hope and intend to soon providing support to the PlayStation 4. When we do, we will provide the port to approved PlayStation 4 developers free of charge. Thank you for your patience and time.","");
    ps4SideNote = new GPE_Label_Paragraph("PS4 Side Note","For more information feel free to contact PlayStation 4 Developer's program:   ","");
    ps4ContactUrl  = new GPE_TextURL("PlayStation Developer contact page","Click here to visit PlayStation Developer page","https://www.playstation.com/en-us/develop/");

    xboxOneComingSoonNote = new GPE_Label_Paragraph("Xbox One Note","We hope and intend to soon providing support to the Xbox One. When we do, we will provide the port to approved Xbox One developers free of charge. Thank you for your patience and time.","");
    xboxOneSideNote = new GPE_Label_Paragraph("Xbox One Side Note","For more information feel free to contact Xbox Developer's program:   ","");
    xboxOneContactUrl  = new GPE_TextURL("Xbox Developer contact page","Click here to visit Xbox Developer contact page","http://www.xbox.com/en-US/developers");


    wiiuNativeBuildTypeOptions = new GPE_RadioButtonControllerBasic("Build Type",0,0,true);
    //wiiuNativeBuildTypeOptions->add_menu_option("Use GPE Native-Build[Experimental]","gpe_native",Native_GamePencil,false);
    wiiuNativeBuildTypeOptions->add_menu_option("Nintendo Web Framework","gpe_nwf",Native_NWF,false);
    //linuxNativeBuildTypeOptions->add_menu_option("C++ Project","gpe_cpp",Native_GamePencil_CPP,false);
    wiiuNativeBuildTypeOptions->add_menu_option("Do not Export...","gpe_no_native",Native_None,true);

    switchNativeBuildTypeOptions = new GPE_RadioButtonControllerBasic("Build Type",0,0,true);
    /*
    switchNativeBuildTypeOptions->add_menu_option("Use GPE Native-Build[Experimental]","gpe_native",Native_GamePencil,false);
    switchNativeBuildTypeOptions->add_menu_option("Nintendo Web Framework","gpe_nwf",Native_NWF,true);
    switchNativeBuildTypeOptions->add_menu_option("C++ Project","gpe_cpp",Native_GamePencil_CPP,false);
    */
    switchNativeBuildTypeOptions->add_menu_option("Do not Export...","gpe_no_native",Native_None,false);

    //Extras Tab
    transformImageDirectoryButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/magic.png","Transform IMG Folder","Transforms all Images in a given folder",-1);
    GPE_Sprite * mainExportOptionsSprite = rsm->sprite_add(APP_DIRECTORY_NAME+"resources/gfx/sprites/main_export_options_icons.png",12,true,0,0,false);

    GPE_Texture * desktopLogoTexture = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/desktop.png");
    GPE_Texture * appleLogoTexture = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/apple.png");
    GPE_Texture * linuxLogoTexture = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/font_awesome_linux.png");
    GPE_Texture * windowsLogoTexture = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/windows.png");
    GPE_Texture * html5LogoTexture = rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/html5.png");
    //GPE_Texture * switchLogoTexture = rrsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/sprites/nintendo_switch_logo.png");

    /*
    if( GPE_FOUND_OS==GPE_IDE_WINDOWS)
    {
        exportSettingsBar->add_option("Windows",GPE_BUILD_WINDOWS,windowsLogoTexture,NULL,6);
        exportSettingsBar->add_option("OSX",GPE_BUILD_MAC,appleLogoTexture,NULL,5);
        exportSettingsBar->add_option("Linux",GPE_BUILD_LINUX,linuxLogoTexture,NULL,4);
    }
    else if( GPE_FOUND_OS==GPE_IDE_MAC)
    {
        exportSettingsBar->add_option("OSX",GPE_BUILD_MAC,appleLogoTexture,NULL,5);
        exportSettingsBar->add_option("Linux",GPE_BUILD_LINUX,linuxLogoTexture,NULL,4);
        exportSettingsBar->add_option("Windows",GPE_BUILD_WINDOWS,windowsLogoTexture,NULL,6);
    }
    else
    {
        exportSettingsBar->add_option("Linux",GPE_BUILD_LINUX,linuxLogoTexture,NULL,4);
        exportSettingsBar->add_option("Windows",GPE_BUILD_WINDOWS,windowsLogoTexture,NULL,6);
        exportSettingsBar->add_option("OSX",GPE_BUILD_MAC,appleLogoTexture,NULL,5);
    }*/
    exportSettingsBar->add_option("HTML5",GPE_BUILD_HTML5,html5LogoTexture);
    exportSettingsBar->add_option("PC",GPE_BUILD_DESKTOP,desktopLogoTexture);
    exportSettingsBar->add_option("WiiU",GPE_BUILD_WIIU,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/sprites/nintendo_wiiu_logo.png"),NULL,2, false, false);
    /*
    Most Recent 4/14/2017
    exportSettingsBar->add_option("Switch",GPE_BUILD_SWITCH,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/sprites/nintendo_switch_logo.png"),NULL,-1, false, false );
    //exportSettingsBar->add_option("PS4",GPE_BUILD_PS4,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/sprites/playstation_4_logo.png"),NULL,-1, false, false );
    exportSettingsBar->add_option("PS4",GPE_BUILD_PS4,NULL,NULL,-1, false, false );
    //exportSettingsBar->add_option("Xbox One",GPE_BUILD_SWITCH,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/sprites/xbox_one_logo.png"),NULL,-1, false, false );
    exportSettingsBar->add_option("Xbox One",GPE_BUILD_XBOXONE, NULL,NULL,-1, false, false );
    */
    /*
    exportSettingsBar->add_option("Playstation 4",NULL,mainExportOptionsSprite,1);
    exportSettingsBar->add_option("WiiU",NULL,mainExportOptionsSprite,2);
    exportSettingsBar->add_option("XboxOne",NULL,mainExportOptionsSprite,3);
    exportSettingsBar->add_option("Windows",NULL,mainExportOptionsSprite,6);
    exportSettingsBar->add_option("Mac",NULL,mainExportOptionsSprite,5);
    exportSettingsBar->add_option("Linux",NULL,mainExportOptionsSprite,4);

    exportSettingsBar->add_option("Android",NULL,mainExportOptionsSprite,7);
    exportSettingsBar->add_option("iOS",NULL,mainExportOptionsSprite,8);
    exportSettingsBar->add_option("Windows Phone",NULL,mainExportOptionsSprite,9);
    exportSettingsBar->add_option("Tizen",NULL,mainExportOptionsSprite,10);
    exportSettingsBar->add_option("Raspberry Pi",NULL,mainExportOptionsSprite,11);
    */
    sceneEditorPaneMode = new GPE_ToolIconButtonBar(0,0,32,true);
    sceneEditorPaneMode->adkb_dton(APP_DIRECTORY_NAME+"resources/gfx/buttons/newspaper-o.png","Mode0 - 2 Panes",-1,false);
    sceneEditorPaneMode->adkb_dton(APP_DIRECTORY_NAME+"resources/gfx/buttons/map.png","Mode1 - 3 Panes",-1,false);

    projectSettingsTabBar = new GPE_TabBar();
    projectSettingsTabBar->set_coords(16, 16);
    projectSettingsTabBar->add_new_tab("Settings");
    projectSettingsTabBar->add_new_tab("Platforms");
    projectSettingsTabBar->add_new_tab("Colors");
    projectSettingsTabBar->add_new_tab("Macros");
    projectSettingsTabBar->add_new_tab("Notes");
    projectSettingsTabBar->add_new_tab("Physics");
    projectSettingsTabBar->add_new_tab("Extras");

    //projectSettingsTabBar->add_new_tab("Other Settings");
    projectSettingsTabBar->open_tab(0);

    //HTML5 Settings
    warnOnCloseCheckbox = new GPE_CheckBoxBasic("Warn on Close","If checked warn user on close",0,0,true);
    headerCodeBeforeGPECheckbox = new GPE_CheckBoxBasic("Run HTML5 HeaderCode before GPE","If checked header code is inserted above GPE",0,0,true);
    projectHTML5SettingsTabBar = new GPE_TabBar();
    projectHTML5SettingsTabBar->set_coords(16, 16);
    projectHTML5SettingsTabBar->add_new_tab("General");
    projectHTML5SettingsTabBar->add_new_tab("CSS");
    projectHTML5SettingsTabBar->add_new_tab("HTML");
    projectHTML5SettingsTabBar->add_new_tab("HTML-Header");
    projectHTML5SettingsTabBar->open_tab(0);

    desktopSettingsTabBar = new GPE_TabBar();
    desktopSettingsTabBar->set_coords(16, 16);
    desktopSettingsTabBar->add_new_tab("Windows");
    desktopSettingsTabBar->add_new_tab("OSX");
    desktopSettingsTabBar->add_new_tab("Linux");
    desktopSettingsTabBar->open_tab(0);

    projectCSSCode = new GPE_TextAreaInputBasic();
    projectCSSCode->codeEditorType = 1;

    projectHtmlCode = new GPE_TextAreaInputBasic();
    projectHtmlCode->isCodeEditor = false;

    projectHtmlHeaderCode = new GPE_TextAreaInputBasic();
    projectHtmlHeaderCode->isCodeEditor = false;
    renameBox->disable_self();
    loadResourceButton->disable_self();
    //confirmResourceButton->disable_self();
    //cancelResourceButton->disable_self();
    saveResourceButton->disable_self();
    resourcePostProcessed = false;
    projectSettingsList = new GPE_GuiElementList();
    projectSettingsList->barXPadding = GENERAL_GPE_PADDING;
    projectSettingsList->barXMargin = 0;
    subViewedSpace.x = 0;
    subViewedSpace.y = 0;
    subViewedSpace.w = SCREEN_WIDTH;
    subViewedSpace.h = SCREEN_HEIGHT;
}

projectPropertiesResource::~projectPropertiesResource()
{
    for( int iLayerN = 31; iLayerN >=0; iLayerN--)
    {
        /*projectLayerNamesColumnTitles[iLayerN] = NULL;
        projectLayerNamesRowTitles[iLayerN] = NULL;*/
    }
    if( projectLayerEmptyTitle!=NULL)
    {
        delete projectLayerEmptyTitle;
        projectLayerEmptyTitle = NULL;
    }
    //Macros Tab
    if( projectGameMacros!=NULL)
    {
        delete projectGameMacros;
        projectGameMacros = NULL;
    }
    //Game Notes Tab
    if( projectGameNotes!=NULL)
    {
        delete projectGameNotes;
        projectGameNotes = NULL;
    }

    //Publisher Tab
    if( Title_GameSettings!=NULL)
    {
        delete Title_GameSettings;
        Title_GameSettings = NULL;
    }
    if( Title_Publisher!=NULL)
    {
        delete Title_Publisher;
        Title_Publisher = NULL;
    }
    if( projectGameTitleField!=NULL)
    {
        delete projectGameTitleField;
        projectGameTitleField = NULL;
    }
    if( projectGameDateField!=NULL)
    {
        delete projectGameDateField;
        projectGameDateField = NULL;
    }
    if( projectGameDescriptionField!=NULL)
    {
        delete projectGameDescriptionField;
        projectGameDescriptionField = NULL;
    }
    if( projectGamePublisherField!=NULL)
    {
        delete projectGamePublisherField;
        projectGamePublisherField = NULL;
    }
    if( projectGameDeveloperField!=NULL)
    {
        delete projectGameDeveloperField;
        projectGameDeveloperField = NULL;
    }
    //Settings Tab
    if( projectScreenWidthField!=NULL)
    {
        delete projectScreenWidthField;
        projectScreenWidthField = NULL;
    }
    if( projectScreenHeightField!=NULL)
    {
        delete projectScreenHeightField;
        projectScreenHeightField = NULL;
    }
    if( projectGameVersionField!=NULL)
    {
        delete projectGameVersionField;
        projectGameVersionField = NULL;
    }
    if( projectGameSubVersionField!=NULL)
    {
        delete projectGameSubVersionField;
        projectGameSubVersionField = NULL;
    }
    if( projectScaleSettings!=NULL)
    {
        delete projectScaleSettings;
        projectScaleSettings = NULL;
    }
    if( projectSettingsFPSRateLabel!=NULL)
    {
        delete projectSettingsFPSRateLabel;
        projectSettingsFPSRateLabel = NULL;
    }
    if( projectSettingsFPSRate!=NULL)
    {
        delete projectSettingsFPSRate;
        projectSettingsFPSRate = NULL;
    }
    if( projectGameCopyrightField!=NULL)
    {
        delete projectGameCopyrightField;
        projectGameCopyrightField = NULL;
    }
    if( projectGameWebsiteField!=NULL)
    {
        delete projectGameWebsiteField;
        projectGameWebsiteField = NULL;
    }
    if( projectGameEmailField!=NULL)
    {
        delete projectGameEmailField;
        projectGameEmailField = NULL;
    }

    if( checkBoxShowPublisherInfo!=NULL)
    {
        delete checkBoxShowPublisherInfo;
        checkBoxShowPublisherInfo = NULL;
    }


    //Colors Tab
    if(projectBorderColor!=NULL)
    {
        delete projectBorderColor;
        projectBorderColor = NULL;
    }
    if( projectWebsiteBackgroundColor!=NULL)
    {
        delete projectWebsiteBackgroundColor;
        projectWebsiteBackgroundColor = NULL;
    }
    if(projectGameBackgroundColor!=NULL)
    {
        delete projectGameBackgroundColor;
        projectGameBackgroundColor = NULL;
    }
    if(projectTextHeaderColor!=NULL)
    {
        delete projectTextHeaderColor;
        projectTextHeaderColor = NULL;
    }
    if(projectTextParagraphColor!=NULL)
    {
        delete projectTextParagraphColor;
        projectTextParagraphColor = NULL;
    }
    if(projectTextLinkColor!=NULL)
    {
        delete projectTextLinkColor;
        projectTextLinkColor = NULL;
    }

    if(projectTextLinkActiveColor!=NULL)
    {
        delete projectTextLinkActiveColor;
        projectTextLinkActiveColor = NULL;
    }

    if(projectTextLinkHoverColor!=NULL)
    {
        delete projectTextLinkHoverColor;
        projectTextLinkHoverColor = NULL;
    }

    if(projectTextLinkVisitedColor!=NULL)
    {
        delete projectTextLinkVisitedColor;
        projectTextLinkVisitedColor = NULL;
    }

    //Platforms tab
        //HTML5 Platforms
        if( warnOnCloseCheckbox!=NULL)
        {
            delete warnOnCloseCheckbox;
            warnOnCloseCheckbox = NULL;
        }
        if( headerCodeBeforeGPECheckbox!=NULL)
        {
            delete headerCodeBeforeGPECheckbox;
            headerCodeBeforeGPECheckbox = NULL;
        }
        if( projectCSSCode!=NULL)
        {
            delete projectCSSCode;
            projectCSSCode = NULL;
        }
        if( projectHtmlCode!=NULL)
        {
            delete projectHtmlCode;
            projectHtmlCode = NULL;
        }
        if( projectHtmlHeaderCode!=NULL)
        {
            delete projectHtmlHeaderCode;
            projectHtmlHeaderCode = NULL;
        }

        if( windowsNativeBuildTypeOptions!=NULL)
        {
            delete windowsNativeBuildTypeOptions;
            windowsNativeBuildTypeOptions = NULL;
        }

        if( osxNativeBuildTypeOptions!=NULL)
        {
            delete osxNativeBuildTypeOptions;
            osxNativeBuildTypeOptions = NULL;
        }

        if( linuxNativeBuildTypeOptions!=NULL)
        {
            delete linuxNativeBuildTypeOptions;
            linuxNativeBuildTypeOptions = NULL;
        }

        if( wiiuNativeBuildTypeOptions!=NULL)
        {
            delete wiiuNativeBuildTypeOptions;
            wiiuNativeBuildTypeOptions = NULL;
        }

        if( switchNativeBuildTypeOptions!=NULL)
        {
            delete switchNativeBuildTypeOptions;
            switchNativeBuildTypeOptions = NULL;
        }

    //Extras Tab
    if( transformImageDirectoryButton!=NULL)
    {
        delete transformImageDirectoryButton;
        transformImageDirectoryButton = NULL;
    }
    //Other variables
    if( projectSettingsTabBar!=NULL)
    {
        delete projectSettingsTabBar;
        projectSettingsTabBar = NULL;
    }
    if( projectSettingsList!=NULL)
    {
        delete projectSettingsList;
        projectSettingsList = NULL;
    }
}

bool projectPropertiesResource::export_and_play_native(bool launchProgram)
{
    bool buildAndRunSuccessful = false;
    if( CURRENT_PROJECT!=NULL)
    {
        int buildBits = 32;
        if( GPE_FOUND_OS==GPE_IDE_WINDOWS )
        {
            if( win64BitCheckbox->is_clicked() )
            {
                buildBits = 64;
            }
            CURRENT_PROJECT->save_project();
            buildAndRunSuccessful = CURRENT_PROJECT->export_project_windows("",buildBits,launchProgram, winDebugModeCheckbox->is_clicked(), windowsNativeBuildTypeOptions->get_selected_value() );
        }
        else if( GPE_FOUND_OS==GPE_IDE_LINUX )
        {
            if( linux64BitCheckbox->is_clicked() )
            {
                buildBits = 64;
            }
            CURRENT_PROJECT->save_project();
            buildAndRunSuccessful = CURRENT_PROJECT->export_project_linux("",buildBits,launchProgram, linuxDebugModeCheckbox->is_clicked() , linuxNativeBuildTypeOptions->get_selected_value() );
        }
        else if( GPE_FOUND_OS==GPE_IDE_MAC )
        {
            buildBits = 64;
            CURRENT_PROJECT->save_project();
            buildAndRunSuccessful = CURRENT_PROJECT->export_project_osx("",buildBits,launchProgram, osxDebugModeCheckbox->is_clicked(), osxNativeBuildTypeOptions->get_selected_value()  );
        }
        else if( GPE_Main_TabManager!=NULL )
        {
            GPE_Main_TabManager->add_new_tab(this );
            projectSettingsTabBar->open_tab("Platforms");
        }
    }
    return buildAndRunSuccessful;
}

void projectPropertiesResource::integrate_into_synthax()
{
    //parses the macros text editor for joy...
    if( projectGameMacros!=NULL && CURRENT_PROJECT!=NULL)
    {
        std::string currentLineToRead = "";
        std::string foundVariable = "";
        std::string variableString = "";
        bool isInBlockCommentMode = false;
        bool isInDoubleQuoteMode = false;
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
                    if( isInBlockCommentMode==false && isInDoubleQuoteMode==false && isInSingleQuoteMode==false)
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
                                isInDoubleQuoteMode = true;
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
                            else if (GPE_MINI_COMPILER->charIsSymbol(currentLineToRead[lineParseXCursorPos]))
                            {
                                lineParseXCursorPos++;
                                commentFoundInSymbols = false;
                                while( ( commentFoundInSymbols==false && lineParseXCursorPos < currentLineSize && GPE_MINI_COMPILER->charIsSymbol(currentLineToRead[lineParseXCursorPos] ) )|| currentLineToRead[lineParseXCursorPos] ==' ')
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
                                        variableString = get_substring(currentLineToRead,varDeclarationPos+4);
                                        variableString = string_replace_all(variableString," ","");
                                        if( (int)variableString.size() > 3)
                                        {
                                            commaPos = variableString.find(",",0);
                                            lastCommaPos = 0;
                                            if( commaPos > 0)
                                            {
                                                while( commaPos >=0)
                                                {
                                                    foundVariable = get_substring(variableString,lastCommaPos, commaPos);
                                                    if( (int)foundVariable.size() > 3 )
                                                    {
                                                        while( char_is_alnum(foundVariable[variableEndPos],false,true) )
                                                        {
                                                            variableEndPos++;
                                                            lineParseXCursorPos++;
                                                        }
                                                        foundVariable = get_substring(foundVariable,0,variableEndPos);
                                                        if( (int)foundVariable.size() > 0 )
                                                        {
                                                            CURRENT_PROJECT->add_project_keyword(foundVariable,"Defined in Project Marcos",-1,"User Global");
                                                            record_error("Found user macro:   ["+foundVariable+"].");
                                                        }
                                                        lastCommaPos = commaPos;
                                                        commaPos = std::max(commaPos,lineParseXCursorPos);
                                                        if( commaPos > 0)
                                                        {
                                                            commaPos = variableString.find(",",commaPos);
                                                            variableEndPos = 0;
                                                            lineParseXCursorPos = commaPos;
                                                            if( commaPos > 0)
                                                            {
                                                                commaPos++;
                                                            }
                                                            else
                                                            {
                                                                commaPos = (int)variableString.size();
                                                            }
                                                        }
                                                        else
                                                        {
                                                            commaPos = (int)variableString.size();
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
                                                while( char_is_alnum(variableString[variableEndPos],false,true) )
                                                {
                                                    variableEndPos++;
                                                    lineParseXCursorPos++;
                                                }
                                                foundVariable = get_substring(variableString,0,variableEndPos);
                                                if( (int)foundVariable.size() > 0 )
                                                {
                                                    CURRENT_PROJECT->add_project_keyword(foundVariable,"Defined in Project Marcos",-1,"User Global");
                                                    record_error("Found user macro:   ["+foundVariable+"].");
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
                    else if( isInDoubleQuoteMode)
                    {
                        endDQuoteCommentPos = currentLineToRead.find('"',lineParseXCursorPos);
                        if( endDQuoteCommentPos==(int)std::string::npos)
                        {
                            lineParseXCursorPos = currentLineSize;
                        }
                        else
                        {
                            lineParseXCursorPos = endDQuoteCommentPos+1;
                            isInDoubleQuoteMode = false;
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

void projectPropertiesResource::open_code(int lineNumb, int colNumb, std::string codeTitle )
{
    if( (int)codeTitle.size() > 0 )
    {
        GPE_TextAreaInputBasic * foundTextArea = NULL;

        if( codeTitle == "Macros")
        {
            foundTextArea = projectGameMacros;
            projectSettingsTabBar->open_tab("Macros");
        }
        else if( codeTitle == "Notes")
        {
            foundTextArea = projectGameNotes;
            projectSettingsTabBar->open_tab("Notes");

        }
        else if( codeTitle == "CSS")
        {
            foundTextArea = projectCSSCode;
            projectSettingsTabBar->open_tab("Platforms");
            projectHTML5SettingsTabBar->open_tab("CSS");
            exportSettingsBar->set_selected_option("HTML5");
        }
        else if( codeTitle == "HTML")
        {
            foundTextArea = projectHtmlCode;
            projectSettingsTabBar->open_tab("Platforms");
            projectHTML5SettingsTabBar->open_tab("HTML");
            exportSettingsBar->set_selected_option("HTML5");
        }
        else if( codeTitle == "HTML-Header")
        {
            foundTextArea = projectHtmlHeaderCode;
            projectSettingsTabBar->open_tab("Platforms");
            projectHTML5SettingsTabBar->open_tab("HTML-Header");
            exportSettingsBar->set_selected_option("HTML5");
        }

        if( foundTextArea!=NULL && foundTextArea->has_content() )
        {
            foundTextArea->scroll_to_pos(lineNumb, colNumb);
        }
    }
}

void projectPropertiesResource::prerender_self(GPE_Renderer * cRender )
{
	standardEditableGameResource::prerender_self( cRender);
	if( checkBoxShowPublisherInfo!=NULL)
    {
        checkBoxShowPublisherInfo->prerender_self(cRender);
    }
}

void projectPropertiesResource::preprocess_self(std::string alternatePath)
{
    if( resourcePostProcessed ==false  || file_exists(alternatePath) )
    {
        displayMessageTitle = "Processing";
        displayMessageSubtitle = "Project Properties";
        displayMessageString = "...";
        display_user_messaage();

        std::string otherColContainerName = "";

        std::string newFileIn ="";
        if( file_exists(alternatePath) )
        {
            newFileIn = alternatePath;
        }
        else
        {
            newFileIn = fileToDir(parentProjectName)+"/gpe_project/project_settings.gpf";
        }

        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        int equalPos = 0;
        std::string firstChar="";
        std::string section="";
        std::string cur_layer="";
        std::string data_format="";
        std::string keyString="";
        std::string valString="";
        std::string subValString="";
        std::string currLine="";
        std::string currLineToBeProcessed;
        std::string sprDataStr;
        float foundFileVersion = 0;

        record_error("Loading project properties - "+newFileIn);
        //If the level file could be loaded
        if( gameResourceFileIn != NULL )
        {
            record_error("Processing project properties file...");
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
                while ( gameResourceFileIn.good() )
                {
                    getline (gameResourceFileIn,currLine); //gets the next line of the file
                    currLineToBeProcessed = trim_left_inplace(currLine);
                    currLineToBeProcessed = trim_right_inplace(currLineToBeProcessed);

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
                                        foundFileVersion = string_to_double(valString);
                                    }
                                }
                            }
                        }
                    }
                    else if( foundFileVersion < 2)
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

                                if( keyString=="ScreenWidth")
                                {
                                    projectScreenWidthField->set_number( std::max(160, string_to_int(valString) )  );
                                }
                                else if( keyString=="ScreenHeight")
                                {
                                    projectScreenHeightField->set_number( std::max(120, string_to_int(valString) ) );
                                }
                                else if( keyString=="ProjectVersion")
                                {
                                    projectGameVersionField->set_string(valString);
                                }
                                else if( keyString=="ProjectSubVersion")
                                {
                                    projectGameSubVersionField->set_string(valString);
                                }
                                else if( keyString=="ProjectIcon")
                                {
                                    iconFileName = fileToDir(parentProjectName)+"/"+valString;
                                    if( file_exists(iconFileName) && file_is_image(iconFileName) )
                                    {
                                        if( projectIconImg!=NULL)
                                        {
                                            projectIconImg->load_label_image(iconFileName);
                                            projectIconImg->set_height(128);                                        }
                                    }
                                    else
                                    {
                                        iconFileName = "";
                                    }
                                }
                                else if( keyString=="ProjectScaleSettings")
                                {
                                    projectScaleSettings->set_selection( string_to_int(valString) );
                                }
                                else if( keyString=="ProjectFPSCap")
                                {
                                    projectSettingsFPSRate->set_id( string_to_int(valString) );
                                }
                                else if( keyString=="ProjectTitle")
                                {
                                    projectGameTitleField->set_string(valString);
                                }
                                else if( keyString=="ProjectShortTitle")
                                {
                                    projectGameShortTitleField->set_string(valString);
                                }
                                else if( keyString=="ProjectDate")
                                {
                                    projectGameDateField->set_string(valString);
                                }
                                else if( keyString=="ProjectDescription")
                                {
                                    projectGameDescriptionField->set_string(valString);
                                }
                                //
                                else if( keyString=="ProjectPublisher")
                                {
                                    projectGamePublisherField->set_string(valString);
                                }
                                else if( keyString=="ProjectDeveloper")
                                {
                                    projectGameDeveloperField->set_string(valString);
                                }
                                else if( keyString=="ProjectCopyright")
                                {
                                    projectGameCopyrightField->set_string(valString);
                                }
                                else if( keyString=="ProjectWebsite")
                                {
                                    projectGameWebsiteField->set_string(valString);
                                }
                                else if( keyString=="ProjectEmail")
                                {
                                    projectGameEmailField->set_string(valString);
                                }

                                else if( keyString=="BorderColor")
                                {
                                    projectBorderColor->set_color_from_rgb(valString);
                                }
                                else if( keyString=="GameBackgroundColor")
                                {
                                    projectGameBackgroundColor->set_color_from_rgb(valString);
                                }
                                else if( keyString=="WebsiteBackgroundColor")
                                {
                                    projectWebsiteBackgroundColor->set_color_from_rgb(valString);
                                }
                                else if( keyString=="HeaderColor")
                                {
                                    projectTextHeaderColor->set_color_from_rgb(valString);
                                }
                                else if( keyString=="ParagraphColor")
                                {
                                    projectTextParagraphColor->set_color_from_rgb(valString);
                                }
                                else if( keyString=="LinkColor")
                                {
                                    projectTextLinkColor->set_color_from_rgb(valString);
                                }
                                else if( keyString=="LinkActiveColor")
                                {
                                    projectTextLinkActiveColor->set_color_from_rgb(valString);
                                }
                                else if( keyString=="LinkHoverColor")
                                {
                                    projectTextLinkHoverColor->set_color_from_rgb(valString);
                                }
                                else if( keyString=="LinkVisitedColor")
                                {
                                    projectTextLinkVisitedColor->set_color_from_rgb(valString);
                                }
                                else if( keyString=="HTML5WarnOnClose")
                                {
                                    warnOnCloseCheckbox->set_clicked(is_bool(valString) );
                                }
                                else if( keyString=="HTML5CodeBeforeGPE")
                                {
                                    headerCodeBeforeGPECheckbox->set_clicked(is_bool(valString) );
                                }
                            }
                        }
                    }
                }
            }
        }

        if( CURRENT_PROJECT!=NULL)
        {
            for( int iLayerN = 0; iLayerN < 32; iLayerN++)
            {
                projectLayerNamesColumnTitles[iLayerN]->set_name(CURRENT_PROJECT->projectLayerNames[iLayerN] );
                projectLayerNamesRowTitles[iLayerN]->set_name(CURRENT_PROJECT->projectLayerNames[iLayerN] );
            }

            std::string projectCollisionMatrixFileName = projectParentFolder->parentProjectDirectory+"/gpe_project/project_layer_matrix.gpf";
            record_error("Loading project collision matrix to "+projectCollisionMatrixFileName);
            std::ofstream projectCollisionMatrixFile (projectCollisionMatrixFileName.c_str() );
            int jLayerCol = 0;
            int iLayerRow = 0;
            while (projectCollisionMatrixFile.is_open() )
            {
                getline (gameResourceFileIn,currLine); //gets the next line of the file
                currLineToBeProcessed = trim_left_inplace(currLine);
                currLineToBeProcessed = trim_right_inplace(currLineToBeProcessed);

                //Comment skipping is only allowed at the top of the file
                if(!currLineToBeProcessed.empty() &&  currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                {
                    //searches for an equal character and parses through the variable
                    equalPos=currLineToBeProcessed.find_first_of("=");
                    if(equalPos!=(int)std::string::npos)
                    {
                        //if the equalPos is present, then parse on through and carryon
                        keyString = currLineToBeProcessed.substr(0,equalPos);
                        valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                        for(  iLayerRow = 0; iLayerRow < 12; iLayerRow++)
                        {
                            if( keyString=="CollisionLayer["+int_to_string(iLayerRow)+"]")
                            {
                                for(  jLayerCol = 0; jLayerCol < 12-iLayerRow; jLayerCol++)
                                {
                                    valString = split_first_int(valString,',');
                                    projectCollisionLayerMatrix[iLayerRow*12+jLayerCol]->set_clicked( is_bool(valString) );
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
                    valString = split_first_int(valString,',');
                    projectCollisionLayerMatrix[iLayerRow*32+jLayerCol]->descriptionText = "["+CURRENT_PROJECT->projectLayerNames[iLayerRow]+" * "+CURRENT_PROJECT->projectLayerNames[jLayerCol]+"]";
                }
            }
        }
    }
    //Project TextAreas
    if( projectGameMacros!=NULL)
    {
        projectGameMacros->import_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_macros.gpf");
    }
    if( projectGameNotes!=NULL)
    {
        projectGameNotes->import_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_notes.gpf");
    }

    //
    if( projectCSSCode!=NULL)
    {
        projectCSSCode->import_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_css.css");
    }
    if( projectHtmlCode!=NULL)
    {
        projectHtmlCode->import_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_html.html");
    }

    if( projectHtmlHeaderCode!=NULL)
    {
        projectHtmlHeaderCode->import_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_html_header.html");
    }
}

void projectPropertiesResource::process_self(SDL_Rect * viewedSpace, SDL_Rect * cam)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( cam!=NULL && projectSettingsList!=NULL && projectSettingsTabBar!=NULL && viewedSpace!=NULL)
    {
        std::string nExportOptionName = exportSettingsBar->get_selected_name();
        std::string nExportOptionHtml5 = projectHTML5SettingsTabBar->get_selected_name();

        std::string nExportDesktopOS = desktopSettingsTabBar->get_selected_name();
        subViewedSpace.x = projectSettingsTabBar->get_xpos()+viewedSpace->x;
        subViewedSpace.y = projectSettingsTabBar->get_y2pos()+viewedSpace->y;
        subViewedSpace.w = viewedSpace->w-projectSettingsTabBar->get_xpos();
        subViewedSpace.h = viewedSpace->h-projectSettingsTabBar->get_y2pos();
        int prevTab = projectSettingsTabBar->tabInUse;
        projectSettingsTabBar->process_self(viewedSpace,cam);
        if( prevTab!=projectSettingsTabBar->tabInUse)
        {
            projectSettingsList->reset_self();
            /*userInput->reset_all_input();
            process_self(viewedSpace,cam);
            return;*/
        }
        projectSettingsList->clear_list();
        projectSettingsList->set_coords(0,0);
        projectSettingsList->set_width(subViewedSpace.w);
        projectSettingsList->set_height(subViewedSpace.h );
        projectSettingsList->barXPadding = GENERAL_GPE_PADDING;
        projectSettingsList->barYPadding = GENERAL_GPE_PADDING*2;
        projectSettingsList->barXMargin = 0;
        if( projectSettingsTabBar->get_selected_name()=="Extras")
        {
            projectSettingsList->add_gui_element(transformImageDirectoryButton,true);
            projectSettingsList->process_self(&subViewedSpace,cam);
            if( transformImageDirectoryButton->is_clicked()  )
            {
                GPE_open_context_menu();
                MAIN_CONTEXT_MENU->set_width(256);
                MAIN_CONTEXT_MENU->add_menu_option("Erase BG Color On All Images",0);
                MAIN_CONTEXT_MENU->add_menu_option("Invert Colors On All Images",1);
                MAIN_CONTEXT_MENU->add_menu_option("Make GrayScale On All Images",2);
                MAIN_CONTEXT_MENU->add_menu_option("Exit Menu",10);
                int menuSelection = get_popupmenu_result();
                if( menuSelection>=0 && menuSelection <=3)
                {
                    std::string lastDirectory = "";
                    std::string directoryToMod = GPE_GetDirectoryName("Transform All Images in this directory", lastDirectory,false);
                    int imagesModified = modify_folder_images(directoryToMod,menuSelection);
                }
            }
        }
        else
        {
            if( projectSettingsTabBar->get_selected_name()=="Settings")
            {
                projectSettingsList->add_gui_element(Title_GameSettings,true);
                projectSettingsList->add_gui_element(projectScreenWidthField,true);
                projectSettingsList->add_gui_element(projectScreenHeightField,true);
                projectSettingsList->add_gui_element(projectGameVersionField,true);
                projectSettingsList->add_gui_element(projectGameSubVersionField,true);

                projectSettingsList->add_gui_element(projectIconLabel,false);
                projectSettingsList->add_gui_element(browseIconButton,true);
                projectSettingsList->add_gui_element(projectIconImg,true);

                projectSettingsList->add_gui_element(projectScaleSettings,true);
                projectSettingsList->add_gui_element(projectSettingsFPSRateLabel,true);
                projectSettingsList->add_gui_element(projectSettingsFPSRate,true);

                projectSettingsList->add_gui_element(Title_Publisher,true);
                projectSettingsList->add_gui_element(projectGameTitleField,true);
                projectSettingsList->add_gui_element(projectGameShortTitleField,true);
                projectSettingsList->add_gui_element(projectGamePublisherField,true);
                projectSettingsList->add_gui_element(projectGameDeveloperField,true);
                projectSettingsList->add_gui_element(projectGameDescriptionField,true);
                projectSettingsList->add_gui_element(projectGameCopyrightField,true);
                projectSettingsList->add_gui_element(projectGameDateField,true);
                projectSettingsList->add_gui_element(projectGameWebsiteField,true);
                projectSettingsList->add_gui_element(projectGameEmailField,true);
                projectSettingsList->add_gui_element(checkBoxShowPublisherInfo,true);
                projectSettingsList->add_gui_element(confirmResourceButton,true);
                projectSettingsList->process_self(&subViewedSpace,cam);

                if( browseIconButton->is_clicked() )
                {
                    if( (int)GPE_MAIN_GUI->fileOpenProjectIconDir.size()==0 && CURRENT_PROJECT!=NULL )
                    {
                        GPE_MAIN_GUI->fileOpenProjectIconDir = CURRENT_PROJECT->get_project_directory();
                    }
                    std::string newProjectIconName = GPE_GetOpenFileName("Find Project Icon","Image",GPE_MAIN_GUI->fileOpenProjectIconDir );
                    if( file_exists(newProjectIconName ) )
                    {
                        if( file_is_image(newProjectIconName) )
                        {
                            if( projectIconImg!=NULL)
                            {
                                projectIconImg->load_label_image( newProjectIconName );
                                projectIconImg->set_height(128);
                            }
                            if( CURRENT_PROJECT!=NULL )
                            {
                                iconFileName = CURRENT_PROJECT->projectIconName = get_local_from_global_file(newProjectIconName );
                                copy_file(newProjectIconName, CURRENT_PROJECT->get_project_directory()+"/"+CURRENT_PROJECT->projectIconName  );
                            }
                        }
                    }
                }
            }
            else if(projectSettingsTabBar->get_selected_name()=="Platforms")
            {
                exportSettingsBar->set_height(subViewedSpace.h);
                exportSettingsBar->optionIconWidth = 64;
                exportSettingsBar->process_self(&subViewedSpace,cam);
                projectSettingsList->set_coords(exportSettingsBar->get_x2pos(), 0 );
                projectSettingsList->set_width(subViewedSpace.w -projectSettingsList->get_xpos() );
                projectSettingsList->set_height(subViewedSpace.h );
                if( nExportOptionName=="HTML5" )
                {
                    projectSettingsList->add_gui_element(projectHTML5SettingsTabBar,true);
                    if( nExportOptionHtml5=="General" )
                    {
                        projectSettingsList->add_gui_element(exportApplicationLabel,true);
                        projectSettingsList->add_gui_element(warnOnCloseCheckbox,false);
                        projectSettingsList->add_gui_element(headerCodeBeforeGPECheckbox,true);
                        projectSettingsList->add_gui_element(html5DebugModeCheckbox,true);
                    }
                    else if( nExportOptionHtml5=="HTML" )
                    {
                        projectHtmlCode->set_width(projectSettingsList->get_width()-128);
                        projectHtmlCode->set_height(projectSettingsList->get_height()-64);
                        projectSettingsList->add_gui_element(projectHtmlCode,true);
                    }
                    else if( nExportOptionHtml5=="HTML-Header" )
                    {
                        projectHtmlHeaderCode->set_width(projectSettingsList->get_width()-128);
                        projectHtmlHeaderCode->set_height(projectSettingsList->get_height()-64);
                        projectSettingsList->add_gui_element(projectHtmlHeaderCode,true);
                    }
                    else if( nExportOptionHtml5=="CSS" )
                    {
                        projectCSSCode->set_width(projectSettingsList->get_width()-128);
                        projectCSSCode->set_height(projectSettingsList->get_height()-64);
                        projectSettingsList->add_gui_element(projectCSSCode,true);
                    }
                    projectSettingsList->add_gui_element(exportPushButton,false);
                    projectSettingsList->add_gui_element(exportAndPlayPushButton,false );
                    projectSettingsList->add_gui_element(playProgramPushButton , true);
                    projectSettingsList->process_self(&subViewedSpace,cam);

                    if( CURRENT_PROJECT!=NULL )
                    {
                        if( exportPushButton->is_clicked() )
                        {
                            CURRENT_PROJECT->save_project();
                            CURRENT_PROJECT->export_project_html5("",GPE_BUILD_HTML5,false);
                        }
                        else if( exportAndPlayPushButton->is_clicked() )
                        {
                            CURRENT_PROJECT->save_project();
                            CURRENT_PROJECT->export_project_html5("",GPE_BUILD_HTML5,true);
                        }
                        else if( playProgramPushButton->is_clicked() )
                        {
                            CURRENT_PROJECT->run_project("",GPE_BUILD_HTML5);
                        }
                    }
                }
                else if( nExportOptionName=="Nintendo WiiU" || nExportOptionName=="WiiU" )
                {
                    if( GPE_MAIN_GUI->includeNintendoWiiUExport )
                    {
                        projectSettingsList->add_gui_element(exportApplicationLabel,true);
                        projectSettingsList->add_gui_element(wiiuDebugModeCheckbox , true);
                        projectSettingsList->add_gui_element(exportPushButton,false);
                        projectSettingsList->process_self(&subViewedSpace,cam);
                        if( exportPushButton->is_clicked() && CURRENT_PROJECT!=NULL)
                        {
                            CURRENT_PROJECT->save_project();
                            CURRENT_PROJECT->export_project_wiiu(wiiuDebugModeCheckbox->is_clicked() );
                            //winnativeBuildTypeOptions->set_clicked(false);
                        }
                    }
                    else
                    {
                        projectSettingsList->add_gui_element(wiiuCompileNote,true);
                        projectSettingsList->add_gui_element(wiiuCompileUrl, true);
                        projectSettingsList->process_self(&subViewedSpace,cam);
                    }
                }
                /*
                else if( nExportOptionName=="Nintendo Switch" || nExportOptionName=="Switch" )
                {
                    projectSettingsList->add_gui_element(comingSoonTitle,true);
                    //switchComingSoonNote->set_width(subViewedSpace.w);
                    projectSettingsList->add_gui_element(switchComingSoonNote,true);
                    //nSwitchSideNote->set_width(subViewedSpace.w);
                    projectSettingsList->add_gui_element(nSwitchSideNote,true);
                    projectSettingsList->add_gui_element(nSwitchThirdNote,true);
                    projectSettingsList->add_gui_element(nSwitchContactUrl,true);
                    projectSettingsList->process_self(&subViewedSpace,cam);
                }
                else if( nExportOptionName=="PlayStation 4" || nExportOptionName=="PS4" )
                {
                    projectSettingsList->add_gui_element(comingSoonTitle,true);
                    //switchComingSoonNote->set_width(subViewedSpace.w);
                    projectSettingsList->add_gui_element(ps4ComingSoonNote,true);
                    //nSwitchSideNote->set_width(subViewedSpace.w);
                    projectSettingsList->add_gui_element(ps4SideNote,true);
                    projectSettingsList->add_gui_element(ps4ContactUrl,true);
                    projectSettingsList->process_self(&subViewedSpace,cam);
                }
                else if( nExportOptionName=="Xbox One" || nExportOptionName=="XboxOne" )
                {
                    projectSettingsList->add_gui_element(comingSoonTitle,true);
                    //switchComingSoonNote->set_width(subViewedSpace.w);
                    projectSettingsList->add_gui_element(xboxOneComingSoonNote,true);
                    //nSwitchSideNote->set_width(subViewedSpace.w);
                    projectSettingsList->add_gui_element(xboxOneSideNote,true);
                    projectSettingsList->add_gui_element(xboxOneContactUrl,true);
                    projectSettingsList->process_self(&subViewedSpace,cam);
                }
                */
                else
                {
                    int buildBits = 32;
                    if( nExportOptionName=="PC" )
                    {
                        projectSettingsList->add_gui_element(desktopSettingsTabBar,true);
                        if( nExportDesktopOS=="Windows" )
                        {
                            projectSettingsList->add_gui_element(exportApplicationLabel,true);
                            projectSettingsList->add_gui_element(win64BitCheckbox,false );
                            projectSettingsList->add_gui_element(winDebugModeCheckbox , true);
                            projectSettingsList->add_gui_element(windowsNativeBuildTypeOptions , true);
                            if( GPE_FOUND_OS==GPE_IDE_WINDOWS )
                            {
                                projectSettingsList->add_gui_element(exportPushButton,false);
                                projectSettingsList->add_gui_element(exportAndPlayPushButton,false );
                                projectSettingsList->add_gui_element(playProgramPushButton , true);
                            }
                            else
                            {
                                projectSettingsList->add_gui_element(exportPushButton,true);
                            }
                            projectSettingsList->add_gui_element(electronCompileNote,true);
                            projectSettingsList->add_gui_element(electronCompileUrl,true);

                            projectSettingsList->process_self(&subViewedSpace,cam);

                            if( win64BitCheckbox->is_clicked() )
                            {
                                buildBits = 64;
                            }
                            if( exportPushButton->is_clicked() && CURRENT_PROJECT!=NULL)
                            {
                                CURRENT_PROJECT->save_project();
                                CURRENT_PROJECT->export_project_windows("",buildBits,false, winDebugModeCheckbox->is_clicked(), windowsNativeBuildTypeOptions->get_selected_value() );
                                //winnativeBuildTypeOptions->set_clicked(false);
                            }
                            else if( GPE_FOUND_OS==GPE_IDE_WINDOWS && CURRENT_PROJECT!=NULL )
                            {
                                if( exportAndPlayPushButton->is_clicked() )
                                {
                                    export_and_play_native(true );
                                }
                                else if( playProgramPushButton->is_clicked() )
                                {
                                    CURRENT_PROJECT->run_project("",GPE_BUILD_WINDOWS,buildBits, winDebugModeCheckbox->is_clicked() );
                                }
                            }
                        }
                        else if( nExportDesktopOS=="Linux" )
                        {
                            projectSettingsList->add_gui_element(exportApplicationLabel,true);
                            projectSettingsList->add_gui_element(linux64BitCheckbox ,false);
                            projectSettingsList->add_gui_element(linuxDebugModeCheckbox , true);
                            projectSettingsList->add_gui_element(linuxNativeBuildTypeOptions , true);
                            if( GPE_FOUND_OS==GPE_IDE_LINUX )
                            {
                                projectSettingsList->add_gui_element(exportPushButton,false);
                                projectSettingsList->add_gui_element(exportAndPlayPushButton,false);
                                projectSettingsList->add_gui_element(playProgramPushButton , true);
                            }
                            else
                            {
                                projectSettingsList->add_gui_element(exportPushButton,true);
                            }
                            projectSettingsList->add_gui_element(electronCompileNote,true);
                            projectSettingsList->add_gui_element(electronCompileUrl,true);

                            projectSettingsList->process_self(&subViewedSpace,cam);

                            if( linux64BitCheckbox->is_clicked() )
                            {
                                buildBits = 64;
                            }
                            if( exportPushButton->is_clicked() && CURRENT_PROJECT!=NULL)
                            {
                                CURRENT_PROJECT->save_project();
                                CURRENT_PROJECT->export_project_linux("", buildBits,false, linuxDebugModeCheckbox->is_clicked(), linuxNativeBuildTypeOptions->get_selected_value() );
                                //linuxnativeBuildTypeOptions->set_clicked(false);
                            }
                            else if( GPE_FOUND_OS==GPE_IDE_LINUX && CURRENT_PROJECT!=NULL )
                            {
                                if( exportAndPlayPushButton->is_clicked() )
                                {
                                    export_and_play_native(true);
                                }
                                else if( playProgramPushButton->is_clicked() )
                                {
                                    CURRENT_PROJECT->run_project("",GPE_BUILD_LINUX,buildBits, linuxDebugModeCheckbox->is_clicked() );
                                }
                            }
                        }
                        else if( nExportDesktopOS=="OSX" )
                        {
                            projectSettingsList->add_gui_element(exportApplicationLabel,true);
                            projectSettingsList->add_gui_element(osx64BitCheckbox,false);
                            projectSettingsList->add_gui_element(osxDebugModeCheckbox , true);
                            projectSettingsList->add_gui_element(osxNativeBuildTypeOptions , true);

                            if( GPE_FOUND_OS==GPE_IDE_MAC )
                            {
                                projectSettingsList->add_gui_element(exportPushButton,false);
                                projectSettingsList->add_gui_element(exportAndPlayPushButton,false );
                                projectSettingsList->add_gui_element(playProgramPushButton , true);
                            }
                            else
                            {
                                projectSettingsList->add_gui_element(exportPushButton,true);
                            }
                            projectSettingsList->add_gui_element(electronCompileNote,true);
                            projectSettingsList->add_gui_element(electronCompileUrl,true);

                            projectSettingsList->process_self(&subViewedSpace,cam);

                            if( osx64BitCheckbox->is_clicked() )
                            {
                                buildBits = 64;
                            }

                            if( exportPushButton->is_clicked() && CURRENT_PROJECT!=NULL)
                            {
                                CURRENT_PROJECT->save_project();
                                CURRENT_PROJECT->export_project_osx("",buildBits,false, osxDebugModeCheckbox->is_clicked(), osxNativeBuildTypeOptions->get_selected_value()  );
                                //osxnativeBuildTypeOptions->set_clicked(false);
                            }
                            else if( GPE_FOUND_OS==GPE_IDE_MAC && CURRENT_PROJECT!=NULL )
                            {
                                if( exportAndPlayPushButton->is_clicked() )
                                {
                                    export_and_play_native(true);
                                }
                                else if( playProgramPushButton->is_clicked() )
                                {
                                    CURRENT_PROJECT->run_project("",GPE_BUILD_MAC,buildBits, osxDebugModeCheckbox->is_clicked() );
                                }
                            }
                        }
                    }
                }
            }
            else if( projectSettingsTabBar->get_selected_name()=="Colors")
            {
                projectSettingsList->add_gui_element(projectBorderColor,true);
                projectSettingsList->add_gui_element(projectGameBackgroundColor,true);
                ///
                projectSettingsList->add_gui_element(projectWebsiteBackgroundColor,true);
                projectSettingsList->add_gui_element(projectTextHeaderColor,true);
                projectSettingsList->add_gui_element(projectTextParagraphColor,true);

                projectSettingsList->add_gui_element(projectTextLinkColor,true);
                projectSettingsList->add_gui_element(projectTextLinkActiveColor,true);
                projectSettingsList->add_gui_element(projectTextLinkHoverColor,true);
                projectSettingsList->add_gui_element(projectTextLinkVisitedColor,true);
                projectSettingsList->process_self(&subViewedSpace,cam);
            }
            else if( projectSettingsTabBar->get_selected_name()=="Macros")
            {
                projectGameMacros->set_width(subViewedSpace.w-32 );
                projectGameMacros->set_height(subViewedSpace.h-96 );
                projectSettingsList->add_gui_element(projectGameMacros,true);
                projectSettingsList->process_self(&subViewedSpace,cam);

            }
            else if( projectSettingsTabBar->get_selected_name()=="Notes")
            {
                projectGameNotes->set_width(subViewedSpace.w-32 );
                projectGameNotes->set_height(subViewedSpace.h-96 );
                projectSettingsList->add_gui_element(projectGameNotes,true);
                projectSettingsList->process_self(&subViewedSpace,cam);

            }
            else if( projectSettingsTabBar->get_selected_name()=="Physics")
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
                projectSettingsList->add_gui_element( projectLayerEmptyTitle,false );
                for( iLayerN = 0; iLayerN < maxCollisionLayerCount; iLayerN++)
                {
                    projectLayerNamesColumnTitles[iLayerN]->set_height(widestTitle);
                    //projectLayerNamesColumnTitles[iLayerN]->set_width(16);
                    projectSettingsList->add_gui_element(projectLayerNamesColumnTitles[iLayerN],(iLayerN==maxCollisionLayerCount-1) );
                }
                for( iLayerN = 0; iLayerN < maxCollisionLayerCount; iLayerN++)
                {
                    projectLayerNamesRowTitles[iLayerN]->set_width(widestTitle);
                    projectSettingsList->add_gui_element(projectLayerNamesRowTitles[iLayerN],false);
                    for( iColMatrixColumn = 0; iColMatrixColumn < maxCollisionLayerCount-iLayerN; iColMatrixColumn++)
                    {
                        projectSettingsList->add_gui_element(projectCollisionLayerMatrix[iLayerN*32+iColMatrixColumn],(iColMatrixColumn==maxCollisionLayerCount-1-iLayerN) );
                    }
                }
                projectSettingsList->add_gui_element(selectAllCollisionBoxes, false);
                projectSettingsList->add_gui_element(unselectAllCollisionBoxes, true);
                projectSettingsList->add_gui_element(confirmResourceButton,true);
                projectSettingsList->process_self(&subViewedSpace,cam);

                bool renameLayer = false;
                for( int iLayerVal = 0; iLayerVal < 32 && renameLayer==false; iLayerVal++)
                {
                    if( projectLayerNamesRowTitles[iLayerVal]!=NULL && ( projectLayerNamesRowTitles[iLayerVal]->is_clicked() || projectLayerNamesRowTitles[iLayerVal]->is_rightclicked() ) )
                    {
                        renameLayer = true;
                    }
                    else if( projectLayerNamesColumnTitles[iLayerVal]!=NULL && ( projectLayerNamesColumnTitles[iLayerVal]->is_clicked() || projectLayerNamesColumnTitles[iLayerVal]->is_rightclicked() ) )
                    {
                        renameLayer = true;
                    }
                    if( renameLayer)
                    {
                        std::string newLayerName = get_string_from_popup("Rename this project layer","Please enter an unique alphanumeric name",projectLayerNamesRowTitles[iLayerVal]->get_name() );
                        bool layerValueAllowed = true;
                        if( is_alnum(newLayerName,true,true) )
                        {
                            for( int jLayerVal = 0; jLayerVal < 32; jLayerVal++)
                            {
                                if( iLayerVal!=jLayerVal)
                                {
                                    if( projectLayerNamesColumnTitles[jLayerVal] )
                                    {
                                        if( newLayerName==projectLayerNamesColumnTitles[jLayerVal]->get_name() )
                                        {
                                            display_user_alert("Invalid name given","Layer names must all be unique!");
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
                            display_user_alert("Invalid name given","Layer names must be alphanumerical in value.");
                        }
                        if( layerValueAllowed)
                        {
                            CURRENT_PROJECT->projectLayerNames[iLayerVal] = newLayerName;
                            update_project_layers();
                        }
                        userInput->reset_all_input();
                    }

                }

                if( !renameLayer )
                {
                    if(selectAllCollisionBoxes!=NULL && selectAllCollisionBoxes->is_clicked() )
                    {
                        for( int iMatrix = 0; iMatrix < 1024; iMatrix++)
                        {
                            projectCollisionLayerMatrix[iMatrix]->set_clicked(true);
                        }
                    }
                    else if( unselectAllCollisionBoxes!=NULL && unselectAllCollisionBoxes->is_clicked() )
                    {
                        for( int iMatrix = 0; iMatrix < 1024; iMatrix++)
                        {
                            projectCollisionLayerMatrix[iMatrix]->set_clicked(false);
                        }
                    }
                }
            }
        }
        //projectSettingsList->set_maxed_out_width();

        if( confirmResourceButton->is_clicked() )
        {
            save_resource();
        }
        if( nExportOptionHtml5 != projectHTML5SettingsTabBar->get_selected_name() )
        {
            //process_self(viewedSpace,cam);
        }
        if( nExportOptionName != exportSettingsBar->get_selected_name() )
        {
            //process_self(viewedSpace,cam);
        }
    }
}

void projectPropertiesResource::render_self(GPE_Renderer * cRender,SDL_Rect * viewedSpace, SDL_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam!=NULL && projectSettingsList!=NULL && projectSettingsTabBar!=NULL && viewedSpace!=NULL)
    {
        //if( forceRedraw)
        {
            render_rectangle(cRender,0,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);
            projectSettingsTabBar->render_self(cRender,viewedSpace,cam,true);
        }
        SDL_RenderSetViewport(cRender->get_renderer(),NULL);
        //SDL_RenderSetViewport(cRender->get_renderer(),viewedSpace);
        SDL_RenderSetViewport(cRender->get_renderer(),&subViewedSpace);
        projectSettingsList->render_self(cRender,&subViewedSpace,cam, true);

        if(projectSettingsTabBar->get_selected_name()=="Platforms")
        {
            exportSettingsBar->render_self(cRender,&subViewedSpace,cam,true);
        }

        SDL_RenderSetViewport(cRender->get_renderer(),NULL);
        SDL_RenderSetViewport(cRender->get_renderer(),viewedSpace);

    }
}

void projectPropertiesResource::save_resource(std::string alternatePath, int backupId)
{
    bool usingAltSaveSource = false;
    isModified = false;
    if( projectParentFolder!=NULL)
    {
        std::string projectSettingsFileName = projectParentFolder->parentProjectDirectory+"/gpe_project/project_settings.gpf";
        record_error("Saving project settings to "+projectSettingsFileName);
        std::ofstream newSaveDataFile (projectSettingsFileName.c_str() );
        if (newSaveDataFile.is_open() )
        {
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    Game Pencil Engine Project Settings DataFile \n";
            newSaveDataFile << "#    Created automatically via the Game Pencil Engine Editor \n";
            newSaveDataFile << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
            newSaveDataFile << "#    If you have any problems reading this file please report it to debug@pawbyte.com . \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
            //newSaveDataFile << "ResourceName=" << resourceName << "\n";
            newSaveDataFile << "#Project Settings     \n";
            newSaveDataFile << "ProjectIcon=" << get_local_from_global_file(iconFileName) << "\n";
            if( projectScreenWidthField!=NULL)
            {
                newSaveDataFile << "ScreenWidth=" << projectScreenWidthField->get_held_number() << "\n";
            }
            if( projectScreenHeightField!=NULL)
            {
                newSaveDataFile << "ScreenHeight=" << projectScreenHeightField->get_held_number() << "\n";
            }

            if( projectGameVersionField!=NULL)
            {
                newSaveDataFile << "ProjectVersion=" << projectGameVersionField->get_held_number() << "\n";
            }
            if( projectGameSubVersionField!=NULL)
            {
                newSaveDataFile << "ProjectSubVersion=" << projectGameSubVersionField->get_string() << "\n";
            }
            if( projectScaleSettings!=NULL)
            {
                newSaveDataFile << "ProjectScaleSettings=" << projectScaleSettings->get_selected_id() << "\n";
            }

            if( projectSettingsFPSRate!=NULL)
            {
                newSaveDataFile << "ProjectFPSCap=" << projectSettingsFPSRate->get_selected_id() << "\n";
            }

            newSaveDataFile << "#Project Publishing MetaData     \n";
            if( projectGameTitleField!=NULL)
            {
                newSaveDataFile << "ProjectTitle=" << projectGameTitleField->get_string() << "\n";
            }
            if( projectGameShortTitleField!=NULL)
            {
                newSaveDataFile << "ProjectShortTitle=" << projectGameShortTitleField->get_string() << "\n";
            }
            if( projectGameDateField!=NULL)
            {
                newSaveDataFile << "ProjectDate=" << projectGameDateField->get_string() << "\n";
            }
            if( projectGameDescriptionField!=NULL)
            {
                newSaveDataFile << "ProjectDescription=" << projectGameDescriptionField->get_string() << "\n";
            }
            if( projectGamePublisherField!=NULL)
            {
                newSaveDataFile << "ProjectPublisher=" << projectGamePublisherField->get_string() << "\n";
            }
            if( projectGameDeveloperField!=NULL)
            {
                newSaveDataFile << "ProjectDeveloper=" << projectGameDeveloperField->get_string() << "\n";
            }
            if( projectGameCopyrightField!=NULL)
            {
                newSaveDataFile << "ProjectCopyright=" << projectGameCopyrightField->get_string() << "\n";
            }
            if( projectGameWebsiteField!=NULL)
            {
                newSaveDataFile << "ProjectWebsite=" << projectGameWebsiteField->get_string() << "\n";
            }
            if( projectGameEmailField!=NULL )
            {
                newSaveDataFile << "ProjectEmail=" << projectGameEmailField->get_string() << "\n";
            }

            //Colors
            newSaveDataFile << "#Project Colors     \n";
            if( projectBorderColor!=NULL)
            {
                newSaveDataFile << "BorderColor=" << projectBorderColor->get_rgb_string() << "\n";
            }
            if( projectGameBackgroundColor!=NULL)
            {
                newSaveDataFile << "GameBackgroundColor=" << projectGameBackgroundColor->get_rgb_string() << "\n";
            }
            if( projectWebsiteBackgroundColor!=NULL)
            {
                newSaveDataFile << "WebsiteBackgroundColor=" << projectWebsiteBackgroundColor->get_rgb_string() << "\n";
            }
            if( projectTextHeaderColor!=NULL)
            {
                newSaveDataFile << "HeaderColor=" << projectTextHeaderColor->get_rgb_string() << "\n";
            }
            if( projectTextParagraphColor!=NULL)
            {
                newSaveDataFile << "ParagraphColor=" << projectTextParagraphColor->get_rgb_string() << "\n";
            }
            if( projectTextLinkColor!=NULL)
            {
                newSaveDataFile << "LinkColor=" << projectTextLinkColor->get_rgb_string() << "\n";
            }

            if( projectTextLinkColor!=NULL)
            {
                newSaveDataFile << "LinkActiveColor=" << projectTextLinkActiveColor->get_rgb_string() << "\n";
            }
            if( projectTextLinkColor!=NULL)
            {
                newSaveDataFile << "LinkHoverColor=" << projectTextLinkHoverColor->get_rgb_string() << "\n";
            }
            if( projectTextLinkColor!=NULL)
            {
                newSaveDataFile << "LinkVisitedColor=" << projectTextLinkVisitedColor->get_rgb_string() << "\n";
            }
            if( warnOnCloseCheckbox!=NULL)
            {
                newSaveDataFile << "HTML5WarnOnClose=" << warnOnCloseCheckbox->is_clicked() << "\n";
            }
            if( headerCodeBeforeGPECheckbox!=NULL)
            {
                newSaveDataFile << "HTML5CodeBeforeGPE=" << headerCodeBeforeGPECheckbox->is_clicked() << "\n";
            }

            //Project TextAreas
            if( projectGameMacros!=NULL)
            {
                projectGameMacros->export_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_macros.gpf");
            }
            if( projectGameNotes!=NULL)
            {
                projectGameNotes->export_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_notes.gpf");
            }

            if( projectCSSCode!=NULL)
            {
                projectCSSCode->export_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_css.css");
            }
            if( projectHtmlCode!=NULL)
            {
                projectHtmlCode->export_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_html.html");
            }
            if( projectHtmlHeaderCode!=NULL)
            {
                projectHtmlHeaderCode->export_text(projectParentFolder->parentProjectDirectory+"/gpe_project/project_html_header.html");
            }

            newSaveDataFile.close();
        }

        std::string projectCollisionMatrixFileName = projectParentFolder->parentProjectDirectory+"/gpe_project/project_layer_matrix.gpf";
        record_error("Saving project collision matrix to "+projectCollisionMatrixFileName);
        std::ofstream projectCollisionMatrixFile (projectCollisionMatrixFileName.c_str() );
        if (projectCollisionMatrixFile.is_open() )
        {
            projectCollisionMatrixFile << "#    --------------------------------------------------  # \n";
            projectCollisionMatrixFile << "#     \n";
            projectCollisionMatrixFile << "#     \n";
            projectCollisionMatrixFile << "#    Game Pencil Engine Project Settings DataFile \n";
            projectCollisionMatrixFile << "#    Created automatically via the Game Pencil Engine Editor \n";
            projectCollisionMatrixFile << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
            projectCollisionMatrixFile << "#    If you have any problems reading this file please report it to debug@pawbyte.com . \n";
            projectCollisionMatrixFile << "#     \n";
            projectCollisionMatrixFile << "#     \n";
            projectCollisionMatrixFile << "#    --------------------------------------------------  # \n";
            projectCollisionMatrixFile << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
            //newSaveDataFile << "ResourceName=" << resourceName << "\n";
            projectCollisionMatrixFile << "#Project Layer Collision Matrix Settings     \n";
            int jLayerCol = 0;
            for( int iLayerRow = 0; iLayerRow < 32; iLayerRow++)
            {
                projectCollisionMatrixFile << "CollisionLayer["+int_to_string(iLayerRow)+"]=";
                for(  jLayerCol = 0; jLayerCol < 32-iLayerRow; jLayerCol++)
                {
                    projectCollisionMatrixFile << int_to_string( projectCollisionLayerMatrix[iLayerRow*32+jLayerCol]->is_clicked() )+",";
                }
                projectCollisionMatrixFile << "\n";
            }
            projectCollisionMatrixFile.close();
        }
    }
}


int projectPropertiesResource::search_for_string(std::string needle)
{
    int foundStrings = 0;
    GPE_Main_Logs->log_general_comment("Searching [Project Settings]..");
    if( GPE_MAIN_GUI!=NULL)
    {
        GPE_MAIN_GUI->searchResultProjectName = parentProjectName;
        GPE_MAIN_GUI->searchResultResourceId = globalResouceIdNumber;
        GPE_MAIN_GUI->searchResultResourceName = resourceName;
        if( projectGameMacros!=NULL && projectGameMacros->has_content() )
        {
            foundStrings+=projectGameMacros->find_all_strings(needle,GPE_MAIN_GUI->findMatchCase->is_clicked() ,true,"Macros");
        }

        if( projectGameNotes!=NULL && projectGameNotes->has_content() )
        {
            foundStrings+=projectGameNotes->find_all_strings(needle,GPE_MAIN_GUI->findMatchCase->is_clicked() ,true,"Notes");
        }

        if( projectCSSCode!=NULL && projectCSSCode->has_content() )
        {
            foundStrings+=projectCSSCode->find_all_strings(needle,GPE_MAIN_GUI->findMatchCase->is_clicked() ,true,"CSS");
        }

        if( projectHtmlCode!=NULL && projectHtmlCode->has_content() )
        {
            foundStrings+=projectHtmlCode->find_all_strings(needle,GPE_MAIN_GUI->findMatchCase->is_clicked() ,true,"HTML");
        }

        if( projectHtmlHeaderCode!=NULL && projectHtmlHeaderCode->has_content() )
        {
            foundStrings+=projectHtmlHeaderCode->find_all_strings(needle,GPE_MAIN_GUI->findMatchCase->is_clicked() ,true,"HTML-Header");
        }
    }
    return foundStrings;
}

int projectPropertiesResource::search_and_replace_string(std::string needle, std::string newStr )
{
    int foundStrings = 0;
    int tempFoundCount = 0;

    GPE_Main_Logs->log_general_comment("Searching [Project Settings]..");
    if( GPE_MAIN_GUI!=NULL)
    {
        GPE_MAIN_GUI->searchResultProjectName = parentProjectName;
        GPE_MAIN_GUI->searchResultResourceId = globalResouceIdNumber;
        GPE_MAIN_GUI->searchResultResourceName = resourceName;
        if( projectGameMacros!=NULL && projectGameMacros->has_content() )
        {
            tempFoundCount = projectGameMacros->find_all_strings(needle,GPE_MAIN_GUI->findMatchCase->is_clicked() ,true,"Macros");
            if( tempFoundCount > 0)
            {
                foundStrings+=tempFoundCount;
                displayMessageString = "Replaced "+int_to_string( projectGameMacros->replace_all_found(needle, newStr ) )+" copies.";;
            }
        }

        if( projectGameNotes!=NULL && projectGameNotes->has_content() )
        {
            tempFoundCount = projectGameNotes->find_all_strings(needle,GPE_MAIN_GUI->findMatchCase->is_clicked() ,true,"Notes");
            if( tempFoundCount > 0)
            {
                foundStrings+=tempFoundCount;
                displayMessageString = "Replaced "+int_to_string( projectGameNotes->replace_all_found(needle, newStr ) )+" copies.";;
            }
        }

        if( projectCSSCode!=NULL && projectCSSCode->has_content() )
        {
            tempFoundCount = projectCSSCode->find_all_strings(needle,GPE_MAIN_GUI->findMatchCase->is_clicked() ,true,"CSS");
            if( tempFoundCount > 0)
            {
                foundStrings+=tempFoundCount;
                displayMessageString = "Replaced "+int_to_string( projectCSSCode->replace_all_found(needle, newStr ) )+" copies.";;
            }
        }

        if( projectHtmlCode!=NULL && projectHtmlCode->has_content() )
        {
            tempFoundCount = projectHtmlCode->find_all_strings(needle,GPE_MAIN_GUI->findMatchCase->is_clicked() ,true,"HTML");
            if( tempFoundCount > 0)
            {
                foundStrings+=tempFoundCount;
                displayMessageString = "Replaced "+int_to_string( projectHtmlCode->replace_all_found(needle, newStr ) )+" copies.";;
            }
        }

        if( projectHtmlHeaderCode!=NULL && projectHtmlHeaderCode->has_content() )
        {
            tempFoundCount = projectHtmlHeaderCode->find_all_strings(needle,GPE_MAIN_GUI->findMatchCase->is_clicked() ,true,"HTML-Header");
            if( tempFoundCount > 0)
            {
                foundStrings+=tempFoundCount;
                displayMessageString = "Replaced "+int_to_string( projectHtmlHeaderCode->replace_all_found(needle, newStr ) )+" copies.";;
            }
        }
    }
    return foundStrings;
}


void projectPropertiesResource::update_project_layers()
{
    if( CURRENT_PROJECT!=NULL & CURRENT_PROJECT->RESC_SCENES!=NULL)
    {
        int iLN = 0;
        for(  iLN = 0; iLN < 32; iLN++)
        {
            if( projectLayerNamesRowTitles[iLN]!=NULL)
            {
                projectLayerNamesRowTitles[iLN]->set_name( CURRENT_PROJECT->projectLayerNames[iLN] );
            }
            if( projectLayerNamesColumnTitles[iLN]!=NULL)
            {
                projectLayerNamesColumnTitles[iLN]->set_name( CURRENT_PROJECT->projectLayerNames[iLN] );
            }
        }

        int jLayerCol = 0;
        for(  iLN = 0; iLN < 32; iLN++)
        {
            for(  jLayerCol = 0; jLayerCol < 32-iLN; jLayerCol++)
            {
                projectCollisionLayerMatrix[iLN*32+jLayerCol]->descriptionText = "["+CURRENT_PROJECT->projectLayerNames[iLN]+" * "+CURRENT_PROJECT->projectLayerNames[jLayerCol]+"]";
            }
        }

        std::vector <GPE_ResourceContainer *> projectScenes;
        GPE_ResourceContainer * tempContainer = NULL;
        gameSceneResource * tempScnRes = NULL;
        CURRENT_PROJECT->RESC_SCENES->grab_useable_resources(projectScenes);
        for( int iRes = (int)projectScenes.size()-1; iRes >=0; iRes--)
        {
            tempContainer = projectScenes[iRes];
            if( tempContainer!=NULL)
            {
                if( tempContainer!=NULL && tempContainer->get_held_resource()!=NULL)
                {
                    tempScnRes = (gameSceneResource * ) tempContainer->get_held_resource();
                    if( tempScnRes!=NULL)
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
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            //*fileTarget << "Testing";
            return true;
        }
    }
    return false;
}
