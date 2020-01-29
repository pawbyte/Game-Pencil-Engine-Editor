/*
gpe_editor_settings.h
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

#ifndef GPE_EDITOR_SETTINGS_RESOURCES_H
#define GPE_EDITOR_SETTINGS_RESOURCES_H

#include "gpe_basic_resource_page.h"

const int GPE_EXTERNAL_EDITOR_MAX = 4;
const int GPE_EXTERNAL_EDITOR_IMG = 0;
const int GPE_EXTERNAL_EDITOR_AUD = 1;
const int GPE_EXTERNAL_EDITOR_VID = 2;
const int GPE_EXTERNAL_EDITOR_FONT = 3;

class GPE_Theme_Holder
{
public:
    std::string themeName;
    std::string templateFileName;
    std::string templateBgFileName;
    GPE_TextInputBasic * nameInEditor;
    bool nonDefaultTheme;
    std::vector < GPE_Input_Field_Color * > colorsInEditor;
    GPE_Label_Image * themeIconImg;
    GPE_TextInputBasic * themeBgFileLocation;
    GPE_Theme_Holder( std::string tName, std::string tFileName, bool isCustomTheme = false  );
    ~GPE_Theme_Holder();
    void clear_colors();
    bool copy_theme( GPE_Theme * systemTheme, bool copyToSelf = true );
    bool load_background( std::string bgTextureLocation, bool allOrNothing = false);
};

class gamePencilEditorSettingsResource: public generalGameResource
{
public:
    GPE_ToolPushButton * confirmResourceButton;
    GPE_ToolPushButton * cancelResourceButton;
    GPE_SelectBoxBasic * sideAreaPanel;
    GPE_Rect * sidePanelRect;
    GPE_CheckBoxBasic * showShortProjectNames;
    GPE_CheckBoxBasic * renderSceneBGColor;
    GPE_SelectBoxBasic * subEditorSelectBox;


    //General Editor Settings
    GPE_CheckBoxBasic * userInvertMouseScroll;

    GPE_ToolPushButton * openEditorFolder;
    GPE_ToolPushButton * openLocalSettingsFolder;
    GPE_CheckBoxBasic * autoSaveScreenshots;
    GPE_CheckBoxBasic * makeMetaScreenshots;

    //Coding language related settings
    GPE_Label_Title * CodingLanguageTitle;
    GPE_Label_Text * CodingLanguageLabel;
    GPE_Label_Paragraph * CodingLanguageParagraph;
    GPE_TextURL * CodingLanguageURL;
    GPE_SelectBoxBasic * codingLanguageSelector;
    //FPS Related Items
    GPE_Label_Title * editorGuiSettingsLabel;
    GPE_Label_Title * ideSettingsFPSRateLabel;
    GPE_DropDown_Menu * ideSettingsFPSRate;
    GPE_Label_Text * ideFPSRatioLabel;
    GPE_Label_Text *    ideButtonBarSizeLabel;
    GPE_CheckBoxBasic * showFPSOnEditor;

    GPE_DropDown_Menu * ideButtonBarSize;
    GPE_RadioButtonControllerBasic * ideButtonBarAlignment;

    //External Editor Section
    GPE_Label_Title * externalEditorsTitle;
    GPE_TextInputBasic * pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_MAX];
    GPE_ToolIconButton * pencilExternalEditorsLoadButton[GPE_EXTERNAL_EDITOR_MAX];

    GPE_TextInputNumber * editorTextAreaDelayTime;
    GPE_TextInputNumber * editorTextInputDelayTime;
    GPE_TextInputNumber * editorCursorBlinkDelayTime;
    GPE_TextInputNumber * editorNormalDelayTime;
    GPE_TextInputNumber * editorScrollSpeed;

    GPE_Label_Title * editorStatupTipsLabel;
    GPE_CheckBoxBasic * showStatupTipsBox;
    GPE_CheckBoxBasic * launchLastProjectBox;
    GPE_DropDown_Menu * projectAutoSaveRate;
    GPE_Label_Text * projectAutoSaveRateLabel;

    //Tab Section
    GPE_Label_Title * tabSectionLabel;
    GPE_CheckBoxBasic * mouseAutoFindTabs;
    GPE_TextInputNumber * tabSpaceSize;

    //Themes Section
    GPE_Label_Title * themesLabel;
    GPE_DropDown_Menu * themePicker;
    GPE_ToolIconButton * themeLoadButton;
    GPE_ToolIconButton * themeAddButton;
    GPE_ToolIconButton * themeRemoveButton;
    GPE_ToolLabelButton * themeSetDefaultButton;
    GPE_TextURL * itchLinkForThemes;
    std::vector< GPE_Theme_Holder * > themeHolders;
    int defaultTemplateEndPos;
    int currentThemeInEdit;

    GPE_Label_Text * themeBgLabel;
    GPE_ToolLabelButton * themeBgBrowseButton;
    //Advanced Section
    GPE_Label_Title * advancedAreaLabel;
    GPE_CheckBoxBasic * showHiddenFilesInBrowser;
    GPE_CheckBoxBasic * forceFrameRedraw;
    GPE_ToolPushButton * clearCacheButton;

    //Editor Related variables
    GPE_Rect subViewedSpace;
    GPE_GuiElementList * editorPageList;
    std::string projectFolderListLocation;
    gamePencilEditorSettingsResource();
    ~gamePencilEditorSettingsResource();
    bool include_local_files( std::string pBuildDir , int buildType );

    void load_themes_from_folder( std::string themeFolder );
    void prerender_self( );
    void preprocess_self(std::string alternatePath = "");
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void save_resource(std::string alternatePath = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

extern gamePencilEditorSettingsResource * MAIN_EDITOR_SETTINGS;
#endif // GPE_EDITOR_SETTINGS_RESOURCES_H
