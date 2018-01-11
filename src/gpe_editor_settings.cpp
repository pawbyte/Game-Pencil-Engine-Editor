/*
gpe_editor_settings.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_project_resources.h"

GPE_Theme_Holder::GPE_Theme_Holder( std::string tName, std::string tFileName, bool isCustomTheme )
{
    themeName = tName;
    templateFileName = tFileName;
    nameInEditor = new GPE_TextInputBasic(tName,"");
    themeIconImg = new GPE_Label_Image();
    themeBgFileLocation = new GPE_TextInputBasic("","");
    nonDefaultTheme = isCustomTheme;
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
    GPE_Input_Field_Color * tColor = NULL;
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
        return false;
    }
    GPE_Color * fThemeColor = NULL;
    GPE_Input_Field_Color * myColorInEditor = NULL;
    int jThemeColor = 0;
    int jThemeColorMax = 0;

    if( copyToSelf )
    {
        jThemeColorMax = systemTheme->get_color_count();
        clear_colors();
        nonDefaultTheme = systemTheme->is_custom_theme();
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
                myColorInEditor = new GPE_Input_Field_Color(fThemeColor->get_name(),"" );
                myColorInEditor->set_color_from_rgb( fThemeColor );
                myColorInEditor->set_label( fThemeColor->get_name() );
                colorsInEditor.push_back( myColorInEditor );
            }
        }
        else
        {
            //modifies the actual template
            myColorInEditor = colorsInEditor[jThemeColor];
            if( myColorInEditor!=NULL )
            {
                fThemeColor = systemTheme->find_color(myColorInEditor->get_name() );
                if( fThemeColor!=NULL)
                {
                    fThemeColor->change_rgba( myColorInEditor->get_r(), myColorInEditor->get_g(), myColorInEditor->get_b() );
                }
            }
        }
    }
    return true;
}

bool GPE_Theme_Holder::load_background( std::string bgTextureLocation, bool allOrNothing)
{
    std::string miniBGFileName = "";
    if( file_exists(bgTextureLocation ) && file_is_image(bgTextureLocation) )
    {
        if(themeIconImg!=NULL)
        {
            themeIconImg->load_label_image( bgTextureLocation );
            themeIconImg->set_height(64);
        }
        miniBGFileName = templateBgFileName = get_local_from_global_file( bgTextureLocation  );

        if( nonDefaultTheme )
        {
            miniBGFileName = APP_DIRECTORY_NAME+"themes/custom/"+miniBGFileName;
        }
        else
        {
            miniBGFileName = APP_DIRECTORY_NAME+"themes/"+miniBGFileName;
        }
        copy_file( bgTextureLocation, miniBGFileName );
        templateBgFileName = miniBGFileName;
        themeBgFileLocation->set_string( miniBGFileName );
        return true;
    }
    else if( allOrNothing ==false )
    {
        // in case the file is loaded elsewhere, we see if we can find it on the local themes
        bgTextureLocation = get_local_from_global_file(bgTextureLocation);
        if( nonDefaultTheme )
        {
            miniBGFileName = APP_DIRECTORY_NAME+"themes/custom/"+bgTextureLocation;
        }
        else
        {
            miniBGFileName = APP_DIRECTORY_NAME+"themes/"+bgTextureLocation;
        }
        if( file_exists(miniBGFileName ) && file_is_image(miniBGFileName) )
        {
            //performs "guaranteed recursion after finding local file"
            return load_background( miniBGFileName, true );
        }
    }
    return false;
}

gamePencilEditorSettingsResource::gamePencilEditorSettingsResource(GPE_ResourceContainer * pFolder)
{
    resourceFileName = "";
    resourceName = "Editor Settings";
    parentProjectName = "";
    parentProjectDirectory = "";

    autoSaveScreenshots = new GPE_CheckBoxBasic("Save screenshot on Print-Screen","Saves PrintScreen images to Screenshots Folder",0,0,true);
    makeMetaScreenshots = new GPE_CheckBoxBasic("Add Project Meta to Screenshots","Adds Project Name to screenshots",0,0,true);
    showShortProjectNames = new GPE_CheckBoxBasic("Show Shorter Project Names","Show Shorter Project Names[Reccommended]",0,0,true);

    for( int i = 0; i < GPE_EXTERNAL_EDITOR_MAX; i++)
    {
        pencilExternalEditorsFile[i] = new GPE_TextInputBasic("","");
        pencilExternalEditorsLoadButton[i] = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/binoculars.png","Find Editor..",-1,32);
    }
    pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->set_label("Image Editor");
    pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_AUD]->set_label("Audio Editor");
    pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_VID]->set_label("Video Editor");
    pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_FONT]->set_label("Font Editor");

    showStatupTipsBox = new GPE_CheckBoxBasic("Show Tips at Startup","Leave checked to automatically launch Tips at StartUp",0,0,true);
    launchLastProjectBox = new GPE_CheckBoxBasic("Open last project on startup","Open last project on startup",0,0,true);
    editorStatupTipsLabel = new GPE_Label_Title("Startup  Settings","Startup Settings");
    editorGuiSettingsLabel = new GPE_Label_Title("Editor & Gui Settings","Editor & Gui Settings");

    editorNormalDelayTime = new GPE_TextInputNumber("Valid from 0 to 60",true,0,60);
    editorNormalDelayTime->set_label("User Input Delay Time: (0-60)");
    editorNormalDelayTime->set_number( MAIN_GUI_SETTINGS->normalInputDelayTime );

    editorTextInputDelayTime = new GPE_TextInputNumber("Valid from 0 to 60",true,0,60);
    editorTextInputDelayTime->set_label("Fast Input Delay Time: (0-60)");
    editorTextInputDelayTime->set_number( MAIN_GUI_SETTINGS->textInputDelayTime );

    editorTextAreaDelayTime = new GPE_TextInputNumber("Valid from 0 to 60",true,0,60);
    editorTextAreaDelayTime->set_label("Text Area KeyDelay Time: (0-60)");
    editorTextAreaDelayTime->set_number( MAIN_GUI_SETTINGS->textAreaDelayTime-1 );

    advancedAreaLabel = new GPE_Label_Title("Advanced","Advanged Editor Settings");
    forceFrameRedraw = new GPE_CheckBoxBasic("Redraw every frame[Not reccommended]","Redraws the Editor every frame regardless of user input",0,0,false);
    showHiddenFilesInBrowser = new GPE_CheckBoxBasic("Show Hidden Files in Browser","(Linux only feature",0,0,true);
    clearCacheButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/close.png","Clear Font Cache","Clears the cache of pre-rendered texts");
    ideSettingsFPSRateLabel = new GPE_Label_Title("IDE Frame Rate","Sets the (max) framerate set for the IDE(Usually 30 or 60)");
    ideFPSRatioLabel = new GPE_Label_Text(double_to_string(FPS_RATIO*100)+"%","The FPS Ratio compared to 30 FPS");
    ideSettingsFPSRate = new GPE_DropDown_Menu( "FPS Cap",true);
    ideSettingsFPSRate->add_menu_option("30 FPS","",30,true);
    ideSettingsFPSRate->add_menu_option("48 FPS","",48);
    ideSettingsFPSRate->add_menu_option("50 FPS","",50);
    ideSettingsFPSRate->add_menu_option("60 FPS","",60);
    ideSettingsFPSRate->add_menu_option("72 FPS","",72);
    ideSettingsFPSRate->add_menu_option("90 FPS","",90);
    ideSettingsFPSRate->add_menu_option("120 FPS","",120);
    ideSettingsFPSRate->add_menu_option("240 FPS","",240);
    ideSettingsFPSRate->add_menu_option("300 FPS","",300);
    showFPSOnEditor = new GPE_CheckBoxBasic("Show FPS Counter","Show FPS Counter",0,0,SHOW_FPS_COUNTER );

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

    renderSceneBGColor = new GPE_CheckBoxBasic("Preview Scene Background Color","Use to display scene bg color instead of transparent tiles in scene editor",0,0, false);

    //Themes Section
    currentThemeInEdit = 0;
    themesLabel = new GPE_Label_Title("Themes");
    themeBgLabel = new GPE_Label_Text("Theme Background:    ");
    themeBgBrowseButton = new GPE_ToolLabelButton(0,0,"Browse...","Browse for theme background...");
    themePicker = new GPE_DropDown_Menu("Themes", true );
    themeLoadButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/folder.png","Load Custom Theme");
    themeAddButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/plus.png","Create Custom Theme");
    themeRemoveButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/minus.png","Remove Custom Theme");
    themeSetDefaultButton = new GPE_ToolLabelButton(0,0,"Set as default theme","Set as default theme");
    itchLinkForThemes = new GPE_TextURL("Find Themes on Itch.io","Browse the GPE Themes category on itch.io","https://itch.io/game-assets/tag-gamepencil/tag-theme");
    defaultTemplateEndPos = (int)GPE_MAIN_GUI->gpeThemes.size();

    GPE_Theme_Holder * newLoadedTheme = NULL;
    GPE_Theme * currentThemeBeingLoaded = NULL;
    for( int iTheme = 0; iTheme < defaultTemplateEndPos; iTheme++ )
    {
        currentThemeBeingLoaded = GPE_MAIN_GUI->gpeThemes[iTheme];
        if( currentThemeBeingLoaded !=NULL )
        {
            newLoadedTheme = new GPE_Theme_Holder( currentThemeBeingLoaded->themeName, currentThemeBeingLoaded->themeLocalLocation );

            themePicker->add_menu_option( currentThemeBeingLoaded->themeName,currentThemeBeingLoaded->themeLocalLocation, (int)themeHolders.size(), (iTheme==0) );
            newLoadedTheme->copy_theme(currentThemeBeingLoaded );
            themeHolders.push_back( newLoadedTheme );
        }
    }
    load_themes_from_folder( APP_DIRECTORY_NAME+"themes");
    load_themes_from_folder( APP_DIRECTORY_NAME+"themes/custom");
    //Tab Section
    tabSectionLabel = new GPE_Label_Title("Tabs");
    mouseAutoFindTabs = new GPE_CheckBoxBasic("Mouse detect tabs");
    tabSpaceSize = new GPE_TextInputNumber("Range: 1 to 16",true,1,16);
    tabSpaceSize->set_number(4);
    tabSpaceSize->set_label("Tab SIZE in spaces:");
    MAIN_GUI_SETTINGS->tabSpaceCount = 4;

    editorPageList = new GPE_GuiElementList();
    editorPageList->barXPadding = GENERAL_GPE_PADDING;
    editorPageList->barXMargin = 0;

    editorPageTabBar = new GPE_TabBar();
    editorPageTabBar->set_coords(16, 16);
    editorPageTabBar->add_new_tab("General");
    editorPageTabBar->add_new_tab("External Editors");
    editorPageTabBar->add_new_tab("Themes");
    editorPageTabBar->add_new_tab("Advanced");
    //editorPageTabBar->add_new_tab("Other Settings");
    editorPageTabBar->open_tab(0);

    subViewedSpace.x = 0;
    subViewedSpace.y = 0;
    subViewedSpace.w = SCREEN_WIDTH;
    subViewedSpace.h = SCREEN_HEIGHT;

    subEditorSelectBox = NULL; //Now stop crashing until I add your feature! :-)

}

gamePencilEditorSettingsResource::~gamePencilEditorSettingsResource()
{
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

    if( editorPageTabBar!=NULL)
    {
        delete editorPageTabBar;
        editorPageTabBar = NULL;
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

    GPE_TextInputBasic * tempInput = NULL;
    GPE_ToolIconButton * tempIcon = NULL;
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

void gamePencilEditorSettingsResource::load_themes_from_folder( std::string themeFolder )
{
	//Used for searching directories
    int iFile;
    std::string fileNameDiscovered = "";
    GPE_FileDirectory * dir = new GPE_FileDirectory();
    GPE_File * file = NULL;
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
                        newSystemTheme = GPE_MAIN_GUI->add_theme( fileNameDiscovered, true );
                        newSystemTheme->load_theme( themeFolder+"/"+fileNameDiscovered );
                        newLoadedTheme = new GPE_Theme_Holder( newSystemTheme->themeName, newSystemTheme->themeLocalLocation );
                        themePicker->add_menu_option( newSystemTheme->themeName,newSystemTheme->themeLocalLocation, (int)themeHolders.size(), false );
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

void gamePencilEditorSettingsResource::preprocess_self(std::string alternatePath)
{
    //showStatupTipsBox->set_clicked( GPE_MAIN_GUI->showTipsAtStartUp );
    displayMessageTitle = "Loading Editor Settings";
    displayMessageSubtitle = "Please Wait...";
    displayMessageString = "...";
    display_user_messaage();

    std::string otherColContainerName = "";

    std::string newFileIn = get_user_settings_folder()+"gpe_ide_local_settings.txt";
    std::ifstream gameResourceFileIn( newFileIn.c_str() );

    record_error("Loading Local settings - "+newFileIn);
    GPE_Theme * foundDefaultTheme = NULL;
    //If the level file could be loaded
    if( !gameResourceFileIn.fail() )
    {
        //makes sure the file is open
        if (gameResourceFileIn.is_open())
        {
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
            std::string colorThemeName;
            float foundFileVersion = 0;
            int textDelTime = 0;
            int foundFPSValue = FPS_CAP;
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

                            //record_error("Detected Key:     "+keyString);
                            if( keyString=="DefaultTheme" && GPE_MAIN_GUI!=NULL )
                            {
                                foundDefaultTheme = GPE_MAIN_GUI->find_theme( valString);
                                if( foundDefaultTheme!=NULL )
                                {
                                    GPE_MAIN_THEME = foundDefaultTheme;
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
                                autoSaveScreenshots->set_clicked( is_bool( valString ) );
                            }
                            else if( keyString=="PreviewSceneBGColor")
                            {
                                renderSceneBGColor->set_clicked( is_bool( valString ) );
                            }
                            else if( keyString=="MakeMetaScreenshots")
                            {
                                makeMetaScreenshots->set_clicked( is_bool( valString ) );
                            }
                            else if( keyString=="ShowShortProjectNames")
                            {
                                MAIN_GUI_SETTINGS->useShortProjectNames = is_bool( valString );
                                showShortProjectNames->set_clicked(  MAIN_GUI_SETTINGS->useShortProjectNames );
                            }
                            else if( keyString=="LaunchLastProject")
                            {
                                launchLastProjectBox->set_clicked( is_bool( valString ) );
                            }
                            else if( keyString=="ShowFPSCounter")
                            {
                                SHOW_FPS_COUNTER = is_bool( valString);
                                showFPSOnEditor->set_clicked( SHOW_FPS_COUNTER );
                            }
                            else if( keyString=="ShowTipsAtStart")
                            {
                                showStatupTipsBox->set_clicked( is_bool( valString ) );
                            }
                           else if( keyString=="BrowseHiddenFiles")
                            {
                                showHiddenFilesInBrowser->set_clicked( is_bool( valString ) );
                            }
                            else if( keyString=="ForceRedraw")
                            {
                                forceFrameRedraw->set_clicked( is_bool( valString ) );
                            }
                            else if( keyString=="ProjectFolderList" )
                            {
                                projectFolderListLocation = valString;
                            }
                            else if( keyString=="editorNormalDelayTime")
                            {
                                textDelTime = string_to_int(valString,4);
                                if( textDelTime <= 0)
                                {
                                    textDelTime = 1;
                                }
                                else if( textDelTime > 60)
                                {
                                    textDelTime = 60;
                                }
                                MAIN_GUI_SETTINGS->normalInputDelayTime = textDelTime;
                                editorNormalDelayTime->set_number(textDelTime-1);
                            }
                            else if( keyString=="InputDelayTime")
                            {
                                textDelTime = string_to_int(valString,4);
                                if( textDelTime <= 0)
                                {
                                    textDelTime = 1;
                                }
                                else if( textDelTime > 60)
                                {
                                    textDelTime = 60;
                                }
                                MAIN_GUI_SETTINGS->textInputDelayTime = textDelTime;
                                editorTextInputDelayTime->set_number(textDelTime-1);
                            }
                            else if( keyString=="TextAreaDelayTime")
                            {
                                textDelTime = string_to_int(valString,4);
                                if( textDelTime <= 0)
                                {
                                    textDelTime = 1;
                                }
                                else if( textDelTime > 60)
                                {
                                    textDelTime = 60;
                                }
                                MAIN_GUI_SETTINGS->textAreaDelayTime = textDelTime;
                                editorTextAreaDelayTime->set_number(textDelTime-1);
                            }
                            else if( keyString=="FPS")
                            {
                                foundFPSValue = string_to_int(valString,30);
                                foundFPSValue = std::max( foundFPSValue, 30);
                                foundFPSValue = std::min( foundFPSValue, 300);
                                set_fps(foundFPSValue);
                                ideSettingsFPSRate->set_value(foundFPSValue);
                            }
                            else if( keyString=="ButtonBarSize")
                            {
                                ideButtonBarSize->set_value( string_to_int( valString) );
                            }
                            else if( keyString=="ButtonBarAlignment")
                            {
                                ideButtonBarAlignment->set_value( string_to_int( valString) );
                            }
                        }
                    }
                }
                else
                {
                    record_error("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                }
            }
            //Sets default value based on new found current theme
            if( GPE_MAIN_THEME!=NULL && themePicker!=NULL )
            {
                themePicker->set_option( GPE_MAIN_THEME->themeName );
            }
        }
    }
}

void gamePencilEditorSettingsResource::process_self(GPE_Rect * viewedSpace,GPE_Rect *cam)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( cam!=NULL && editorPageList!=NULL && editorPageTabBar!=NULL && viewedSpace!=NULL)
    {
        /*std::string nExportOptionName = exportSettingsBar->get_selected_name();
        std::string nExportOptionHtml5 = projectHTML5SettingsTabBar->get_selected_name();*/
        subViewedSpace.x = editorPageTabBar->get_xpos()+viewedSpace->x;
        subViewedSpace.y = editorPageTabBar->get_y2pos()+viewedSpace->y;
        subViewedSpace.w = -editorPageTabBar->get_xpos()+viewedSpace->w;
        subViewedSpace.h = -editorPageTabBar->get_y2pos()+viewedSpace->h;
        int prevTab = editorPageTabBar->tabInUse;
        editorPageTabBar->process_self(viewedSpace,cam);
        if( prevTab!=editorPageTabBar->tabInUse)
        {
            editorPageList->reset_self();
        }
        editorPageList->clear_list();
        editorPageList->set_coords(0,0);
        editorPageList->set_width(subViewedSpace.w);
        editorPageList->set_height(subViewedSpace.h );
        editorPageList->barXPadding = GENERAL_GPE_PADDING;
        editorPageList->barXMargin = GENERAL_GPE_PADDING;
        int i = 0;
        GPE_Theme_Holder * currentThemeBeingEdited = NULL;
        int iColor = 0, iThemeColorMax = 0;
        if(editorPageTabBar->get_selected_name()=="General")
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
            ideFPSRatioLabel->set_name("FPS Ratio: "+double_to_string(FPS_RATIO*100)+"%");
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
        else if( editorPageTabBar->get_selected_name()=="Colors" || editorPageTabBar->get_selected_name()=="Themes" )
        {
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
            /*int colorsPerLine = (subViewedSpace.w-GENERAL_GPE_PADDING*4)/256;
            int colorsInLine = 0;*/
            bool goToNextColorLine = false;

            if( currentThemeBeingEdited!=NULL )
            {
                editorPageList->add_gui_element( themeBgLabel, false );
                editorPageList->add_gui_element( currentThemeBeingEdited->themeIconImg , false );
                editorPageList->add_gui_element( currentThemeBeingEdited->themeBgFileLocation , false );
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
                    editorPageList->add_gui_element(currentThemeBeingEdited->colorsInEditor[iColor] , true );
                }
            }

            editorPageList->add_gui_element(confirmResourceButton,false);
            editorPageList->add_gui_element(cancelResourceButton,true);
        }
        else if( editorPageTabBar->get_selected_name()=="External Editors")
        {
            for( i = 0; i < GPE_EXTERNAL_EDITOR_MAX; i++)
            {
                editorPageList->add_gui_element(pencilExternalEditorsFile[i],false);
                editorPageList->add_gui_element(pencilExternalEditorsLoadButton[i],true);
            }
            editorPageList->add_gui_element(confirmResourceButton,true);
        }
        else if( editorPageTabBar->get_selected_name()=="Advanced")
        {
            editorPageList->add_gui_element(advancedAreaLabel,true);
            editorPageList->add_gui_element(clearCacheButton,true);
            editorPageList->add_gui_element(showHiddenFilesInBrowser,true);
            editorPageList->add_gui_element(forceFrameRedraw,true);

            editorPageList->add_gui_element(editorNormalDelayTime,true);
            editorPageList->add_gui_element(editorTextAreaDelayTime,true);
            editorPageList->add_gui_element(editorTextInputDelayTime,true);

            editorPageList->add_gui_element(confirmResourceButton,false);
            editorPageList->add_gui_element(cancelResourceButton,true);
        }
        else
        {

        }

        editorPageList->process_self(&subViewedSpace,cam);

        //Process results of last process...
        if( editorPageTabBar->get_selected_name()=="Colors" || editorPageTabBar->get_selected_name()=="Themes" )
        {
            bool themeChanged = false;
            if( themePicker!=NULL )
            {
                int potentiallySelectedTheme = themePicker->get_selected_id();
                if( currentThemeInEdit!= potentiallySelectedTheme && potentiallySelectedTheme >=0 && potentiallySelectedTheme < (int)themeHolders.size() )
                {
                    currentThemeInEdit = potentiallySelectedTheme;
                    themeChanged = true;
                    /*input->reset_all_input();
                    input->inputHappenedInFrame = true;
                    process_self( viewedSpace, cam );
                    return;
                    */
                }
            }
            if( !themeChanged )
            {
                if( themeSetDefaultButton!=NULL && themeSetDefaultButton->is_clicked() )
                {
                    if( currentThemeInEdit >=0 && currentThemeInEdit < (int)GPE_MAIN_GUI->gpeThemes.size() )
                    {
                        GPE_MAIN_THEME = GPE_MAIN_GUI->gpeThemes[currentThemeInEdit];
                        appendToFile(get_user_settings_folder()+"gpe_ide_local_settings.txt","DefaultTheme="+GPE_MAIN_THEME->themeName );

                    }
                }
                else if( themeAddButton!=NULL && themeAddButton->is_clicked() )
                {
                    //Adds new theme if its a unique name
                    std::string newThemeName = get_string_from_popup("New Theme name","Only unique names allowed");
                    if( newThemeName.size() > 0 && newThemeName!="default" )
                    {
                        if( GPE_MAIN_GUI->find_theme( newThemeName)==NULL )
                        {
                            GPE_Theme * newGPETheme = GPE_MAIN_GUI->add_theme( newThemeName, true );
                            if( newGPETheme !=NULL )
                            {
                                //Now adds it to our editor here...
                                GPE_Theme_Holder * newThemeHolder =  new GPE_Theme_Holder(newThemeName, newThemeName+".gpf" );
                                newThemeHolder->copy_theme( newGPETheme, true );
                                themePicker->add_menu_option( newThemeName+".gpf", newThemeName+".gpf", (int)themeHolders.size() , true );
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
                    if( (int)GPE_MAIN_GUI->fileOpenProjectIconDir.size()==0 && CURRENT_PROJECT!=NULL )
                    {
                        GPE_MAIN_GUI->fileOpenProjectIconDir = CURRENT_PROJECT->get_project_directory();
                    }
                    std::string newThemeBGFileName = GPE_GetOpenFileName("Find Theme Background","Image",GPE_MAIN_GUI->fileOpenProjectIconDir );
                    currentThemeBeingEdited->load_background( newThemeBGFileName );
                }

                if( confirmResourceButton!=NULL && confirmResourceButton->is_clicked() )
                {
                    if( currentThemeBeingEdited!=NULL )
                    {
                        GPE_Theme * fThemeToChange = GPE_MAIN_GUI->find_theme( currentThemeBeingEdited->themeName );
                        if( fThemeToChange !=NULL)
                        {
                            currentThemeBeingEdited->copy_theme( fThemeToChange, false );
                            fThemeToChange->save_theme();
                        }
                    }
                }
            }
        }
        else if(editorPageTabBar->get_selected_name()=="Advanced")
        {
            if( clearCacheButton->is_clicked() )
            {
                if( display_get_prompt("Clear Font Cache?","Are you sure you will like to clear the font cache? Current Size: "+int_to_string(gpe->get_font_cache_size() ) +" textures. This operation may be time consuming",false)==DISPLAY_QUERY_YES)
                {
                    gpe->clear_font_cache();
                }
            }
        }
        else if( editorPageTabBar->get_selected_name()=="External Editors")
        {
            for( i = 0; i < GPE_EXTERNAL_EDITOR_MAX; i++)
            {
                if( pencilExternalEditorsLoadButton[i]->is_clicked() )
                {
                    std::string newFileName = GPE_GetOpenFileName("Find External File Editor","",MAIN_GUI_SETTINGS->fileOpenDefaultDir);
                    if( file_exists(newFileName) )
                    {
                        pencilExternalEditorsFile[i]->set_string(newFileName);
                    }
                }
            }
        }

        if( confirmResourceButton!=NULL && confirmResourceButton->is_clicked() )
        {
            SHOW_FPS_COUNTER = showFPSOnEditor->is_clicked();
            GPE_MAIN_GUI->showTipsAtStartUp = showStatupTipsBox->is_clicked();
            GPE_MAIN_GUI->save_settings();
            int foundDelTime = 0;
            foundDelTime = editorTextAreaDelayTime->get_held_number();
            if( foundDelTime < 0)
            {
                foundDelTime = 0;
            }
            else if( foundDelTime > 60)
            {
                foundDelTime = 60;
            }
            MAIN_GUI_SETTINGS->textAreaDelayTime = foundDelTime+1;
            editorTextAreaDelayTime->set_number(foundDelTime);

            foundDelTime = editorTextInputDelayTime->get_held_number();
            if( foundDelTime <= 0)
            {
                foundDelTime = 1;
            }
            else if( foundDelTime > 60)
            {
                foundDelTime = 60;
            }
            MAIN_GUI_SETTINGS->useShortProjectNames = showShortProjectNames->is_clicked();
            MAIN_GUI_SETTINGS->textInputDelayTime = foundDelTime;
            editorTextInputDelayTime->set_number(foundDelTime-1);

            foundDelTime = editorNormalDelayTime->get_held_number();
            if( foundDelTime <= 0)
            {
                foundDelTime = 1;
            }
            else if( foundDelTime > 60)
            {
                foundDelTime = 60;
            }
            MAIN_GUI_SETTINGS->normalInputDelayTime = foundDelTime;
            editorNormalDelayTime->set_number(foundDelTime-1);
            save_resource();
        }

        if( cancelResourceButton!=NULL && cancelResourceButton->is_clicked() )
        {
            preprocess_self();
            GPE_MAIN_GUI->showTipsAtStartUp = showStatupTipsBox->is_clicked();
            GPE_MAIN_GUI->save_settings();
        }
    }
}

void gamePencilEditorSettingsResource::render_self(GPE_Rect * viewedSpace,GPE_Rect *cam, bool forceRedraw)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( cam!=NULL && editorPageList!=NULL && editorPageTabBar!=NULL && viewedSpace!=NULL)
    {
        if( forceRedraw)
        {
            editorPageTabBar->render_self( viewedSpace,cam,forceRedraw);
        }
        MAIN_RENDERER->reset_viewpoint();
        MAIN_RENDERER->set_viewpoint(&subViewedSpace);
        editorPageList->render_self( &subViewedSpace,cam, forceRedraw);
        /*
        if(editorPageTabBar->get_selected_name()=="Platforms")
        {
            exportSettingsBar->render_self( &subViewedSpace,cam,forceRedraw);
        }*/
        MAIN_RENDERER->reset_viewpoint();
        MAIN_RENDERER->set_viewpoint( viewedSpace);
    }
}

void gamePencilEditorSettingsResource::save_resource(std::string alternatePath, int backupId)
{
    bool usingAltSaveSource = false;
    isModified = false;
    std::string newSaveDataFilename = get_user_settings_folder()+"gpe_ide_local_settings.txt";
    std::ofstream newSaveDataFile( newSaveDataFilename.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            write_header_on_file(&newSaveDataFile);
            newSaveDataFile << "#     \n";
            newSaveDataFile << "DefaultTheme=" << GPE_MAIN_THEME->themeName << "\n";
            newSaveDataFile << "ExternalImageEditor=" << pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string() << "\n";
            newSaveDataFile << "ExternalAudioEditor=" << pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_AUD]->get_string() << "\n";
            newSaveDataFile << "ExternalVideoEditor=" << pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_VID]->get_string() << "\n";
            newSaveDataFile << "ExternalFontEditor=" << pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_FONT]->get_string() << "\n";

            newSaveDataFile << "AutoSaveScreenshots=" << autoSaveScreenshots->is_clicked() << "\n";
            newSaveDataFile << "MakeMetaScreenshots=" << makeMetaScreenshots->is_clicked() << "\n";
            newSaveDataFile << "ShowFPSCounter=" << SHOW_FPS_COUNTER << "\n";
            newSaveDataFile << "ShowShortProjectNames=" << showShortProjectNames->is_clicked() << "\n";
            newSaveDataFile << "LaunchLastProject=" << launchLastProjectBox->is_clicked() << "\n";
            newSaveDataFile << "ShowTipsAtStart=" << showStatupTipsBox->is_clicked() << "\n";
            newSaveDataFile << "InputDelayTime=" << editorTextInputDelayTime->get_held_number() << "\n";
            newSaveDataFile << "TextAreaDelayTime=" << editorTextAreaDelayTime->get_held_number() << "\n";
            newSaveDataFile << "NormalDelayTime=" << editorNormalDelayTime->get_held_number() << "\n";
            newSaveDataFile << "BrowseHiddenFiles=" << showHiddenFilesInBrowser->is_clicked() << "\n";
            newSaveDataFile << "ForceRedraw=" << forceFrameRedraw->is_clicked() << "\n";
            newSaveDataFile << "PreviewSceneBGColor=" << renderSceneBGColor->is_clicked() << "\n";
            newSaveDataFile << "FPS="+int_to_string( ideSettingsFPSRate->get_selected_value() ) << "\n";
            newSaveDataFile << "ProjectFolderList="+projectFolderListLocation << "\n";
            newSaveDataFile << "ButtonBarSize="+int_to_string( ideButtonBarSize->get_selected_value() ) << "\n";
            newSaveDataFile << "ButtonBarAlignment="+int_to_string( ideButtonBarAlignment->get_selected_value() ) << "\n";
            newSaveDataFile << "\n";
        }
    }
    set_fps( ideSettingsFPSRate->get_selected_value() );
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
